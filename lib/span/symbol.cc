#include <span/symbol.hh>

namespace crust {

Interner interner = {};

namespace kw {
#define X(kw, unused) Symbol kw(__COUNTER__);
KEYWORDS()
#undef X
} // namespace kw

auto Symbol::intern(std::string_view symbol) -> Symbol {
  return interner.intern(symbol);
}

auto Symbol::get() -> std::string_view {
  return interner.strings[static_cast<usize>(inner)];
}

auto Symbol::is_special() -> bool { return *this <= kw::Underscore; }

auto Symbol::is_used_keyword_always() -> bool {
  return *this >= kw::As and *this <= kw::While;
}

auto Symbol::is_used_keyword_conditional() -> bool {
  return (*this >= kw::Async and
          *this <= kw::Dyn); // and edition() >= Edition::Edition2018
}

auto Symbol::is_unused_keyword_always() -> bool {
  return *this >= kw::Abstract and *this <= kw::Yield;
}

auto Symbol::is_unused_keyword_conditional() -> bool {
  return *this == kw::Try; // and edition() >= Edition::Edition2018;
}

auto Symbol::is_reserved() -> bool {
  return is_special() or is_used_keyword_always() or
         is_unused_keyword_always() or
         is_used_keyword_conditional(/* edition */) or
         is_unused_keyword_conditional(/* edition */);
}

auto Symbol::is_path_segment_keyword() -> bool {
  return *this == kw::Super or *this == kw::SelfLower or
         *this == kw::SelfUpper or *this == kw::Crate or
         *this == kw::PathRoot or *this == kw::DollarCrate;
}

auto Symbol::is_bool_lit() -> bool {
  return *this == kw::True or *this == kw::False;
}

auto Symbol::can_be_raw() -> bool {
  return *this != kw::Empty and *this != kw::Underscore and
         not is_path_segment_keyword();
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

} // namespace crust
