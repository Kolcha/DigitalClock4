/*
    Digital Clock: quick note plugin
    Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef MESSAGE_WIDGET_H
#define MESSAGE_WIDGET_H

#include <QWidget>
#include <QIcon>

namespace quick_note {

namespace Ui {
class MessageWidget;
}

class MessageWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MessageWidget(QWidget* parent = 0);
  ~MessageWidget();

signals:
  void textChanged(const QString& new_text);
  void textEdited(const QString& new_text);

public slots:
  void setText(const QString& new_text);
  void setPixmap(const QPixmap& img);
  void setIconColor(const QColor& new_color);

  void ShowEditButton(bool show);

protected:
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);

private slots:
  void on_edit_btn_clicked();

private:
  void ColorizeIcon(const QColor& color);

  Ui::MessageWidget* ui;
  QString curr_text_;
  QIcon edit_icon_;
  QPoint drag_position_;
  QColor icon_color_;
};

}  // namespace quick_note

#endif // MESSAGE_WIDGET_H
