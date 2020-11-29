/*
    Digital Clock: schedule plugin
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
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "advanced_settings_dialog.h"
#include "ui_advanced_settings_dialog.h"

#include <QSystemTrayIcon>
#include <QTimer>
#include <QFileDialog>

#include "message_box.h"

#include "core/notification.h"
#include "core/schedule_settings.h"

namespace schedule {

AdvancedSettingsDialog::AdvancedSettingsDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::AdvancedSettingsDialog)
{
  ui->setupUi(this);
}

AdvancedSettingsDialog::~AdvancedSettingsDialog()
{
  delete ui;
}

void AdvancedSettingsDialog::Init(const QSettings::SettingsMap& settings)
{
  Notification::Type t = settings.value(OPT_NOTIFICATION_TYPE).value<Notification::Type>();
  ui->msg_balloon_rbtn->setChecked(t == Notification::TrayMessage);
  ui->msg_dialog_rbtn->setChecked(t == Notification::MessageBox);
  ui->msg_not_show_rbtn->setChecked(t == Notification::None);
  ui->timeout_edit->setValue(settings.value(OPT_NOTIFICATION_TIME).toInt());
  ui->play_sound_enabled->setChecked(settings.value(OPT_PLAY_SOUND).toBool());
  ui->show_tray_icon->setChecked(settings.value(OPT_SHOW_TRAY_ICON).toBool());
  ui->hotkey_add_task_enabled->setChecked(settings.value(OPT_ADD_TASK_SHORTCUT_E).toBool());
  ui->add_task_seq_edit->setKeySequence(QKeySequence(settings.value(OPT_ADD_TASK_SHORTCUT).toString()));
  curr_sound_file_ = settings.value(OPT_SOUND_FILE).toString();
}

void AdvancedSettingsDialog::on_preview_btn_clicked()
{
  QString task_text = tr("This is sample task text. Just to test how notification looks.");
  int task_timeout = ui->timeout_edit->value();

  if (ui->msg_balloon_rbtn->isChecked()) {
    QSystemTrayIcon* tray_icon = new QSystemTrayIcon(QIcon(":/schedule/schedule.svg"), this);
    QTimer* timer = new QTimer(this);
    timer->setInterval(task_timeout * 1000 + 500);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, tray_icon, &QSystemTrayIcon::hide);
    connect(timer, &QTimer::timeout, tray_icon, &QSystemTrayIcon::deleteLater);
    connect(timer, &QTimer::timeout, timer, &QTimer::deleteLater);
    timer->start();
    tray_icon->show();
    tray_icon->showMessage(tr("Task preview"), task_text, QSystemTrayIcon::Information, task_timeout * 1000);
  }

  if (ui->msg_dialog_rbtn->isChecked()) {
    if (task_timeout > 0) {
      TMessageBox dlg(QMessageBox::Information, tr("Task preview"), task_text, QMessageBox::Ok);
      dlg.setTimeout(task_timeout);
      dlg.setAutoClose(true);
      dlg.setDefaultButton(QMessageBox::Ok);
      dlg.exec();
    } else {
      QMessageBox::information(nullptr, tr("Task preview"), task_text, QMessageBox::Ok);
    }
  }
}

void AdvancedSettingsDialog::on_msg_balloon_rbtn_clicked()
{
  emit OptionChanged(OPT_NOTIFICATION_TYPE, Notification::TrayMessage);
}

void AdvancedSettingsDialog::on_msg_dialog_rbtn_clicked()
{
  emit OptionChanged(OPT_NOTIFICATION_TYPE, Notification::MessageBox);
}

void AdvancedSettingsDialog::on_msg_not_show_rbtn_clicked()
{
  emit OptionChanged(OPT_NOTIFICATION_TYPE, Notification::None);
}

void AdvancedSettingsDialog::on_timeout_edit_valueChanged(int arg1)
{
  emit OptionChanged(OPT_NOTIFICATION_TIME, arg1);
}

void AdvancedSettingsDialog::on_play_sound_enabled_clicked(bool checked)
{
  emit OptionChanged(OPT_PLAY_SOUND, checked);
}

void AdvancedSettingsDialog::on_browse_btn_clicked()
{
  QString last_path = curr_sound_file_.isEmpty() ? QDir::homePath() : QFileInfo(curr_sound_file_).path();
  QString sound_file = QFileDialog::getOpenFileName(this, tr("Select sound"), last_path,
                                                    tr("Sounds (*.wav *.mp3 *.ogg *.oga *.m4a);;All files (*.*)"));
  if (sound_file.isEmpty()) return;

  curr_sound_file_ = sound_file;
  emit OptionChanged(OPT_SOUND_FILE, sound_file);
}

void AdvancedSettingsDialog::on_show_tray_icon_clicked(bool checked)
{
  emit OptionChanged(OPT_SHOW_TRAY_ICON, checked);
}

void AdvancedSettingsDialog::on_hotkey_add_task_enabled_clicked(bool checked)
{
  emit OptionChanged(OPT_ADD_TASK_SHORTCUT_E, checked);
}

void AdvancedSettingsDialog::on_add_task_seq_edit_editingFinished()
{
  emit OptionChanged(OPT_ADD_TASK_SHORTCUT, ui->add_task_seq_edit->keySequence().toString());
}

} // namespace schedule
