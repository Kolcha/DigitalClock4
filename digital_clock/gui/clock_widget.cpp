#include "clock_widget.h"

#include <QGridLayout>
#include <QVariant>

#include "skin_drawer.h"

#include "gui/clock_display.h"
#include "gui/colorize_effect.h"

namespace digital_clock {
namespace gui {

ClockWidget::ClockWidget(QWidget* parent) : QWidget(parent) {
  display_ = new ClockDisplay(this);
  QGridLayout* main_layout = new QGridLayout(this);
  main_layout->addWidget(display_);
  setLayout(main_layout);
  adjustSize();

  drawer_ = new ::skin_draw::SkinDrawer(this);
  connect(display_, SIGNAL(SeparatorsChanged(QString)), this, SIGNAL(SeparatorsChanged(QString)));
  connect(display_, SIGNAL(ImageNeeded(QString)), drawer_, SLOT(SetString(QString)));
  connect(display_, SIGNAL(changed()), this, SLOT(Update()));
  connect(display_, &ClockDisplay::changed, this, &ClockWidget::changed);
  connect(drawer_, SIGNAL(DrawingFinished(QImage)), this, SLOT(DrawImage(QImage)));

  colorize_enabled_ = false;
}

ClockDisplay* ClockWidget::GetDisplay() const {
  return display_;
}

void ClockWidget::ApplySkin(skin_draw::ISkin::SkinPtr skin) {
  drawer_->ApplySkin(skin);
}

void ClockWidget::ApplyOption(Option option, const QVariant& value) {
  switch (option) {
    case OPT_SEPARATOR_FLASH:
      display_->SetSeparatorFlash(value.toBool());
      break;

    case OPT_TIME_FORMAT:
      display_->SetTimeFormat(value.toString());
      break;

    case OPT_ZOOM:
      drawer_->SetZoom(value.toReal());
      break;

    case OPT_COLOR:
      drawer_->SetColor(value.value<QColor>());
      break;

    case OPT_TEXTURE:
      drawer_->SetTexture(value.toString());
      break;

    case OPT_TEXTURE_TYPE:
      drawer_->SetCustomizationType(static_cast< ::skin_draw::SkinDrawer::CustomizationType>(value.toInt()));
      break;

    case OPT_TEXTURE_PER_ELEMENT:
      drawer_->SetTexturePerElement(value.toBool());
      break;

    case OPT_TEXTURE_DRAW_MODE:
      drawer_->SetTextureDrawMode((::skin_draw::SkinDrawer::DrawMode)value.toInt());
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

    default:
      break;
  }
}

void ClockWidget::PreviewMode(bool enabled) {
  drawer_->SetPreviewMode(enabled);
}

void ClockWidget::EnablePreviewMode()
{
  drawer_->SetPreviewMode(true);
}

void ClockWidget::DisablePreviewMode()
{
  drawer_->SetPreviewMode(false);
}

void ClockWidget::TimeoutHandler()
{
  display_->TimeoutHandler();
}

void ClockWidget::Update() {
  this->adjustSize();
}

void ClockWidget::DrawImage(const QImage& image) {
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
