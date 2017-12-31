/*
    Digital Clock: chime plugin
    Copyright (C) 2016-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef CHIME_SOUND_DIALOG_H
#define CHIME_SOUND_DIALOG_H

#include <QDialog>

#include <QUrl>
#include <QMediaPlayer>

namespace chime {

namespace Ui {
class SoundDialog;
}

class SoundDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SoundDialog(QWidget* parent = nullptr);
  ~SoundDialog();

  QUrl sound() const;
  int volume() const;

public slots:
  void setSound(const QUrl& last_sound);
  void setVolume(const int last_volume);

  void setLastFile(const QString& file);

private slots:
  void onPlayerStateChanged(QMediaPlayer::State new_state);
  void onPlayerMediaChanged(const QMediaContent& newmedia);

  void on_default_snd_rb_clicked(bool checked);
  void on_defaults_box_activated(int index);
  void on_custom_snd_rb_clicked(bool checked);
  void on_custom_browse_btn_clicked();

private:
  Ui::SoundDialog* ui;

  QString last_cust_file_;

  QMediaPlayer* player_;
};

} // namespace chime

#endif // CHIME_SOUND_DIALOG_H
