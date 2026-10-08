/*
** FREE PROJECT, 2026
** KRONKPOOL
** File description:
** Kronkpool optimizations (attributes)
*/
#ifndef KRONKPOOL_MACROS_OPTIMIZATION_H
    #define KRONKPOOL_MACROS_OPTIMIZATION_H

    // NOTE: __has_attribute(x) must not be reached when the compiler does not know it (MSVC):
    //       even behind a defined(__has_attribute) &&, it would not parse
    #if defined(__has_attribute)
        #define KP_HAS_ATTRIBUTE(x) __has_attribute(x)
    #else
        #define KP_HAS_ATTRIBUTE(x) 0
    #endif

    #if defined(__GNUC__) && (__GNUC__ >= 4)
        #define KP_GNUC 1
    #else
        #define KP_GNUC 0
    #endif

    // NOTE: On Windows, the DLL exports everything (WINDOWS_EXPORT_ALL_SYMBOLS),
    //       and a static library needs nothing
    #if defined(_WIN32) || defined(__CYGWIN__)
        #define KP_API
    #elif KP_GNUC || KP_HAS_ATTRIBUTE(visibility)
        #define KP_API __attribute__((visibility("default")))
    #else
        #define KP_API
    #endif

    #if KP_GNUC || KP_HAS_ATTRIBUTE(unused)
        #define KP_UNUSED __attribute__((unused))
    #else
        #define KP_UNUSED
    #endif

    #if KP_GNUC || KP_HAS_ATTRIBUTE(hot)
        #define KP_HOT __attribute__((hot))
    #else
        #define KP_HOT
    #endif

    #if KP_GNUC || KP_HAS_ATTRIBUTE(cold)
        #define KP_COLD __attribute__((cold))
    #else
        #define KP_COLD
    #endif

#endif /* KRONKPOOL_MACROS_OPTIMIZATION_H */
