#include "clock_state.h"

#include "settings_storage.h"

namespace digital_clock {
namespace core {

ClockState::ClockState(SettingsStorage* backend)
{
  backend_ = backend;
}

void ClockState::SetVariable(const QString& key, const QVariant& value, bool commit)
{
  QString full_key = AddKeyPrefix(key);
  backend_->SetValue(full_key, value);
  if (commit) backend_->Commit(full_key);
}

QVariant ClockState::GetVariable(const QString& key, const QVariant& default_value)
{
  return backend_->GetValue(AddKeyPrefix(key), default_value);
}

QString ClockState::AddKeyPrefix(const QString& key)
{
  return QString("%1/%2").arg("state", key);
}

} // namespace core
} // namespace digital_clock
