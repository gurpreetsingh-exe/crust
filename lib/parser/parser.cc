#include <parser/lexer.hh>
#include <parser/parser.hh>

auto Parser::bump() -> void {}

auto Parser::parse() -> void {
  for (auto& t : m_tokens) {
    auto loc = m_sess.source_map.span_to_string(t.span);
    std::cout << t << std::format(": {}", loc) << std::endl;
  }
}

auto parser_from_file(ParseSess& sess, const fs::path& path) -> Parser {
  auto source_file = sess.source_map.new_file(path);
  auto lexer = Lexer(source_file->content);
  auto tokens = std::vector<Token>();
  while (true) {
    auto token = lexer.next();
    if (token.kind == Eof)
      break;
    tokens.push_back(token);
  }

  return Parser(sess, std::move(tokens));
}
