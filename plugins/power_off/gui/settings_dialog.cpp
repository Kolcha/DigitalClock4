/*
    Digital Clock: power off plugin
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

#include "settings_keys.h"
#include "../power_off_settings.h"
#include "settings_dialog.h"
#include "ui_settings_dialog.h"

namespace power_off {

SettingsDialog::SettingsDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  ui->setupUi(this);
  ui->time_edit->setDisplayFormat("hh:mm");
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

void SettingsDialog::SettingsListener(const QString& key, const QVariant& value)
{
  if (key == OPT_TIME) ui->time_edit->setTime(value.value<QTime>());
  if (key == OPT_FORCE) ui->force_off->setChecked(value.toBool());
}

void SettingsDialog::on_time_edit_timeChanged(const QTime& time)
{
  emit OptionChanged(OPT_TIME, time);
}

void SettingsDialog::on_force_off_toggled(bool checked)
{
  emit OptionChanged(OPT_FORCE, checked);
}

} // namespace power_off
