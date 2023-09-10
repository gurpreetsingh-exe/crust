#ifndef LEXER_H
#define LEXER_H

#include <ast/token.h>
#include <span/source_map.h>

typedef struct {
  const SourceFile* file;
  char c;
  char prev;
  usize index;
  u32 line;
  u32 col;
  bool preceded_by_whitespace;
} Lexer;

[[nodiscard]] Lexer lexer_new(const SourceFile* file);
void lexer_bump(Lexer* lexer);
char lexer_first(Lexer* lexer);
Symbol lexer_symbol(Lexer* lexer, usize start);
[[nodiscard]] Token lexer_next_token(Lexer* lexer);
TokenKind lexer_number(Lexer* lexer, Base* base);
TokenKind lexer_line_comment(Lexer* lexer);
TokenKind lexer_block_comment(Lexer* lexer);
TokenKind lexer_lifetime_or_char(Lexer* lexer, Symbol* sym, usize start);
bool lexer_single_quoted_string(Lexer* lexer);
bool lexer_eat_decimal_digits(Lexer* lexer);
void lexer_eat_literal_suffix(Lexer* lexer);
void lexer_eat_identifier(Lexer* lexer);

#endif // !LEXER_H
