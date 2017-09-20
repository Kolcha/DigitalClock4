/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2016-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef DIGITAL_CLOCK_GUI_LED_TOOL_BUTTON_H
#define DIGITAL_CLOCK_GUI_LED_TOOL_BUTTON_H

#include <QToolButton>
#include <QColor>

#include "clock_common_global.h"

/*!
 * @brief Customized QToolButton intended to color selection
 *
 * This tool button displays a current selected color preview, whick looks like LED.
 */
class CLOCK_COMMON_EXPORT LedToolButton : public QToolButton
{
  Q_OBJECT
  Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
  /*!
   * Constructor.
   * @param parent - parent object
   */
  explicit LedToolButton(QWidget* parent = nullptr);

  /*!
   * @property LedToolButton::color
   * current "LED" color
   */
  QColor color() const;

signals:
  /*!
   * @fn void LedToolButton::colorChanged(const QColor& color)
   * This signal is emitted when the current "LED" color has changed, with the new @a color as an argument.
   */
  void colorChanged(const QColor& color);

public slots:
  /*!
   * Changes current "LED" color to given @a color.
   */
  void setColor(const QColor& color);

private:
  /// current "LED" color
  QColor color_;
};

#endif // DIGITAL_CLOCK_GUI_LED_TOOL_BUTTON_H
