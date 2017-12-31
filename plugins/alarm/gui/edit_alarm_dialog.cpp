/*
    Digital Clock: alarm plugin
    Copyright (C) 2017-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "edit_alarm_dialog.h"
#include "ui_edit_alarm_dialog.h"

#include <algorithm>
#include <vector>

#include <QDate>
#include <QLocale>
#include <QFileDialog>

#include "core/alarm_item.h"

namespace alarm_plugin {

EditAlarmDialog::EditAlarmDialog(AlarmItem* alarm_item, QWidget* parent) :
  QDialog(parent),
  ui(new Ui::EditAlarmDialog),
  last_media_path_(QDir::homePath()),
  alarm_item_(alarm_item),
  orig_item_(new AlarmItem(this))
{
  ui->setupUi(this);

  orig_item_->setEnabled(alarm_item->isEnabled());
  orig_item_->setTime(alarm_item->time());
  orig_item_->setDays(alarm_item->days());
  orig_item_->setMedia(alarm_item->media());
  orig_item_->setVolume(alarm_item->volume());

  player_ = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
  connect(player_, &QMediaPlayer::stateChanged, this, &EditAlarmDialog::onPlayerStateChanged);
  connect(ui->preview_btn, &QToolButton::clicked, player_, &QMediaPlayer::play);
  connect(ui->volume_slider, &QSlider::valueChanged, player_, &QMediaPlayer::setVolume);

  ui->time_edit->setTime(alarm_item->time());
  ui->is_enabled_checkbox->setChecked(alarm_item->isEnabled());

  std::vector<Qt::DayOfWeek> days(7);
  days[0] = Qt::Monday;
  days[1] = Qt::Tuesday;
  days[2] = Qt::Wednesday;
  days[3] = Qt::Thursday;
  days[4] = Qt::Friday;
  days[5] = Qt::Saturday;
  days[6] = Qt::Sunday;

  std::vector<QCheckBox*> checkboxes(7);
  checkboxes[0] = ui->day_1;
  checkboxes[1] = ui->day_2;
  checkboxes[2] = ui->day_3;
  checkboxes[3] = ui->day_4;
  checkboxes[4] = ui->day_5;
  checkboxes[5] = ui->day_6;
  checkboxes[6] = ui->day_7;

  auto day_iter = std::find(days.begin(), days.end(), QLocale::system().firstDayOfWeek());
  QDate d(2017, 2, 7);
  d = d.addDays(std::distance(days.begin(), day_iter) - 1);
  Q_ASSERT(day_iter != days.end());
  for (auto& checkbox : checkboxes) {
    checkbox->setText(d.toString("ddd"));
    checkbox->setToolTip(d.toString("dddd"));
    checkbox->setChecked(alarm_item->days().contains(*day_iter));
    days_boxes_[checkbox] = *day_iter;
    connect(checkbox, &QCheckBox::toggled, this, &EditAlarmDialog::onAnyDayCheckboxToggled);
    d = d.addDays(1);
    ++day_iter;
    if (day_iter == days.end()) day_iter = days.begin();
  }

  ui->filepath_edit->setText(alarm_item->media().toString());
  ui->volume_slider->setValue(alarm_item->volume());

  player_->setMedia(alarm_item->media());
  player_->setVolume(alarm_item->volume());

  connect(ui->time_edit, &QTimeEdit::timeChanged, alarm_item, &AlarmItem::setTime);
  connect(ui->is_enabled_checkbox, &QCheckBox::toggled, alarm_item, &AlarmItem::setEnabled);
  connect(ui->volume_slider, &QSlider::valueChanged, alarm_item, &AlarmItem::setVolume);
}

EditAlarmDialog::~EditAlarmDialog()
{
  delete ui;
}

void EditAlarmDialog::reject()
{
  alarm_item_->setEnabled(orig_item_->isEnabled());
  alarm_item_->setTime(orig_item_->time());
  alarm_item_->setDays(orig_item_->days());
  alarm_item_->setMedia(orig_item_->media());
  alarm_item_->setVolume(orig_item_->volume());
  QDialog::reject();
}

void EditAlarmDialog::setLastMediaPath(const QString& last_path)
{
  if (QFile::exists(last_path)) last_media_path_ = last_path;
}

void EditAlarmDialog::onAnyDayCheckboxToggled(bool checked)
{
  auto d_iter = days_boxes_.find(qobject_cast<QCheckBox*>(sender()));
  Q_ASSERT(d_iter != days_boxes_.end());
  QSet<Qt::DayOfWeek> days = alarm_item_->days();
  if (checked) {
    days.insert(d_iter.value());
  } else {
    days.remove(d_iter.value());
  }
  alarm_item_->setDays(days);
}

void EditAlarmDialog::onPlayerStateChanged(QMediaPlayer::State new_state)
{
  if (new_state == QMediaPlayer::PlayingState) {
    disconnect(ui->preview_btn, &QToolButton::clicked, player_, &QMediaPlayer::play);
    connect(ui->preview_btn, &QToolButton::clicked, player_, &QMediaPlayer::stop);
    ui->preview_btn->setIcon(QIcon(":/alarm/stop.svg.p"));
  } else {
    disconnect(ui->preview_btn, &QToolButton::clicked, player_, &QMediaPlayer::stop);
    connect(ui->preview_btn, &QToolButton::clicked, player_, &QMediaPlayer::play);
    ui->preview_btn->setIcon(QIcon(":/alarm/play.svg.p"));
  }
}

void EditAlarmDialog::on_filepath_edit_textChanged(const QString& arg1)
{
  alarm_item_->setMedia(QUrl(arg1));
}

void EditAlarmDialog::on_browse_btn_clicked()
{
  // *INDENT-OFF*
  QUrl new_file = QFileDialog::getOpenFileUrl(this, tr("Select sound"),
                                              QUrl::fromLocalFile(last_media_path_),
                                              tr("Sounds (*.wav *.mp3 *.ogg *.oga *.m4a);;All files (*.*)"));
  // *INDENT-ON*
  if (!new_file.isValid()) return;
  ui->filepath_edit->setText(new_file.toString());
  ui->filepath_edit->setToolTip(new_file.toString());
  player_->setMedia(new_file);
  if (new_file.isLocalFile()) setLastMediaPath(QFileInfo(new_file.toLocalFile()).absolutePath());
}

} // namespace alarm_plugin
