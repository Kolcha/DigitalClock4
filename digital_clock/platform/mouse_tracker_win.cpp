/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2020  Nick Korotysh <nick.korotysh@gmail.com>

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
#include "mouse_tracker.h"

#include <Windows.h>

namespace digital_clock {

class MouseTrackerPrivate : public QObject
{
  Q_OBJECT

public:
  explicit MouseTrackerPrivate(QObject* parent = nullptr);
  ~MouseTrackerPrivate();

  LRESULT HandleMouseEvent(int nCode, WPARAM wParam, LPARAM lParam);

signals:
  void mousePosChanged(const QPoint& pos, MouseTracker::Modifiers m);

private:
  HHOOK hHook = NULL;
};


static MouseTrackerPrivate* g_pdata = nullptr;

static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  if (g_pdata)
    return g_pdata->HandleMouseEvent(nCode, wParam, lParam);
  else
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


MouseTrackerPrivate::MouseTrackerPrivate(QObject* parent)
  : QObject(parent)
{
  hHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
}

MouseTrackerPrivate::~MouseTrackerPrivate()
{
  if (hHook)
    UnhookWindowsHookEx(hHook);
}

LRESULT MouseTrackerPrivate::HandleMouseEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
  if (wParam == WM_MOUSEMOVE) {
    MSLLHOOKSTRUCT* s = (MSLLHOOKSTRUCT*)lParam;
    MouseTracker::Modifiers m = MouseTracker::NoModifiers;
    if (GetAsyncKeyState(VK_SHIFT) != 0)
      m |= MouseTracker::ShiftModifier;
    if (GetAsyncKeyState(VK_CONTROL) != 0)
      m |= MouseTracker::CtrlModifier;
    if (GetAsyncKeyState(VK_MENU) != 0)
      m |= MouseTracker::AltModifier;
    emit mousePosChanged(QPoint(s->pt.x, s->pt.y), m);
  }
  return CallNextHookEx(hHook, nCode, wParam, lParam);
}


MouseTracker::MouseTracker(QObject* parent)
  : QObject(parent)
  , impl_(nullptr)
{
}

MouseTracker::~MouseTracker() = default;

void MouseTracker::start()
{
  if (impl_)
    return;

  impl_ = new MouseTrackerPrivate(this);
  connect(impl_, &MouseTrackerPrivate::mousePosChanged, this, &MouseTracker::mousePositionChanged);
  g_pdata = impl_;
}

void MouseTracker::stop()
{
  delete impl_;
  impl_ = nullptr;
  g_pdata = nullptr;
}

} // namespace digital_clock

#include "mouse_tracker_win.moc"
