#include <QGridLayout>
#include <QSettings>
#include <QMouseEvent>
#include <QPainter>
#include <QDesktopWidget>
#include "skin_drawer.h"
#include "clock_display.h"
#include "clock_widget.h"
#include "colorize_effect.h"

#define S_OPT_POSITION              "state/clock_position"
#define S_OPT_ALIGNMENT             "state/last_alignment"

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
  connect(drawer_, SIGNAL(DrawingFinished(QImage)), this, SLOT(DrawImage(QImage)));

  QSettings state;
  QPoint last_pos = state.value(S_OPT_POSITION, QPoint(50, 20)).toPoint();

  CAlignment last_align = static_cast<CAlignment>(state.value(S_OPT_ALIGNMENT, CAlignment::A_LEFT).toInt());
  if (last_align == CAlignment::A_RIGHT) {
    last_pos.setX(last_pos.x() - this->width());
  }
  cur_alignment_ = last_align;
  this->move(last_pos);

  colorize_color_ = GetDefaultValue(OPT_COLORIZE_COLOR).value<QColor>();
  colorize_level_ = GetDefaultValue(OPT_COLORIZE_LEVEL).toReal();
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

    case OPT_ALIGNMENT:
    {
      cur_alignment_ = static_cast<CAlignment>(value.toInt());
      if (!display_->pixmap()) break;
      QSettings state;
      state.setValue(S_OPT_ALIGNMENT, static_cast<int>(cur_alignment_));
      QPoint cur_pos = this->pos();
      switch (cur_alignment_) {
        case CAlignment::A_LEFT:
        {
          if (cur_pos.x() < 0) {
            cur_pos.setX(0);
            this->move(cur_pos);
          }
          break;
        }

        case CAlignment::A_RIGHT:
        {
          cur_pos = this->frameGeometry().topRight();
          QDesktopWidget desktop;
          if (cur_pos.x() > desktop.screen()->width()) {
            cur_pos.setX(desktop.screen()->width());
            this->move(cur_pos.x() - this->width(), cur_pos.y());
          }
          break;
        }

        default:
          Q_ASSERT(false);
      }
      state.setValue(S_OPT_POSITION, cur_pos);
      break;
    }

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
    state.setValue(S_OPT_ALIGNMENT, static_cast<int>(cur_alignment_));
    QPoint last_pos = this->pos();
    if (cur_alignment_ == CAlignment::A_RIGHT) {
      last_pos.setX(this->frameGeometry().right());
    }
    state.setValue(S_OPT_POSITION, last_pos);
    event->accept();
  }
}

void ClockWidget::paintEvent(QPaintEvent* /*event*/) {
  QPainter p(this);
  p.setCompositionMode(QPainter::CompositionMode_Clear);
  p.fillRect(this->rect(), Qt::transparent);
}

void ClockWidget::Update() {
  if (cur_alignment_ == CAlignment::A_RIGHT) {
    int old_width = this->frameGeometry().width();
    this->adjustSize();
    int new_width = this->frameGeometry().width();
    QPoint cur_pos = this->pos();
    cur_pos.setX(cur_pos.x() + old_width - new_width);
    this->move(cur_pos);
  } else {
    Q_ASSERT(cur_alignment_ == CAlignment::A_LEFT);
    this->adjustSize();
  }
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

void ClockWidget::SetWindowFlag(Qt::WindowFlags flag, bool set) {
  Qt::WindowFlags flags = windowFlags();
  set ? flags |= flag : flags &= ~flag;
  setWindowFlags(flags);
  show();
}

} // namespace gui
} // namespace digital_clock
