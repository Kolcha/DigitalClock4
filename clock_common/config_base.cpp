#include "config_base.h"

ConfigBase::ConfigBase(SettingsStorage* backend, QObject* parent) : SettingsStorageWrapper(backend, parent)
{
}

QVariant ConfigBase::GetValue(const int id) const
{
  return SettingsStorageWrapper::getValue(GetKey(id), GetDefaultValue(id));
}

void ConfigBase::SetValue(const int id, const QVariant& value)
{
  SettingsStorageWrapper::setValue(GetKey(id), value);
}
