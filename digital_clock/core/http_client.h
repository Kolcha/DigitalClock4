/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef DIGITAL_CLOCK_CORE_HTTP_CLIENT_H
#define DIGITAL_CLOCK_CORE_HTTP_CLIENT_H

#include <QObject>

#include <QUrl>
#include <QNetworkAccessManager>

namespace digital_clock {
namespace core {

class HttpClient : public QObject
{
  Q_OBJECT

public:
  explicit HttpClient(QObject* parent = nullptr);
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

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_HTTP_CLIENT_H
