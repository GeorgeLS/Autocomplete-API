#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>

#include "utils.h"
#include "macros.h"

void _NO_RET fatal(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  printf("FATAL: ");
  vprintf(fmt, args);
  printf("\n");
  va_end(args);
  exit(EXIT_FAILURE);
}

void* buf__grow(const void* buf, size_t new_len, size_t elem_size) {
  assert(buf_cap(buf) <= (SIZE_MAX - 1)/2);
  size_t new_cap = MAX(16, MAX(1 + 2*buf_cap(buf), new_len));
  assert(new_len <= new_cap);
  assert(new_cap <= (SIZE_MAX - offsetof(BufHdr, buf))/elem_size);
  size_t new_size = offsetof(BufHdr, buf) + new_cap*elem_size;
  BufHdr* new_hdr;
  if (buf) {
    new_hdr = xrealloc(buf__hdr(buf), new_size);
  } else {
    new_hdr = xmalloc(new_size);
    new_hdr->len = 0;
  }
  new_hdr->cap = new_cap;
  return new_hdr->buf;
}

char* buf__printf(char* buf, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  size_t cap = buf_cap(buf) - buf_len(buf);
  size_t n = 1 + vsnprintf(buf_end(buf), cap, fmt, args);
  va_end(args);
  if (n > cap) {
    buf_fit(buf, n + buf_len(buf));
    va_start(args, fmt);
    size_t new_cap = buf_cap(buf) - buf_len(buf);
    n = 1 + vsnprintf(buf_end(buf), new_cap, fmt, args);
    assert(n <= new_cap);
    va_end(args);
  }
  buf__hdr(buf)->len += n - 1;
  return buf;
}

void* xrealloc(void* ptr, size_t num_bytes) {
  ptr = realloc(ptr, num_bytes);
  if (!ptr) {
    perror("xrealloc failed");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void* xmalloc(size_t num_bytes) {
  void* ptr = malloc(num_bytes);
  if (!ptr) {
    perror("xmalloc failed");
    exit(EXIT_FAILURE);
  }
  return ptr;
}