#include <QSystemTrayIcon>
#include <QFile>
#include <QDir>
#include "gui/settings_dlg.h"
#include "alarm_settings.h"
#include "alarm.h"

Alarm::Alarm() {
  settings_ = new PluginSettings("Nick Korotysh", "Digital Clock", this);
  icon_changed_ = false;
  player_ = 0;
}

void Alarm::Init(QSystemTrayIcon* tray_icon, QWidget* parent) {
  tray_icon_ = tray_icon;
  parent_ = parent;
  old_icon_ = tray_icon->icon();

  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
}

void Alarm::GetInfo(TPluginInfo* info) {
  info->insert(PI_NAME, "Alarm");
  info->insert(PI_TYPE, "tray");
  info->insert(PI_VERSION, "2.1.1");
  info->insert(PI_AUTHOR, "Nick Korotysh");
  info->insert(PI_EMAIL, "nick.korotysh@gmail.com");
  info->insert(PI_COMMENT, "Set alarm.");
  info->insert(PI_CONFIG, "true");
}

void Alarm::Start() {
  settings_->Load();
  if (!settings_->GetOption(OPT_ENABLED).toBool()) return;

  tray_icon_->setIcon(QIcon(":/alarm_clock.svg"));
  icon_changed_ = true;

  SignalType st = (SignalType)(settings_->GetOption(OPT_SIGNAL_TYPE).toInt());
  if (st == ST_STREAM) {
    // if stream url is invalid force use file
    if (!QUrl(settings_->GetOption(OPT_STREAM_URL).toString(), QUrl::StrictMode).isValid()) {
      st = ST_FILE;
      settings_->SetOption(OPT_SIGNAL_TYPE, (int)ST_FILE);
      tray_icon_->showMessage(tr("Digital Clock Alarm"),
                              tr("Stream url is not valid. Force use file instead of stream."
                                 "Click this message to change settings."),
                              QSystemTrayIcon::Warning);
      connect(tray_icon_, SIGNAL(messageClicked()), this, SLOT(Configure()));
    }
  }
  if (st == ST_FILE) {
    // check is signal mediafile exists
    QString mediafile = settings_->GetOption(OPT_FILENAME).toString();
    if (!QFile::exists(mediafile)) {
      tray_icon_->showMessage(tr("Digital Clock Alarm"),
                              tr("File %1 doesn't exists. Click this message or go to plugin"
                                 "settings to choose another.").arg(QDir::toNativeSeparators(mediafile)),
                              QSystemTrayIcon::Critical);
      connect(tray_icon_, SIGNAL(messageClicked()), this, SLOT(Configure()));
    }
  }
  player_ = new QMediaPlayer();
}

void Alarm::Stop() {
  tray_icon_->setIcon(old_icon_);
  icon_changed_ = false;
  if (player_) {
    if (player_->state() == QMediaPlayer::PlayingState) player_->stop();
    delete player_;
    player_ = 0;
  }
}

void Alarm::Configure() {
  SettingsDlg* dialog = new SettingsDlg(parent_);
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
}

void Alarm::TimeUpdateListener(const QString&) {
  if (settings_->GetOption(OPT_ENABLED).toBool()) {
    if (!icon_changed_) Start();
  } else {
    if (icon_changed_) Stop();
    return;
  }

  QString alarm_time = settings_->GetOption(OPT_TIME).value<QTime>().toString();
  QString curr_time = QTime::currentTime().toString();
  if (alarm_time != curr_time ||
      player_->state() == QMediaPlayer::PlayingState) return;

  QUrl media_url;
  switch ((SignalType)(settings_->GetOption(OPT_SIGNAL_TYPE).toInt())) {
    case ST_FILE:
      media_url = QUrl::fromLocalFile(settings_->GetOption(OPT_FILENAME).toString());
      break;

    case ST_STREAM:
      media_url = settings_->GetOption(OPT_STREAM_URL).toString();
      break;
  }

  player_->setMedia(media_url);
  player_->play();
  if (settings_->GetOption(OPT_SHOW_NOTIFY).toBool()) {
    tray_icon_->showMessage(tr("Digital Clock Alarm"),
                            settings_->GetOption(OPT_NOTIFY_TEXT).toString(),
                            QSystemTrayIcon::Information, 30000);
    connect(tray_icon_, SIGNAL(messageClicked()), player_, SLOT(stop()));
  }
}
