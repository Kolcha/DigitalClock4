#ifndef WIN_ON_TOP_H
#define WIN_ON_TOP_H

#include <Windows.h>
#include "iclock_plugin.h"

namespace win_on_top {

class WinOnTop : public IClockPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "win_on_top.json")
  Q_INTERFACES(IClockPlugin)

public:
  WinOnTop();

public slots:
  void Start();
  void Stop();
  void Configure() {}
  void SettingsListener(Options, const QVariant&) {}
  void TimeUpdateListener() {}
};

} // namespace win_on_top

#endif // WIN_ON_TOP_H
