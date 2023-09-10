#ifndef TOKEN_H
#define TOKEN_H

#include <span/span.h>
#include <span/symbol.h>

#define BINARY_OPS()                                                           \
  X(Plus)                                                                      \
  X(Minus)                                                                     \
  X(Star)                                                                      \
  X(Slash)                                                                     \
  X(Percent)                                                                   \
  X(Caret)                                                                     \
  X(And)                                                                       \
  X(Or)                                                                        \
  X(Shl)                                                                       \
  X(Shr)

#define DELIMITERS()                                                           \
  X(Parenthesis)                                                               \
  X(Brace)                                                                     \
  X(Bracket)                                                                   \
  X(Invisible)

#define LITERALS()                                                             \
  X(Bool)                                                                      \
  X(Byte)                                                                      \
  X(Char)                                                                      \
  X(Integer)                                                                   \
  X(Float)                                                                     \
  X(Str)                                                                       \
  X(StrRaw)                                                                    \
  X(ByteStr)                                                                   \
  X(ByteStrRaw)                                                                \
  X(CStr)                                                                      \
  X(CStrRaw)                                                                   \
  X(Err)

typedef enum TokenKind {
  /// Expression-operator symbols.
  Eq,
  Lt,
  Le,
  EqEq,
  Ne,
  Ge,
  Gt,
  AndAnd,
  OrOr,
  Not,
  Tilde,

#define X(bin_op) bin_op,
  /// Binary operators `+`, `*`
  BINARY_OPS()
#undef X

#define X(bin_op) bin_op##Eq,
  /// Binary eq operators `+=`, `*=`
  BINARY_OPS()
#undef X

  /// Structural symbols
  At,
  Dot,
  DotDot,
  DotDotDot,
  DotDotEq,
  Comma,
  Semi,
  Colon,
  ModSep,
  RArrow,
  LArrow,
  FatArrow,
  Pound,
  Dollar,
  Question,
  /// Used by proc macros for representing lifetimes, not generated by lexer
  /// right now.
  SingleQuote,

#define X(delim) Open##delim,
  /// An opening delimiter (e.g., `{`).
  DELIMITERS()
#undef X

#define X(delim) Close##delim,
  /// A closing delimiter (e.g., `}`).
  DELIMITERS()
#undef X

#define X(lit) Lit##lit,
  /// Literals
  LITERALS()
#undef X

  /// Identifier token.
  Ident,
  /// Lifetime identifier token.
  Lifetime,
  /// A doc comment token `//!`, `/*!`.
  DocCommentInner,
  /// A doc comment token `///`, `/**`.
  DocCommentOuter,
  Eof,
} TokenKind;

const char* token_kind_to_cstr(TokenKind kind);

typedef struct {
  TokenKind kind;
  Span span;
  /// Optional field for `Ident`, `Lifetime` and `DocComment` tags
  Symbol sym;
} Token;

void token_display(FILE* f, Token t);

#endif // !TOKEN_H
