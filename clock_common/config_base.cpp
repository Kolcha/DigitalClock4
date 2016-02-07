#include "config_base.h"

#include "settings_storage.h"

ConfigBase::ConfigBase(SettingsStorage* backend, QObject *parent) :
  QObject(parent),
  backend_(backend)
{
}

const QVariant& ConfigBase::GetValue(const int id) const
{
  return backend_->GetValue(GetKey(id), GetDefaultValue(id));
}

void ConfigBase::SetValue(const int id, const QVariant& value)
{
  backend_->SetValue(GetKey(id), value);
}
