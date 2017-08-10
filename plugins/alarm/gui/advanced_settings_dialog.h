/*
    Digital Clock: alarm plugin
    Copyright (C) 2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef ALARM_PLUGIN_ADVANCED_SETTINGS_DIALOG_H
#define ALARM_PLUGIN_ADVANCED_SETTINGS_DIALOG_H

#include <QDialog>

#include <QSettings>

namespace alarm_plugin {

namespace Ui {
class AdvancedSettingsDialog;
}

class AdvancedSettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AdvancedSettingsDialog(QWidget* parent = nullptr);
  ~AdvancedSettingsDialog();

  void Init(const QSettings::SettingsMap& settings);

signals:
  void OptionChanged(const QString& key, const QVariant& value);

private slots:
  void on_shortcut_edit_editingFinished();

private:
  Ui::AdvancedSettingsDialog* ui;
};

} // namespace alarm_plugin

#endif // ALARM_PLUGIN_ADVANCED_SETTINGS_DIALOG_H
