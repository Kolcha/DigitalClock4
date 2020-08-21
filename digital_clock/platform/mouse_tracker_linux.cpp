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

// almost all this code was found at
// https://keithp.com/blogs/Cursor_tracking/
// it was just wrapped in QThread

#include <string.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>

#include <QThread>
#include <QX11Info>

namespace digital_clock {

class MouseTrackerPrivate : public QThread
{
  Q_OBJECT

public:
  explicit MouseTrackerPrivate(QObject* parent = nullptr);

signals:
  void mousePosChanged(const QPoint& p, MouseTracker::Modifiers m);

public slots:
  void start();
  void stop();

protected:
  void run() override;

private:
  bool should_stop_;
};


/* Return 1 if XI2 is available, 0 otherwise */
static int has_xi2(Display* dpy)
{
  int major, minor;
  int rc;

  /* We support XI 2.2 */
  major = 2;
  minor = 2;

  rc = XIQueryVersion(dpy, &major, &minor);
  if (rc == BadRequest) {
    qWarning("No XI2 support. Server supports version %d.%d only.", major, minor);
    return 0;
  } else if (rc != Success) {
    qCritical("Internal Error! This is a bug in Xlib.");
  }

  qInfo("XI2 supported. Server provides version %d.%d.", major, minor);

  return 1;
}

static void select_events(Display* dpy, Window win)
{
  XIEventMask evmasks[1];
  unsigned char mask1[(XI_LASTEVENT + 7)/8];

  memset(mask1, 0, sizeof(mask1));

  /* select for button and key events from all master devices */
  XISetMask(mask1, XI_RawMotion);

  evmasks[0].deviceid = XIAllMasterDevices;
  evmasks[0].mask_len = sizeof(mask1);
  evmasks[0].mask = mask1;

  XISelectEvents(dpy, win, evmasks, 1);
  XFlush(dpy);
}


MouseTrackerPrivate::MouseTrackerPrivate(QObject* parent)
  : QThread(parent)
  , should_stop_(false)
{
}

void MouseTrackerPrivate::start()
{
  QThread::start(QThread::LowPriority);
}

void MouseTrackerPrivate::stop()
{
  should_stop_ = true;
}

void MouseTrackerPrivate::run()
{
  Display* dpy;
  int xi_opcode, event, error;
  XEvent ev;

  dpy = XOpenDisplay(NULL);

  if (!dpy) {
    qWarning("Failed to open display.");
    return;
  }

  if (!XQueryExtension(dpy, "XInputExtension", &xi_opcode, &event, &error)) {
    qWarning("X Input extension not available.");
    return;
  }

  if (!has_xi2(dpy))
    return;

  /* select for XI2 events */
  select_events(dpy, DefaultRootWindow(dpy));

  while (!should_stop_) {
    XGenericEventCookie* cookie = &ev.xcookie;
    Window       root_ret, child_ret;
    int          root_x, root_y;
    int          win_x, win_y;
    unsigned int mask;

    XNextEvent(dpy, &ev);

    if (cookie->type != GenericEvent ||
        cookie->extension != xi_opcode ||
        !XGetEventData(dpy, cookie))
      continue;

    switch (cookie->evtype) {
      case XI_RawMotion:
        XQueryPointer(dpy, DefaultRootWindow(dpy),
                      &root_ret, &child_ret, &root_x, &root_y, &win_x, &win_y, &mask);
        MouseTracker::Modifiers m = MouseTracker::NoModifiers;
        if (mask & ShiftMask)
          m |= MouseTracker::ShiftModifier;
        if (mask & ControlMask)
          m |= MouseTracker::CtrlModifier;
        if (mask & Mod1Mask)
          m |= MouseTracker::AltModifier;
        emit mousePosChanged(QPoint(root_x, root_y), m);
        break;
    }

    XFreeEventData(dpy, cookie);
  }

  should_stop_ = false;
}


MouseTracker::MouseTracker(QObject* parent)
  : QObject(parent)
  , impl_(new MouseTrackerPrivate(this))
{
  connect(impl_, &MouseTrackerPrivate::mousePosChanged, this, &MouseTracker::mousePositionChanged);
}

MouseTracker::~MouseTracker() = default;

void MouseTracker::start()
{
  if (!QX11Info::isPlatformX11())
    return;

  impl_->start();
}

void MouseTracker::stop()
{
  if (!impl_->isRunning())
    return;

  impl_->stop();
  while (!impl_->isFinished())
    QThread::yieldCurrentThread();
}

} // namespace digital_clock

#include "mouse_tracker_linux.moc"
