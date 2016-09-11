#include "custom_signal_plugin.h"

namespace custom_signal {

CustomSignalPlugin::CustomSignalPlugin()
{
  InitTranslator(QLatin1String(":/custom_signal/custom_signal_"));
  info_.display_name = tr("Custom period signal");
  info_.description = tr("Plays sound with selected period.");
  InitIcon(":/custom_signal/icon.svg");
}

void CustomSignalPlugin::Configure()
{
  //
}

void CustomSignalPlugin::TimeUpdateListener()
{
  //
}

} // namespace custom_signal
