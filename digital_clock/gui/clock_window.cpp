/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2017-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "clock_window.h"

#include <functional>

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QMenu>
#if (QT_VERSION < QT_VERSION_CHECK(5, 10, 0))
#include <QDesktopWidget>
#endif

#include "core/clock_settings.h"
#include "core/clock_state.h"

#include "gui/card_layout.h"
#include "gui/clock_widget.h"
#include "gui/context_menu.h"
#include "gui/hover_buttons.h"


static const char* const S_OPT_POSITION_KEY = "position";
static const char* const S_OPT_VISIBLE_KEY = "visible";

#define S_OPT_POSITION_ID(X) QString("%1/%2").arg(X).arg(S_OPT_POSITION_KEY)

#define S_OPT_POSITION S_OPT_POSITION_ID(id_)
#define S_OPT_VISIBLE QString("%1/%2").arg(id_).arg(S_OPT_VISIBLE_KEY)


namespace digital_clock {
namespace gui {

namespace {

QScreen* findScreen(QWidget* w)
{
  QRect r = w->frameGeometry();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
  QScreen* screen = QGuiApplication::screenAt(r.topLeft());
#else
  QScreen* screen = QGuiApplication::screens()[QApplication::desktop()->screenNumber(w)];
#endif
  if (!screen) {
    int max_intersected = 0;
    for (QScreen* s : QGuiApplication::screens()) {
      QRect ir = r.intersected(s->availableGeometry());
      int sq = ir.width() * ir.height();
      if (sq > max_intersected) {
        max_intersected = sq;
        screen = s;
      }
    }
  }

  if (!screen)
    screen = QGuiApplication::primaryScreen();

  return screen;
}

}

ClockWindow::ClockWindow(core::ClockSettings* app_config, int id, QWidget* parent) :
  QWidget(parent, Qt::Window),
  app_config_(app_config),
  id_(id)
{
  setWindowFlags(Qt::FramelessWindowHint);
#ifdef Q_OS_MACOS
  setWindowFlag(Qt::NoDropShadowWindowHint);
#else
  setWindowFlag(Qt::Tool);
#endif
  setAttribute(Qt::WA_TranslucentBackground);

  c_menu_ = new ContextMenu(this);
  connect(c_menu_, &ContextMenu::VisibilityChanged, this, &ClockWindow::ChangeVisibility);
  connect(c_menu_, &ContextMenu::PositionChanged, this, &ClockWindow::MoveWindow);

  state_ = new core::ClockState(app_config_->GetBackend(), this);
  connect(state_, &core::ClockState::accepted, this, &ClockWindow::SaveState);
  connect(state_, &core::ClockState::rejected, this, &ClockWindow::LoadState);
  connect(state_, &core::ClockState::rejected, this, &ClockWindow::CorrectPosition);

  connect(app_config_, &core::ClockSettings::accepted, state_, &core::ClockState::Accept);
  connect(app_config_, &core::ClockSettings::rejected, state_, &core::ClockState::Reject);

  clock_widget_ = new gui::ClockWidget(this);

  HoverButtons* hb = new HoverButtons(this);
  connect(hb, &HoverButtons::buttonClicked, this, &ClockWindow::onHoverButtonClicked);

  CardLayout* main_layout = new CardLayout(this);
  main_layout->setSizeConstraint(QLayout::SetFixedSize);
  main_layout->setSpacing(0);
  main_layout->setMargin(2);
  main_layout->addWidget(hb);
  main_layout->addWidget(clock_widget_);
  setLayout(main_layout);

  dragging_ = false;
  last_visibility_ = true;
  fullscreen_detect_enabled_ = false;
  keep_always_visible_ = true;

  connect(QGuiApplication::screens()[id-1], &QScreen::geometryChanged, this, &ClockWindow::LoadState);
  connect(QGuiApplication::screens()[id-1], &QScreen::geometryChanged, this, &ClockWindow::CorrectPosition);
}

bool ClockWindow::previewMode() const
{
  return clock_widget_->preview();
}

void ClockWindow::showEvent(QShowEvent* event)
{
  CorrectPositionImpl();
  c_menu_->visibilityAction()->setChecked(true);
  QWidget::showEvent(event);
}

void ClockWindow::hideEvent(QHideEvent* event)
{
  c_menu_->visibilityAction()->setChecked(false);
  QWidget::hideEvent(event);
}

void ClockWindow::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) {
    drag_position_ = event->globalPos() - frameGeometry().topLeft();
    dragging_ = true;
    this->layout()->itemAt(0)->widget()->setDisabled(true);
    event->accept();
  }
}

