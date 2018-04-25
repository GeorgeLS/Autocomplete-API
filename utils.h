#if !defined(__UTILS_HEADER__)
#define __UTILS_HEADER__

#include <stdlib.h>
#include <stddef.h>
#include <wchar.h>

#include "macros.h"

/* Macros and functions for strechy buffers */

typedef struct BufHdr {
  size_t len;
  size_t cap;
  wchar_t buf[];
} BufHdr;

void _NO_RET fatal(const char* fmt, ...);
void* buf__grow(const void* buf, size_t new_len, size_t elem_size);
char* buf__print(wchar_t* buf, const wchar_t* fmt, ...);
void* xmalloc(size_t num_bytes);
void* xrealloc(void* ptr, size_t numb_bytes);

#define buf__hdr(b) ((BufHdr*)((char*)(b) - offsetof(BufHdr, buf)))

#define buf_len(b) ((b) ? buf__hdr(b)->len : 0)
#define buf_cap(b) ((b) ? buf__hdr(b)->cap : 0)
#define buf_end(b) ((b) + buf_len(b))
#define buf_sizeof(b) ((b) ? buf_len(b) * sizeof(*b) : 0)

#define buf_free(b) ((b) ? (free(buf__hdr(b)), (b) = NULL) : 0)
#define buf_fit(b, n) ((n) <= buf_cap(b) ? 0 : ((b) = buf__grow((b), (n), sizeof(*(b)))))
#define buf_push(b, ...) (buf_fit((b), 1 + buf_len(b)), buf__hdr(b)->buf[buf__hdr(b)->len++] = (__VA_ARGS__))
#define buf_printf(b, ...) ((b) = buf__printf((b), __VA_ARGS__))
#define buf_clear(b) ((b) ? buf__hdr(b)->len = 0 : 0)

#endif