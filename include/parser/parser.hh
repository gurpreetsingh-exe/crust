#ifndef PARSER_H
#define PARSER_H

#include <ast/token.hh>
#include <session/parse.hh>

class Parser {
public:
  Parser(ParseSess& sess, std::vector<Token> tokens)
      : m_sess(sess), m_tokens(std::move(tokens)) {
    bump();
  }

public:
  auto parse() -> void;

private:
  auto bump() -> void;

private:
  ParseSess& m_sess;
  std::vector<Token> m_tokens;
  Token m_token;
};

[[nodiscard]]
auto parser_from_file(ParseSess& sess, const fs::path& path) -> Parser;

#endif // !PARSER_H
