#include <ctype.h>
#include <parser/lexer.h>

Lexer lexer_new(const SourceFile* file) {
  return (Lexer) {
    .file = file,
    .c = file->src[0],
    .index = 0,
    .line = 1,
    .col = 1,
    .preceded_by_whitespace = false,
  };
}

void lexer_bump(Lexer* lexer) {
  lexer->prev = lexer->c;
  if (lexer->index < lexer->file->size - 1) {
    lexer->c = lexer->file->src[++lexer->index];
    if (lexer->c == '\n') {
      ++lexer->line;
      lexer->col = 0;
    } else {
      ++lexer->col;
    }
  } else {
    ++lexer->index;
    lexer->c = 0;
  }
}

char lexer_first(Lexer* lexer) {
  if (lexer->index < lexer->file->size - 2) {
    return lexer->file->src[lexer->index + 1];
  } else {
    return 0;
  }
}

Symbol lexer_symbol(Lexer* lexer, usize start) {
  string_view string =
      string_view_new(lexer->file->src + start, lexer->index - start);
  return symbol_intern(string);
}

Token lexer_next_token(Lexer* lexer) {
  lexer->preceded_by_whitespace = false;
  while (true) {
    usize start = lexer->index;
    TokenKind kind = (TokenKind)-1;
    Symbol sym = {};

    switch (lexer->c) {
      case ';': {
        kind = Semi;
        lexer_bump(lexer);
      } break;
      case ',': {
        kind = Comma;
        lexer_bump(lexer);
      } break;
      case '.': {
        kind = Dot;
        lexer_bump(lexer);
      } break;
      case '(': {
        kind = OpenParenthesis;
        lexer_bump(lexer);
      } break;
      case ')': {
        kind = CloseParenthesis;
        lexer_bump(lexer);
      } break;
      case '{': {
        kind = OpenBrace;
        lexer_bump(lexer);
      } break;
      case '}': {
        kind = CloseBrace;
        lexer_bump(lexer);
      } break;
      case '[': {
        kind = OpenBracket;
        lexer_bump(lexer);
      } break;
      case ']': {
        kind = CloseBracket;
        lexer_bump(lexer);
      } break;
      case '@': {
        kind = At;
        lexer_bump(lexer);
      } break;
      case '#': {
        kind = Pound;
        lexer_bump(lexer);
      } break;
      case '~': {
        kind = Tilde;
        lexer_bump(lexer);
      } break;
      case '?': {
        kind = Question;
        lexer_bump(lexer);
      } break;
      case ':': {
        kind = Colon;
        lexer_bump(lexer);
      } break;
      case '$': {
        kind = Dollar;
        lexer_bump(lexer);
      } break;
      case '=': {
        kind = Eq;
        lexer_bump(lexer);
      } break;
      case '!': {
        kind = Not;
        lexer_bump(lexer);
      } break;
      case '<': {
        kind = Lt;
        lexer_bump(lexer);
      } break;
      case '>': {
        kind = Gt;
        lexer_bump(lexer);
      } break;
      case '-': {
        kind = Minus;
        lexer_bump(lexer);
      } break;
      case '&': {
        kind = And;
        lexer_bump(lexer);
      } break;
      case '|': {
        kind = Or;
        lexer_bump(lexer);
      } break;
      case '+': {
        kind = Plus;
        lexer_bump(lexer);
      } break;
      case '*': {
        kind = Star;
        lexer_bump(lexer);
      } break;
      case '/': {
        lexer_bump(lexer);
        if (lexer->c == '/') {
          kind = lexer_line_comment(lexer);
          if ((i32)kind == -1) {
            lexer->preceded_by_whitespace = true;
            continue;
          }
          sym = lexer_symbol(lexer, start);
          return (Token) {
            .kind = kind,
            .span = span_new((u32)start, (u32)lexer->index),
            .extra = { .sym = sym },
          };
        } else if (lexer->c == '*') {
          kind = lexer_block_comment(lexer);
          if ((i32)kind == -1) {
            lexer->preceded_by_whitespace = true;
            continue;
          }
          sym = lexer_symbol(lexer, start);
          return (Token) {
            .kind = kind,
            .span = span_new((u32)start, (u32)lexer->index),
            .extra = { .sym = sym },
          };
        }
        kind = Slash;
      } break;
      case '^': {
        kind = Caret;
        lexer_bump(lexer);
      } break;
      case '%': {
        kind = Percent;
        lexer_bump(lexer);
      } break;
      case '\'': {
        lexer_bump(lexer);
        kind = lexer_lifetime_or_char(lexer, &sym, start);
        if (kind == LitChar) {
          Literal lit = (Literal) { .sym = sym, .suffix = {} };
          return (Token) {
            .kind = kind,
            .span = span_new((u32)start, (u32)lexer->index),
            .extra = { .lit = lit },
          };
        } else if (kind == Lifetime) {
          return (Token) {
            .kind = kind,
            .span = span_new((u32)start, (u32)lexer->index),
            .extra = { .sym = sym },
          };
        }
      } break;
      case '\0': {
        kind = Eof;
      } break;
      default: {
        if (isalpha(lexer->c) || lexer->c == '_') {
          lexer_eat_identifier(lexer);
          sym = lexer_symbol(lexer, start);
          return (Token) {
            .kind = Ident,
            .span = span_new((u32)start, (u32)lexer->index),
            .extra = { .sym = sym },
          };
        } else if (isdigit(lexer->c)) {
          Base base = Decimal;
          kind = lexer_number(lexer, &base);
          sym = lexer_symbol(lexer, start);
          Literal lit = { .sym = sym, .suffix = {} };
          return (Token) {
            .kind = kind,
            .span = span_new((u32)start, (u32)lexer->index),
            .extra = { .lit = lit },
          };
        } else if (is_whitespace(lexer->c)) {
          lexer->preceded_by_whitespace = true;
          lexer_bump(lexer);
          continue;
        }
      } break;
    }

    ASSERT((i32)kind >= 0, "(%c, %d)", lexer->c, lexer->c);
    return (Token) {
      .kind = kind,
      .span = span_new((u32)start, (u32)lexer->index),
      .extra = { sym },
    };
  }
}

