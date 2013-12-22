/*!
 * @file skin_draw_global.h
 *
 * @brief File wih export/import macro.
 *
 * There some macroses to correctly export/import everything from library.
 * You are not needed in including this file somewhere, it already included where it really needed.
 */

#ifndef SKIN_DRAW_GLOBAL_H
#define SKIN_DRAW_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SKIN_DRAW_LIBRARY)
#  define SKIN_DRAWSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SKIN_DRAWSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SKIN_DRAW_GLOBAL_H
