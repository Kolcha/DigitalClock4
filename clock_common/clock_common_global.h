/*!
 * @file clock_common_global.h
 *
 * @brief File wih export/import macro.
 *
 * There some macroses to correctly export/import everything from library.
 * You are not needed in including this file somewhere, it already included where it really needed.
 */

#ifndef CLOCK_COMMON_GLOBAL_H
#define CLOCK_COMMON_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CLOCK_COMMON_LIBRARY)
#  define CLOCK_COMMON_EXPORT Q_DECL_EXPORT
#else
#  define CLOCK_COMMON_EXPORT Q_DECL_IMPORT
#endif

#endif // CLOCK_COMMON_GLOBAL_H
