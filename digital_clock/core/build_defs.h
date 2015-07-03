/*!
 * @file build_defs.h
 *
 * @brief version info related macroses
 *
 * original was found at
 * http://stackoverflow.com/questions/11697820/how-to-use-date-and-time-predefined-macros-in-as-two-integers-then-stri
 */

#ifndef BUILD_DEFS
#define BUILD_DEFS

// Example of __DATE__ string: "Jul 27 2012"
//                              01234567890

#define BUILD_YEAR_CH0 (__DATE__[ 7])
#define BUILD_YEAR_CH1 (__DATE__[ 8])
#define BUILD_YEAR_CH2 (__DATE__[ 9])
#define BUILD_YEAR_CH3 (__DATE__[10])


#define BUILD_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILD_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILD_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILD_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILD_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILD_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILD_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILD_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILD_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILD_MONTH_IS_DEC (__DATE__[0] == 'D')


#define BUILD_MONTH_CH0 \
    ((BUILD_MONTH_IS_OCT || BUILD_MONTH_IS_NOV || BUILD_MONTH_IS_DEC) ? '1' : '0')

#define BUILD_MONTH_CH1 \
    ( \
        (BUILD_MONTH_IS_JAN) ? '1' : \
        (BUILD_MONTH_IS_FEB) ? '2' : \
        (BUILD_MONTH_IS_MAR) ? '3' : \
        (BUILD_MONTH_IS_APR) ? '4' : \
        (BUILD_MONTH_IS_MAY) ? '5' : \
        (BUILD_MONTH_IS_JUN) ? '6' : \
        (BUILD_MONTH_IS_JUL) ? '7' : \
        (BUILD_MONTH_IS_AUG) ? '8' : \
        (BUILD_MONTH_IS_SEP) ? '9' : \
        (BUILD_MONTH_IS_OCT) ? '0' : \
        (BUILD_MONTH_IS_NOV) ? '1' : \
        (BUILD_MONTH_IS_DEC) ? '2' : \
        /* error default */    '?' \
    )

#define BUILD_DAY_CH0 ((__DATE__[4] >= '0') ? (__DATE__[4]) : '0')
#define BUILD_DAY_CH1 (__DATE__[ 5])



// Example of __TIME__ string: "21:06:19"
//                              01234567

#define BUILD_HOUR_CH0 (__TIME__[0])
#define BUILD_HOUR_CH1 (__TIME__[1])

#define BUILD_MIN_CH0 (__TIME__[3])
#define BUILD_MIN_CH1 (__TIME__[4])

#define BUILD_SEC_CH0 (__TIME__[6])
#define BUILD_SEC_CH1 (__TIME__[7])



#if VERSION_MAJOR > 100

#define VERSION_MAJOR_INIT \
    ((VERSION_MAJOR / 100) + '0'), \
    (((VERSION_MAJOR % 100) / 10) + '0'), \
    ((VERSION_MAJOR % 10) + '0')

#elif VERSION_MAJOR > 10

#define VERSION_MAJOR_INIT \
    ((VERSION_MAJOR / 10) + '0'), \
    ((VERSION_MAJOR % 10) + '0')

#else

#define VERSION_MAJOR_INIT \
    (VERSION_MAJOR + '0')

#endif


#if VERSION_MINOR > 100

#define VERSION_MINOR_INIT \
    ((VERSION_MINOR / 100) + '0'), \
    (((VERSION_MINOR % 100) / 10) + '0'), \
    ((VERSION_MINOR % 10) + '0')

#elif VERSION_MINOR > 10

#define VERSION_MINOR_INIT \
    ((VERSION_MINOR / 10) + '0'), \
    ((VERSION_MINOR % 10) + '0')

#else

#define VERSION_MINOR_INIT \
    (VERSION_MINOR + '0')

#endif


#if VERSION_PATCH > 100

#define VERSION_PATCH_INIT \
    ((VERSION_PATCH / 100) + '0'), \
    (((VERSION_PATCH % 100) / 10) + '0'), \
    ((VERSION_PATCH % 10) + '0')

#elif VERSION_PATCH > 10

#define VERSION_PATCH_INIT \
    ((VERSION_PATCH / 10) + '0'), \
    ((VERSION_PATCH % 10) + '0')

#else

#define VERSION_PATCH_INIT \
    (VERSION_PATCH + '0')

#endif

#endif // BUILD_DEFS
