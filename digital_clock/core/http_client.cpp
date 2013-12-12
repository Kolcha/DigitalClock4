#include <QDebug>
#include <boost/bind.hpp>
#include "http_client.h"

using boost::asio::ip::tcp;

HttpClient::HttpClient(const QString& server, const QString& path, QObject* parent)
  : QThread(parent), resolver_(io_service_), socket_(io_service_) {
  // Form the request. We specify the "Connection: close" header so that the
  // server will close the socket after transmitting the response. This will
  // allow us to treat all data up until the EOF as the content.
  std::ostream request_stream(&request_);
  request_stream << "GET " << path.toStdString() << " HTTP/1.0\r\n";
  request_stream << "Host: " << server.toStdString() << "\r\n";
  request_stream << "Accept: */*\r\n";
  request_stream << "Connection: close\r\n\r\n";

  // Start an asynchronous resolve to translate the server and service names
  // into a list of endpoints.
  tcp::resolver::query query(server.toStdString(), "http");
  resolver_.async_resolve(query,
      boost::bind(&HttpClient::HandleResolve, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::iterator));
}

void HttpClient::run() {
  io_service_.run();
}

void HttpClient::HandleResolve(const boost::system::error_code& err,
                               tcp::resolver::iterator endpoint_iterator) {
  if (!err) {
    // Attempt a connection to the first endpoint in the list. Each endpoint
    // will be tried until we successfully establish a connection.
    tcp::endpoint endpoint = *endpoint_iterator;
    socket_.async_connect(endpoint,
        boost::bind(&HttpClient::HandleConnect, this,
          boost::asio::placeholders::error, ++endpoint_iterator));
  } else {
    qWarning() << "Error:" << QString::fromStdString(err.message());
    emit ErrorMessage(QString::fromStdString(err.message()));
  }
}

void HttpClient::HandleConnect(const boost::system::error_code& err,
                               tcp::resolver::iterator endpoint_iterator) {
  if (!err) {
    // The connection was successful. Send the request.
    boost::asio::async_write(socket_, request_,
        boost::bind(&HttpClient::HandleWriteRequest, this,
          boost::asio::placeholders::error));
  } else if (endpoint_iterator != tcp::resolver::iterator()) {
    // The connection failed. Try the next endpoint in the list.
    socket_.close();
    tcp::endpoint endpoint = *endpoint_iterator;
    socket_.async_connect(endpoint,
        boost::bind(&HttpClient::HandleConnect, this,
          boost::asio::placeholders::error, ++endpoint_iterator));
  } else {
    qWarning() << "Error:" << QString::fromStdString(err.message());
    emit ErrorMessage(QString::fromStdString(err.message()));
  }
}

void HttpClient::HandleWriteRequest(const boost::system::error_code& err) {
  if (!err) {
    // Read the response status line.
    boost::asio::async_read_until(socket_, response_, "\r\n",
        boost::bind(&HttpClient::HandleReadStatusLine, this,
          boost::asio::placeholders::error));
  } else {
    qWarning() << "Error:" << QString::fromStdString(err.message());
    emit ErrorMessage(QString::fromStdString(err.message()));
  }
}

void HttpClient::HandleReadStatusLine(const boost::system::error_code& err) {
  if (!err) {
    // Check that response is OK.
    std::istream response_stream(&response_);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
      qWarning() << "Invalid response";
      emit ErrorMessage("Invalid response");
      return;
    }
    if (status_code != 200) {
      qWarning() << "Response returned with status code" << status_code;
      emit ErrorMessage(QString("Response returned with status cede %1").arg(status_code));
      return;
    }

    // Read the response headers, which are terminated by a blank line.
    boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
        boost::bind(&HttpClient::HandleReadHeaders, this,
          boost::asio::placeholders::error));
  } else {
    qWarning() << "Error:" << QString::fromStdString(err.message());
    emit ErrorMessage(QString::fromStdString(err.message()));
  }
}

void HttpClient::HandleReadHeaders(const boost::system::error_code& err) {
  if (!err) {
    // Process the response headers.
    std::istream response_stream(&response_);
    std::string header;
    while (std::getline(response_stream, header) && header != "\r");

    // Write whatever content we already have to output.
    if (response_.size() > 0) {
      std::string sdata((std::istreambuf_iterator<char>(&response_)),
                        std::istreambuf_iterator<char>());
      QByteArray data(sdata.data(), (int)sdata.size());
      emit DataDownloaded(data);
    }

    // Start reading remaining data until EOF.
    boost::asio::async_read(socket_, response_,
        boost::asio::transfer_at_least(1),
        boost::bind(&HttpClient::HandleReadContent, this,
          boost::asio::placeholders::error));
  } else {
    qWarning() << "Error:" << QString::fromStdString(err.message());
    emit ErrorMessage(QString::fromStdString(err.message()));
  }
}

void HttpClient::HandleReadContent(const boost::system::error_code& err) {
  if (!err) {
    // Write all of the data that has been read so far.
    std::string sdata((std::istreambuf_iterator<char>(&response_)),
                      std::istreambuf_iterator<char>());
    QByteArray data(sdata.data(), (int)sdata.size());
    emit DataDownloaded(data);

    // Continue reading remaining data until EOF.
    boost::asio::async_read(socket_, response_,
        boost::asio::transfer_at_least(1),
        boost::bind(&HttpClient::HandleReadContent, this,
          boost::asio::placeholders::error));
  } else if (err != boost::asio::error::eof) {
    qWarning() << "Error:" << QString::fromStdString(err.message());
    emit ErrorMessage(QString::fromStdString(err.message()));
  }
}
