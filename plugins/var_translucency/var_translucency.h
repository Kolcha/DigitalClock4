#ifndef VAR_TRANSLUCENCY_H
#define VAR_TRANSLUCENCY_H

#include "iclock_plugin.h"
#include "iplugin_init.h"

namespace var_translucency {

class VarTranslucency : public ISettingsPlugin, public ISettingsPluginInit {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "var_translucency.json")
  Q_INTERFACES(IClockPlugin ISettingsPlugin ISettingsPluginInit)

public:
  VarTranslucency();
  void Init(const QMap<Option, QVariant>& current_settings);

public slots:
  void Start();
  void Stop();
  void TimeUpdateListener();

private:
  qreal old_opacity_;
  qreal cur_opacity_;
  qreal op_stap_;
};

} // namespace var_translucency

#endif // VAR_TRANSLUCENCY_H
