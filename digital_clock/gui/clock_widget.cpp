#include <QGridLayout>
#include <QSettings>
#include <QMouseEvent>
#include <QPainter>
#include <QGraphicsColorizeEffect>
#include "skin_drawer.h"
#include "clock_display.h"
#include "clock_widget.h"

#define S_OPT_POSITION              "state/clock_position"

namespace digital_clock {
namespace gui {

ClockWidget::ClockWidget(QWidget* parent) : QWidget(parent) {
  display_ = new ClockDisplay(this);
  QGridLayout* main_layout = new QGridLayout(this);
  main_layout->addWidget(display_);
  setLayout(main_layout);

  drawer_ = new ::skin_draw::SkinDrawer(this);
  connect(display_, SIGNAL(SeparatorsChanged(QString)), this, SIGNAL(SeparatorsChanged(QString)));
  connect(display_, SIGNAL(ImageNeeded(QString)), drawer_, SLOT(SetString(QString)));
  connect(drawer_, SIGNAL(DrawingFinished(QImage)), display_, SLOT(DrawImage(QImage)));

  QSettings state;
  move(state.value(S_OPT_POSITION, QPoint(50, 20)).toPoint());

  colorize_color_ = GetDefaultValue(OPT_COLORIZE_COLOR).value<QColor>();
  colorize_level_ = GetDefaultValue(OPT_COLORIZE_LEVEL).toReal();
}

ClockDisplay* ClockWidget::GetDisplay() const {
  return display_;
}

void ClockWidget::ApplySkin(skin_draw::ISkin::SkinPtr skin) {
  drawer_->ApplySkin(skin);
}

void ClockWidget::ApplyOption(Options option, const QVariant& value) {
  switch (option) {
    case OPT_OPACITY:
      setWindowOpacity(value.toReal());
      break;

    case OPT_STAY_ON_TOP:
      SetWindowFlag(Qt::WindowStaysOnTopHint, value.toBool());
      break;

    case OPT_TRANSP_FOR_INPUT:
      SetWindowFlag(Qt::WindowTransparentForInput, value.toBool());
      break;

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
          this->setGraphicsEffect(0);
          drawer_->SetCustomizationType(::skin_draw::SkinDrawer::CT_NONE);
          break;

        case Customization::C_TEXTURING:
          this->setGraphicsEffect(0);
          break;

        case Customization::C_COLORIZE:
        {
          drawer_->SetCustomizationType(::skin_draw::SkinDrawer::CT_NONE);
          QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect();
          effect->setColor(colorize_color_);
          effect->setStrength(colorize_level_);
          this->setGraphicsEffect(effect);
          break;
        }
      }
      break;
    }

    case OPT_SPACING:
      drawer_->SetSpace(value.toInt());
      break;

    case OPT_COLORIZE_COLOR:
    {
      colorize_color_ = value.value<QColor>();
      QGraphicsColorizeEffect* effect = qobject_cast<QGraphicsColorizeEffect*>(graphicsEffect());
      if (effect) effect->setColor(colorize_color_);
      break;
    }

    case OPT_COLORIZE_LEVEL:
    {
      colorize_level_ = value.toReal();
      QGraphicsColorizeEffect* effect = qobject_cast<QGraphicsColorizeEffect*>(graphicsEffect());
      if (effect) effect->setStrength(colorize_level_);
      break;
    }

    default:
      break;
  }
}

void ClockWidget::PreviewMode(bool enabled) {
  drawer_->SetPreviewMode(enabled);
}

void ClockWidget::mouseMoveEvent(QMouseEvent* event) {
  if (event->buttons() & Qt::LeftButton) {
    move(event->globalPos() - drag_position_);
    event->accept();
  }
}

void ClockWidget::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    drag_position_ = event->globalPos() - frameGeometry().topLeft();
    event->accept();
  }
}

void ClockWidget::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    QSettings state;
    state.setValue(S_OPT_POSITION, pos());
    event->accept();
  }
}

void ClockWidget::paintEvent(QPaintEvent* /*event*/) {
  QPainter p(this);
  p.setCompositionMode(QPainter::CompositionMode_Clear);
  p.fillRect(this->rect(), Qt::transparent);
}

void ClockWidget::SetWindowFlag(Qt::WindowFlags flag, bool set) {
  Qt::WindowFlags flags = windowFlags();
  set ? flags |= flag : flags &= ~flag;
  setWindowFlags(flags);
  show();
}

} // namespace gui
} // namespace digital_clock
