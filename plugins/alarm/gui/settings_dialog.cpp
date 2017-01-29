/*
    Digital Clock: alarm plugin
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include <QFileInfo>
#include <QDir>
#include <QFileDialog>

#include "../alarm_settings.h"

namespace alarm_plugin {

SettingsDialog::SettingsDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  setAttribute(Qt::WA_DeleteOnClose);
  ui->setupUi(this);
  ui->time_edit->setDisplayFormat("hh:mm");

  player_ = new QMediaPlayer(this);
  connect(player_, SIGNAL(stateChanged(QMediaPlayer::State)),
          this, SLOT(PlayerStateChanged(QMediaPlayer::State)));
  connect(ui->volume_slider, SIGNAL(valueChanged(int)), player_, SLOT(setVolume(int)));
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

void SettingsDialog::SettingsListener(const QString& key, const QVariant& value)
{
  if (key == OPT_ENABLED) {
    ui->alarm_enabled->setChecked(value.toBool());
  }
  if (key == OPT_TIME) {
    ui->time_edit->setTime(value.value<QTime>());
  }
  if (key == OPT_SIGNAL_TYPE) {
    SignalType st = (SignalType)value.toInt();
    ui->st_file->setChecked(st == ST_FILE);
    ui->st_stream->setChecked(st == ST_STREAM);
  }
  if (key == OPT_FILENAME) {
    QString file = value.toString();
    QFileInfo info(file);
    last_file_path_ = file.isEmpty() ? "." : info.absolutePath();
    if (info.exists()) {
      ui->signal_label->setText(info.baseName());
      ui->signal_label->setToolTip(QDir::toNativeSeparators(file));
      if (ui->st_file->isChecked()) player_->setMedia(QUrl::fromLocalFile(file));
    }
  }
  if (key == OPT_STREAM_URL) {
    QString stream = value.toString();
    ui->stream_url_edit->setText(stream);
    ui->stream_url_edit->setToolTip(stream);
    if (ui->st_stream->isChecked()) player_->setMedia(QUrl(stream));
  }
  if (key == OPT_VOLUME) {
    int volume = value.toInt();
    ui->volume_slider->setValue(volume);
    player_->setVolume(volume);
  }
  if (key == OPT_SHOW_NOTIFY) {
    ui->notification_enabled->setChecked(value.toBool());
  }
  if (key == OPT_NOTIFY_TEXT) {
    ui->message_edit->setPlainText(value.toString());
  }
}

void SettingsDialog::PlayerStateChanged(QMediaPlayer::State state)
{
  switch (state) {
    case QMediaPlayer::PlayingState:
      ui->play_btn->setIcon(QIcon(":/alarm/stop.svg"));
      break;

    case QMediaPlayer::PausedState:
    case QMediaPlayer::StoppedState:
      ui->play_btn->setIcon(QIcon(":/alarm/play.svg"));
      break;
  }
}

void SettingsDialog::on_time_edit_timeChanged(const QTime& time)
{
  emit OptionChanged(OPT_TIME, time);
}

void SettingsDialog::on_alarm_enabled_toggled(bool checked)
{
  emit OptionChanged(OPT_ENABLED, checked);
}

void SettingsDialog::on_browse_btn_clicked()
{
  QString sound_file = QFileDialog::getOpenFileName(this, tr("Select sound file"),
                       last_file_path_,
                       tr("MP3 Files (*.mp3)"));
  if (!sound_file.isEmpty()) {
    ui->signal_label->setText(QFileInfo(sound_file).baseName());
    ui->signal_label->setToolTip(QDir::toNativeSeparators(sound_file));
    emit OptionChanged(OPT_FILENAME, sound_file);
    last_file_path_ = QFileInfo(sound_file).absolutePath();
    player_->setMedia(QUrl::fromLocalFile(sound_file));
  }
}

void SettingsDialog::on_notification_enabled_toggled(bool checked)
{
  emit OptionChanged(OPT_SHOW_NOTIFY, checked);
}

void SettingsDialog::on_message_edit_textChanged()
{
  emit OptionChanged(OPT_NOTIFY_TEXT, ui->message_edit->toPlainText());
}

void SettingsDialog::on_st_file_clicked()
{
  emit OptionChanged(OPT_SIGNAL_TYPE, (int)ST_FILE);
  player_->setMedia(QUrl::fromLocalFile(
                      QDir::fromNativeSeparators(ui->signal_label->toolTip())));
}

void SettingsDialog::on_st_stream_clicked()
{
  emit OptionChanged(OPT_SIGNAL_TYPE, (int)ST_STREAM);
  player_->setMedia(QUrl(ui->stream_url_edit->text()));
}

void SettingsDialog::on_stream_url_edit_textEdited(const QString& arg1)
{
  emit OptionChanged(OPT_STREAM_URL, arg1);
  player_->setMedia(QUrl(arg1));
}

void SettingsDialog::on_volume_slider_valueChanged(int value)
{
  emit OptionChanged(OPT_VOLUME, value);
}

void SettingsDialog::on_play_btn_clicked()
{
  if (player_->state() == QMediaPlayer::PlayingState) {
    player_->stop();
  } else {
    player_->play();
  }
}

} // namespace alarm_plugin