TokenKind lexer_number(Lexer* lexer, Base* base) {
  if (lexer->c == '0') {
    lexer_bump(lexer);
    switch (lexer->c) {
      case 'b':
        *base = Binary;
        TODO();
      case 'o':
        TODO();
      case 'x':
        TODO();
      case '0' ... '9':
      case '_':
        lexer_eat_decimal_digits(lexer);
        break;
      case '.':
      case 'e':
      case 'E':
        break;
      default:
        return LitInteger;
    }
  } else {
    lexer_eat_decimal_digits(lexer);
  }

  if (lexer->c == '.' && lexer_first(lexer) != '.') {
    lexer_bump(lexer);
    TODO("float");
  } else if (lexer->c == 'e' || lexer->c == 'E') {
    TODO();
  } else {
    return LitInteger;
  }
  UNREACHABLE();
}

TokenKind lexer_line_comment(Lexer* lexer) {
  lexer_bump(lexer);
  TokenKind kind = {};

  if (lexer->c == '!') {
    // `//!` inner doc comment
    kind = DocCommentInner;
  } else if (lexer->c == '/' && lexer_first(lexer) != '/') {
    // `///` is an outer doc comment, `////` (more than 3 slashes) is not
    kind = DocCommentOuter;
  } else {
    kind = (TokenKind)-1;
  }

  while (lexer->c != '\n') lexer_bump(lexer);
  return kind;
}

TokenKind lexer_block_comment(Lexer* lexer) {
  lexer_bump(lexer);
  TokenKind kind = {};

  if (lexer->c == '!') {
    // `/*!` inner block doc comment
    kind = DocCommentInner;
  } else if (lexer->c == '*' && lexer_first(lexer) != '*' &&
             lexer_first(lexer) != '/') {
    // `/**` is an outer block doc comment
    // `/***` and `/**/` are not
    kind = DocCommentOuter;
  } else {
    kind = (TokenKind)-1;
  }

  usize depth = 1;
  while (true) {
    if (lexer->c == '/' && lexer_first(lexer) == '*') {
      lexer_bump(lexer);
      depth++;
    } else if (lexer->c == '*' && lexer_first(lexer) == '/') {
      lexer_bump(lexer);
      depth--;
      if (depth == 0)
        break;
    }
    lexer_bump(lexer);
  }

  if (depth != 0) {
    PANIC("block comment not terminated");
  }

  return kind;
}

#define IS_ID_START(c)                                                         \
  ((c) == '_' || ('a' <= (c) && (c) <= 'z') || ('A' <= (c) && (c) <= 'Z') ||   \
   ((c) > '\x7f'))

TokenKind lexer_lifetime_or_char(Lexer* lexer, Symbol* sym, usize start) {
  ASSERT(lexer->prev == '\'');
  bool can_be_lifetime = false;
  if (lexer_first(lexer) == '\'') {
    can_be_lifetime = false;
  } else {
    can_be_lifetime = IS_ID_START(lexer->c) || isdigit(lexer->c);
  }

  if (!can_be_lifetime) {
    bool terminated = lexer_single_quoted_string(lexer);
    // usize suffix_start = lexer->index;
    if (terminated) {
      *sym = lexer_symbol(lexer, start);
      lexer_eat_literal_suffix(lexer);
      return LitChar;
    }
  }

  if (isdigit(lexer->c)) {
    PANIC("lifetime cannot start with a number");
  }

  lexer_bump(lexer);
  while (isalnum(lexer->c) || lexer->c == '_') lexer_bump(lexer);
  if (lexer->c == '\'') {
    lexer_bump(lexer);
    *sym = lexer_symbol(lexer, start);
    return LitChar;
  }

  *sym = lexer_symbol(lexer, start);
  return Lifetime;
}

bool lexer_single_quoted_string(Lexer* lexer) {
  if (lexer_first(lexer) == '\'' && lexer->c != '\\') {
    lexer_bump(lexer);
    lexer_bump(lexer);
    return true;
  }

  while (true) {
    switch (lexer->c) {
      case '\'': {
        lexer_bump(lexer);
        return true;
      }
      case '/':
        goto end;
      case '\n': {
        if (lexer_first(lexer) != '\'')
          goto end;
      } break;
      case '\0': {
        if (lexer->c == '\0')
          goto end;
      } break;
      case '\\': {
        lexer_bump(lexer);
        lexer_bump(lexer);
      } break;
      default: {
        lexer_bump(lexer);
      } break;
    }
  }
end:
  return false;
}

bool lexer_eat_decimal_digits(Lexer* lexer) {
  bool has_digits = false;

  while (true) {
    if (lexer->c == '_') {
      lexer_bump(lexer);
      continue;
    }
    if (isdigit(lexer->c)) {
      has_digits = true;
      lexer_bump(lexer);
      continue;
    }
    break;
  }

  return has_digits;
}

void lexer_eat_literal_suffix(Lexer* lexer) { lexer_eat_identifier(lexer); }

void lexer_eat_identifier(Lexer* lexer) {
  if (!IS_ID_START(lexer->c))
    return;

  lexer_bump(lexer);
  while (isalnum(lexer->c) || lexer->c == '_') lexer_bump(lexer);
}
