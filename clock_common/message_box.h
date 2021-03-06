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

#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H

#include "clock_common_global.h"
#include <QMessageBox>

/**
 * @brief Improved QMessageBox class.
 *
 * This class has ability to 'click' default button by timeout, when user don't interact
 * with dialog. Remaining time is displayed on default button.
 * @see TMessageBox::setAutoClose(), TMessageBox::setTimeout()
 */
class CLOCK_COMMON_EXPORT TMessageBox : public QMessageBox
{
  Q_OBJECT

  Q_PROPERTY(bool autoClose READ autoClose WRITE setAutoClose)
  Q_PROPERTY(int timeout READ timeout WRITE setTimeout)

public:
  /**
   * @brief Constructor.
   *
   * Constructs a message box with no text and no buttons.
   * @param parent - parent widget
   */
  explicit TMessageBox(QWidget* parent = nullptr);
  /**
   * @brief Constructor.
   *
   * Constructs a message box with the given icon, title, text, and standard buttons. All arguments
   * are passed to the QMessageBox constructor.
   * @param icon - dialog icon
   * @param title - message box window title
   * @param text - message text
   * @param buttons - standard buttons
   * @param parent - parent widget
   * @param flags - window flags
   */
  TMessageBox(Icon icon, const QString& title, const QString& text,
              StandardButtons buttons = NoButton, QWidget* parent = nullptr,
              Qt::WindowFlags flags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

  /*!
   * @property TMessageBox::autoClose
   * automaticaly close dialog after some timeout
   * @note disabled by default
   * @see TMessageBox::timeout
   */
  bool autoClose() const { return auto_close_; }
  /**
   * @brief Sets auto close option.
   *
   * If enabled, dialol will be closed by timeout.
   * @param enable - value to set, false by default
   * @see setTimeout()
   */
  void setAutoClose(bool enable);
  /*!
   * @property TMessageBox::timeout
   * @brief dialog autoclose timeout in seconds
   *
   * If TMessageBox::autoClose is set to @e true, and this timeout is expired,
   * dialog will be closed with default button pressed.
   * By default this value is set to 30 seconds.
   *
   * @see TMessageBox::autoClose, TMessageBox::setTimeout()
   */
  int timeout() const { return timeout_; }
  /**
   * @brief Sets auto close timeout.
   *
   * After this timeout default button will be clicked.
   * @param seconds - timeout in seconds
   * @note Default button must be set explicitly using setDefaultButton()
   * @see setAutoClose()
   */
  void setTimeout(int seconds);

public slots:
  /**
   * Reimplemented from QMessageBox::exec()
   * @return clicked button
   */
  int exec() override;

private slots:
  void TimedOut();

private:
  bool auto_close_;
  int timeout_;
  QTimer* timer_;
  QString def_btn_text_;
};

#endif // MESSAGE_BOX_H
