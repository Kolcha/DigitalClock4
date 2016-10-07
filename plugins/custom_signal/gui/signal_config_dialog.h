/*
    Digital Clock: custom signal plugin
    Copyright (C) 2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef CUSTOM_SIGNAL_SIGNAL_CONFIG_DIALOG_H
#define CUSTOM_SIGNAL_SIGNAL_CONFIG_DIALOG_H

#include <QDialog>

#include <QUrl>
#include <QMediaPlayer>

namespace custom_signal {

namespace Ui {
class SignalConfigDialog;
}

class SignalConfigDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SignalConfigDialog(QWidget* parent = 0);
  ~SignalConfigDialog();

  QUrl sound() const;
  int volume() const;

public slots:
  void setSound(const QUrl& last_sound);
  void setVolume(const int last_volume);

  void setLastFile(const QString& file);

private slots:
  void on_default_selected_toggled(bool checked);
  void on_defaults_box_currentIndexChanged(int index);

  void on_custom_selected_toggled(bool checked);
  void on_custom_browse_btn_clicked();

  void on_volume_slider_valueChanged(int value);

  void onPlayerStateChanged(QMediaPlayer::State new_state);

private:
  Ui::SignalConfigDialog* ui;

  QString last_cust_file_;

  QMediaPlayer* player_;
};

} // namespace custom_signal

#endif // CUSTOM_SIGNAL_SIGNAL_CONFIG_DIALOG_H
