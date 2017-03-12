/*
    Digital Clock: quick note plugin
    Copyright (C) 2015-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "message_widget.h"

#include <QInputDialog>
#include <QMouseEvent>

namespace quick_note {

MessageWidget::MessageWidget(QWidget* parent) : QLabel(parent)
{
}

void MessageWidget::setText(const QString& new_text)
{
  curr_text_ = new_text;
  emit textChanged(new_text);
}

void MessageWidget::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) {
    drag_position_ = event->globalPos();
  }
  event->ignore();
}

void MessageWidget::mouseReleaseEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton &&
      event->globalPos() == drag_position_) {
    bool ok = false;
    QString str = QInputDialog::getText(nullptr, tr("Edit note"), tr("text:"),
                                        QLineEdit::Normal, curr_text_, &ok);
    if (ok) {
      setText(str);
      emit textEdited(str);
    }
  }
  event->ignore();
}

}  // namespace quick_note
