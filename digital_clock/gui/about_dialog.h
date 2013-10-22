#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H

#include "centered_dialog.h"

namespace Ui {
class AboutDialog;
}

class AboutDialog : public CenteredDialog {
  Q_OBJECT

public:
  explicit AboutDialog(QWidget* parent = 0);
  ~AboutDialog();

private:
  Ui::AboutDialog *ui;
};

#endif // ABOUT_DIALOG_H
