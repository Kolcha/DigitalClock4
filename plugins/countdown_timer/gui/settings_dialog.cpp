/*
    Digital Clock: date plugin
    Copyright (C) 2014-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "core/settings.h"

namespace countdown_timer {

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
  for (auto iter = settings.constBegin(); iter != settings.constEnd(); ++iter) {
//    if (iter.key() == OPT_DATE_FORMAT_TYPE) {
//      ui->int_type_button->setChecked((FormatType)(iter.value().toInt()) == FormatType::FT_INT);
//      ui->str_type_button->setChecked((FormatType)(iter.value().toInt()) == FormatType::FT_STR);
//    }
//    if (iter.key() == OPT_DATE_FORMAT_INT) {
//      ui->int_type_box->setCurrentText(date_formats_[(Qt::DateFormat)(iter.value().toInt())]);
//    }
//    if (iter.key() == OPT_DATE_FORMAT_STR) {
//      ui->str_type_box->setCurrentText(iter.value().toString());
//    }
  }
}

} // namespace countdown_timer
