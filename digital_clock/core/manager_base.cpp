#include "manager_base.h"

namespace digital_clock {
namespace core {

ManagerBase::ManagerBase(QObject* parent) : QObject(parent)
{
}

void ManagerBase::AddSearchPath(const QString& path) {
  if (!search_dirs_.contains(path)) search_dirs_.append(QDir(path));
}

void ManagerBase::DelSearchPath(const QString& path) {
  search_dirs_.removeOne(QDir(path));
}

} // namespace core
} // namespace digital_clock
