#ifndef POWER_OFF_SETTINGS_DIALOG_H
#define POWER_OFF_SETTINGS_DIALOG_H

#include <QDialog>

namespace power_off {

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = 0);
  ~SettingsDialog();

private:
  Ui::SettingsDialog* ui;
};

} // namespace power_off

#endif // POWER_OFF_SETTINGS_DIALOG_H
