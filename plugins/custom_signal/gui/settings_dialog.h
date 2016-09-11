#ifndef CUSTOM_SIGNAL_SETTINGS_DIALOG_H
#define CUSTOM_SIGNAL_SETTINGS_DIALOG_H

#include <QDialog>

namespace custom_signal {

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = 0);
  ~SettingsDialog();

private:
  Ui::SettingsDialog* ui;
};

} // namespace custom_signal

#endif // CUSTOM_SIGNAL_SETTINGS_DIALOG_H
