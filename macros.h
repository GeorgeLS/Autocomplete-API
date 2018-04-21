#if !defined(__MACROS_HEADER__)
#define __MACROS_HEADER__

#include <stdlib.h>

#define MALLOC(size, type) ((type*) malloc((size) * sizeof(type)))

#if __GNUC__

#define _FORCE_INLINE __attribute__((always_inline))
#define _NO_RET __attribute__((noreturn))

#else

#define _FORCE_INLINE
#define _NO_RET

#endif

#endif