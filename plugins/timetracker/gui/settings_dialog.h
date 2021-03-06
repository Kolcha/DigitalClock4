/*
    Digital Clock: timetracker plugin
    Copyright (C) 2020  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef TIMETRACKER_SETTINGS_DIALOG_H
#define TIMETRACKER_SETTINGS_DIALOG_H

#include <QDialog>
#include <QSettings>

namespace timetracker {

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = nullptr);
  ~SettingsDialog();

  void AddCommonWidget(QWidget* widget);

  void Init(const QSettings::SettingsMap& settings);

signals:
  void OptionChanged(const QString&, const QVariant&);

private slots:
  void on_hide_if_inactive_clicked(bool checked);

  void on_pause_seq_edit_editingFinished();
  void on_restart_seq_edit_editingFinished();
  void on_settings_seq_edit_editingFinished();

private:
  Ui::SettingsDialog* ui;
};

} // namespace timetracker

#endif // TIMETRACKER_SETTINGS_DIALOG_H
