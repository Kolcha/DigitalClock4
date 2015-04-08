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
  const QVariant& GetOption(Options opt);
  const QMap<Options, QVariant>& GetSettings();

  void ExportSettings(QSettings::SettingsMap* settings);
  void ImportSettings(const QSettings::SettingsMap& settings);

signals:
  void OptionChanged(Options opt, const QVariant& value);
  void SettingsExported();
  void SettingsImported();

public slots:
  void Load();
  void Save();
  void SetOption(Options opt, const QVariant& value);
  void TrackChanges(bool set);
  void EmitSettings();
  void LoadDefaults();

private:
  QMap<Options, QString> keys_;
  QMap<Options, QVariant> values_;
  QSettings settings_;
  bool track_change_;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_CLOCK_SETTINGS_H
