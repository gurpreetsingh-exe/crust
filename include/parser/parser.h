#ifndef PARSER_H
#define PARSER_H

#include <parser/lexer.h>

typedef struct {
  Lexer lexer;
  const SourceFile* file;
} ParseCtx;

[[nodiscard]] ParseCtx* parser_from_file(const SourceFile* file);
void parse(ParseCtx* pcx);

#endif // !PARSER_H
