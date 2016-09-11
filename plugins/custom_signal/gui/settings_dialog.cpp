#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#include "core/custom_signal_settings.h"
#include "gui/signal_config_dialog.h"

namespace custom_signal {

SettingsDialog::SettingsDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  setAttribute(Qt::WA_DeleteOnClose);
  ui->setupUi(this);
  ui->custom_offset->setMaximum(ui->custom_period->value() - 1);
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

void SettingsDialog::Init(const QSettings::SettingsMap& settings)
{
  ui->eh_signal_enabled->setChecked(settings[OPT_EVERY_HOUR_ENABLED].toBool());
  ui->qh_signal_enabled->setChecked(settings[OPT_QUARTER_HOUR_ENABLED].toBool());
  ui->custom_enabled->setChecked(settings[OPT_CUSTOM_PERIOD_ENABLED].toBool());
  ui->custom_period->setValue(settings[OPT_CUSTOM_PERIOD_PERIOD].toInt());
  ui->custom_offset->setValue(settings[OPT_CUSTOM_PERIOD_OFFSET].toInt());
  settings_ = settings;
}

void SettingsDialog::on_eh_signal_enabled_clicked(bool checked)
{
  emit OptionChanged(OPT_EVERY_HOUR_ENABLED, checked);
}

void SettingsDialog::on_eh_config_btn_clicked()
{
  SignalConfigDialog dlg(this);
  dlg.setSound(settings_[OPT_EVERY_HOUR_SIGNAL].toUrl());
  dlg.setVolume(settings_[OPT_EVERY_HOUR_VOLUME].toInt());
  dlg.setLastFile(settings_[OPT_EVERY_HOUR_LAST_FILE].toString());
  if (dlg.exec() == QDialog::Rejected) return;
  Q_ASSERT(dlg.result() == QDialog::Accepted);
  emit OptionChanged(OPT_EVERY_HOUR_SIGNAL, dlg.sound());
  emit OptionChanged(OPT_EVERY_HOUR_VOLUME, dlg.volume());
  if (dlg.sound().scheme() == "file")
    emit OptionChanged(OPT_EVERY_HOUR_LAST_FILE, dlg.sound().toLocalFile());
}

void SettingsDialog::on_qh_signal_enabled_clicked(bool checked)
{
  emit OptionChanged(OPT_QUARTER_HOUR_ENABLED, checked);
}

void SettingsDialog::on_qh_config_btn_clicked()
{
  SignalConfigDialog dlg(this);
  dlg.setSound(settings_[OPT_QUARTER_HOUR_SIGNAL].toUrl());
  dlg.setVolume(settings_[OPT_QUARTER_HOUR_VOLUME].toInt());
  dlg.setLastFile(settings_[OPT_QUARTER_HOUR_LAST_FILE].toString());
  if (dlg.exec() == QDialog::Rejected) return;
  Q_ASSERT(dlg.result() == QDialog::Accepted);
  emit OptionChanged(OPT_QUARTER_HOUR_SIGNAL, dlg.sound());
  emit OptionChanged(OPT_QUARTER_HOUR_VOLUME, dlg.volume());
  if (dlg.sound().scheme() == "file")
    emit OptionChanged(OPT_QUARTER_HOUR_LAST_FILE, dlg.sound().toLocalFile());
}

void SettingsDialog::on_custom_enabled_clicked(bool checked)
{
  emit OptionChanged(OPT_CUSTOM_PERIOD_ENABLED, checked);
}

void SettingsDialog::on_custom_config_btn_clicked()
{
  SignalConfigDialog dlg(this);
  dlg.setSound(settings_[OPT_CUSTOM_PERIOD_SIGNAL].toUrl());
  dlg.setVolume(settings_[OPT_CUSTOM_PERIOD_VOLUME].toInt());
  dlg.setLastFile(settings_[OPT_CUSTOM_PERIOD_LAST_FILE].toString());
  if (dlg.exec() == QDialog::Rejected) return;
  Q_ASSERT(dlg.result() == QDialog::Accepted);
  emit OptionChanged(OPT_CUSTOM_PERIOD_SIGNAL, dlg.sound());
  emit OptionChanged(OPT_CUSTOM_PERIOD_VOLUME, dlg.volume());
  if (dlg.sound().scheme() == "file")
    emit OptionChanged(OPT_CUSTOM_PERIOD_LAST_FILE, dlg.sound().toLocalFile());
}

void SettingsDialog::on_custom_period_valueChanged(int arg1)
{
  ui->custom_offset->setMaximum(arg1-1);
  emit OptionChanged(OPT_CUSTOM_PERIOD_PERIOD, arg1);
}

void SettingsDialog::on_custom_offset_valueChanged(int arg1)
{
  emit OptionChanged(OPT_CUSTOM_PERIOD_OFFSET, arg1);
}

} // namespace custom_signal
