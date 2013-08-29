#include "../alarm_settings.h"
#include "settings_dlg.h"
#include "ui_settings_dlg.h"

SettingsDlg::SettingsDlg(QWidget* parent)
  : QDialog(parent), ui(new Ui::SettingsDlg) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowIcon(QIcon(":/settings.svg"));
}

SettingsDlg::~SettingsDlg() {
  delete ui;
}

void SettingsDlg::SettingsListener(const QString& key, const QVariant& value) {
  if (key == OPT_ENABLED) {
    ui->alarm_enabled->setChecked(value.toBool());
  }
  if (key == OPT_TIME) {
    ui->time_edit->setTime(value.value<QTime>());
  }
  if (key == OPT_SIGNAL) {
    QString file = value.toString();
    ui->default_signal->setChecked(file == DEFAULT_SIGNAL);
    ui->custom_signal->setChecked(file != DEFAULT_SIGNAL);
  }
  if (key == OPT_SHOW_NOTIFY) {
    ui->notification_enabled->setChecked(value.toBool());
  }
  if (key == OPT_NOTIFY_TEXT) {
    ui->message_edit->setPlainText(value.toString());
  }
}
