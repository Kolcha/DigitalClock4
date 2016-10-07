/*
    Digital Clock: move clock plugin
    Copyright (C) 2014-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#include <QDesktopWidget>
#include "settings_dialog.h"
#include "../move_clock_settings.h"
#include "ui_settings_dialog.h"

namespace move_clock {

SettingsDialog::SettingsDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  setAttribute(Qt::WA_DeleteOnClose);
  ui->setupUi(this);

  QDesktopWidget desktop;
  int sw = desktop.screen()->width();
  int sh = desktop.screen()->height();
  ui->x0_value->setMaximum(sw);
  ui->ax_value->setMinimum(-sw);
  ui->ax_value->setMaximum(sw);
  ui->y0_value->setMaximum(sh);
  ui->ay_value->setMinimum(-sh);
  ui->ay_value->setMaximum(sh);
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

void SettingsDialog::SettingsListener(const QString& key, const QVariant& value)
{
  if (key == OPT_X0) ui->x0_value->setValue(value.toInt());
  if (key == OPT_KX) ui->kx_value->setValue(value.toReal());
  if (key == OPT_AX) ui->ax_value->setValue(value.toInt());
  if (key == OPT_NX) ui->nx_value->setValue(value.toReal());
  if (key == OPT_Y0) ui->y0_value->setValue(value.toInt());
  if (key == OPT_KY) ui->ky_value->setValue(value.toReal());
  if (key == OPT_AY) ui->ay_value->setValue(value.toInt());
  if (key == OPT_NY) ui->ny_value->setValue(value.toReal());
  if (key == OPT_TIMEOUT) ui->speed_slider->setValue(value.toInt());
}

} // namespace move_clock

void move_clock::SettingsDialog::on_x0_value_valueChanged(int arg1)
{
  emit OptionChanged(OPT_X0, arg1);
}

void move_clock::SettingsDialog::on_kx_value_valueChanged(double arg1)
{
  emit OptionChanged(OPT_KX, arg1);
}

void move_clock::SettingsDialog::on_ax_value_valueChanged(int arg1)
{
  emit OptionChanged(OPT_AX, arg1);
}

void move_clock::SettingsDialog::on_nx_value_valueChanged(double arg1)
{
  emit OptionChanged(OPT_NX, arg1);
}

void move_clock::SettingsDialog::on_y0_value_valueChanged(int arg1)
{
  emit OptionChanged(OPT_Y0, arg1);
}

void move_clock::SettingsDialog::on_ky_value_valueChanged(double arg1)
{
  emit OptionChanged(OPT_KY, arg1);
}

void move_clock::SettingsDialog::on_ay_value_valueChanged(int arg1)
{
  emit OptionChanged(OPT_AY, arg1);
}

void move_clock::SettingsDialog::on_ny_value_valueChanged(double arg1)
{
  emit OptionChanged(OPT_NY, arg1);
}

void move_clock::SettingsDialog::on_speed_slider_valueChanged(int value)
{
  emit OptionChanged(OPT_TIMEOUT, value);
}
