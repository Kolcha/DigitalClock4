#ifndef DIGITAL_CLOCK_CORE_MANAGER_BASE_H
#define DIGITAL_CLOCK_CORE_MANAGER_BASE_H

#include <QObject>

#include <QDir>
#include <QList>

namespace digital_clock {
namespace core {

class ManagerBase : public QObject {
  Q_OBJECT

public:
  explicit ManagerBase(QObject* parent = 0);

public slots:
  void AddSearchDir(const QDir& dir);
  void DelSearchDir(const QDir& dir);
  void ResetSearchDirs(const QList<QDir>& new_list = QList<QDir>());

protected:
  QList<QDir> search_dirs_;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_MANAGER_BASE_H
