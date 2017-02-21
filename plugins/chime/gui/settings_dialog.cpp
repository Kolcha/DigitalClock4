/*
    Digital Clock: chime plugin
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

#include "core/chime_settings.h"
#include "core/repeat_type.h"

#include "gui/sound_dialog.h"

namespace chime {

SettingsDialog::SettingsDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  setAttribute(Qt::WA_DeleteOnClose);
  ui->setupUi(this);

  ui->hour_count_box->addItem(tr("Once"), static_cast<int>(Repeat::Once));
  ui->hour_count_box->addItem(tr("1-12 times"), static_cast<int>(Repeat::Dynamic));

  ui->quarter_count_box->addItem(tr("Once"), static_cast<int>(Repeat::Once));
  ui->quarter_count_box->addItem(tr("1-3 times"), static_cast<int>(Repeat::Dynamic));
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

void SettingsDialog::Init(const QSettings::SettingsMap& settings)
{
  this->blockSignals(true);
  ui->hour_enabled_cb->setChecked(settings[OPT_EVERY_HOUR_ENABLED].toBool());
  switch (static_cast<Repeat>(settings[OPT_EVERY_HOUR_REPEAT].toInt())) {
    case Repeat::Once:
      ui->hour_count_box->setCurrentIndex(0);
      break;

    case Repeat::Dynamic:
      ui->hour_count_box->setCurrentIndex(1);
      break;
  }
  ui->quarter_enabled_cb->setChecked(settings[OPT_QUARTER_HOUR_ENABLED].toBool());
  switch (static_cast<Repeat>(settings[OPT_QUARTER_HOUR_REPEAT].toInt())) {
    case Repeat::Once:
      ui->quarter_count_box->setCurrentIndex(0);
      break;

    case Repeat::Dynamic:
      ui->quarter_count_box->setCurrentIndex(1);
      break;
  }
  ui->q_time_start->setTime(settings[OPT_QUIET_HOURS_START].toTime());
  ui->q_time_end->setTime(settings[OPT_QUIET_HOURS_END].toTime());
  this->blockSignals(false);

  ui->quiet_hours_enabled->setChecked(settings[OPT_QUIET_HOURS_ENABLED].toBool());

  settings_ = settings;
}

void SettingsDialog::on_hour_enabled_cb_clicked(bool checked)
{
  emit OptionChanged(OPT_EVERY_HOUR_ENABLED, checked);
}

void SettingsDialog::on_hour_count_box_activated(int index)
{
  Q_ASSERT(index >= 0);
  emit OptionChanged(OPT_EVERY_HOUR_REPEAT, ui->hour_count_box->itemData(index));
}

void SettingsDialog::on_hour_sound_btn_clicked()
{
  SoundDialog dlg(this);
  dlg.setSound(settings_[OPT_EVERY_HOUR_SIGNAL].toUrl());
  dlg.setVolume(settings_[OPT_EVERY_HOUR_VOLUME].toInt());
  dlg.setLastFile(settings_[OPT_EVERY_HOUR_LAST_FILE].toString());
  if (dlg.exec() == QDialog::Rejected) return;
  Q_ASSERT(dlg.result() == QDialog::Accepted);
  emit OptionChanged(OPT_EVERY_HOUR_SIGNAL, dlg.sound());
  emit OptionChanged(OPT_EVERY_HOUR_VOLUME, dlg.volume());
  if (dlg.sound().scheme() == "file")
    emit OptionChanged(OPT_EVERY_HOUR_LAST_FILE, dlg.sound().toLocalFile());
}

void SettingsDialog::on_quarter_enabled_cb_clicked(bool checked)
{
  emit OptionChanged(OPT_QUARTER_HOUR_ENABLED, checked);
}

void SettingsDialog::on_quarter_count_box_activated(int index)
{
  Q_ASSERT(index >= 0);
  emit OptionChanged(OPT_QUARTER_HOUR_REPEAT, ui->quarter_count_box->itemData(index));
}

void SettingsDialog::on_quarter_sound_btn_clicked()
{
  SoundDialog dlg(this);
  dlg.setSound(settings_[OPT_QUARTER_HOUR_SIGNAL].toUrl());
  dlg.setVolume(settings_[OPT_QUARTER_HOUR_VOLUME].toInt());
  dlg.setLastFile(settings_[OPT_QUARTER_HOUR_LAST_FILE].toString());
  if (dlg.exec() == QDialog::Rejected) return;
  Q_ASSERT(dlg.result() == QDialog::Accepted);
  emit OptionChanged(OPT_QUARTER_HOUR_SIGNAL, dlg.sound());
  emit OptionChanged(OPT_QUARTER_HOUR_VOLUME, dlg.volume());
  if (dlg.sound().scheme() == "file")
    emit OptionChanged(OPT_QUARTER_HOUR_LAST_FILE, dlg.sound().toLocalFile());
}

void SettingsDialog::on_quiet_hours_enabled_clicked(bool checked)
{
  emit OptionChanged(OPT_QUIET_HOURS_ENABLED, checked);
}

void SettingsDialog::on_q_time_start_timeChanged(const QTime& time)
{
  emit OptionChanged(OPT_QUIET_HOURS_START, time);
}

void SettingsDialog::on_q_time_end_timeChanged(const QTime& time)
{
  emit OptionChanged(OPT_QUIET_HOURS_END, time);
}

} // namespace chime
