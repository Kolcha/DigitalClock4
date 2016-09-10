/*!
 * @file plugin_core_global.h
 *
 * @brief File wih export/import macro.
 *
 * There are some macroses to correctly export/import everything from library.
 * You don't need in including this file somewhere, it already included where it really needed.
 */

#ifndef PLUGIN_CORE_GLOBAL_H
#define PLUGIN_CORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLUGIN_CORE_LIBRARY)
#  define PLUGIN_CORE_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define PLUGIN_CORE_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PLUGIN_CORE_GLOBAL_H
