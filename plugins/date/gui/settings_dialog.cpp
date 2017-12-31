/*
    Digital Clock: date plugin
    Copyright (C) 2014-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "../date_settings.h"

namespace date {

SettingsDialog::SettingsDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  setAttribute(Qt::WA_DeleteOnClose);
  ui->setupUi(this);

  date_formats_[Qt::TextDate] = tr("default");
  date_formats_[Qt::ISODate] = QString("ISO 8601");
  date_formats_[Qt::DefaultLocaleShortDate] = tr("short");
  date_formats_[Qt::DefaultLocaleLongDate] = tr("long");
  ui->int_type_box->clear();
  for (auto iter = date_formats_.begin(); iter != date_formats_.end(); ++iter) {
    ui->int_type_box->addItem(iter.value(), iter.key());
  }
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

void SettingsDialog::Init(const QSettings::SettingsMap& settings)
{
  for (auto iter = settings.constBegin(); iter != settings.constEnd(); ++iter) {
    if (iter.key() == OPT_DATE_FORMAT_TYPE) {
      ui->int_type_button->setChecked((FormatType)(iter.value().toInt()) == FormatType::FT_INT);
      ui->str_type_button->setChecked((FormatType)(iter.value().toInt()) == FormatType::FT_STR);
    }
    if (iter.key() == OPT_DATE_FORMAT_INT) {
      ui->int_type_box->setCurrentText(date_formats_[(Qt::DateFormat)(iter.value().toInt())]);
    }
    if (iter.key() == OPT_DATE_FORMAT_STR) {
      ui->str_type_box->setCurrentText(iter.value().toString());
    }
  }
}

void SettingsDialog::on_int_type_button_toggled(bool checked)
{
  if (checked) emit OptionChanged(OPT_DATE_FORMAT_TYPE, (int)FormatType::FT_INT);
}

void SettingsDialog::on_str_type_button_toggled(bool checked)
{
  if (checked) emit OptionChanged(OPT_DATE_FORMAT_TYPE, (int)FormatType::FT_STR);
}

void SettingsDialog::on_int_type_box_currentIndexChanged(int index)
{
  emit OptionChanged(OPT_DATE_FORMAT_INT, ui->int_type_box->itemData(index));
}

void SettingsDialog::on_str_type_box_currentTextChanged(const QString& arg1)
{
  emit OptionChanged(OPT_DATE_FORMAT_STR, arg1);
}

} // namespace date
