#ifndef SETTINGS_DLG_H
#define SETTINGS_DLG_H

#include <QDialog>

namespace Ui {
class SettingsDlg;
}
// TODO: inherit from CenteredDialog
class SettingsDlg : public QDialog {
  Q_OBJECT

public:
  explicit SettingsDlg(QWidget* parent = 0);
  ~SettingsDlg();

public slots:
  void SettingsListener(const QString& key, const QVariant& value);

signals:
  void OptionChanged(const QString&, const QVariant&);

protected:
  void showEvent(QShowEvent* e);

private slots:
  void on_time_edit_timeChanged(const QTime& time);
  void on_alarm_enabled_toggled(bool checked);
  void on_browse_btn_clicked();
  void on_notification_enabled_toggled(bool checked);
  void on_message_edit_textChanged();
  void on_st_file_clicked();
  void on_st_stream_clicked();
  void on_stream_url_edit_textEdited(const QString &arg1);

private:
  Ui::SettingsDlg* ui;
  QString last_file_path_;
};

#endif // SETTINGS_DLG_H