void ClockWindow::mouseMoveEvent(QMouseEvent* event)
{
  if (event->buttons() & Qt::LeftButton) {
    QPoint target_pos = event->globalPos() - drag_position_;
    if (snap_to_edges_) {
      QRect screen = findScreen(this)->availableGeometry();
      QRect widget = frameGeometry();
      if (qAbs(target_pos.x() - screen.left()) <= snap_threshold_)
        target_pos.setX(screen.left());
      if (qAbs(target_pos.y() - screen.top()) <= snap_threshold_)
        target_pos.setY(screen.top());
      if (qAbs(screen.right() - (target_pos.x() + widget.width())) <= snap_threshold_)
        target_pos.setX(screen.right() - widget.width());
      if (qAbs(screen.bottom() - (target_pos.y() + widget.height())) <= snap_threshold_)
        target_pos.setY(screen.bottom() - widget.height());
    }
    move(target_pos);
    event->accept();
  }
}

void ClockWindow::mouseReleaseEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) {
    QPoint last_pos;
    switch (cur_alignment_) {
      case CAlignment::A_LEFT:
        last_pos = this->pos();
        break;

      case CAlignment::A_RIGHT:
        last_pos = this->frameGeometry().topRight();
        break;

      case CAlignment::A_CENTER:
        last_pos = this->frameGeometry().center();
        break;
    }
    state_->SetVariable(S_OPT_POSITION, last_pos, !clock_widget_->preview());
    this->layout()->itemAt(0)->widget()->setEnabled(app_config_->GetValue(OPT_USE_HOVER_BUTTONS).toBool());
    event->accept();
    dragging_ = false;
  }
}

void ClockWindow::paintEvent(QPaintEvent* /*event*/)
{
  QPainter p(this);
  p.setCompositionMode(QPainter::CompositionMode_Clear);
  p.fillRect(this->rect(), Qt::transparent);
  if (!testAttribute(Qt::WA_TranslucentBackground)) {
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.fillRect(this->rect(), bg_color_);
  }
  if (clock_widget_->preview() && show_border_) {
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    p.setPen(QPen(Qt::red, 2, Qt::DotLine, Qt::SquareCap, Qt::MiterJoin));
    p.drawRect(rect().adjusted(1, 1, -1, -1));
  }
}

void ClockWindow::resizeEvent(QResizeEvent* event)
{
  // ignore spontaneous resize events sent by system
  // they often have incorrect window geometry, which leads to strange
  // effects when right or center alignment is used, especially on Linux
  if (event->spontaneous()) {
    event->ignore();
    return;
  }

  if (cur_alignment_ != CAlignment::A_LEFT && event->oldSize().isValid()) {
    QPoint cur_pos = this->pos();
    if (cur_alignment_ == CAlignment::A_RIGHT) {
      cur_pos.setX(cur_pos.x() + event->oldSize().width() - event->size().width());
    } else {
      Q_ASSERT(cur_alignment_ == CAlignment::A_CENTER);
      cur_pos.setX(cur_pos.x() + (event->oldSize().width() - event->size().width()) / 2);
      cur_pos.setY(cur_pos.y() + (event->oldSize().height() - event->size().height()) / 2);
    }
    this->move(cur_pos);
  }
  CorrectPosition();
  event->accept();
}

void ClockWindow::moveEvent(QMoveEvent* event)
{
  if (!dragging_ && (event->pos() != event->oldPos()) && !clock_widget_->preview())
    SaveState();
  QWidget::moveEvent(event);
}

void ClockWindow::contextMenuEvent(QContextMenuEvent* event)
{
  c_menu_->menu()->popup(event->globalPos());
  event->accept();
}

