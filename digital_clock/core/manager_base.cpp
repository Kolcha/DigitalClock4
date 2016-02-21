#include "manager_base.h"

namespace digital_clock {
namespace core {

ManagerBase::ManagerBase(QObject* parent) : QObject(parent)
{
}

void ManagerBase::AddSearchDir(const QDir& dir) {
  if (!search_dirs_.contains(dir)) search_dirs_.append(dir);
}

void ManagerBase::DelSearchDir(const QDir& dir) {
  search_dirs_.removeOne(dir);
}

void ManagerBase::ResetSearchDirs(const QList<QDir>& new_list) {
  search_dirs_ = new_list;
}

} // namespace core
} // namespace digital_clock
