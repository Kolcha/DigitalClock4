/*
    Digital Clock: talking clock plugin
    Copyright (C) 2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef TALKING_CLOCK_SETTINGS_DIALOG_H
#define TALKING_CLOCK_SETTINGS_DIALOG_H

#include <QDialog>

#include <QSettings>

namespace talking_clock {

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = 0);
  ~SettingsDialog();

  void Init(const QSettings::SettingsMap& settings);

signals:
  void OptionChanged(const QString& key, const QVariant& value);

private:
  Ui::SettingsDialog* ui;
};

} // namespace talking_clock

#endif // TALKING_CLOCK_SETTINGS_DIALOG_H