void ClockWindow::ApplyOption(const Option opt, const QVariant& value)
{
  switch (opt) {
    case OPT_OPACITY:
      setWindowOpacity(value.toReal());
      break;

    case OPT_FULLSCREEN_DETECT:
      fullscreen_detect_enabled_ = value.toBool();
      break;

    case OPT_STAY_ON_TOP:
      SetWindowFlag(Qt::WindowStaysOnTopHint, value.toBool());
      if (app_config_->GetValue(OPT_BETTER_STAY_ON_TOP).toBool())
        SetWindowFlag(Qt::X11BypassWindowManagerHint, value.toBool());
#ifdef Q_OS_WIN
      WinOnTopWorkaround();
#endif
      break;

    case OPT_TRANSP_FOR_INPUT:
      SetWindowFlag(Qt::WindowTransparentForInput, value.toBool());
      break;

    case OPT_ALIGNMENT:
      cur_alignment_ = static_cast<CAlignment>(value.toInt());
      clock_widget_->ApplyOption(opt, value);
      break;

    case OPT_BACKGROUND_ENABLED:
      this->setAttribute(Qt::WA_TranslucentBackground, !value.toBool());
      this->repaint();
      break;

    case OPT_BACKGROUND_COLOR:
      bg_color_ = value.value<QColor>();
      this->repaint();
      break;

    case OPT_SHOW_WINDOW_BORDER:
      show_border_ = value.toBool();
      break;

    case OPT_SNAP_TO_EDGES:
      snap_to_edges_ = value.toBool();
      break;

    case OPT_SNAP_THRESHOLD:
      snap_threshold_ = value.toInt();
      break;

    case OPT_EXPORT_STATE:
      state_->SetExportable(value.toBool());
      break;

    case OPT_KEEP_ALWAYS_VISIBLE:
      keep_always_visible_ = value.toBool();
      this->CorrectPosition();
      break;

    case OPT_SHOW_HIDE_ENABLED:
      c_menu_->visibilityAction()->setVisible(value.toBool());
      break;

    case OPT_SHOW_ON_ALL_DESKTOPS:
      SetVisibleOnAllDesktops(value.toBool());
      break;

    case OPT_USE_HOVER_BUTTONS:
      this->layout()->itemAt(0)->widget()->setVisible(value.toBool());
      break;

    default:
      clock_widget_->ApplyOption(opt, value);
  }
}

void ClockWindow::ApplySkin(skin_draw::ISkin::SkinPtr skin)
{
  clock_widget_->ApplySkin(skin);
}

void ClockWindow::ChangeVisibility(bool visible)
{
  c_menu_->visibilityAction()->setChecked(visible);
  this->setVisible(visible);
  state_->SetVariable(S_OPT_VISIBLE, visible);
}

void ClockWindow::EnsureVisible()
{
  last_visibility_ = this->isVisible();
  if (!this->isVisible()) this->setVisible(true);
  c_menu_->visibilityAction()->setDisabled(true);
}

void ClockWindow::RestoreVisibility()
{
  this->setVisible(last_visibility_);
  c_menu_->visibilityAction()->setEnabled(true);
}

void ClockWindow::EnablePreviewMode()
{
  setPreviewMode(true);
}

void ClockWindow::DisablePreviewMode()
{
  setPreviewMode(false);
}

void ClockWindow::setPreviewMode(bool enable)
{
  if (enable)
    clock_widget_->EnablePreviewMode();
  else
    clock_widget_->DisablePreviewMode();
  this->update();
}

void ClockWindow::TimeoutHandler()
{
  clock_widget_->TimeoutHandler();
#ifdef Q_OS_WIN
  WinOnTopWorkaround();
#endif
}

void ClockWindow::LoadState()
{
  QPoint last_pos;
  QVariant var = state_->GetVariable(S_OPT_POSITION);
  if (var.isValid()) {
    last_pos = var.toPoint();
  } else {
    // no position was found for this windows...
    // try to get for first window
    var = state_->GetVariable(S_OPT_POSITION_ID(1));
    if (var.isValid()) {
      // found! calculate current window position based on first window position
      last_pos = var.toPoint();
      last_pos -= QGuiApplication::primaryScreen()->availableGeometry().topLeft();
    } else {
      // no luck... trying to read old position key
      var = state_->GetVariable("clock_position");
      if (var.isValid()) {
        // found! calculate current window position based on old clock position
        last_pos = var.toPoint();
        last_pos -= QGuiApplication::primaryScreen()->availableGeometry().topLeft();
      } else {
        // nothing found... falling back to default value
        last_pos = QPoint(50, 20);
      }
    }
    last_pos += QGuiApplication::screens()[id_-1]->availableGeometry().topLeft();
  }

  CAlignment last_align = static_cast<CAlignment>(app_config_->GetValue(OPT_ALIGNMENT).toInt());
  switch (last_align) {
    case CAlignment::A_RIGHT:
      last_pos.setX(last_pos.x() - sizeHint().width() + 1);
      break;

    case CAlignment::A_CENTER:
      last_pos.setX(last_pos.x() - sizeHint().width() / 2 + 1);
      last_pos.setY(last_pos.y() - sizeHint().height() / 2 + 1);
      break;

    default:
      Q_ASSERT(last_align == CAlignment::A_LEFT);
  }
  cur_alignment_ = last_align;
  this->move(last_pos);

  this->setVisible(state_->GetVariable(S_OPT_VISIBLE, true).toBool());
  last_visibility_ = this->isVisible();
#ifdef Q_OS_WIN
  if (this->isVisible()) KeepOnDesktop();
#endif
}

