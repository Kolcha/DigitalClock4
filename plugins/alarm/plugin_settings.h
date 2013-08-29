#ifndef PLUGIN_SETTINGS_H
#define PLUGIN_SETTINGS_H

#include <QSettings>

class PluginSettings : public QObject {
  Q_OBJECT

public:
  PluginSettings(const QString& org_name, const QString& app_name, QObject* parent = 0);

  void SetDefaultValues(const QSettings::SettingsMap& values);
  const QVariant& GetOption(const QString& key) const;

public slots:
  void Load();
  void Save();

  void SetOption(const QString& key, const QVariant& value);
  void TrackChanges(bool is_track);

private:
  QSettings settings_;
  QSettings::SettingsMap settings_map_;
  bool track_changes_;

signals:
  void OptionChanged(const QString&, const QVariant&);
};

#endif // PLUGIN_SETTINGS_H
