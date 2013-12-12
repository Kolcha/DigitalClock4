#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <QThread>
#ifndef Q_MOC_RUN
#include <boost/asio.hpp>
#endif // Q_MOC_RUN

class HttpClient : public QThread {
  Q_OBJECT
public:
  explicit HttpClient(const QString& server, const QString& path, QObject* parent = 0);

signals:
  void DataDownloaded(const QByteArray& data);
  void ErrorMessage(const QString& msg);

public slots:

protected:
  void run();

private:
  void HandleResolve(const boost::system::error_code& err,
      boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
  void HandleConnect(const boost::system::error_code& err,
      boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
  void HandleWriteRequest(const boost::system::error_code& err);
  void HandleReadStatusLine(const boost::system::error_code& err);
  void HandleReadHeaders(const boost::system::error_code& err);
  void HandleReadContent(const boost::system::error_code& err);

  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::resolver resolver_;
  boost::asio::ip::tcp::socket socket_;
  boost::asio::streambuf request_;
  boost::asio::streambuf response_;
};

#endif // HTTP_CLIENT_H
