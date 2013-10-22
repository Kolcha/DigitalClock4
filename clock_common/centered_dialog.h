#ifndef CENTERED_DIALOG_H
#define CENTERED_DIALOG_H

#include "clock_common_global.h"
#include <QDialog>

/*!
 * @brief The CenteredDialog class.
 *
 * CenteredDialog class provides screen centered dialog.
 *@note This dialog is self deleting: attribute WA_DeleteOnClose is set by default.
 */
class CLOCK_COMMON_EXPORT CenteredDialog : public QDialog {
  Q_OBJECT

public:
  /*!
   * Constructor.
   * @param parent - parent widget
   */
  explicit CenteredDialog(QWidget* parent = 0);
  /*!
   * Virtual destructor.
   */
  virtual ~CenteredDialog();

protected:
  /*!
   * @brief Show event handler.
   *
   * Reimplemented show event handler to show dialog at screen center every time.
   * @param e - pointer to event.
   */
  void showEvent(QShowEvent* e);
};

#endif // CENTERED_DIALOG_H
