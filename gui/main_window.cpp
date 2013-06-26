#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
  : QWidget(parent) {
  setWindowFlags(Qt::FramelessWindowHint);
  setAttribute(Qt::WA_TranslucentBackground);
}

void MainWindow::SetStaysOnTop(bool set) {
  SetWindowFlag(Qt::WindowStaysOnTopHint, set);
}

void MainWindow::SetTransparentForInput(bool set) {
  SetWindowFlag(Qt::WindowTransparentForInput, set);
}

void MainWindow::SetWindowFlag(Qt::WindowFlags flag, bool set) {
  hide();
  Qt::WindowFlags flags = windowFlags();
  set ? flags |= flag : flags &= ~flag;
  setWindowFlags(flags);
  show();
}
