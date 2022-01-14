#ifndef LIBAENIGMA_GLOBAL_H
#define LIBAENIGMA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBAENIGMA_LIBRARY)
#  define LIBAENIGMA_EXPORT Q_DECL_EXPORT
#else
#  define LIBAENIGMA_EXPORT Q_DECL_IMPORT
#endif

constexpr quint8 boxSize{9};
constexpr quint8 gridSize{81};
constexpr quint8 rowSize{9};

#endif // LIBAENIGMA_GLOBAL_H
