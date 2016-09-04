#ifndef PLUGIN_CORE_GLOBAL_H
#define PLUGIN_CORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLUGIN_CORE_LIBRARY)
#  define PLUGIN_CORE_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define PLUGIN_CORE_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PLUGIN_CORE_GLOBAL_H
