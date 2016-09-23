#include "signal_config_dialog.h"
#include "ui_signal_config_dialog.h"

#include <QFileDialog>
#include <QFileInfo>

namespace custom_signal {

SignalConfigDialog::SignalConfigDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SignalConfigDialog)
{
  ui->setupUi(this);

  ui->defaults_box->blockSignals(true);
  ui->defaults_box->addItem(tr("sound 1"), QUrl("qrc:/custom_signal/eh_signal.wav"));
  ui->defaults_box->addItem(tr("sound 2"), QUrl("qrc:/custom_signal/qh_signal.wav"));
  ui->defaults_box->addItem(tr("sound 3"), QUrl("qrc:/custom_signal/bell.wav"));
  ui->defaults_box->blockSignals(false);

  player_ = new QMediaPlayer(this);
  connect(player_, &QMediaPlayer::stateChanged, this, &SignalConfigDialog::onPlayerStateChanged);
  connect(ui->preview_btn, &QToolButton::clicked, player_, &QMediaPlayer::play);
  connect(ui->volume_slider, &QSlider::valueChanged, player_, &QMediaPlayer::setVolume);

  adjustSize();
}

SignalConfigDialog::~SignalConfigDialog()
{
  player_->stop();
  player_->setMedia(QMediaContent());
  delete ui;
}

QUrl SignalConfigDialog::sound() const
{
  return player_->currentMedia().canonicalUrl();
}

int SignalConfigDialog::volume() const
{
  return player_->volume();
}

void SignalConfigDialog::setSound(const QUrl& last_sound)
{
  ui->default_selected->setChecked(last_sound.scheme() == "qrc");
  if (ui->default_selected->isChecked()) {
    for (int i = 0; i < ui->defaults_box->count(); ++i) {
      if (ui->defaults_box->itemData(i, Qt::UserRole).toUrl() == last_sound) {
        ui->defaults_box->setCurrentIndex(i);
        break;
      }
    }
  }
  ui->custom_selected->setChecked(last_sound.scheme() == "file");
  player_->setMedia(last_sound);
}

void SignalConfigDialog::setVolume(const int last_volume)
{
  ui->volume_slider->setValue(last_volume);
  player_->setVolume(last_volume);
}

void SignalConfigDialog::setLastFile(const QString& file)
{
  last_cust_file_ = file;
}

void SignalConfigDialog::on_default_selected_toggled(bool checked)
{
  if (checked) player_->setMedia(ui->defaults_box->currentData(Qt::UserRole).toUrl());
}

void SignalConfigDialog::on_defaults_box_currentIndexChanged(int index)
{
  player_->setMedia(ui->defaults_box->itemData(index, Qt::UserRole).toUrl());
}

void SignalConfigDialog::on_custom_selected_toggled(bool checked)
{
  if (checked) player_->setMedia(last_cust_file_.isEmpty() ? QMediaContent() : QUrl::fromLocalFile(last_cust_file_));
}

void SignalConfigDialog::on_custom_browse_btn_clicked()
{
  QString last_path = QDir::homePath();
  if (!last_cust_file_.isEmpty()) {
    QFileInfo last_cust_info(last_cust_file_);
    last_path = last_cust_info.absolutePath();
  }
  QString new_sound = QFileDialog::getOpenFileName(this, tr("Select sound"),
                                                   last_path,
                                                   tr("Sounds (*.wav *.mp3 *.ogg *.oga);;All files (*.*)"));
  if (new_sound.isEmpty()) return;
  last_cust_file_ = new_sound;
  player_->setMedia(QUrl::fromLocalFile(new_sound));
}

void SignalConfigDialog::on_volume_slider_valueChanged(int value)
{
  ui->volume_slider->setToolTip(QString::number(value));
}

void SignalConfigDialog::onPlayerStateChanged(QMediaPlayer::State new_state)
{
  if (new_state == QMediaPlayer::PlayingState) {
    disconnect(ui->preview_btn, SIGNAL(clicked()), player_, SLOT(play()));
    connect(ui->preview_btn, SIGNAL(clicked()), player_, SLOT(stop()));
    ui->preview_btn->setIcon(QIcon(":/custom_signal/stop.svg"));
  } else {
    disconnect(ui->preview_btn, SIGNAL(clicked()), player_, SLOT(stop()));
    connect(ui->preview_btn, SIGNAL(clicked()), player_, SLOT(play()));
    ui->preview_btn->setIcon(QIcon(":/custom_signal/play.svg"));
  }
}

} // namespace custom_signal
