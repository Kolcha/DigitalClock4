#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>

class Updater : public QObject {
  Q_OBJECT
public:
  explicit Updater(QObject* parent = 0);

signals:
  void UpdateAvailable(const QString& new_version);

public slots:
  void CheckForUpdates();
  void SetCheckForBeta(bool check);

private:
  bool check_beta_;
};

#endif // UPDATER_H
