#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include "../alarm_settings.h"
#include "settings_dlg.h"
#include "ui_settings_dlg.h"

SettingsDlg::SettingsDlg(QWidget* parent)
  : CenteredDialog(parent), ui(new Ui::SettingsDlg) {
  ui->setupUi(this);
  setWindowIcon(QIcon(":/settings.svg"));
  ui->play_btn->setIcon(QIcon(":/play.svg"));

  player_ = new QMediaPlayer(this);
  connect(player_, SIGNAL(stateChanged(QMediaPlayer::State)),
          this, SLOT(PlayerStateChanged(QMediaPlayer::State)));
  connect(ui->volume_slider, SIGNAL(valueChanged(int)), player_, SLOT(setVolume(int)));
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
  if (key == OPT_SIGNAL_TYPE) {
    SignalType st = (SignalType)value.toInt();
    ui->st_file->setChecked(st == ST_FILE);
    ui->st_stream->setChecked(st == ST_STREAM);
  }
  if (key == OPT_FILENAME) {
    QString file = value.toString();
    QFileInfo info(file);
    last_file_path_ = file.isEmpty() ? "." : info.absolutePath();
    if (info.exists()) {
      ui->signal_label->setText(info.baseName());
      ui->signal_label->setToolTip(QDir::toNativeSeparators(file));
      if (ui->st_file->isChecked()) player_->setMedia(QUrl::fromLocalFile(file));
    }
  }
  if (key == OPT_STREAM_URL) {
    QString stream = value.toString();
    ui->stream_url_edit->setText(stream);
    ui->stream_url_edit->setToolTip(stream);
    if (ui->st_stream->isChecked()) player_->setMedia(QUrl(stream));
  }
  if (key == OPT_VOLUME) {
    int volume = value.toInt();
    ui->volume_slider->setValue(volume);
    player_->setVolume(volume);
  }
  if (key == OPT_SHOW_NOTIFY) {
    ui->notification_enabled->setChecked(value.toBool());
  }
  if (key == OPT_NOTIFY_TEXT) {
    ui->message_edit->setPlainText(value.toString());
  }
}

void SettingsDlg::PlayerStateChanged(QMediaPlayer::State state) {
  switch (state) {
    case QMediaPlayer::PlayingState:
      ui->play_btn->setIcon(QIcon(":/stop.svg"));
      break;

    case QMediaPlayer::PausedState:
    case QMediaPlayer::StoppedState:
      ui->play_btn->setIcon(QIcon(":/play.svg"));
      break;
  }
}

void SettingsDlg::on_time_edit_timeChanged(const QTime& time) {
  emit OptionChanged(OPT_TIME, time);
}

void SettingsDlg::on_alarm_enabled_toggled(bool checked) {
  emit OptionChanged(OPT_ENABLED, checked);
}

void SettingsDlg::on_browse_btn_clicked() {
  QString sound_file = QFileDialog::getOpenFileName(this, tr("Select sound file"),
                       last_file_path_,
                       tr("MP3 Files (*.mp3)"));
  if (!sound_file.isEmpty()) {
    ui->signal_label->setText(QFileInfo(sound_file).baseName());
    ui->signal_label->setToolTip(QDir::toNativeSeparators(sound_file));
    emit OptionChanged(OPT_FILENAME, sound_file);
    last_file_path_ = QFileInfo(sound_file).absolutePath();
    player_->setMedia(QUrl::fromLocalFile(sound_file));
  }
}

void SettingsDlg::on_notification_enabled_toggled(bool checked) {
  emit OptionChanged(OPT_SHOW_NOTIFY, checked);
}

void SettingsDlg::on_message_edit_textChanged() {
  emit OptionChanged(OPT_NOTIFY_TEXT, ui->message_edit->toPlainText());
}

void SettingsDlg::on_st_file_clicked() {
  emit OptionChanged(OPT_SIGNAL_TYPE, (int)ST_FILE);
  player_->setMedia(QUrl::fromLocalFile(
                      QDir::fromNativeSeparators(ui->signal_label->toolTip())));
}

void SettingsDlg::on_st_stream_clicked() {
  emit OptionChanged(OPT_SIGNAL_TYPE, (int)ST_STREAM);
  player_->setMedia(QUrl(ui->stream_url_edit->text()));
}

void SettingsDlg::on_stream_url_edit_textEdited(const QString& arg1) {
  emit OptionChanged(OPT_STREAM_URL, arg1);
  player_->setMedia(QUrl(arg1));
}

void SettingsDlg::on_volume_slider_valueChanged(int value) {
  emit OptionChanged(OPT_VOLUME, value);
}

void SettingsDlg::on_play_btn_clicked() {
  if (player_->state() == QMediaPlayer::PlayingState) {
    player_->stop();
  } else {
    player_->play();
  }
}
