#include "settings_storage_wrapper.h"

#include "settings_storage.h"

SettingsStorageWrapper::SettingsStorageWrapper(SettingsStorage* backend, QObject* parent) : QObject(parent)
{
  backend_ = backend;
  connect(backend_, &SettingsStorage::reloaded, this, &SettingsStorageWrapper::Reject);
}

void SettingsStorageWrapper::Accept()
{
  for (auto& key : changed_keys_) {
    backend_->Commit(key);
  }
  changed_keys_.clear();
  emit accepted();
}

void SettingsStorageWrapper::Reject()
{
  for (auto& key : changed_keys_) {
    backend_->Revert(key);
  }
  changed_keys_.clear();
  emit rejected();
}

void SettingsStorageWrapper::setValue(const QString& key, const QVariant& value)
{
  changed_keys_.insert(key);
  backend_->SetValue(key, value);
}

QVariant SettingsStorageWrapper::getValue(const QString& key, const QVariant& default_value) const
{
  return backend_->GetValue(key, default_value);
}

void SettingsStorageWrapper::remove(const QString& key)
{
  changed_keys_.insert(key);
  backend_->Remove(key);
}
