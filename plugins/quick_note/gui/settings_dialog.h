/*
    Digital Clock: quick note plugin
    Copyright (C) 2016-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef QUICK_NOTE_SETTINGS_DIALOG_H
#define QUICK_NOTE_SETTINGS_DIALOG_H

#include <QDialog>
#include <QSettings>

namespace quick_note {

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = nullptr);
  ~SettingsDialog();

  void Init(const QSettings::SettingsMap& settings);

signals:
  void OptionChanged(const QString& key, const QVariant& new_value);

private slots:
  void on_note_edit_textEdited(const QString& arg1);

private:
  Ui::SettingsDialog* ui;
};


} // namespace quick_note

#endif // QUICK_NOTE_SETTINGS_DIALOG_H
