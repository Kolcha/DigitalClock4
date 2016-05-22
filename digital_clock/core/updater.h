#ifndef DIGITAL_CLOCK_CORE_UPDATER_H
#define DIGITAL_CLOCK_CORE_UPDATER_H

#include <QObject>

#include <QDate>
#include <QString>
#include <QByteArray>

namespace digital_clock {
namespace core {

class ClockState;
class HttpClient;

class Updater : public QObject {
  Q_OBJECT

public:
  explicit Updater(ClockState* state, QObject* parent = 0);
  ~Updater();

signals:
  void NewVersion(const QString& new_version, const QString& link);
  void UpToDate();
  void ErrorMessage(const QString& msg);

public slots:
  void CheckForUpdates();
  void SetCheckForBeta(bool check);
  void SetAutoupdate(bool update);
  void SetUpdatePeriod(int period);
  void TimeoutHandler();

private slots:
  void ProcessData();

private:
  void RunCheckForUpdates(bool force);

  ClockState* state_;
  HttpClient* downloader_;
  bool check_beta_;
  QByteArray data_;
  QDate last_update_;
  bool autoupdate_;
  int update_period_;
  bool force_update_;
  bool was_error_;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_UPDATER_H
