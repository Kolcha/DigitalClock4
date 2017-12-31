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

#ifndef DIGITAL_CLOCK_CORE_UPDATER_H
#define DIGITAL_CLOCK_CORE_UPDATER_H

#include <QObject>

#include <QDate>
#include <QString>
#include <QByteArray>

namespace digital_clock {
namespace core {

class ClockState;
class HttpClient;

class Updater : public QObject
{
  Q_OBJECT

public:
  explicit Updater(ClockState* state, QObject* parent = nullptr);
  ~Updater();

signals:
  void NewVersion(const QString& new_version, const QString& link);
  void UpToDate();
  void ErrorMessage(const QString& msg);

public slots:
  void CheckForUpdates();
  void SetCheckForBeta(bool check);
  void SetAutoupdate(bool update);
  void SetUpdatePeriod(int period);
  void TimeoutHandler();

private slots:
  void ProcessData();

private:
  void RunCheckForUpdates(bool force);

  ClockState* state_;
  HttpClient* downloader_;
  bool check_beta_;
  QByteArray data_;
  QDate last_update_;
  bool autoupdate_;
  int update_period_;
  bool force_update_;
  bool was_error_;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_UPDATER_H
