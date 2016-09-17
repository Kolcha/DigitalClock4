#include "custom_signal_settings.h"

#include <QUrl>

namespace custom_signal {

void InitDefaults(QSettings::SettingsMap* defaults)
{
  defaults->insert(OPT_EVERY_HOUR_ENABLED, true);
  defaults->insert(OPT_EVERY_HOUR_SIGNAL, QUrl("qrc:/custom_signal/eh_signal.wav"));
  defaults->insert(OPT_EVERY_HOUR_VOLUME, 90);
  defaults->insert(OPT_EVERY_HOUR_LAST_FILE, QString());

  defaults->insert(OPT_QUARTER_HOUR_ENABLED, false);
  defaults->insert(OPT_QUARTER_HOUR_SIGNAL, QUrl("qrc:/custom_signal/qh_signal.wav"));
  defaults->insert(OPT_QUARTER_HOUR_VOLUME, 60);
  defaults->insert(OPT_QUARTER_HOUR_LAST_FILE, QString());

  defaults->insert(OPT_CUSTOM_PERIOD_ENABLED, false);
  defaults->insert(OPT_CUSTOM_PERIOD_SIGNAL, QUrl("qrc:/custom_signal/bell.wav"));
  defaults->insert(OPT_CUSTOM_PERIOD_VOLUME, 30);
  defaults->insert(OPT_CUSTOM_PERIOD_PERIOD, 10);
  defaults->insert(OPT_CUSTOM_PERIOD_OFFSET, 0);
  defaults->insert(OPT_CUSTOM_PERIOD_LAST_FILE, QString());
}

} // namespace custom_signal
