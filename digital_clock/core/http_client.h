#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <QUrl>
#include <QNetworkAccessManager>

class HttpClient : public QObject {
  Q_OBJECT
public:
  explicit HttpClient(QObject* parent = 0);
  ~HttpClient();

  void startRequest(QUrl url);
  bool isRunning() const;
  void cancel();

signals:
  void DataDownloaded(const QByteArray& data);
  void ErrorMessage(const QString& msg);
  void finished();

private slots:
  void httpFinished();
  void httpReadyRead();

private:
  QUrl url_;
  QNetworkAccessManager qnam_;
  QNetworkReply* reply_;
  bool is_running_;
};

#endif // HTTP_CLIENT_H
