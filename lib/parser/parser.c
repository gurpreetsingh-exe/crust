#include <alloc/alloc.h>
#include <parser/parser.h>

ParseCtx* parser_from_file(const SourceFile* file) {
  ParseCtx* pcx = alloc(ParseCtx);
  pcx->file = file;
  pcx->lexer = lexer_new(file);
  return pcx;
}

void parse(ParseCtx* pcx) {
  while (true) {
    Token t = lexer_next_token(&pcx->lexer);
    token_display(stdout, t);
    printf("\n");
    if (t.kind == Eof)
      break;
  }
}
