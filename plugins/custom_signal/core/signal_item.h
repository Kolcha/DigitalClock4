#ifndef CUSTOM_SIGNAL_SIGNAL_ITEM_H
#define CUSTOM_SIGNAL_SIGNAL_ITEM_H

#include <QObject>

#include <QString>
#include <QTime>

namespace custom_signal {

class SignalItem : public QObject
{
  Q_OBJECT
public:
  explicit SignalItem(QObject* parent = 0);

  int period() const;
  int offset() const;

  QString file() const;

  QTime next() const;

signals:
  void timeout();

public slots:
  void setPeriod(int period);
  void setOffset(int offset);

  void setFile(const QString& filename);

  void checkTime(const QTime& t);

private:
  int period_;
  int offset_;
  QString filename_;
  QTime next_shot_;
};

} // namespace custom_signal

#endif // CUSTOM_SIGNAL_SIGNAL_ITEM_H
