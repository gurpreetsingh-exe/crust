#include <alloc/alloc.h>
#include <alloc/str.h>

NODISCARD str str_new(const char* cstr) {
  usize len = strlen(cstr);
  if (len <= 0) {
    return (str) {};
  }
  str s = { .buf = alloc_zeroed(char, len + 1), .len = len };
  memcpy(s.buf, cstr, len);
  return s;
}

void str_drop(str* s) {
  drop(s->buf);
  s->buf = NULL;
  s->len = 0;
}
