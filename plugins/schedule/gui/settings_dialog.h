#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include "centered_dialog.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public CenteredDialog {
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = 0);
  ~SettingsDialog();

private slots:
  void on_add_btn_clicked();

private:
  Ui::SettingsDialog* ui;
};

#endif // SETTINGS_DIALOG_H
