#ifndef CLOCK_COMMON_GLOBAL_H
#define CLOCK_COMMON_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CLOCK_COMMON_LIBRARY)
#  define CLOCK_COMMON_EXPORT Q_DECL_EXPORT
#else
#  define CLOCK_COMMON_EXPORT Q_DECL_IMPORT
#endif

#endif // CLOCK_COMMON_GLOBAL_H
