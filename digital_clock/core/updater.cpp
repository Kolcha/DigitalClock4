/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "updater.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>
#include <QVersionNumber>

#include "core/build_date.h"
#include "core/clock_state.h"
#include "core/http_client.h"

#define S_OPT_LAST_UPDATE       "last_update"

namespace digital_clock {
namespace core {

Updater::Updater(ClockState* state, QObject* parent) :
  QObject(parent),
  state_(state),
  check_beta_(false), autoupdate_(true), update_period_(3),
  force_update_(false), was_error_(false)
{
  last_update_ = state_->GetVariable(S_OPT_LAST_UPDATE, QDate(2013, 6, 18)).value<QDate>();
  downloader_ = new HttpClient(this);
  connect(downloader_, &HttpClient::ErrorMessage, [=] (const QString& msg) {
    was_error_ = true;
    emit ErrorMessage(msg);
  });
  connect(downloader_, &HttpClient::DataDownloaded, [=] (const QByteArray& data) { data_.append(data); });
  connect(downloader_, &HttpClient::finished, this, &Updater::ProcessData);
}

Updater::~Updater()
{
  if (downloader_->isRunning()) downloader_->cancel();
}

void Updater::CheckForUpdates()
{
  RunCheckForUpdates(true);
}

void Updater::SetCheckForBeta(bool check)
{
  check_beta_ = check;
}

void Updater::SetAutoupdate(bool update)
{
  autoupdate_ = update;
}

void Updater::SetUpdatePeriod(int period)
{
  update_period_ = period;
}

void Updater::TimeoutHandler()
{
  if (!autoupdate_ || (downloader_ && downloader_->isRunning())) return;
  if (last_update_.daysTo(QDate::currentDate()) >= update_period_) RunCheckForUpdates(false);
}

void Updater::ProcessData()
{
  if (was_error_) return;
  QJsonParseError err;
  QJsonDocument js_doc = QJsonDocument::fromJson(data_, &err);
  if (err.error != QJsonParseError::NoError) {
    was_error_ = true;
    emit ErrorMessage(err.errorString());
    return;
  }
  QJsonObject js_obj = js_doc.object().value("stable").toObject();
  QVersionNumber latest = QVersionNumber::fromString(js_obj.value("version").toString());
  QDate last_build = QDate::fromString(js_obj.value("timestamp").toString().simplified(), "dd-MM-yyyy");
  QString link = js_obj.value("download").toString();
  if (check_beta_) {
    js_obj = js_doc.object().value("testing").toObject();
    QString t_version = js_obj.value("version").toString("-");
    if (t_version != "-") {
      latest = QVersionNumber::fromString(t_version);
      last_build = QDate::fromString(js_obj.value("timestamp").toString().simplified(), "dd-MM-yyyy");
      link = js_obj.value("download").toString();
    }
  }

  if (latest > QVersionNumber::fromString(QCoreApplication::applicationVersion()) || build_date() < last_build) {
    QString latest_str = latest.toString() + ", ";
    latest_str += last_build.toString(Qt::DefaultLocaleShortDate);
    emit NewVersion(latest_str, link);
  } else {
    if (force_update_) emit UpToDate();
  }
  data_.clear();
  state_->SetVariable(S_OPT_LAST_UPDATE, last_update_);
}

void Updater::RunCheckForUpdates(bool force)
{
  force_update_ = force;
  was_error_ = false;
  data_.clear();
  last_update_ = QDate::currentDate();
  downloader_->startRequest(QUrl("http://digitalclock4.sourceforge.net/cgi-bin/update.py"));
}

} // namespace core
} // namespace digital_clock
