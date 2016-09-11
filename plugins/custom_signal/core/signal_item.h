#ifndef SignalItem_H
#define SignalItem_H

#include <QObject>

#include <QString>
#include <QTime>

class SignalItem : public QObject
{
  Q_OBJECT
public:
  explicit SignalItem(QObject *parent = 0);

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

#endif // SignalItem_H
