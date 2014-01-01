#include <QLabel>
#include <QGridLayout>
#include <QFontMetricsF>
#include <QDate>
#include "skin_drawer.h"
#include "text_skin.h"
#include "date.h"

namespace date {

Date::Date() : avail_width_(0), last_zoom_(1.0), last_date_("-") {
  drawer_ = new skin_draw::SkinDrawer(this);

  InitTranslator(QLatin1String(":/date/date_"));
  info_.display_name = tr("Date");
  info_.description = tr("Allows to display current date under clock.");
  info_.icon.load(":/date/icon.png");
}

void Date::Init(QWidget* main_wnd) {
  main_layout_ = qobject_cast<QGridLayout*>(main_wnd->layout());
  main_wnd_ = main_wnd;
  drawer_->SetSpace(0);
}

void Date::Start() {
  msg_label_ = new QLabel();
  msg_label_->setAlignment(Qt::AlignCenter);
  main_layout_->addWidget(msg_label_, main_layout_->rowCount(), 0, 1, main_layout_->columnCount());
  connect(drawer_, &skin_draw::SkinDrawer::DrawingFinished, [=] (const QImage& img) {
    msg_label_->setPixmap(QPixmap::fromImage(img));
    main_wnd_->adjustSize();
  });
}

void Date::Stop() {
  main_layout_->removeWidget(msg_label_);
  main_wnd_->adjustSize();
  disconnect(drawer_, &skin_draw::SkinDrawer::DrawingFinished, 0, 0);
  delete msg_label_;
}

void Date::SettingsListener(Options option, const QVariant& new_value) {
  switch (option) {
    case OPT_SKIN_NAME:
      if (!msg_label_) break;  // init, not started yet
      Stop();
      avail_width_ = main_layout_->cellRect(0, 0).width() / last_zoom_ - 20;
      last_date_ = "-";
      Start();
      break;

    case OPT_FONT:
      font_ = new_value.value<QFont>();
      drawer_->ApplySkin(skin_draw::ISkin::SkinPtr(new skin_draw::TextSkin(font_)));
      break;

    case OPT_ZOOM:
    {
      last_zoom_ = new_value.toReal();
      if (avail_width_ == 0) {  // first init
        avail_width_ = main_layout_->cellRect(0, 0).width() / last_zoom_ - 20;
      }
      QFontMetricsF fmf(font_);
      qreal tw = fmf.width(last_date_);
      drawer_->SetZoom(avail_width_ * last_zoom_ / tw);
      break;
    }

    case OPT_COLOR:
      drawer_->SetColor(new_value.value<QColor>());
      break;

    case OPT_TEXTURE:
      drawer_->SetTexture(new_value.toString());
      break;

    case OPT_TEXTURE_PER_ELEMENT:
      drawer_->SetTexturePerElement(new_value.toBool());
      break;

    case OPT_TEXTURE_DRAW_MODE:
      drawer_->SetTextureDrawMode((skin_draw::SkinDrawer::DrawMode)new_value.toInt());
      break;

    case OPT_CUSTOMIZATION:
      drawer_->SetCustomizationType((skin_draw::SkinDrawer::CustomizationType)new_value.toInt());
      break;
  }
}

void Date::TimeUpdateListener() {
  QString date = QDate::currentDate().toString(Qt::SystemLocaleLongDate);
  if (date == last_date_ || !msg_label_) return;
  drawer_->SetString(QString());
  QFontMetricsF fmf(font_);
  drawer_->SetZoom(avail_width_ * last_zoom_ / fmf.width(date));
  drawer_->SetString(date);
  last_date_ = date;
}

} // namespace date
