#ifndef UPDATER_H
#define UPDATER_H

#include "http_client.h"

class Updater : public QObject {
  Q_OBJECT
public:
  explicit Updater(QObject* parent = 0);
  ~Updater();

signals:
  void LatestVersion(const QString& new_version);
  void ErrorMessage(const QString& msg);

public slots:
  void CheckForUpdates();
  void SetCheckForBeta(bool check);

private slots:
  void ProcessData();

private:
  HttpClient downloader_;
  bool check_beta_;
  QByteArray data_;
};

#endif // UPDATER_H
