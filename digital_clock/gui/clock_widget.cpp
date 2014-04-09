#include <QGridLayout>
#include <QMouseEvent>
#include "skin_drawer.h"
#include "clock_display.h"
#include "clock_widget.h"

namespace digital_clock {
namespace gui {

ClockWidget::ClockWidget(QWidget* parent) : QWidget(parent) {
  display_ = new ClockDisplay(this);
  QGridLayout* main_layout = new QGridLayout(this);
  main_layout->addWidget(display_);
  setLayout(main_layout);
  drawer_ = new ::skin_draw::SkinDrawer(this);
  connect(display_, SIGNAL(SeparatorsChanged(QString)), this, SIGNAL(SeparatorsChanged(QString)));
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

    case OPT_TEXTURE_PER_ELEMENT:
      drawer_->SetTexturePerElement(value.toBool());
      break;

    case OPT_TEXTURE_DRAW_MODE:
      drawer_->SetTextureDrawMode((::skin_draw::SkinDrawer::DrawMode)value.toInt());
      break;

    case OPT_CUSTOMIZATION:
      drawer_->SetCustomizationType((::skin_draw::SkinDrawer::CustomizationType)value.toInt());
      break;

    case OPT_SPACING:
      drawer_->SetSpace(value.toInt());
      break;
  }
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
    emit PositionChanged(pos());
    event->accept();
  }
}

void ClockWidget::SetWindowFlag(Qt::WindowFlags flag, bool set) {
  Qt::WindowFlags flags = windowFlags();
  set ? flags |= flag : flags &= ~flag;
  setWindowFlags(flags);
  show();
}

} // namespace gui
} // namespace digital_clock
