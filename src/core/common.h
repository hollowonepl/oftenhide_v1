
#ifndef USHINE_CORE_COMMON
#define USHINE_CORE_COMMON

#ifdef _WIN32
#define DYNLIB_EXPORT
#define DYNLIB_EXPORT_HIDDEN
#else
#define DYNLIB_EXPORT __attribute__((visibility("default")))
#define DYNLIB_EXPORT_HIDDEN __attribute__((visibility("hidden")))
#endif

#include <memory>

#endif
