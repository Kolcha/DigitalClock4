/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2017-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef DIGITAL_CLOCK_GUI_CLICKABLE_LABEL_H
#define DIGITAL_CLOCK_GUI_CLICKABLE_LABEL_H

#include <QLabel>

namespace digital_clock {
namespace gui {

class ClickableLabel : public QLabel
{
  Q_OBJECT

  Q_PROPERTY(int requiredClicksCount READ requiredClicksCount WRITE setRequiredClicksCount)
  Q_PROPERTY(int clickTimeout READ clickTimeout WRITE setClickTimeout)

public:
  explicit ClickableLabel(QWidget* parent = nullptr);

  int requiredClicksCount() const;
  int clickTimeout() const;

signals:
  void clicked();

public slots:
  void setRequiredClicksCount(int req_clicks);
  void setClickTimeout(int timeout);

protected:
  void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
  void onTimer();

private:
  QTimer* timer_;
  int req_clicks_;
  int cur_clicks_;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_CLICKABLE_LABEL_H
