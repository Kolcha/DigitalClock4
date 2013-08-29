#ifndef SETTINGS_DLG_H
#define SETTINGS_DLG_H

#include <QDialog>

namespace Ui {
class SettingsDlg;
}

class SettingsDlg : public QDialog {
  Q_OBJECT

public:
  explicit SettingsDlg(QWidget* parent = 0);
  ~SettingsDlg();

private:
  Ui::SettingsDlg* ui;
};

#endif // SETTINGS_DLG_H
