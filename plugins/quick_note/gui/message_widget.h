/*
    Digital Clock: quick note plugin
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

#ifndef QUICK_NOTE_MESSAGE_WIDGET_H
#define QUICK_NOTE_MESSAGE_WIDGET_H

#include <QLabel>

namespace quick_note {

class MessageWidget : public QLabel
{
  Q_OBJECT

  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
  explicit MessageWidget(QWidget* parent = nullptr);

  QString text() const { return curr_text_; }

signals:
  void textChanged(const QString& new_text);
  void textEdited(const QString& new_text);

public slots:
  void setText(const QString& new_text);

protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

  QString curr_text_;
  QPoint drag_position_;
};

}  // namespace quick_note

#endif // QUICK_NOTE_MESSAGE_WIDGET_H
