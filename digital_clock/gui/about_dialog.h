#ifndef DIGITAL_CLOCK_GUI_ABOUT_DIALOG_H
#define DIGITAL_CLOCK_GUI_ABOUT_DIALOG_H

#include "centered_dialog.h"

namespace digital_clock {
namespace gui {

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

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_ABOUT_DIALOG_H
