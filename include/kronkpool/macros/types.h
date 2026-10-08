/*
** FREE PROJECT, 2026
** KRONKPOOL
** File description:
** Kronkpool types
*/
#ifndef KRONKPOOL_MACROS_TYPES_H
    #define KRONKPOOL_MACROS_TYPES_H
    #include <stdbool.h>
    #include <stddef.h>
    #include <stdint.h>

    typedef int kpBool;
    #define kpTrue  1
    #define kpFalse 0

    // NOTE: ssize_t, which MSVC does not have (POSIX and MinGW do). Guarded the
    //       same way as kronknet's, so that both can be included together
    #if defined(_MSC_VER) && !defined(_SSIZE_T_DEFINED)
        typedef intptr_t ssize_t;
        #define _SSIZE_T_DEFINED
    #elif !defined(_MSC_VER)
        #include <sys/types.h>
    #endif /* _MSC_VER */

#endif /* KRONKPOOL_MACROS_TYPES_H */
