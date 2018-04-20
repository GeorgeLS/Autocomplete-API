#if !defined(__MACROS_HEADER__)
#define __MACROS_HEADER__

#if __GNUC__

#define _FORCE_INLINE __attribute__((always_inline))
#define _NO_RET __attribute__((noreturn))

#else

#define _FORCE_INLINE
#define _NO_RET

#endif

#endif