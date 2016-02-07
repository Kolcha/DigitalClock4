#ifndef DIGITAL_CLOCK_CORE_CLOCK_SETTINGS_H
#define DIGITAL_CLOCK_CORE_CLOCK_SETTINGS_H

#include "config_base.h"

namespace digital_clock {
namespace core {

class ClockSettings : public ConfigBase
{
public:
  explicit ClockSettings(SettingsStorage* backend, QObject* parent = 0);

protected:
  QString GetKey(const int id) const;
  QVariant GetDefaultValue(const int id) const;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_CLOCK_SETTINGS_H
