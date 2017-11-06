/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2014-2017  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "clock_widget.h"

#include <QGridLayout>
#include <QVariant>

#include "skin_drawer.h"

#include "gui/clock_display.h"
#include "gui/colorize_effect.h"

namespace digital_clock {
namespace gui {

ClockWidget::ClockWidget(QWidget* parent) :
  QWidget(parent),
  colorize_color_(Qt::magenta), colorize_level_(0.8), colorize_enabled_(false),
  preview_mode_(false)
{
  display_ = new ClockDisplay(this);
  QGridLayout* main_layout = new QGridLayout(this);
  main_layout->setSizeConstraint(QLayout::SetFixedSize);
  main_layout->addWidget(display_);
  main_layout->setMargin(0);
  main_layout->setSpacing(4);
  setLayout(main_layout);

  drawer_ = new ::skin_draw::SkinDrawer(this);
  connect(display_, SIGNAL(SeparatorsChanged(QString)), this, SIGNAL(SeparatorsChanged(QString)));
  connect(display_, SIGNAL(ImageNeeded(QString)), drawer_, SLOT(SetString(QString)));
  connect(drawer_, SIGNAL(DrawingFinished(QImage)), this, SLOT(DrawImage(QImage)));
}

ClockDisplay* ClockWidget::GetDisplay() const
{
  return display_;
}

skin_draw::ISkin::SkinPtr ClockWidget::skin() const
{
  return drawer_->currentSkin();
}

void ClockWidget::ApplySkin(skin_draw::ISkin::SkinPtr skin)
{
  skin->SetDevicePixelRatio(this->devicePixelRatioF());
  drawer_->ApplySkin(skin);
  emit SkinChanged(skin);
}

void ClockWidget::ApplyOption(Option option, const QVariant& value)
{
  switch (option) {
    case OPT_SEPARATOR_FLASH:
      display_->SetSeparatorFlash(value.toBool());
      break;

    case OPT_TIME_FORMAT:
      display_->SetTimeFormat(value.toString());
      break;

    case OPT_DISPLAY_LOCAL_TIME:
      display_->SetDisplayLocalTime(value.toBool());
      break;

    case OPT_TIME_ZONE:
    {
      QByteArray ba = value.canConvert<QString>() ? value.toString().toLatin1() : value.toByteArray();
      display_->SetTimeZone(QTimeZone(ba));
      break;
    }

    case OPT_ZOOM:
    {
      ::skin_draw::Zoom zoom = value.value< ::skin_draw::Zoom>();
      drawer_->SetZoom(zoom.isValid() ? zoom : value.toReal());
      break;
    }

    case OPT_COLOR:
      drawer_->SetColor(value.value<QColor>());
      break;

    case OPT_TEXTURE:
      drawer_->SetTexture(value.toString());
      break;

    case OPT_TEXTURE_TYPE:
      drawer_->SetCustomizationType(value.value< ::skin_draw::SkinDrawer::CustomizationType>());
      break;

    case OPT_TEXTURE_PER_ELEMENT:
      drawer_->SetTexturePerElement(value.toBool());
      break;

    case OPT_TEXTURE_DRAW_MODE:
      drawer_->SetTextureDrawMode(value.value< ::skin_draw::SkinDrawer::DrawMode>());
      break;

    case OPT_CUSTOMIZATION:
    {
      Customization cust = static_cast<Customization>(value.toInt());
      switch (cust) {
        case Customization::C_NONE:
          colorize_enabled_ = false;
          drawer_->SetCustomizationType(::skin_draw::SkinDrawer::CT_NONE);
          break;

        case Customization::C_TEXTURING:
          colorize_enabled_ = false;
          if (!last_image_.isNull()) DrawImage(last_image_);
          break;

        case Customization::C_COLORIZE:
        {
          drawer_->SetCustomizationType(::skin_draw::SkinDrawer::CT_NONE);
          colorize_enabled_ = true;
          if (!last_image_.isNull()) DrawImage(last_image_);
          break;
        }
      }
      break;
    }

    case OPT_SPACING:
      drawer_->SetSpace(value.toInt());
      break;

    case OPT_COLORIZE_COLOR:
      colorize_color_ = value.value<QColor>();
      if (!last_image_.isNull()) DrawImage(last_image_);
      break;

    case OPT_COLORIZE_LEVEL:
      colorize_level_ = value.toReal();
      if (!last_image_.isNull()) DrawImage(last_image_);
      break;

    case OPT_CLOCK_URL_ENABLED:
      display_->SetURLEnabled(value.toBool());
      break;

    case OPT_CLOCK_URL_STRING:
      display_->SetURL(value.toString());
      break;

    default:
      break;
  }
}

void ClockWidget::EnablePreviewMode()
{
  preview_mode_ = true;
  drawer_->SetPreviewMode(true);
}

void ClockWidget::DisablePreviewMode()
{
  preview_mode_ = false;
  drawer_->SetPreviewMode(false);
}

void ClockWidget::TimeoutHandler()
{
  display_->TimeoutHandler();
}

void ClockWidget::DrawImage(const QImage& image)
{
  if (colorize_enabled_) {
    QImage dst_image;
    colorize_image(image, dst_image, colorize_color_, colorize_level_);
    display_->DrawImage(dst_image);
  } else {
    display_->DrawImage(image);
  }
  last_image_ = image;
}

} // namespace gui
} // namespace digital_clock
