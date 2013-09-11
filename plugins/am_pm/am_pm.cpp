#include <QSettings>
//#include <QLocale>
#include <QFontMetrics>
#include <QTime>
#include "settings_keys.h"
#include "am_pm.h"

ShowAMPM::ShowAMPM() {
  am_pm_label_ = 0;
  drawer_ = new SkinDrawer(this);
  drawer_->LoadSkin(font_);
  connect(drawer_, SIGNAL(DrawingFinished(QImage)), this, SLOT(SetImage(QImage)));
}

void ShowAMPM::Init(QWidget* main_wnd) {
  main_layout_ = qobject_cast<QGridLayout*>(main_wnd->layout());
  main_widget_ = main_wnd;
  // load settings
  QMap<Options, QString> opt_keys;
  GetOptionsKeys(opt_keys);
  QSettings settings("Nick Korotysh", "Digital Clock");
  drawer_->SetColor(
        settings.value(opt_keys[OPT_COLOR], GetDefaultValue(OPT_COLOR)).value<QColor>());
  drawer_->SetTexture(
        settings.value(opt_keys[OPT_TEXTURE], GetDefaultValue(OPT_TEXTURE)).toString());
  drawer_->SetTexturePerElement(
        settings.value(opt_keys[OPT_TEXTURE_PER_ELEMENT],
                       GetDefaultValue(OPT_TEXTURE_PER_ELEMENT)).toBool());
  drawer_->SetTextureDrawMode(
        (SkinDrawer::DrawMode)settings.value(opt_keys[OPT_TEXTURE_DRAW_MODE],
                                             GetDefaultValue(OPT_TEXTURE_DRAW_MODE)).toInt());
  drawer_->SetUseTexture(
        settings.value(opt_keys[OPT_USE_TEXTURE], GetDefaultValue(OPT_USE_TEXTURE)).toBool());
  font_ = settings.value(opt_keys[OPT_FONT], GetDefaultValue(OPT_FONT)).value<QFont>();
  drawer_->LoadSkin(font_);
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
  emit started();
}

void ShowAMPM::Stop() {
  main_layout_->removeWidget(am_pm_label_);
  delete am_pm_label_;
  emit stopped();
}
#include <QDebug>
void ShowAMPM::SettingsListener(Options option, const QVariant& value) {
  switch (option) {
    case OPT_COLOR:
      drawer_->SetColor(value.value<QColor>());
      break;

    case OPT_TEXTURE:
      drawer_->SetTexture(value.toString());
      break;

    case OPT_TEXTURE_PER_ELEMENT:
      drawer_->SetTexturePerElement(value.toBool());
      break;

    case OPT_TEXTURE_DRAW_MODE:
      drawer_->SetTextureDrawMode((SkinDrawer::DrawMode)value.toInt());
      break;

    case OPT_USE_TEXTURE:
      drawer_->SetUseTexture(value.toBool());
      break;

    case OPT_FONT:
      font_ = value.value<QFont>();
      drawer_->LoadSkin(font_);
      break;
  }
}

void ShowAMPM::TimeUpdateListener(const QString&) {
//  qDebug() << QLocale::system().timeFormat();
  QFontMetrics fm(font_);
  drawer_->SetZoom(0.35 * main_widget_->height() / fm.height());
  drawer_->SetString(QTime::currentTime().toString("AP"));
}

void ShowAMPM::SetImage(const QImage& img) {
  if (!am_pm_label_) return;
  am_pm_label_->setPixmap(QPixmap::fromImage(img));
}
