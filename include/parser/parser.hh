#ifndef PARSER_H
#define PARSER_H

#include <ast/token.hh>
#include <parser/lexer.hh>
#include <session/parse.hh>

namespace crust {

class Parser {
public:
  Parser(ParseSess& sess, Rc<SourceFile> file) : m_sess(sess) {
    m_lexer = std::make_unique<Lexer>(file->content);
    bump();
  }

public:
  auto parse() -> void;

private:
  auto bump() -> void;

private:
  ParseSess& m_sess;
  Box<Lexer> m_lexer;
  std::vector<Token> m_tokens;
  Token m_token;
  usize m_idx = SIZE_MAX;
};

[[nodiscard]]
auto parser_from_file(ParseSess& sess, const fs::path& path) -> Parser;

} // namespace crust

#endif // !PARSER_H
