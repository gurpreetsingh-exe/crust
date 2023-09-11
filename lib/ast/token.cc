#include <ast/token.hh>

auto token_kind_to_cstr(TokenKind kind) -> std::string_view {
  switch (kind) {
    case Eq:
      return "=";
    case Lt:
      return "<";
    case Le:
      return "<=";
    case EqEq:
      return "==";
    case Ne:
      return "!=";
    case Ge:
      return ">=";
    case Gt:
      return ">";
    case AndAnd:
      return "&&";
    case OrOr:
      return "||";
    case Not:
      return "!";
    case Tilde:
      return "~";
    case At:
      return "@";
    case Dot:
      return ".";
    case DotDot:
      return "..";
    case DotDotDot:
      return "...";
    case DotDotEq:
      return "..=";
    case Comma:
      return ",";
    case Semi:
      return ";";
    case Colon:
      return ":";
    case ModSep:
      return "::";
    case RArrow:
      return "->";
    case LArrow:
      return "<-";
    case FatArrow:
      return "=>";
    case Pound:
      return "#";
    case Dollar:
      return "$";
    case Question:
      return "?";
    case SingleQuote:
      return "'";
    case Ident:
      return "Ident";
    case Lifetime:
      return "'a";
    case DocCommentInner:
      return "//!";
    case DocCommentOuter:
      return "///";
    case Eof:
      return "<eof>";
#define X(bin_op)                                                              \
  case bin_op:                                                                 \
    return #bin_op;
      BINARY_OPS()
#undef X

#define X(bin_op)                                                              \
  case bin_op##Eq:                                                             \
    return CAT(bin_op, Eq);
      BINARY_OPS()
#undef X

#define X(delim)                                                               \
  case Open##delim:                                                            \
    return CAT(Open, delim);
      DELIMITERS()
#undef X

#define X(delim)                                                               \
  case Close##delim:                                                           \
    return CAT(Close, delim);
      DELIMITERS()
#undef X

#define X(lit)                                                                 \
  case Lit##lit:                                                               \
    return #lit;
      LITERALS()
#undef X
  }

  UNREACHABLE();
}

auto operator<<(std::ostream& s, const Token& t) -> std::ostream& {
  s << "Token { kind: " << token_kind_to_cstr(t.kind)
    << ", span: { lo: " << t.span.lo << ", hi: " << t.span.hi << " }";

  PUSH_IGNORE_WARNING("-Wswitch-enum")
  switch (t.kind) {
    default:
      break;
    case Ident:
    case DocCommentOuter:
    case DocCommentInner:
    case Lifetime: {
      auto sym = std::get<Symbol>(t.extra);
      s << ", sym: \"" << sym.get() << "\"";
    } break;
#define X(lit) case Lit##lit:
      LITERALS()
#undef X
      auto lit = std::get<Literal>(t.extra);
      s << ", lit: \"" << lit.sym.get() << "\"";
      break;
  }
  POP_IGNORE_WARNING()

  return s << " }";
}
