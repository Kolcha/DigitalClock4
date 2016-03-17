#include "config_base.h"

#include "settings_storage.h"

ConfigBase::ConfigBase(SettingsStorage* backend, QObject *parent) :
  QObject(parent),
  backend_(backend)
{
  connect(backend_, &SettingsStorage::reloaded, this, &ConfigBase::Load);
}

const QVariant& ConfigBase::GetValue(const int id) const
{
  auto c_iter = current_values_.find(id);
  if (c_iter != current_values_.end()) return c_iter.value();
  return backend_->GetValue(GetKey(id), GetDefaultValue(id));
}

void ConfigBase::SetValue(const int id, const QVariant& value)
{
  current_values_[id] = value;
  backend_->SetValue(GetKey(id), value);
}

void ConfigBase::Load()
{
  for (auto iter = current_values_.begin(); iter != current_values_.end(); ++iter) {
    *iter = backend_->GetValue(GetKey(iter.key()), GetDefaultValue(iter.key()));
  }
  emit reloaded();
}

void ConfigBase::Save()
{
  for (auto iter = current_values_.begin(); iter != current_values_.end(); ++iter) {
    backend_->CommitValue(GetKey(iter.key()), iter.value());
  }
  emit saved();
}
