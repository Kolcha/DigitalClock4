#include <QShowEvent>
#include <QApplication>
#include <QDesktopWidget>
#include "centered_dialog.h"

CenteredDialog::CenteredDialog(QWidget* parent)
  : QDialog(parent) {
  setAttribute(Qt::WA_DeleteOnClose);
}

CenteredDialog::~CenteredDialog() {
}

void CenteredDialog::showEvent(QShowEvent* e) {
  e->accept();

  int scrn = 0;
  const QWidget* w = window();

  if (w)
    scrn = QApplication::desktop()->screenNumber(w);
  else if (QApplication::desktop()->isVirtualDesktop())
    scrn = QApplication::desktop()->screenNumber(QCursor::pos());
  else
    scrn = QApplication::desktop()->screenNumber(this);

  QRect desk(QApplication::desktop()->availableGeometry(scrn));
  move((desk.width() - frameGeometry().width()) / 2,
       (desk.height() - frameGeometry().height()) / 2);
}
