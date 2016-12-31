/*
    Digital Clock: talking clock plugin
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

#ifndef TALKING_CLOCK_VOICE_CONFIG_DIALOG_H
#define TALKING_CLOCK_VOICE_CONFIG_DIALOG_H

#include <QDialog>

#include <QVector>
#include <QVoice>
#include <QTextToSpeech>

namespace talking_clock {

namespace Ui {
class VoiceConfigDialog;
}

class VoiceConfigDialog : public QDialog
{
  Q_OBJECT

public:
  explicit VoiceConfigDialog(QWidget* parent = 0);
  ~VoiceConfigDialog();

public slots:
  void speak();
  void stop();

  void setRate(int rate);
  void setPitch(int pitch);
  void setVolume(int volume);

  void stateChanged(QTextToSpeech::State state);
  void engineSelected(int index);
  void languageSelected(int language);
  void voiceSelected(int index);

  void localeChanged(const QLocale& locale);

private:
  Ui::VoiceConfigDialog* ui;
  QTextToSpeech* m_speech;
  QVector<QVoice> m_voices;
};

} // namespace talking_clock

#endif // TALKING_CLOCK_VOICE_CONFIG_DIALOG_H
