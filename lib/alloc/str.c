#include <alloc/alloc.h>
#include <alloc/str.h>

str str_new(const char* cstr) {
  usize len = strlen(cstr);
  if (len <= 0) {
    return (str) {};
  }
  str s = { .buf = alloc_zeroed(char, len + 1), .len = len };
  memcpy(s.buf, cstr, len);
  return s;
}

str str_from_view(string_view string) {
  if (string.len <= 0) {
    return (str) {};
  }
  str s = { .buf = alloc_zeroed(char, string.len + 1), .len = string.len };
  memcpy(s.buf, string.buf, string.len);
  return s;
}

// bool str_eq(str a, str b) {
//   return a.len == b.len && *a.buf == *b.buf && memcmp(a.buf, b.buf, a.len);
// }

void str_drop(str* s) {
  drop(s->buf);
  s->buf = NULL;
  s->len = 0;
}