void ClockWindow::SaveState()
{
  QPoint last_pos;
  switch (cur_alignment_) {
    case CAlignment::A_LEFT:
      last_pos = this->pos();
      break;

    case CAlignment::A_RIGHT:
      last_pos = this->frameGeometry().topRight();
      break;

    case CAlignment::A_CENTER:
      last_pos = this->frameGeometry().center();
      break;
  }
  state_->SetVariable(S_OPT_POSITION, last_pos);
  state_->SetVariable(S_OPT_VISIBLE, last_visibility_);
}

void ClockWindow::MoveWindow(Qt::Alignment align)
{
  bool show_on_all_monitors = app_config_->GetValue(OPT_SHOW_ON_ALL_MONITORS).toBool();
  QScreen* scr = show_on_all_monitors ? QGuiApplication::screens()[id_-1] : findScreen(this);
  QRect screen = scr->availableGeometry();
  QRect window = this->frameGeometry();
  QPoint curr_pos = this->pos();
  if (align & Qt::AlignLeft) curr_pos.setX(screen.left());
  if (align & Qt::AlignHCenter) curr_pos.setX(screen.center().x() - window.width() / 2);
  if (align & Qt::AlignRight) curr_pos.setX(screen.right() - window.width());
  if (align & Qt::AlignTop) curr_pos.setY(screen.top());
  if (align & Qt::AlignVCenter) curr_pos.setY(screen.center().y() - window.height() / 2);
  if (align & Qt::AlignBottom) curr_pos.setY(screen.bottom() - window.height());
  if (curr_pos != this->pos()) this->move(curr_pos);
}

void ClockWindow::HandleMouseMove(const QPoint& global_pos)
{
  if (!app_config_->GetValue(OPT_TRANSPARENT_ON_HOVER).toBool())
    return;

  static bool entered = false;

  if (frameGeometry().contains(global_pos) && !entered) {
    entered = true;
    setWindowOpacity(0.25);
  }

  if (!frameGeometry().contains(global_pos) && entered) {
    entered = false;
    setWindowOpacity(app_config_->GetValue(OPT_OPACITY).toReal());
  }
}

void ClockWindow::onHoverButtonClicked(HoverButtons::Direction direction)
{
  QPoint p = this->pos();
  const int step = app_config_->GetValue(OPT_WINDOW_MOVE_STEP).toInt();

  switch (direction) {
    case HoverButtons::Left:
      p.rx() -= step;
      break;

    case HoverButtons::Right:
      p.rx() += step;
      break;

    case HoverButtons::Top:
      p.ry() -= step;
      break;

    case HoverButtons::Bottom:
      p.ry() += step;
      break;

    default:
      break;
  }

  this->move(p);
  CorrectPositionImpl();
}

void ClockWindow::CorrectPosition()
{
  if (keep_always_visible_) CorrectPositionImpl();
}

void ClockWindow::CorrectPositionImpl()
{
  QPoint curr_pos = this->pos();
  bool show_on_all_monitors = app_config_->GetValue(OPT_SHOW_ON_ALL_MONITORS).toBool();
  QScreen* scr = show_on_all_monitors ? QGuiApplication::screens()[id_-1] : findScreen(this);
  QRect screen = scr->geometry();
  curr_pos.setX(std::max(curr_pos.x(), screen.left()));
  curr_pos.setX(std::min(curr_pos.x(), screen.left() + screen.width() - this->width()));
  curr_pos.setY(std::max(curr_pos.y(), screen.top()));
  curr_pos.setY(std::min(curr_pos.y(), screen.top() + screen.height() - this->height()));
  if (curr_pos != this->pos()) this->move(curr_pos);
}

void ClockWindow::SetWindowFlag(Qt::WindowType flag, bool set)
{
  QWidget* aw = QApplication::activeWindow();
  bool last_visible = isVisible();
  setWindowFlag(flag, set);
  if (last_visible != isVisible()) show();
  if (aw) aw->activateWindow();
}
#if !defined(Q_OS_MACOS) && !defined(Q_OS_LINUX)
void ClockWindow::SetVisibleOnAllDesktops(bool set)
{
  Q_UNUSED(set)
}
#endif
} // namespace gui
} // namespace digital_clock
