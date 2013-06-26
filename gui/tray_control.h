#ifndef TRAY_CONTROL_H
#define TRAY_CONTROL_H

#include <QObject>

class TrayControl : public QObject {
  Q_OBJECT
public:
  explicit TrayControl(QObject *parent = 0);

signals:

public slots:

};

#endif // TRAY_CONTROL_H
