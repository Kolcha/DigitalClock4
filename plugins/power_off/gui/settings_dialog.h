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

public slots:
  void SettingsListener(const QString& key, const QVariant& value);

signals:
  void OptionChanged(const QString&, const QVariant&);

private slots:
  void on_time_edit_timeChanged(const QTime& time);

private:
  Ui::SettingsDialog* ui;
};

} // namespace power_off

#endif // POWER_OFF_SETTINGS_DIALOG_H
