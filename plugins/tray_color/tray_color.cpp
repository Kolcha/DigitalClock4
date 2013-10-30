#include <QSystemTrayIcon>
#include <QColorDialog>
#include <QPainter>
#include "tray_color_settings.h"
#include "tray_color.h"

TrayColor::TrayColor() {
  settings_ = new PluginSettings("Nick Korotysh", "Digital Clock", this);
  is_enabled_ = false;
}

void TrayColor::Init(QSystemTrayIcon* tray_icon, QWidget* parent) {
  tray_icon_ = tray_icon;
  parent_ = parent;

  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->Load();
}

void TrayColor::GetInfo(TPluginInfo* info) {
  info->insert(PI_NAME, "Tray color");
  info->insert(PI_TYPE, "tray");
  info->insert(PI_VERSION, "1.0");
  info->insert(PI_AUTHOR, "Nick Korotysh");
  info->insert(PI_EMAIL, "nick.korotysh@gmail.com");
  info->insert(PI_COMMENT, "Change tray icon color.");
  info->insert(PI_CONFIG, "true");
}

void TrayColor::Start() {
  is_enabled_ = true;
  RedrawTrayIcon(settings_->GetOption(OPT_TRAY_COLOR).value<QColor>());
}

void TrayColor::Stop() {
  RedrawTrayIcon(Qt::black);
  is_enabled_ = false;
}

void TrayColor::Configure() {
  QColor color = QColorDialog::getColor(
        settings_->GetOption(OPT_TRAY_COLOR).value<QColor>(), parent_);
  if (color.isValid()) {
    settings_->SetOption(OPT_TRAY_COLOR, color);
    if (is_enabled_) RedrawTrayIcon(color);
  }
  settings_->Save();
}

void TrayColor::RedrawTrayIcon(const QColor& color) {
  QSize size = tray_icon_->icon().actualSize(QSize(48, 48));

  QImage result(size, QImage::Format_ARGB32_Premultiplied);
  QPainter painter(&result);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result.rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

  painter.drawPixmap(0, 0, tray_icon_->icon().pixmap(size));

  QPixmap texture(2, 2);
  texture.fill(color);
  painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
  painter.drawTiledPixmap(result.rect(), texture);
  painter.end();

  tray_icon_->setIcon(QPixmap::fromImage(result));
}
