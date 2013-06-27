#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>
#include "../core/settings_keys.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = 0);
  ~SettingsDialog();

signals:
  void OptionChanged(Options opt, const QVariant& value);

public slots:
  void SettingsListener(Options opt, const QVariant& value);

protected:
  void changeEvent(QEvent* e);

private:
  Ui::SettingsDialog* ui;
};

#endif // SETTINGS_DIALOG_H
