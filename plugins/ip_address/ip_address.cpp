#include "ip_address.h"

#include <QLabel>
#include <QGridLayout>
#include <QFontMetricsF>
#include <QNetworkInterface>

#include "skin_drawer.h"
#include "text_skin.h"

namespace ip_address {

IPAddress::IPAddress() : avail_width_(0), last_zoom_(1.0)
{
  drawer_ = new skin_draw::SkinDrawer(this);

  InitTranslator(QLatin1String(":/ip_address/ip_address_"));
  info_.display_name = tr("IP address");
  info_.description = tr("Displays local IP address(es) under clock.");
  InitIcon(":/ip_address/icon.svg");
}

void IPAddress::Init(const QMap<Option, QVariant>& current_settings) {
  for (auto iter = current_settings.begin(); iter != current_settings.end(); ++iter) {
    switch (iter.key()) {
      case OPT_FONT:
        font_ = iter.value().value<QFont>();
        drawer_->ApplySkin(skin_draw::ISkin::SkinPtr(new skin_draw::TextSkin(font_)));
        break;

      case OPT_ZOOM:
        last_zoom_ = iter.value().toReal();
        break;

      case OPT_COLOR:
        msg_color_ = iter.value().value<QColor>();
        drawer_->SetColor(msg_color_);
        break;

      case OPT_TEXTURE:
        drawer_->SetTexture(iter.value().toString());
        break;

      case OPT_TEXTURE_TYPE:
        drawer_->SetCustomizationType(static_cast<skin_draw::SkinDrawer::CustomizationType>(iter.value().toInt()));
        break;

      case OPT_TEXTURE_PER_ELEMENT:
        drawer_->SetTexturePerElement(iter.value().toBool());
        break;

      case OPT_TEXTURE_DRAW_MODE:
        drawer_->SetTextureDrawMode((skin_draw::SkinDrawer::DrawMode)iter.value().toInt());
        break;

      case OPT_CUSTOMIZATION:
      {
        Customization cust = static_cast<Customization>(iter.value().toInt());
        switch (cust) {
          case Customization::C_NONE:
          case Customization::C_COLORIZE:
            drawer_->SetCustomizationType(::skin_draw::SkinDrawer::CT_NONE);
            break;

          default:
            break;
        }
        break;
      }

      default:
        break;
    }
  }
  drawer_->SetSpace(0);
}

void IPAddress::Init(QWidget* main_wnd) {
  main_layout_ = qobject_cast<QGridLayout*>(main_wnd->layout());
  main_wnd_ = main_wnd;
  avail_width_ = main_layout_->cellRect(0, 0).width() / last_zoom_ - 7;
}

void IPAddress::Start() {
  msg_label_ = new QLabel();
  main_layout_->addWidget(msg_label_, main_layout_->rowCount(), 0, 1, main_layout_->columnCount());
  connect(drawer_, &skin_draw::SkinDrawer::DrawingFinished, [=] (const QImage& img) {
    msg_label_->setPixmap(QPixmap::fromImage(img));
    main_wnd_->adjustSize();
  });

  ip_list_.clear();
  foreach (const QNetworkInterface& iface, QNetworkInterface::allInterfaces()) {
    if (iface.flags() & QNetworkInterface::IsLoopBack) continue;
    if (iface.flags() & QNetworkInterface::IsUp) {
      foreach (const QNetworkAddressEntry& addren, iface.addressEntries()) {
        if (addren.ip().protocol() == QAbstractSocket::IPv4Protocol) {
          ip_list_ += addren.ip().toString() + '\n';
        }
      }
    }
  }
  if (!ip_list_.isEmpty()) ip_list_.chop(1);
  ApplyString(ip_list_);
}

void IPAddress::Stop() {
  main_layout_->removeWidget(msg_label_);
  main_wnd_->adjustSize();
  disconnect(drawer_, &skin_draw::SkinDrawer::DrawingFinished, 0, 0);
  delete msg_label_;
}

void IPAddress::SettingsListener(Option option, const QVariant& new_value) {
  switch (option) {
    case OPT_SKIN_NAME:
      avail_width_ = main_layout_->cellRect(0, 0).width() / last_zoom_ - 16;
      this->ApplyString(ip_list_);
      break;

    case OPT_FONT:
      font_ = new_value.value<QFont>();
      drawer_->ApplySkin(skin_draw::ISkin::SkinPtr(new skin_draw::TextSkin(font_)));
      break;

    case OPT_ZOOM:
    {
      last_zoom_ = new_value.toReal();
      QFontMetricsF fmf(font_);
      qreal tw = fmf.width(ip_list_);
      drawer_->SetZoom(avail_width_ * last_zoom_ / tw);
      break;
    }

    case OPT_COLOR:
      msg_color_ = new_value.value<QColor>();
      drawer_->SetColor(msg_color_);
      break;

    case OPT_TEXTURE:
      drawer_->SetTexture(new_value.toString());
      break;

    case OPT_TEXTURE_TYPE:
      drawer_->SetCustomizationType(static_cast<skin_draw::SkinDrawer::CustomizationType>(new_value.toInt()));
      break;

    case OPT_TEXTURE_PER_ELEMENT:
      drawer_->SetTexturePerElement(new_value.toBool());
      break;

    case OPT_TEXTURE_DRAW_MODE:
      drawer_->SetTextureDrawMode((skin_draw::SkinDrawer::DrawMode)new_value.toInt());
      break;

    case OPT_CUSTOMIZATION:
    {
      Customization cust = static_cast<Customization>(new_value.toInt());
      switch (cust) {
        case Customization::C_NONE:
        case Customization::C_COLORIZE:
          drawer_->SetCustomizationType(::skin_draw::SkinDrawer::CT_NONE);
          break;

        default:
          break;
      }
      break;
    }

    default:
      break;
  }
}

void IPAddress::ApplyString(const QString& str) {
  drawer_->SetString(QString());
  QFontMetricsF fmf(font_);
  drawer_->SetZoom(avail_width_ * last_zoom_ / fmf.width(str));
  drawer_->SetString(str);
}

} // namespace ip_address
