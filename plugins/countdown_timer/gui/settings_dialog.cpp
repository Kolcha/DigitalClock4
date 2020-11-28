/*
    Digital Clock: countdown timer plugin
    Copyright (C) 2017-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#include <QFileDialog>

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

void SettingsDialog::AddCommonWidget(QWidget* widget)
{
  widget->layout()->setMargin(12);
  ui->tabWidget->addTab(widget, tr("Appearance"));
}

void SettingsDialog::Init(const QSettings::SettingsMap& settings)
{
  ui->target_time_rbtn->setChecked(settings.value(OPT_USE_TARGET_TIME).toBool());
  ui->interval_rbtn->setChecked(!ui->target_time_rbtn->isChecked());
  ui->target_time_edit->setDateTime(settings.value(OPT_TARGET_DATETIME).toDateTime());
  ui->h_edit->setValue(settings.value(OPT_INTERVAL_HOURS).toInt());
  ui->m_edit->setValue(settings.value(OPT_INTERVAL_MINUTES).toInt());
  ui->s_edit->setValue(settings.value(OPT_INTERVAL_SECONDS).toInt());
  ui->min_days_edit->setValue(settings.value(OPT_HIDE_DAYS_THRESHOLD).toInt());
  ui->also_hide_hours->setChecked(settings.value(OPT_ALSO_HIDE_HOURS).toBool());
  ui->restart_on_dblclik->setChecked(settings.value(OPT_RESTART_ON_DBLCLIK).toBool());
  ui->restart_on_timeout->setChecked(settings.value(OPT_RESTART_ON_TIMEOUT).toBool());
  ui->hide_if_inactive->setChecked(settings.value(OPT_HIDE_INACTIVE).toBool());
  ui->reverse_counting->setChecked(settings.value(OPT_REVERSE_COUNTING).toBool());
  ui->chime_on_timeout->setChecked(settings.value(OPT_CHIME_ON_TIMEOUT).toBool());
  ui->show_msg->setChecked(settings.value(OPT_SHOW_MESSAGE).toBool());
  ui->msg_text_edit->setPlainText(settings.value(OPT_MESSAGE_TEXT).toString());
  ui->pause_seq_edit->setKeySequence(QKeySequence(settings.value(OPT_PAUSE_HOTKEY).toString()));
  ui->restart_seq_edit->setKeySequence(QKeySequence(settings.value(OPT_RESTART_HOTKEY).toString()));
  ui->settings_seq_edit->setKeySequence(QKeySequence(settings.value(OPT_SETTINGS_HOTKEY).toString()));
#ifndef HAVE_QHOTKEY
  ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->hotkeys_tab));
#endif
}

void SettingsDialog::on_target_time_rbtn_clicked()
{
  emit OptionChanged(OPT_USE_TARGET_TIME, true);
}

void SettingsDialog::on_interval_rbtn_clicked()
{
  emit OptionChanged(OPT_USE_TARGET_TIME, false);
}

void SettingsDialog::on_target_time_edit_dateTimeChanged(const QDateTime& date_time)
{
  emit OptionChanged(OPT_TARGET_DATETIME, date_time);
}

void SettingsDialog::on_h_edit_valueChanged(int arg1)
{
  emit OptionChanged(OPT_INTERVAL_HOURS, arg1);
}

void SettingsDialog::on_m_edit_valueChanged(int arg1)
{
  emit OptionChanged(OPT_INTERVAL_MINUTES, arg1);
}

void SettingsDialog::on_s_edit_valueChanged(int arg1)
{
  emit OptionChanged(OPT_INTERVAL_SECONDS, arg1);
}

void SettingsDialog::on_restart_on_dblclik_clicked(bool checked)
{
  emit OptionChanged(OPT_RESTART_ON_DBLCLIK, checked);
}

void SettingsDialog::on_restart_on_timeout_clicked(bool checked)
{
  emit OptionChanged(OPT_RESTART_ON_TIMEOUT, checked);
}

void SettingsDialog::on_hide_if_inactive_clicked(bool checked)
{
  emit OptionChanged(OPT_HIDE_INACTIVE, checked);
}

void SettingsDialog::on_reverse_counting_clicked(bool checked)
{
  emit OptionChanged(OPT_REVERSE_COUNTING, checked);
}

void SettingsDialog::on_chime_on_timeout_clicked(bool checked)
{
  emit OptionChanged(OPT_CHIME_ON_TIMEOUT, checked);
}

void SettingsDialog::on_browse_sound_btn_clicked()
{
  QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                   QDir::homePath(),
                                                   tr("Sounds (*.wav *.mp3 *.ogg *.oga *.m4a)"));
  if (!filename.isEmpty()) emit OptionChanged(OPT_CHIME_SOUND_FILE, filename);
}

void SettingsDialog::on_show_msg_clicked(bool checked)
{
  emit OptionChanged(OPT_SHOW_MESSAGE, checked);
}

void SettingsDialog::on_msg_text_edit_textChanged()
{
  emit OptionChanged(OPT_MESSAGE_TEXT, ui->msg_text_edit->toPlainText());
}

void SettingsDialog::on_min_days_edit_valueChanged(int arg1)
{
  ui->also_hide_hours->setEnabled(arg1 == -1);
  emit OptionChanged(OPT_HIDE_DAYS_THRESHOLD, arg1);
}

void SettingsDialog::on_also_hide_hours_clicked(bool checked)
{
  emit OptionChanged(OPT_ALSO_HIDE_HOURS, checked);
}

void SettingsDialog::on_pause_seq_edit_editingFinished()
{
  emit OptionChanged(OPT_PAUSE_HOTKEY, ui->pause_seq_edit->keySequence().toString());
}

void SettingsDialog::on_restart_seq_edit_editingFinished()
{
  emit OptionChanged(OPT_RESTART_HOTKEY, ui->restart_seq_edit->keySequence().toString());
}

void SettingsDialog::on_settings_seq_edit_editingFinished()
{
  emit OptionChanged(OPT_SETTINGS_HOTKEY, ui->settings_seq_edit->keySequence().toString());
}

} // namespace countdown_timer
