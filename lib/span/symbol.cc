#include <span/symbol.hh>

Interner interner = {};

#define X(kw, unused) Symbol kw(__COUNTER__);
KEYWORDS()
#undef X

auto Symbol::intern(std::string_view symbol) -> Symbol {
  return interner.intern(symbol);
}

auto Symbol::get() -> std::string_view {
  return interner.strings[static_cast<usize>(inner)];
}

auto Interner::fresh() -> void {
#define X(unused, val)                                                         \
  do {                                                                         \
    std::string_view string = val;                                             \
    Symbol sym(static_cast<u32>(interner.strings.size()));                     \
    interner.strings.push_back(string);                                        \
    interner.names[string] = sym;                                              \
  } while (0);
  KEYWORDS()
#undef X
}

auto Interner::intern(std::string_view string) -> Symbol {
  if (interner.names.contains(string)) {
    return interner.names.at(string);
  }

  Symbol sym(static_cast<u32>(interner.strings.size()));
  interner.strings.push_back(string);
  interner.names[string] = sym;
  return sym;
}
