/*
    Digital Clock: talking clock plugin
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

#include <QTime>

#include "talking_clock_settings.h"
#include "voice_config_dialog.h"

namespace talking_clock {

SettingsDialog::SettingsDialog(const QSettings::SettingsMap& settings, QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog),
  settings_(settings)
{
  ui->setupUi(this);
  ui->every_hour->setChecked(settings.value(OPT_EVERY_HOUR_ENABLED).toBool());
  ui->hour_format_edit->setText(settings.value(OPT_EVERY_HOUR_FORMAT).toString());
  ui->hour_format_edit->setToolTip(QTime::currentTime().toString(ui->hour_format_edit->text()));
  ui->quarter_hour->setChecked(settings.value(OPT_QUARTER_HOUR_ENABLED).toBool());
  ui->quarter_format_edit->setText(settings.value(OPT_QUARTER_HOUR_FORMAT).toString());
  ui->quarter_format_edit->setToolTip(QTime::currentTime().toString(ui->quarter_format_edit->text()));
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

void SettingsDialog::on_voice_config_btn_clicked()
{
  VoiceConfigDialog dlg(settings_, this);
  if (dlg.exec() == QDialog::Accepted) {
    settings_[OPT_SYNTHESIS_VOLUME] = dlg.volume();
    emit OptionChanged(OPT_SYNTHESIS_VOLUME, dlg.volume());
    settings_[OPT_SYNTHESIS_RATE] = dlg.rate();
    emit OptionChanged(OPT_SYNTHESIS_RATE, dlg.rate());
    settings_[OPT_SYNTHESIS_PITCH] = dlg.pitch();
    emit OptionChanged(OPT_SYNTHESIS_PITCH, dlg.pitch());
    settings_[OPT_SYNTHESIS_ENGINE] = dlg.engine();
    emit OptionChanged(OPT_SYNTHESIS_ENGINE, dlg.engine());
    settings_[OPT_SYNTHESIS_LANGUAGE] = dlg.language();
    emit OptionChanged(OPT_SYNTHESIS_LANGUAGE, dlg.language());
    settings_[OPT_SYNTHESIS_VOICE] = dlg.voice();
    emit OptionChanged(OPT_SYNTHESIS_VOICE, dlg.voice());
  }
}

void SettingsDialog::on_every_hour_clicked(bool checked)
{
  emit OptionChanged(OPT_EVERY_HOUR_ENABLED, checked);
}

void SettingsDialog::on_quarter_hour_clicked(bool checked)
{
  emit OptionChanged(OPT_QUARTER_HOUR_ENABLED, checked);
}

void SettingsDialog::on_hour_format_edit_textEdited(const QString& arg1)
{
  ui->hour_format_edit->setToolTip(QTime::currentTime().toString(arg1));
  emit OptionChanged(OPT_EVERY_HOUR_FORMAT, arg1);
}

void SettingsDialog::on_quarter_format_edit_textEdited(const QString& arg1)
{
  ui->quarter_format_edit->setToolTip(QTime::currentTime().toString(arg1));
  emit OptionChanged(OPT_QUARTER_HOUR_FORMAT, arg1);
}

} // namespace talking_clock
