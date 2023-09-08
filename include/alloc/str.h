#ifndef STR_H
#define STR_H

typedef struct {
  char* buf;
  usize len;
} str;

NODISCARD str str_new(const char* s);
void str_drop(str* s);

#define str_is_empty(s) ((s).len == 0)
#define str_is_null(s) ((s).buf == NULL)

#endif // !STR_H
