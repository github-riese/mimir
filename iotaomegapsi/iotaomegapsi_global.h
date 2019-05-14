#ifndef TOOLS_GLOBAL_H
#define TOOLS_GLOBAL_H

#  ifdef _MSC_VER
#    define LIB_EXPORT     __declspec(dllexport)
#    define LIB_IMPORT     __declspec(dllimport)
#    define LIB_HIDDEN
#  else
#    define LIB_EXPORT     __attribute__((visibility("default")))
#    define LIB_IMPORT     __attribute__((visibility("default")))
#    define LIB_HIDDEN     __attribute__((visibility("hidden")))
#  endif


#if defined(TOOLS_LIBRARY)
#  define TOOLS_EXPORT LIB_EXPORT
#else
#  define TOOLS_EXPORT LIB_IMPORT
#endif

#endif // TOOLS_GLOBAL_H
