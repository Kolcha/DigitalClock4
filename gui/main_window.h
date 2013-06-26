#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>

class MainWindow : public QWidget {
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = 0);

signals:

public slots:
  void SetStaysOnTop(bool set);
  void SetTransparentForInput(bool set);

private:
  void SetWindowFlag(Qt::WindowFlags flag, bool set);
};

#endif // MAIN_WINDOW_H
