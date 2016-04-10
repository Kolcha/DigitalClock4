#include "tower_clock_signal.h"

#include <QSound>
#include <QTime>


namespace tower_clock_signal {

TowerCLockSignal::TowerCLockSignal()
{
  InitTranslator(QLatin1String(":/tower_clock_signal/tower_clock_signal_"));
  info_.display_name = tr("Tower clock hour signal (bell per hour)");
  info_.description = tr("Plays bell strikes every hour like tower clock.");
  InitIcon(":/tower_clock_signal/icon.svg");

  bell_sound_ = new QSound(":/tower_clock_signal/bell.wav", this);
}

void TowerCLockSignal::Stop()
{
  bell_sound_->stop();
}

void TowerCLockSignal::TimeUpdateListener()
{
  QTime curr_time = QTime::currentTime();
  if (curr_time.minute() != 0 || curr_time.second() != 0 ||
      !bell_sound_->isFinished() || bell_sound_->loopsRemaining() != 0) return;

  int hour = curr_time.hour();
  if (hour > 12) hour -= 12;
  if (hour == 0) hour  = 12;

  Q_ASSERT(1 <= hour && hour <= 12);

  bell_sound_->setLoops(hour);
  bell_sound_->play();
}

} // namespace tower_clock_signal
