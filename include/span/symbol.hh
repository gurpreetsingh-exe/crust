#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <unordered_map>
#include <vector>

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
  static auto intern(std::string_view string) -> Symbol;
  auto get() -> std::string_view;
};

struct Interner {
  std::vector<std::string_view> strings;
  std::unordered_map<std::string_view, Symbol> names;

  auto fresh() -> void;
  auto intern(std::string_view string) -> Symbol;
};

/// Symbol interner
extern Interner interner;

#define X(kw, unused) extern Symbol kw;
KEYWORDS()
#undef X

#endif // !SYMBOL_H
