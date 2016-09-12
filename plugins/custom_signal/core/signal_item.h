#ifndef CUSTOM_SIGNAL_SIGNAL_ITEM_H
#define CUSTOM_SIGNAL_SIGNAL_ITEM_H

#include <QObject>

#include <QUrl>
#include <QTime>

namespace custom_signal {

class SignalItem : public QObject
{
  Q_OBJECT
public:
  explicit SignalItem(QObject* parent = 0);

  int period() const;
  int offset() const;

  int volume() const;
  QUrl sound() const;

  QTime next() const;

  bool enabled() const;

signals:
  void timeout();

public slots:
  void setPeriod(int period);
  void setOffset(int offset);

  void setVolume(int volume);
  void setSound(const QUrl& sound);

  void setEnabled(bool enabled);

  void checkTime(const QTime& t);

private:
  int period_;
  int offset_;
  int volume_;
  QUrl sound_;
  QTime next_shot_;
  bool enabled_;
};

QTime next_time(const QTime& now, const int period, const int offset);

} // namespace custom_signal

#endif // CUSTOM_SIGNAL_SIGNAL_ITEM_H
