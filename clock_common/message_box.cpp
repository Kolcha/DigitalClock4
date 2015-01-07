#include "message_box.h"
#include <QPushButton>
#include <QTimer>

TMessageBox::TMessageBox(QWidget* parent) : QMessageBox(parent),
  auto_close_(false), timeout_(30), timer_(0) {
}

TMessageBox::TMessageBox(QMessageBox::Icon icon, const QString& title, const QString& text,
                         QMessageBox::StandardButtons buttons, QWidget* parent, Qt::WindowFlags flags)
  : QMessageBox(icon, title, text, buttons, parent, flags),
    auto_close_(false), timeout_(30), timer_(0) {
}

TMessageBox::~TMessageBox() {
}

void TMessageBox::setAutoClose(bool enable) {
  auto_close_ = enable;
}

void TMessageBox::setTimeout(int seconds) {
  timeout_ = seconds;
}

int TMessageBox::exec() {
  if (auto_close_ && defaultButton()) {
    def_btn_text_ = defaultButton()->text();
    timer_ = new QTimer(this);
    timer_->setSingleShot(false);
    timer_->setInterval(1000);
    connect(timer_, SIGNAL(timeout()), this, SLOT(TimedOut()));
    connect(this, SIGNAL(buttonClicked(QAbstractButton*)), timer_, SLOT(stop()));
    timer_->start();
  }
  return QMessageBox::exec();
}

void TMessageBox::TimedOut() {
  --timeout_;
  defaultButton()->setText(QString("%1 (%2)").arg(def_btn_text_).arg(timeout_));
  if (timeout_ == 0) defaultButton()->click();
}
