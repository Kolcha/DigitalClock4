#include <QSystemTrayIcon>
#include <QColorDialog>
#include <QPainter>
#include "plugin_settings.h"
#include "tray_color_settings.h"
#include "tray_color.h"

namespace tray_color {

TrayColor::TrayColor()
{
  is_enabled_ = false;

  InitTranslator(QLatin1String(":/tray_color/tray_color_"));
  info_.display_name = tr("Tray icon color");
  info_.description = tr("Allows to change tray icon color.");
  InitIcon(":/tray_color/icon.png");
}

void TrayColor::Init(QSystemTrayIcon* tray_icon)
{
  tray_icon_ = tray_icon;

  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->Load();
}

void TrayColor::Start()
{
  is_enabled_ = true;
  RedrawTrayIcon(settings_->GetOption(OPT_TRAY_COLOR).value<QColor>());
}

void TrayColor::Stop()
{
  RedrawTrayIcon(Qt::black);
  is_enabled_ = false;
}

void TrayColor::Configure()
{
  QColor color = QColorDialog::getColor(settings_->GetOption(OPT_TRAY_COLOR).value<QColor>());
  if (color.isValid()) {
    settings_->SetOption(OPT_TRAY_COLOR, color);
    if (is_enabled_) RedrawTrayIcon(color);
  }
  settings_->Save();
}

void TrayColor::SettingsListener(Option opt, const QVariant& value)
{
  if (opt == OPT_COLOR) RedrawTrayIcon(value.value<QColor>());
}

void TrayColor::RedrawTrayIcon(const QColor& color)
{
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

} // namespace tray_color
