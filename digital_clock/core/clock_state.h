#ifndef DIGITAL_CLOCK_CORE_CLOCK_STATE_H
#define DIGITAL_CLOCK_CORE_CLOCK_STATE_H

#include <QString>
#include <QVariant>

class SettingsStorage;

namespace digital_clock {
namespace core {

class ClockState
{
public:
  explicit ClockState(SettingsStorage* backend);

  void SetVariable(const QString& key, const QVariant& value, bool commit = true);
  QVariant GetVariable(const QString& key, const QVariant& default_value = QVariant());

private:
  static QString AddKeyPrefix(const QString& key);

  SettingsStorage* backend_;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_CLOCK_STATE_H
