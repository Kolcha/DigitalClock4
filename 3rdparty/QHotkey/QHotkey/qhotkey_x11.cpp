#include "qhotkey.h"
#include "qhotkey_p.h"
#include <QDebug>
#include <QX11Info>
#include <QThreadStorage>
#include <X11/Xlib.h>
#include <xcb/xcb.h>

class QHotkeyPrivateX11 : public QHotkeyPrivate
{
public:
    // QAbstractNativeEventFilter interface
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) Q_DECL_OVERRIDE;

protected:
    // QHotkeyPrivate interface
    quint32 nativeKeycode(Qt::Key keycode) Q_DECL_OVERRIDE;
    quint32 nativeModifiers(Qt::KeyboardModifiers modifiers) Q_DECL_OVERRIDE;
    bool registerShortcut(QHotkey::NativeShortcut shortcut) Q_DECL_OVERRIDE;
    bool unregisterShortcut(QHotkey::NativeShortcut shortcut) Q_DECL_OVERRIDE;

private:
    static const QVector<quint32> specialModifiers;
    static const quint32 validModsMask;

    static QString formatX11Error(Display *display, int errorCode);

    class HotkeyErrorHandler {
    public:
        HotkeyErrorHandler();
        ~HotkeyErrorHandler();

		static bool hasError;
		static QString errorString;

    private:
        XErrorHandler prevHandler;

        static int handleError(Display *display, XErrorEvent *error);
    };
};
NATIVE_INSTANCE(QHotkeyPrivateX11)

const QVector<quint32> QHotkeyPrivateX11::specialModifiers = {0, Mod2Mask, LockMask, (Mod2Mask | LockMask)};
const quint32 QHotkeyPrivateX11::validModsMask = ShiftMask | ControlMask | Mod1Mask | Mod4Mask;

bool QHotkeyPrivateX11::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);

    xcb_generic_event_t *genericEvent = static_cast<xcb_generic_event_t *>(message);
    if (genericEvent->response_type == XCB_KEY_PRESS) {
        xcb_key_press_event_t *keyEvent = static_cast<xcb_key_press_event_t *>(message);
        this->activateShortcut({keyEvent->detail, keyEvent->state & QHotkeyPrivateX11::validModsMask});
    }

    return false;
}

quint32 QHotkeyPrivateX11::nativeKeycode(Qt::Key keycode)
{
    KeySym keysym = XStringToKeysym(QKeySequence(keycode).toString(QKeySequence::NativeText).toLatin1().constData());
    if (keysym == NoSymbol) {
        //not found -> just use the key
        if(keycode <= 0xFFFF)
            keysym = keycode;
        else
            return 0;
    }

    Display *display = QX11Info::display();
    if(display)
        return XKeysymToKeycode(QX11Info::display(), keysym);
    else
        return 0;
}

quint32 QHotkeyPrivateX11::nativeModifiers(Qt::KeyboardModifiers modifiers)
{
    quint32 nMods = 0;
    if (modifiers & Qt::ShiftModifier)
        nMods |= ShiftMask;
    if (modifiers & Qt::ControlModifier)
        nMods |= ControlMask;
    if (modifiers & Qt::AltModifier)
        nMods |= Mod1Mask;
    if (modifiers & Qt::MetaModifier)
        nMods |= Mod4Mask;
    return nMods;
}

bool QHotkeyPrivateX11::registerShortcut(QHotkey::NativeShortcut shortcut)
{
    Display *display = QX11Info::display();
    if(!display)
        return false;

    HotkeyErrorHandler errorHandler;
    for(quint32 specialMod : QHotkeyPrivateX11::specialModifiers) {
        XGrabKey(display,
				 shortcut.key,
				 shortcut.modifier | specialMod,
                 DefaultRootWindow(display),
                 True,
                 GrabModeAsync,
                 GrabModeAsync);
    }

    if(errorHandler.hasError) {
		qCWarning(logQHotkey) << "[QHotkey]   Failed to register hotkey. Error:"
							  << qPrintable(errorHandler.errorString);
        this->unregisterShortcut(shortcut);
        return false;
    } else
        return true;
}

bool QHotkeyPrivateX11::unregisterShortcut(QHotkey::NativeShortcut shortcut)
{
    Display *display = QX11Info::display();
    if(!display)
        return false;

    HotkeyErrorHandler errorHandler;
    for(quint32 specialMod : QHotkeyPrivateX11::specialModifiers) {
        XUngrabKey(display,
				   shortcut.key,
				   shortcut.modifier | specialMod,
                   DefaultRootWindow(display));
    }

	if(errorHandler.hasError) {
		qCWarning(logQHotkey) << "Failed to unregister hotkey. Error:"
							  << qPrintable(errorHandler.errorString);
        this->unregisterShortcut(shortcut);
        return false;
    } else
        return true;
}

QString QHotkeyPrivateX11::formatX11Error(Display *display, int errorCode)
{
    char errStr[256];
    XGetErrorText(display, errorCode, errStr, 256);
    return QString::fromLatin1(errStr);
}



// ---------- QHotkeyPrivateX11::HotkeyErrorHandler implementation ----------

bool QHotkeyPrivateX11::HotkeyErrorHandler::hasError = false;
QString QHotkeyPrivateX11::HotkeyErrorHandler::errorString;

QHotkeyPrivateX11::HotkeyErrorHandler::HotkeyErrorHandler()
{
    this->prevHandler = XSetErrorHandler(&HotkeyErrorHandler::handleError);
}

QHotkeyPrivateX11::HotkeyErrorHandler::~HotkeyErrorHandler()
{
    XSetErrorHandler(this->prevHandler);
}

int QHotkeyPrivateX11::HotkeyErrorHandler::handleError(Display *display, XErrorEvent *error)
{
    switch (error->error_code) {
    case BadAccess:
    case BadValue:
    case BadWindow:
        if (error->request_code == 33 || //grab key
            error->request_code == 34) {// ungrab key
            HotkeyErrorHandler::hasError = true;
            HotkeyErrorHandler::errorString = QHotkeyPrivateX11::formatX11Error(display, error->error_code);
            return 1;
        }
    default:
        return 0;
    }
}
