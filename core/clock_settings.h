#ifndef CLOCK_SETTINGS_H
#define CLOCK_SETTINGS_H

#include <QSettings>
#include "settings_keys.h"

class ClockSettings : public QObject {
  Q_OBJECT
public:
  explicit ClockSettings(QObject *parent = 0);

signals:
  void OptionChanged(Options opt, const QVariant& value);

public slots:
  void Load();
  void Save();
  void SetOption(Options opt, const QVariant& value);

private:
  QMap<Options, QString> keys_;
  QMap<Options, QVariant> values_;
  QSettings settings_;
};

#endif // CLOCK_SETTINGS_H
