#ifndef DIGITAL_CLOCK_CORE_CLOCK_SETTINGS_H
#define DIGITAL_CLOCK_CORE_CLOCK_SETTINGS_H

#include <QSettings>
#include "settings_keys.h"

namespace digital_clock {
namespace core {

class ClockSettings : public QObject {
  Q_OBJECT

public:
  explicit ClockSettings(QObject* parent = 0);
  const QVariant& GetOption(Option opt);
  const QMap<Option, QVariant>& GetSettings();

  void ExportSettings(QSettings::SettingsMap* settings);
  void ImportSettings(const QSettings::SettingsMap& settings);

signals:
  void OptionChanged(Option opt, const QVariant& value);
  void SettingsExported();
  void SettingsImported();

public slots:
  void Load();
  void Save();
  void SetOption(Option opt, const QVariant& value);
  void TrackChanges(bool set);
  void EmitSettings();
  void LoadDefaults();

private:
  QMap<Option, QString> keys_;
  QMap<Option, QVariant> values_;
  QSettings settings_;
  bool track_change_;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_CLOCK_SETTINGS_H
