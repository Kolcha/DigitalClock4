/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2017-2019  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "clickable_label.h"

#include <QTimer>
#include <QMouseEvent>

namespace digital_clock {
namespace gui {

ClickableLabel::ClickableLabel(QWidget* parent) :
  QLabel(parent),
  req_clicks_(3),
  cur_clicks_(0)
{
  timer_ = new QTimer(this);
  timer_->setInterval(250);
  timer_->setSingleShot(true);
  connect(timer_, &QTimer::timeout, this, &ClickableLabel::onTimer);
}

int ClickableLabel::requiredClicksCount() const
{
  return req_clicks_;
}

int ClickableLabel::clickTimeout() const
{
  return timer_->interval();
}

void ClickableLabel::setRequiredClicksCount(int req_clicks)
{
  req_clicks_ = req_clicks;
}

void ClickableLabel::setClickTimeout(int timeout)
{
  timer_->setInterval(timeout);
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) {
    if (++cur_clicks_ == requiredClicksCount()) emit clicked();
    timer_->start();
    event->accept();
    return;
  }
  QLabel::mouseReleaseEvent(event);
}

void ClickableLabel::onTimer()
{
  cur_clicks_ = 0;
}

} // namespace gui
} // namespace digital_clock
