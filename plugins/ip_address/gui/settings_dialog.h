#ifndef IP_ADDRESS_SETTINGS_DIALOG_H
#define IP_ADDRESS_SETTINGS_DIALOG_H

#include <QDialog>

namespace ip_address {

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


} // namespace ip_address

#endif // IP_ADDRESS_SETTINGS_DIALOG_H
