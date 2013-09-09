#include <QTime>
#include <QLocale>
#include "am_pm.h"

void ShowAMPM::Init(QWidget* main_wnd) {
  main_layout_ = qobject_cast<QGridLayout*>(main_wnd->layout());
}

void ShowAMPM::GetInfo(TPluginInfo* info) {
  info->insert(PI_NAME, "Display AM/PM");
  info->insert(PI_TYPE, "widget");
  info->insert(PI_VERSION, "1.0");
  info->insert(PI_AUTHOR, "Nick Korotysh");
  info->insert(PI_EMAIL, "nick.korotysh@gmail.com");
  info->insert(PI_COMMENT, "Display AM/PM if needed.");
  info->insert(PI_CONFIG, "false");
}

void ShowAMPM::Start() {
  am_pm_label_ = new QLabel();
  main_layout_->addWidget(am_pm_label_, 0, 1);
}

void ShowAMPM::Stop() {
  main_layout_->removeWidget(am_pm_label_);
  delete am_pm_label_;
}
#include <QDebug>
void ShowAMPM::SettingsListener(Options option, const QVariant& value) {
  qDebug() << (int)option << value;
}

void ShowAMPM::TimeUpdateListener(const QString&) {
  qDebug() << QLocale::system().timeFormat();
  am_pm_label_->setText(QTime::currentTime().toString("ap"));
}
