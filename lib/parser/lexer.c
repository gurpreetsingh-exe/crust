#include <ctype.h>
#include <parser/lexer.h>

Lexer lexer_new(const SourceFile* file) {
  return (Lexer) {
    .file = file,
    .c = file->src[0],
    .index = 0,
    .line = 1,
    .col = 1,
  };
}

void lexer_bump(Lexer* lexer) {
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

void lexer_skip_whitespace(Lexer* lexer) {
  while (isspace(lexer->c)) lexer_bump(lexer);
}

Token lexer_next_token(Lexer* lexer) {
  lexer_skip_whitespace(lexer);
  usize start = lexer->index;
  TokenKind kind = {};
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
      kind = Slash;
      lexer_bump(lexer);
    } break;
    case '^': {
      kind = Caret;
      lexer_bump(lexer);
    } break;
    case '%': {
      kind = Percent;
      lexer_bump(lexer);
    } break;
    case 0: {
      kind = Eof;
      lexer_bump(lexer);
    } break;
    default: {
      if (isalpha(lexer->c) || lexer->c == '_') {
        while (isalnum(lexer->c) || lexer->c == '_') lexer_bump(lexer);
        string_view string =
            string_view_new(lexer->file->src + start, lexer->index - start);
        sym = symbol_intern(string);
        kind = Ident;
      }
    } break;
  }

  ASSERT(kind, "(%c, %d)", lexer->c, lexer->c);
  return (Token) {
    .kind = kind,
    .span = span_new((u32)start, (u32)lexer->index),
    .sym = sym,
  };
}
