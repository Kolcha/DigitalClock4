#ifndef DIGITAL_CLOCK_CORE_UPDATER_H
#define DIGITAL_CLOCK_CORE_UPDATER_H

#include <QDate>
#include "http_client.h"

namespace digital_clock {
namespace core {

class Updater : public QObject {
  Q_OBJECT

public:
  explicit Updater(QObject* parent = 0);
  ~Updater();

signals:
  void NewVersion(const QString& new_version, const QString& link);
  void UpToDate();
  void ErrorMessage(const QString& msg);

public slots:
  void CheckForUpdates(bool force);
  void SetCheckForBeta(bool check);
  void SetAutoupdate(bool update);
  void SetUpdatePeriod(qint64 period);
  void TimeoutHandler();

private slots:
  void ProcessData();

private:
  HttpClient* downloader_;
  bool check_beta_;
  QByteArray data_;
  QDate last_update_;
  bool autoupdate_;
  qint64 update_period_;
  bool force_update_;
  bool was_error_;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_UPDATER_H
