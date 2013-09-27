#include <QSystemTrayIcon>
#include <QFile>
#include <QDir>
#include "gui/settings_dlg.h"
#include "alarm_settings.h"
#include "alarm.h"

Alarm::Alarm() {
  settings_ = new PluginSettings("Nick Korotysh", "Digital Clock", this);
  player_ = new QMediaPlayer(this);
}

void Alarm::Init(QSystemTrayIcon* tray_icon) {
  tray_icon_ = tray_icon;
  old_icon_ = tray_icon->icon();

  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
}

void Alarm::GetInfo(TPluginInfo* info) {
  info->insert(PI_NAME, "Alarm");
  info->insert(PI_TYPE, "tray");
  info->insert(PI_VERSION, "2.0");
  info->insert(PI_AUTHOR, "Nick Korotysh");
  info->insert(PI_EMAIL, "nick.korotysh@gmail.com");
  info->insert(PI_COMMENT, "Set alarm.");
  info->insert(PI_CONFIG, "true");
}

void Alarm::Start() {
  settings_->Load();
  tray_icon_->setIcon(QIcon(":/alarm_clock.svg"));

  // check is signal mediafile is exists
  QString mediafile = settings_->GetOption(OPT_FILENAME).toString();
  if (!QFile::exists(mediafile)) {
    tray_icon_->showMessage(tr("Digital Clock Alarm"),
                            tr("File %1 doesn't exists. Click this message or go to plugin"
                               "settings to choose another.").arg(QDir::toNativeSeparators(mediafile)),
                            QSystemTrayIcon::Critical
                            );
    connect(tray_icon_, SIGNAL(messageClicked()), this, SLOT(Configure()));
  }
  emit started();
}

void Alarm::Stop() {
  tray_icon_->setIcon(old_icon_);
  emit stopped();
}

void Alarm::Configure() {
  SettingsDlg* dialog = new SettingsDlg();
  // load current settings to dialog
  connect(settings_, SIGNAL(OptionChanged(QString,QVariant)),
        dialog, SLOT(SettingsListener(QString,QVariant)));
  settings_->TrackChanges(true);
  settings_->Load();
  settings_->TrackChanges(false);
  // connect main signals/slots
  connect(dialog, SIGNAL(OptionChanged(QString,QVariant)),
          settings_, SLOT(SetOption(QString,QVariant)));
  connect(dialog, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(dialog, SIGNAL(rejected()), settings_, SLOT(Load()));
  dialog->show();
  connect(dialog, SIGNAL(destroyed()), this, SIGNAL(configured()));
}

void Alarm::TimeUpdateListener(const QString&) {
  QString alarm_time = settings_->GetOption(OPT_TIME).value<QTime>().toString();
  QString curr_time = QTime::currentTime().toString();
  if (alarm_time != curr_time ||
      player_->state() == QMediaPlayer::PlayingState) return;
  player_->setMedia(QUrl::fromLocalFile(settings_->GetOption(OPT_FILENAME).toString()));
  player_->play();
  if (settings_->GetOption(OPT_SHOW_NOTIFY).toBool()) {
    tray_icon_->showMessage(tr("Digital Clock Alarm"),
                            settings_->GetOption(OPT_NOTIFY_TEXT).toString(),
                            QSystemTrayIcon::Information,
                            player_->duration());
    connect(tray_icon_, SIGNAL(messageClicked()), player_, SLOT(stop()));
  }
}
