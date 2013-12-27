#ifndef VAR_TRANSLUCENCY_H
#define VAR_TRANSLUCENCY_H

#include "iclock_plugin.h"

namespace var_translucency {

class VarTranslucency : public ISettingsPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID SETTINGS_PLUGIN_INTERFACE_IID FILE "var_translucency.json")
  Q_INTERFACES(IClockPlugin ISettingsPlugin)

public:
  VarTranslucency();
  void Init(const QMap<Options, QVariant>& current_settings);

public slots:
  void Start();
  void Stop();
  void Configure() {}
  void SettingsListener(Options, const QVariant&) {}
  void TimeUpdateListener();

private:
  qreal old_opacity_;
  qreal cur_opacity_;
  qreal op_stap_;
};

} // namespace var_translucency

#endif // VAR_TRANSLUCENCY_H
