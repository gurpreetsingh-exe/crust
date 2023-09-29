#include <ctype.h>
#include <parser/lexer.hh>

namespace crust {

auto Lexer::bump() -> void {
  m_prev = m_c;
  if (m_index < m_src.size() - 1) {
    m_c = m_src[++m_index];
  } else {
    ++m_index;
    m_c = 0;
  }
}

auto Lexer::first() -> char {
  if (m_index < m_src.size() - 2) {
    return m_src[m_index + 1];
  } else {
    return 0;
  }
}

auto Lexer::symbol(usize start) -> Symbol {
  auto string = std::string_view(m_src.data() + start, m_index - start);
  return Symbol::intern(string);
}

auto Lexer::next() -> Token {
  m_preceded_by_whitespace = false;
  while (true) {
    usize start = m_index;
    Option<TokenKind> kind = {};
    Symbol sym = { 0 };

    switch (m_c) {
      case ';': {
        kind = TokenKind::Semi;
        bump();
      } break;
      case ',': {
        kind = TokenKind::Comma;
        bump();
      } break;
      case '.': {
        kind = TokenKind::Dot;
        bump();
      } break;
      case '(': {
        kind = TokenKind::OpenParenthesis;
        bump();
      } break;
      case ')': {
        kind = TokenKind::CloseParenthesis;
        bump();
      } break;
      case '{': {
        kind = TokenKind::OpenBrace;
        bump();
      } break;
      case '}': {
        kind = TokenKind::CloseBrace;
        bump();
      } break;
      case '[': {
        kind = TokenKind::OpenBracket;
        bump();
      } break;
      case ']': {
        kind = TokenKind::CloseBracket;
        bump();
      } break;
      case '@': {
        kind = TokenKind::At;
        bump();
      } break;
      case '#': {
        kind = TokenKind::Pound;
        bump();
      } break;
      case '~': {
        kind = TokenKind::Tilde;
        bump();
      } break;
      case '?': {
        kind = TokenKind::Question;
        bump();
      } break;
      case ':': {
        kind = TokenKind::Colon;
        bump();
      } break;
      case '$': {
        kind = TokenKind::Dollar;
        bump();
      } break;
      case '=': {
        kind = TokenKind::Eq;
        bump();
      } break;
      case '!': {
        kind = TokenKind::Not;
        bump();
      } break;
      case '<': {
        kind = TokenKind::Lt;
        bump();
      } break;
      case '>': {
        kind = TokenKind::Gt;
        bump();
      } break;
      case '-': {
        kind = TokenKind::Minus;
        bump();
      } break;
      case '&': {
        kind = TokenKind::And;
        bump();
      } break;
      case '|': {
        kind = TokenKind::Or;
        bump();
      } break;
      case '+': {
        kind = TokenKind::Plus;
        bump();
      } break;
      case '*': {
        kind = TokenKind::Star;
        bump();
      } break;
      case '/': {
        bump();
        if (m_c == '/') {
          kind = line_comment();
          if (not kind.has_value()) {
            m_preceded_by_whitespace = true;
            continue;
          }
          sym = symbol(start);
          return Token(kind.value(), Span((u32)start, (u32)m_index), sym);
        } else if (m_c == '*') {
          kind = block_comment();
          if (not kind.has_value()) {
            m_preceded_by_whitespace = true;
            continue;
          }
          sym = symbol(start);
          return Token(kind.value(), Span((u32)start, (u32)m_index), sym);
        }
        kind = TokenKind::Slash;
      } break;
      case '^': {
        kind = TokenKind::Caret;
        bump();
      } break;
      case '%': {
        kind = TokenKind::Percent;
        bump();
      } break;
      case '\'': {
        bump();
        kind = lifetime_or_char(&sym, start);
        if (kind == TokenKind::LitChar) {
          auto lit = Literal { .sym = sym, .suffix = { 0 } };
          return Token(kind.value(), Span((u32)start, (u32)m_index), lit);
        } else if (kind == TokenKind::Lifetime) {
          return Token(kind.value(), Span((u32)start, (u32)m_index), sym);
        }
      } break;
      case '"': {
        bump();
        bool terminated = double_quoted_string();
        // usize suffix_start = m_index;
        if (terminated) {
          sym = symbol(start);
          eat_literal_suffix();
        }
        auto lit = Literal { .sym = sym, .suffix = { 0 } };
        return Token(TokenKind::LitStr, Span((u32)start, (u32)m_index), lit);
      } break;
      case '\0': {
        kind = TokenKind::Eof;
      } break;
      default: {
        if (isalpha(m_c) || m_c == '_') {
          eat_identifier();
          sym = symbol(start);
          return Token(TokenKind::Ident, Span((u32)start, (u32)m_index), sym);
        } else if (isdigit(m_c)) {
          Base base = Decimal;
          kind = number(&base);
          sym = symbol(start);
          Literal lit = { .sym = sym, .suffix = { 0 } };
          return Token(kind.value(), Span((u32)start, (u32)m_index), lit);
        } else if (is_whitespace(m_c)) {
          m_preceded_by_whitespace = true;
          bump();
          continue;
        }
      } break;
    }

    ASSERT(kind.has_value(), "(%c, %d)", m_c, m_c);
    return Token(kind.value(), Span((u32)start, (u32)m_index), sym);
  }
}

auto Lexer::number(Base* base) -> TokenKind {
  if (m_c == '0') {
    bump();
    switch (m_c) {
      case 'b':
        *base = Binary;
        TODO();
      case 'o':
        TODO();
      case 'x':
        TODO();
      case '0' ... '9':
      case '_':
        eat_decimal_digits();
        break;
      case '.':
      case 'e':
      case 'E':
        break;
      default:
        return TokenKind::LitInteger;
    }
  } else {
    eat_decimal_digits();
  }

  if (m_c == '.' && first() != '.') {
    bump();
    TODO("float");
  } else if (m_c == 'e' || m_c == 'E') {
    TODO();
  } else {
    return TokenKind::LitInteger;
  }
  UNREACHABLE();
}

auto Lexer::line_comment() -> Option<TokenKind> {
  bump();
  Option<TokenKind> kind = {};

  if (m_c == '!') {
    // `//!` inner doc comment
    kind = TokenKind::DocCommentInner;
  } else if (m_c == '/' && first() != '/') {
    // `///` is an outer doc comment, `////` (more than 3 slashes) is not
    kind = TokenKind::DocCommentOuter;
  }

  while (m_c != '\n') bump();
  return kind;
}

auto Lexer::block_comment() -> Option<TokenKind> {
  bump();
  Option<TokenKind> kind = {};

  if (m_c == '!') {
    // `/*!` inner block doc comment
    kind = TokenKind::DocCommentInner;
  } else if (m_c == '*' && first() != '*' && first() != '/') {
    // `/**` is an outer block doc comment
    // `/***` and `/**/` are not
    kind = TokenKind::DocCommentOuter;
  }

  usize depth = 1;
  while (true) {
    if (m_c == '/' && first() == '*') {
      bump();
      depth++;
    } else if (m_c == '*' && first() == '/') {
      bump();
      depth--;
      if (depth == 0)
        break;
    }
    bump();
  }

  if (depth != 0) {
    PANIC("block comment not terminated");
  }

  return kind;
}

#define IS_ID_START(c)                                                         \
  ((c) == '_' || ('a' <= (c) && (c) <= 'z') || ('A' <= (c) && (c) <= 'Z') ||   \
   ((c) > '\x7f'))

auto Lexer::lifetime_or_char(Symbol* sym, usize start) -> TokenKind {
  ASSERT(m_prev == '\'');
  bool can_be_lifetime = false;
  if (first() == '\'') {
    can_be_lifetime = false;
  } else {
    can_be_lifetime = IS_ID_START(m_c) || isdigit(m_c);
  }

  if (!can_be_lifetime) {
    bool terminated = single_quoted_string();
    // usize suffix_start = m_index;
    if (terminated) {
      *sym = symbol(start);
      eat_literal_suffix();
      return TokenKind::LitChar;
    }
  }

  if (isdigit(m_c)) {
    PANIC("lifetime cannot start with a number");
  }

  bump();
  while (isalnum(m_c) || m_c == '_') bump();
  if (m_c == '\'') {
    bump();
    *sym = symbol(start);
    return TokenKind::LitChar;
  }

  *sym = symbol(start);
  return TokenKind::Lifetime;
}

auto Lexer::single_quoted_string() -> bool {
  ASSERT(m_prev == '\'');
  if (first() == '\'' && m_c != '\\') {
    bump();
    bump();
    return true;
  }

  while (true) {
    switch (m_c) {
      case '\'': {
        bump();
        return true;
      }
      case '/':
        goto end;
      case '\n': {
        if (first() != '\'')
          goto end;
      } break;
      case '\0': {
        if (m_c == '\0')
          goto end;
      } break;
      case '\\': {
        bump();
        bump();
      } break;
      default: {
        bump();
      } break;
    }
  }
end:
  return false;
}

auto Lexer::double_quoted_string() -> bool {
  ASSERT(m_prev == '"');
  while (true) {
    char c = m_c;
    bump();
    if (c == '"') {
      return true;
    } else if (c == '\\' && (m_c == '\\' || m_c == '"')) {
      bump();
    }
  }
  return false;
}

auto Lexer::eat_decimal_digits() -> bool {
  bool has_digits = false;

  while (true) {
    if (m_c == '_') {
      bump();
      continue;
    }
    if (isdigit(m_c)) {
      has_digits = true;
      bump();
      continue;
    }
    break;
  }

  return has_digits;
}

auto Lexer::eat_literal_suffix() -> void { eat_identifier(); }

auto Lexer::eat_identifier() -> void {
  if (!IS_ID_START(m_c))
    return;

  bump();
  while (isalnum(m_c) || m_c == '_') bump();
}

} // namespace crust
