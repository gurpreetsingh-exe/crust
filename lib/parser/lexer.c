#include <parser/lexer.h>

Lexer lexer_new(const SourceFile* file) {
  return (Lexer) {
    .file = file,
    .c = 0,
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
