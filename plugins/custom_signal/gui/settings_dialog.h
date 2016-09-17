#ifndef CUSTOM_SIGNAL_SETTINGS_DIALOG_H
#define CUSTOM_SIGNAL_SETTINGS_DIALOG_H

#include <QDialog>

#include <QSettings>

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

  void Init(const QSettings::SettingsMap& settings);

signals:
  void OptionChanged(const QString& key, const QVariant& value);

public slots:
  void setNextCustomTime(const QTime& next_time);

private slots:
  void on_eh_signal_enabled_clicked(bool checked);
  void on_eh_config_btn_clicked();
  void on_qh_signal_enabled_clicked(bool checked);
  void on_qh_config_btn_clicked();
  void on_custom_enabled_clicked(bool checked);
  void on_custom_config_btn_clicked();

  void on_custom_period_valueChanged(int arg1);
  void on_custom_offset_valueChanged(int arg1);

private:
  Ui::SettingsDialog* ui;
  QSettings::SettingsMap settings_;
};

} // namespace custom_signal

#endif // CUSTOM_SIGNAL_SETTINGS_DIALOG_H
