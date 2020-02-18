/*
    Digital Clock: clock common library
    Copyright (C) 2015-2020  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "message_box.h"
#include <QPushButton>
#include <QTimer>

TMessageBox::TMessageBox(QWidget* parent) :
  QMessageBox(parent),
  auto_close_(false), timeout_(30), timer_(nullptr)
{
}

TMessageBox::TMessageBox(QMessageBox::Icon icon, const QString& title, const QString& text,
                         QMessageBox::StandardButtons buttons, QWidget* parent, Qt::WindowFlags flags)
  : QMessageBox(icon, title, text, buttons, parent, flags),
    auto_close_(false), timeout_(30), timer_(nullptr)
{
}

void TMessageBox::setAutoClose(bool enable)
{
  auto_close_ = enable;
}

void TMessageBox::setTimeout(int seconds)
{
  timeout_ = seconds;
}

int TMessageBox::exec()
{
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

void TMessageBox::TimedOut()
{
  --timeout_;
  defaultButton()->setText(QString("%1 (%2)").arg(def_btn_text_).arg(timeout_));
  if (timeout_ == 0) defaultButton()->click();
}
