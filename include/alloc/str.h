#ifndef STR_H
#define STR_H

typedef struct {
  const char* buf;
  usize len;
} string_view;

#define string_view_new(s, l)                                                  \
  (string_view) { .buf = (s), .len = (l) }

typedef struct {
  char* buf;
  usize len;
} str;

typedef struct {
  const void* ptr;
  usize len;
} slice;

#define slice_new(slice, start, end)                                           \
  ({                                                                           \
    ASSERT(end - start == slice.len);                                          \
    (slice) { .ptr = slice.ptr + s, .len }                                     \
  })

[[nodiscard]] str str_new(const char* s);
[[nodiscard]] str str_from_view(string_view s);
[[gnu::always_inline]] inline bool str_eq(str a, str b) {
  return a.len == b.len && memcmp(a.buf, b.buf, a.len) == 0;
}
void str_drop(str* s);

#define str_is_empty(s) ((s).len == 0)
#define str_is_null(s) ((s).buf == NULL)

#endif // !STR_H
