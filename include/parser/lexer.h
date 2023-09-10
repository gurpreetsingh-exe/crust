#ifndef LEXER_H
#define LEXER_H

#include <ast/token.h>
#include <span/source_map.h>

typedef struct {
  const SourceFile* file;
  char c;
  usize index;
  u32 line;
  u32 col;
} Lexer;

[[nodiscard]] Lexer lexer_new(const SourceFile* file);
void lexer_bump(Lexer* lexer);
void lexer_skip_whitespace(Lexer* lexer);
[[nodiscard]] Token lexer_next_token(Lexer* lexer);

#endif // !LEXER_H
