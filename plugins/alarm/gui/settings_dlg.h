#ifndef SETTINGS_DLG_H
#define SETTINGS_DLG_H

#include <QMediaPlayer>
#include "centered_dialog.h"

namespace Ui {
class SettingsDlg;
}

class SettingsDlg : public CenteredDialog {
  Q_OBJECT

public:
  explicit SettingsDlg(QWidget* parent = 0);
  ~SettingsDlg();

public slots:
  void SettingsListener(const QString& key, const QVariant& value);

signals:
  void OptionChanged(const QString&, const QVariant&);

private slots:
  void PlayerStateChanged(QMediaPlayer::State state);
  void on_time_edit_timeChanged(const QTime& time);
  void on_alarm_enabled_toggled(bool checked);
  void on_browse_btn_clicked();
  void on_notification_enabled_toggled(bool checked);
  void on_message_edit_textChanged();
  void on_st_file_clicked();
  void on_st_stream_clicked();
  void on_stream_url_edit_textEdited(const QString &arg1);
  void on_volume_slider_valueChanged(int value);
  void on_play_btn_clicked();

private:
  Ui::SettingsDlg* ui;
  QString last_file_path_;
  QMediaPlayer* player_;
};

#endif // SETTINGS_DLG_H
