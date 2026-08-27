/*
** FREE PROJECT, 2026
** KRONKPOOL
** File description:
** Kronkpool optimizations (attributes)
*/
#ifndef KRONKPOOL_MACROS_OPTIMIZATION_H
    #define KRONKPOOL_MACROS_OPTIMIZATION_H

    #if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__has_attribute) && __has_attribute(visibility)
        #define KP_API __attribute__((visibility("default")))
    #else
        #define KP_API
    #endif

    #if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__has_attribute) && __has_attribute(unused)
        #define KP_UNUSED __attribute__((unused))
    #else
        #define KP_UNUSED
    #endif

    #if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__has_attribute) && __has_attribute(hot)
        #define KP_HOT __attribute__((hot))
    #else
        #define KP_HOT
    #endif

    #if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__has_attribute) && __has_attribute(cold)
        #define KP_COLD __attribute__((cold))
    #else
        #define KP_COLD
    #endif

#endif /* KRONKPOOL_MACROS_OPTIMIZATION_H */
