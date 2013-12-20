#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>
#include "updater.h"

#define OPT_LAST_UPDATE    "updater/last_update"

Updater::Updater(QObject* parent)
  : QObject(parent),
    check_beta_(false), autoupdate_(true), update_period_(3),
    force_update_(false), was_error_(false) {
  QSettings settings;
  last_update_ = settings.value(OPT_LAST_UPDATE, QDate(2013, 6, 18)).value<QDate>();
}

Updater::~Updater() {
  if (downloader_ && downloader_->isRunning()) {
    downloader_->wait();
    delete downloader_;
  }
  QSettings settings;
  settings.setValue(OPT_LAST_UPDATE, last_update_);
}

void Updater::CheckForUpdates(bool force) {
  force_update_ = force;
  was_error_ = false;
  data_.clear();
  downloader_ = new HttpClient("digitalclock4.sourceforge.net", "/latest.json");
  connect(downloader_, &HttpClient::ErrorMessage, [=] (const QString& msg) {
    was_error_ = true;
    emit ErrorMessage(msg);
  });
  connect(downloader_, &HttpClient::DataDownloaded,
          [=] (const QByteArray& data) { data_.append(data); });
  connect(downloader_, &HttpClient::finished, this, &Updater::ProcessData);
  downloader_->start();
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
  if (!autoupdate_ || (downloader_ && downloader_->isRunning())) return;
  if (last_update_.daysTo(QDate::currentDate()) >= update_period_) CheckForUpdates(false);
}

void Updater::ProcessData() {
  if (was_error_) return;
  QJsonParseError err;
  QJsonDocument js_doc = QJsonDocument::fromJson(data_, &err);
  if (err.error != QJsonParseError::NoError) {
    emit ErrorMessage(err.errorString());
    delete downloader_;
    return;
  }
  QJsonObject js_obj = js_doc.object().value("stable").toObject();
  QString latest = js_obj.value("version").toString();
  QString link = js_obj.value("download").toString();
  if (check_beta_) {
    js_obj = js_doc.object().value("testing").toObject();
    QString t_version = js_obj.value("version").toString("-");
    if (t_version != "-") {
      latest = t_version;
      link = js_obj.value("download").toString();
    }
  }

  if (latest > QCoreApplication::applicationVersion()) {
    emit NewVersion(latest, link);
  } else {
    if (force_update_) emit UpToDate();
  }
  last_update_ = QDate::currentDate();
  data_.clear();
  delete downloader_;
}
