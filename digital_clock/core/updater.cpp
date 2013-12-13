#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include "updater.h"

#define OPT_LAST_UPDATE    "updater/last_update"

Updater::Updater(QObject* parent)
  : QObject(parent),
    downloader_("digitalclock4.sourceforge.net", "/latest.json"),
    check_beta_(false), autoupdate_(true), update_period_(3) {
  connect(&downloader_, &HttpClient::ErrorMessage, this, &Updater::ErrorMessage);
  connect(&downloader_, &HttpClient::DataDownloaded,
          [=] (const QByteArray& data) { data_.append(data); });
  connect(&downloader_, &HttpClient::finished, this, &Updater::ProcessData);

  QSettings settings;
  last_update_ = settings.value(OPT_LAST_UPDATE).value<QDate>();
}

Updater::~Updater() {
  if (downloader_.isRunning()) downloader_.wait();
  QSettings settings;
  settings.setValue(OPT_LAST_UPDATE, last_update_);
}

void Updater::CheckForUpdates() {
  downloader_.start();
}

void Updater::SetCheckForBeta(bool check) {
  check_beta_ = check;
}

void Updater::SetAutoupdate(bool update) {
  autoupdate_ = update;
}

void Updater::SetUpdatePeriod(qint64 period) {
  update_period_ = period;
}

void Updater::TimeoutHandler() {
  if (!autoupdate_ || downloader_.isRunning()) return;
  if (last_update_.daysTo(QDate::currentDate()) >= update_period_) CheckForUpdates();
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
  last_update_ = QDate::currentDate();
}
