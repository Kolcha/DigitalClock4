#ifndef MOVE_CLOCK_SETTINGS_DIALOG_H
#define MOVE_CLOCK_SETTINGS_DIALOG_H

#include <QDialog>

namespace move_clock {

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = 0);
  ~SettingsDialog();

public slots:
  void SettingsListener(const QString& key, const QVariant& value);

signals:
  void OptionChanged(const QString&, const QVariant&);

private:
  Ui::SettingsDialog* ui;
};


} // namespace move_clock

#endif // MOVE_CLOCK_SETTINGS_DIALOG_H
