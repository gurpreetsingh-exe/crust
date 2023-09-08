#ifndef LEXER_H
#define LEXER_H

#include <span/source_map.h>

typedef struct {
  const SourceFile* file;
  char c;
  usize index;
  u32 line;
  u32 col;
} Lexer;

NODISCARD Lexer lexer_new(const SourceFile* file);
void lexer_bump(Lexer* lexer);

#endif // !LEXER_H
