/*
    Digital Clock: chime plugin
    Copyright (C) 2016-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "sound_dialog.h"
#include "ui_sound_dialog.h"

#include <QDir>
#include <QFileInfo>
#include <QFileDialog>

namespace chime {

SoundDialog::SoundDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SoundDialog)
{
  ui->setupUi(this);

  ui->defaults_box->blockSignals(true);
  ui->defaults_box->addItem(tr("sound 1"), QUrl("qrc:/chime/hour_signal.wav"));
  ui->defaults_box->addItem(tr("sound 2"), QUrl("qrc:/chime/quarter_signal.wav"));
  ui->defaults_box->addItem(tr("sound 3"), QUrl("qrc:/chime/big_ben_strikes_1.mp3"));
  ui->defaults_box->addItem(tr("sound 4"), QUrl("qrc:/chime/quarter_hour_chime_beg.mp3"));
  ui->defaults_box->blockSignals(false);

  player_ = new QMediaPlayer(this);
  connect(player_, &QMediaPlayer::stateChanged, this, &SoundDialog::onPlayerStateChanged);
  connect(player_, &QMediaPlayer::currentMediaChanged, this, &SoundDialog::onPlayerMediaChanged);
  connect(ui->preview_btn, &QToolButton::clicked, player_, &QMediaPlayer::play);
  connect(ui->volume_slider, &QSlider::valueChanged, player_, &QMediaPlayer::setVolume);
}

SoundDialog::~SoundDialog()
{
  player_->stop();
  player_->setMedia(QMediaContent());
  delete ui;
}

QUrl SoundDialog::sound() const
{
  return player_->currentMedia().canonicalUrl();
}

int SoundDialog::volume() const
{
  return player_->volume();
}

void SoundDialog::setSound(const QUrl& last_sound)
{
  ui->default_snd_rb->setChecked(last_sound.scheme() == "qrc");
  if (ui->default_snd_rb->isChecked()) {
    for (int i = 0; i < ui->defaults_box->count(); ++i) {
      if (ui->defaults_box->itemData(i, Qt::UserRole).toUrl() == last_sound) {
        ui->defaults_box->setCurrentIndex(i);
        break;
      }
    }
  }
  ui->custom_snd_rb->setChecked(last_sound.scheme() == "file");
  player_->setMedia(last_sound);
}

void SoundDialog::setVolume(const int last_volume)
{
  ui->volume_slider->setValue(last_volume);
  player_->setVolume(last_volume);
}

void SoundDialog::setLastFile(const QString& file)
{
  last_cust_file_ = file;
}

void SoundDialog::onPlayerStateChanged(QMediaPlayer::State new_state)
{
  if (new_state == QMediaPlayer::PlayingState) {
    disconnect(ui->preview_btn, SIGNAL(clicked()), player_, SLOT(play()));
    connect(ui->preview_btn, SIGNAL(clicked()), player_, SLOT(stop()));
    ui->preview_btn->setIcon(QIcon(":/chime/stop.svg.p"));
  } else {
    disconnect(ui->preview_btn, SIGNAL(clicked()), player_, SLOT(stop()));
    connect(ui->preview_btn, SIGNAL(clicked()), player_, SLOT(play()));
    ui->preview_btn->setIcon(QIcon(":/chime/play.svg.p"));
  }
}

void SoundDialog::onPlayerMediaChanged(const QMediaContent& newmedia)
{
  ui->current_file_value->setText(newmedia.canonicalUrl().fileName());
}

void SoundDialog::on_default_snd_rb_clicked(bool checked)
{
  if (checked) player_->setMedia(ui->defaults_box->currentData(Qt::UserRole).toUrl());
}

void SoundDialog::on_defaults_box_activated(int index)
{
  player_->setMedia(ui->defaults_box->itemData(index, Qt::UserRole).toUrl());
}

void SoundDialog::on_custom_snd_rb_clicked(bool checked)
{
  if (checked) player_->setMedia(last_cust_file_.isEmpty() ? QMediaContent() : QUrl::fromLocalFile(last_cust_file_));
}

void SoundDialog::on_custom_browse_btn_clicked()
{
  QString last_path = QDir::homePath();
  if (!last_cust_file_.isEmpty()) {
    QFileInfo last_cust_info(last_cust_file_);
    last_path = last_cust_info.absolutePath();
  }
  // *INDENT-OFF*
  QString new_sound = QFileDialog::getOpenFileName(this, tr("Select sound"),
                                                   last_path,
                                                   tr("Sounds (*.wav *.mp3 *.ogg *.oga *.m4a);;All files (*.*)"));
  // *INDENT-ON*
  if (new_sound.isEmpty()) return;
  last_cust_file_ = new_sound;
  player_->setMedia(QUrl::fromLocalFile(new_sound));
}

} // namespace chime
