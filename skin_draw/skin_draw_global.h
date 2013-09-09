#ifndef SKIN_DRAW_GLOBAL_H
#define SKIN_DRAW_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SKIN_DRAW_LIBRARY)
#  define SKIN_DRAWSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SKIN_DRAWSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SKIN_DRAW_GLOBAL_H
