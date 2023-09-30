#ifndef SYMBOL_H
#define SYMBOL_H

#include <span/span.hh>

namespace crust {

/// Special reserved identifiers used internally for elided lifetimes,
/// unnamed method parameters, crate root module, error recovery etc.
/// https://github.com/rust-lang/rust/blob/5ede9408945b46ab183dd228253297bdf62304f7/compiler/rustc_span/src/symbol.rs#L22
#define KEYWORDS()                                                             \
  X(Empty, "")                                                                 \
  X(PathRoot, "{{root}}")                                                      \
  X(DollarCrate, "$crate")                                                     \
  X(Underscore, "_")                                                           \
  X(As, "as")                                                                  \
  X(Break, "break")                                                            \
  X(Const, "const")                                                            \
  X(Continue, "continue")                                                      \
  X(Crate, "crate")                                                            \
  X(Else, "else")                                                              \
  X(Enum, "enum")                                                              \
  X(Extern, "extern")                                                          \
  X(False, "false")                                                            \
  X(Fn, "fn")                                                                  \
  X(For, "for")                                                                \
  X(If, "if")                                                                  \
  X(Impl, "impl")                                                              \
  X(In, "in")                                                                  \
  X(Let, "let")                                                                \
  X(Loop, "loop")                                                              \
  X(Match, "match")                                                            \
  X(Mod, "mod")                                                                \
  X(Move, "move")                                                              \
  X(Mut, "mut")                                                                \
  X(Pub, "pub")                                                                \
  X(Ref, "ref")                                                                \
  X(Return, "return")                                                          \
  X(SelfLower, "self")                                                         \
  X(SelfUpper, "Self")                                                         \
  X(Static, "static")                                                          \
  X(Struct, "struct")                                                          \
  X(Super, "super")                                                            \
  X(Trait, "trait")                                                            \
  X(True, "true")                                                              \
  X(Type, "type")                                                              \
  X(Unsafe, "unsafe")                                                          \
  X(Use, "use")                                                                \
  X(Where, "where")                                                            \
  X(While, "while")                                                            \
  X(Abstract, "abstract")                                                      \
  X(Become, "become")                                                          \
  X(Box, "box")                                                                \
  X(Do, "do")                                                                  \
  X(Final, "final")                                                            \
  X(Macro, "macro")                                                            \
  X(Override, "override")                                                      \
  X(Priv, "priv")                                                              \
  X(Typeof, "typeof")                                                          \
  X(Unsized, "unsized")                                                        \
  X(Virtual, "virtual")                                                        \
  X(Yield, "yield")                                                            \
  X(Async, "async")                                                            \
  X(Await, "await")                                                            \
  X(Dyn, "dyn")                                                                \
  X(Try, "try")                                                                \
  X(UnderscoreLifetime, "'_")                                                  \
  X(StaticLifetime, "'static")                                                 \
  X(Auto, "auto")                                                              \
  X(Builtin, "builtin")                                                        \
  X(Catch, "catch")                                                            \
  X(Default, "default")                                                        \
  X(MacroRules, "macro_rules")                                                 \
  X(Raw, "raw")                                                                \
  X(Union, "union")                                                            \
  X(Yeet, "yeet")

struct Symbol {
  u32 inner;

  Symbol() {};
  Symbol(u32 index) : inner(index) {}
  auto operator==(Symbol other) -> bool { return inner == other.inner; }
  auto operator==(Symbol other) const -> bool { return inner == other.inner; }
  auto operator<=(Symbol other) -> bool { return inner <= other.inner; }
  auto operator>=(Symbol other) -> bool { return inner >= other.inner; }

  static auto intern(std::string_view string) -> Symbol;
  auto get() -> std::string_view;

  auto is_special() -> bool;
  auto is_used_keyword_always() -> bool;
  auto is_used_keyword_conditional() -> bool;
  auto is_unused_keyword_always() -> bool;
  auto is_unused_keyword_conditional() -> bool;
  auto is_reserved() -> bool;
  auto is_path_segment_keyword() -> bool;
  auto is_bool_lit() -> bool;
  auto can_be_raw() -> bool;
};

struct Ident {
  Symbol name;
  Span span;

  Ident(Symbol n, Span sp) : name(n), span(sp) {}

  static auto from_str(std::string_view s) -> Ident;

  auto is_special() -> bool { return name.is_special(); }

  auto is_used_keyword() -> bool {
    return name.is_used_keyword_always() or name.is_used_keyword_conditional();
  }

  auto is_unused_keyword() -> bool {
    return name.is_unused_keyword_always() or
           name.is_unused_keyword_conditional();
  }

  auto is_path_segment_keyword() -> bool {
    return name.is_path_segment_keyword();
  }
};

struct Interner {
  std::vector<std::string_view> strings;
  std::unordered_map<std::string_view, Symbol> names;

  auto fresh() -> void;
  auto clear() -> void;
  auto intern(std::string_view string) -> Symbol;
};

/// Symbol interner
extern Interner interner;

namespace kw {
#define X(kw, unused) extern Symbol kw;
KEYWORDS()
#undef X
} // namespace kw

} // namespace crust

template <>
struct fmt::formatter<crust::Symbol> {
  constexpr auto parse(format_parse_context& ctx)
      -> format_parse_context::iterator {
    return ctx.begin();
  }

  auto format(crust::Symbol sym, format_context& ctx)
      -> format_context::iterator {
    return fmt::format_to(ctx.out(), "{}", sym.get());
  }
};

#endif // !SYMBOL_H
