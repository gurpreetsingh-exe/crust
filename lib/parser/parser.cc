#include <parser/parser.hh>
#include <span/symbol.hh>

namespace crust {

auto Parser::bump() -> void { m_token = m_lexer->next(); }

auto Parser::parse() -> void {
  while (m_token.kind != TokenKind::Eof) {
    auto t = m_token;
    switch (t.kind) {
      case TokenKind::Ident: {
        auto sym = std::get<Symbol>(t.extra);
        if (sym == kw::Pub) {
          auto loc = m_sess.source_map.span_to_string(t.span);
          fmt::println("{} {} {}", t, sym, loc);
        }
      } break;
      default:
        break;
    }
    bump();
  }
}

auto parser_from_file(ParseSess& sess, const fs::path& path) -> Parser {
  auto source_file = sess.source_map.new_file(path);
  return Parser(sess, source_file);
}

} // namespace crust
