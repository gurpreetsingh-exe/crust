#include <span/symbol.h>

Interner interner = {};

#define X(kw, unused) Symbol kw = { .inner = __COUNTER__ };
KEYWORDS()
#undef X

Symbol symbol_intern(string_view symbol) {
  str string = str_from_view(symbol);
  return interner_intern(string);
}

str symbol_get(Symbol symbol) {
  return interner.strings.ptr[(usize)symbol.inner];
}

void interner_new() {
  vector_init(str, interner.strings);
  hash_map_init(str, Symbol, interner.names);
}

void interner_fresh() {
  interner_new();
  str string = {};
#define X(unused, val)                                                         \
  do {                                                                         \
    string = str_new(val);                                                     \
    Symbol sym = { .inner = (u32)interner.strings.len };                       \
    vector_push(str, interner.strings, string);                                \
    hash_map_insert(interner.names, string, sym.inner)                         \
  } while (0);
  KEYWORDS()
#undef X
}

Symbol interner_intern(str string) {
  Symbol value = {};
  hash_map_get(interner.names, string, value.inner);
  if ((i32)value.inner >= 0) {
    str_drop(&string);
    return value;
  }

  value = (Symbol) { .inner = (u32)interner.strings.len };
  vector_push(str, interner.strings, string);
  hash_map_insert(interner.names, string, value.inner);
  return value;
}

void interner_drop() {
  hash_map_drop(interner.names);
  vector_map(str, interner.strings, string, str_drop(string));
  vector_drop(interner.strings);
}
