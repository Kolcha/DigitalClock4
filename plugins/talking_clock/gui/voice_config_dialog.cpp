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

#include "voice_config_dialog.h"
#include "ui_voice_config_dialog.h"

namespace talking_clock {

VoiceConfigDialog::VoiceConfigDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::VoiceConfigDialog),
  m_speech(nullptr)
{
  ui->setupUi(this);

  // Populate engine selection list
  ui->engine->addItem("Default", QString("default"));
  foreach (QString engine, QTextToSpeech::availableEngines())
    ui->engine->addItem(engine, engine);
  ui->engine->setCurrentIndex(0);
  engineSelected(0);

  connect(ui->speakButton, &QPushButton::clicked, this, &VoiceConfigDialog::speak);
  connect(ui->pitch, &QSlider::valueChanged, this, &VoiceConfigDialog::setPitch);
  connect(ui->rate, &QSlider::valueChanged, this, &VoiceConfigDialog::setRate);
  connect(ui->volume, &QSlider::valueChanged, this, &VoiceConfigDialog::setVolume);
  connect(ui->engine, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &VoiceConfigDialog::engineSelected);
}

VoiceConfigDialog::~VoiceConfigDialog()
{
  delete ui;
}

void VoiceConfigDialog::speak()
{
  m_speech->say(ui->plainTextEdit->toPlainText());
}
void VoiceConfigDialog::stop()
{
  m_speech->stop();
}

void VoiceConfigDialog::setRate(int rate)
{
  m_speech->setRate(rate / 10.0);
}

void VoiceConfigDialog::setPitch(int pitch)
{
  m_speech->setPitch(pitch / 10.0);
}

void VoiceConfigDialog::setVolume(int volume)
{
  m_speech->setVolume(volume / 100.0);
}

void VoiceConfigDialog::stateChanged(QTextToSpeech::State state)
{
  ui->pauseButton->setEnabled(state == QTextToSpeech::Speaking);
  ui->resumeButton->setEnabled(state == QTextToSpeech::Paused);
  ui->stopButton->setEnabled(state == QTextToSpeech::Speaking || QTextToSpeech::Paused);
}

void VoiceConfigDialog::engineSelected(int index)
{
  QString engineName = ui->engine->itemData(index).toString();
  delete m_speech;
  if (engineName == "default")
    m_speech = new QTextToSpeech(this);
  else
    m_speech = new QTextToSpeech(engineName, this);
  disconnect(ui->language, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &VoiceConfigDialog::languageSelected);
  ui->language->clear();
  // Populate the languages combobox before connecting its signal.
  QVector<QLocale> locales = m_speech->availableLocales();
  QLocale current = m_speech->locale();
  foreach (const QLocale& locale, locales) {
    QString name(QString("%1 (%2)")
                 .arg(QLocale::languageToString(locale.language()))
                 .arg(QLocale::countryToString(locale.country())));
    QVariant localeVariant(locale);
    ui->language->addItem(name, localeVariant);
    if (locale.name() == current.name())
      current = locale;
  }
  setRate(ui->rate->value());
  setPitch(ui->pitch->value());
  setVolume(ui->volume->value());
  connect(ui->stopButton, &QPushButton::clicked, m_speech, &QTextToSpeech::stop);
  connect(ui->pauseButton, &QPushButton::clicked, m_speech, &QTextToSpeech::pause);
  connect(ui->resumeButton, &QPushButton::clicked, m_speech, &QTextToSpeech::resume);

  connect(m_speech, &QTextToSpeech::stateChanged, this, &VoiceConfigDialog::stateChanged);
  connect(m_speech, &QTextToSpeech::localeChanged, this, &VoiceConfigDialog::localeChanged);

  connect(ui->language, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &VoiceConfigDialog::languageSelected);
  localeChanged(current);
}

void VoiceConfigDialog::languageSelected(int language)
{
  QLocale locale = ui->language->itemData(language).toLocale();
  m_speech->setLocale(locale);
}

void VoiceConfigDialog::voiceSelected(int index)
{
  m_speech->setVoice(m_voices.at(index));
}

void VoiceConfigDialog::localeChanged(const QLocale& locale)
{
  QVariant localeVariant(locale);
  ui->language->setCurrentIndex(ui->language->findData(localeVariant));

  disconnect(ui->voice, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &VoiceConfigDialog::voiceSelected);
  ui->voice->clear();

  m_voices = m_speech->availableVoices();
  QVoice currentVoice = m_speech->voice();
  foreach (const QVoice& voice, m_voices) {
    ui->voice->addItem(QString("%1 - %2 - %3").arg(voice.name())
                       .arg(QVoice::genderName(voice.gender()))
                       .arg(QVoice::ageName(voice.age())));
    if (voice.name() == currentVoice.name())
      ui->voice->setCurrentIndex(ui->voice->count() - 1);
  }
  connect(ui->voice, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &VoiceConfigDialog::voiceSelected);
}

} // namespace talking_clock
