#include <QJsonDocument>
#include <QJsonObject>
#include "updater.h"

Updater::Updater(QObject* parent)
  : QObject(parent),
    downloader_("digitalclock4.sourceforge.net", "/latest.json"),
    check_beta_(false) {
  connect(&downloader_, &HttpClient::ErrorMessage, this, &Updater::ErrorMessage);
  connect(&downloader_, &HttpClient::DataDownloaded,
          [=] (const QByteArray& data) { data_.append(data); });
  connect(&downloader_, &HttpClient::finished, this, &Updater::ProcessData);
}

Updater::~Updater() {
  if (downloader_.isRunning()) downloader_.wait();
}

void Updater::CheckForUpdates() {
  downloader_.start();
}

void Updater::SetCheckForBeta(bool check) {
  check_beta_ = check;
}

void Updater::ProcessData() {
  QJsonParseError err;
  QJsonDocument js_doc = QJsonDocument::fromJson(data_, &err);
  if (err.error != QJsonParseError::NoError) {
    emit ErrorMessage(err.errorString());
    return;
  }
  QJsonObject js_obj = js_doc.object().value("stable").toObject();
  QString latest = js_obj.value("version").toString();
  if (check_beta_) {
    js_obj = js_doc.object().value("testing").toObject();
    QString t_version = js_obj.value("version").toString("-");
    latest = t_version == "-" ? latest : t_version;
  }
  emit LatestVersion(latest);
}
