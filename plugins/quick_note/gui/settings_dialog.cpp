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

#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#include "quick_note_settings.h"

namespace quick_note {

SettingsDialog::SettingsDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  setAttribute(Qt::WA_DeleteOnClose);
  ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

void SettingsDialog::Init(const QSettings::SettingsMap& settings)
{
  this->blockSignals(true);
  for (auto iter = settings.constBegin(); iter != settings.constEnd(); ++iter) {
    if (iter.key() == OPT_QUICK_NOTE_MSG) {
      ui->note_edit->setText(iter.value().toString());
    }
  }
  this->blockSignals(false);
}

void SettingsDialog::on_note_edit_textEdited(const QString& arg1)
{
  emit OptionChanged(OPT_QUICK_NOTE_MSG, arg1);
}

} // namespace quick_note
