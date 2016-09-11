#include "signal_item.h"

namespace custom_signal {

SignalItem::SignalItem(QObject* parent) : QObject(parent)
{
  period_ = 1;
  offset_ = 0;
  next_shot_ = QTime::currentTime();
  next_shot_ = next_shot_.addMSecs(-next_shot_.msec());
  next_shot_ = next_shot_.addSecs(period_);
}

int SignalItem::period() const
{
  return period_;
}

int SignalItem::offset() const
{
  return offset_;
}

QString SignalItem::file() const
{
  return filename_;
}

QTime SignalItem::next() const
{
  return next_shot_;
}

void SignalItem::setPeriod(int period)
{
  period_ = period;
  QTime now = QTime::currentTime();
  int target = (now.second() / period_ + 1) * period_ + offset_;
  if (target >= 60) {
    next_shot_ = QTime(next_shot_.hour(), now.minute()).addSecs(60);
  } else {
    next_shot_ = QTime(next_shot_.hour(), next_shot_.minute(), target);
  }
}

void SignalItem::setOffset(int offset)
{
  offset_ = offset;
  QTime now = QTime::currentTime();
  int target = (now.second() / period_ + 1) * period_ + offset_;
  if (target >= 60) {
    next_shot_ = QTime(next_shot_.hour(), now.minute()).addSecs(60);
  } else {
    next_shot_ = QTime(next_shot_.hour(), next_shot_.minute(), target);
  }
}

void SignalItem::setFile(const QString& filename)
{
  filename_ = filename;
}

void SignalItem::checkTime(const QTime& t)
{
  if (t.addMSecs(-t.msec()) != next_shot_) return;
  next_shot_ = next_shot_.addSecs(period_);
  emit timeout();
}

} // namespace custom_signal
