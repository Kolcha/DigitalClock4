/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "http_client.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#ifdef Q_OS_LINUX
#include <QSysInfo>
#endif

namespace digital_clock {
namespace core {

HttpClient::HttpClient(QObject* parent) :
  QObject(parent),
  is_running_(false)
{
}

HttpClient::~HttpClient()
{
  if (is_running_) cancel();
}

void HttpClient::startRequest(QUrl url)
{
  url_ = url;
  is_running_ = true;
  QNetworkRequest req(url);
#ifdef Q_OS_MACOS
  req.setRawHeader("User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.10; rv:43.0) Gecko/20100101 Firefox/43.0");
#endif
#ifdef Q_OS_LINUX
  if (QSysInfo::WordSize == 64) {
    req.setRawHeader("User-Agent", "Mozilla/5.0 (X11; Fedora; Linux x86_64; rv:43.0) Gecko/20100101 Firefox/43.0");
  } else {
    req.setRawHeader("User-Agent", "Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:43.0) Gecko/20100101 Firefox/43.0");
  }
#endif
#ifdef Q_OS_WIN
#ifdef Q_OS_WIN64
  req.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.3; Win64; x64; rv:45.0) Gecko/20100101 Firefox/45.0");
#else
  req.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.3; rv:45.0) Gecko/20100101 Firefox/45.0");
#endif
#endif
  reply_ = qnam_.get(req);
  connect(reply_, SIGNAL(finished()), this, SLOT(httpFinished()));
  connect(reply_, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
}

bool HttpClient::isRunning() const
{
  return is_running_;
}

void HttpClient::cancel()
{
  if (is_running_) reply_->abort();
}

void HttpClient::httpFinished()
{
  QVariant redirectionTarget = reply_->attribute(QNetworkRequest::RedirectionTargetAttribute);
  if (reply_->error()) {
    emit ErrorMessage(reply_->errorString());
  } else if (!redirectionTarget.isNull()) {
    // redirect to provided url
    QUrl newUrl = url_.resolved(redirectionTarget.toUrl());
    url_ = newUrl;
    reply_->deleteLater();
    startRequest(url_);
    return;
  } else {
    // download was successful, process data
    emit finished();
  }

  reply_->deleteLater();
  reply_ = 0;
  is_running_ = false;
}

void HttpClient::httpReadyRead()
{
  // this slot gets called every time the QNetworkReply has new data.
  if (reply_->attribute(QNetworkRequest::RedirectionTargetAttribute).isNull()) {
    emit DataDownloaded(reply_->readAll());
  }
}

} // namespace core
} // namespace digital_clock
