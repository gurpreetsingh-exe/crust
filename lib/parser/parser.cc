#include <parser/parser.hh>

namespace crust {

auto Parser::bump() -> void {
  m_num_bump_calls += 1;
  m_prev_token = m_token;
  m_token = m_lexer->next();
  if (not m_lexer->preceded_by_whitespace()) {
    // TODO: glue the token
  }
}

auto Parser::expect(TokenKind kind) -> PResult<bool> {
  if (m_expected_tokens.empty()) {
    if (m_token.kind == kind) {
      bump();
      return false;
    } else {
      return unexpected_try_recover(kind);
    }
  } else {
    return expect_one_of({ kind }, {});
  }
}

auto Parser::expect_one_of(const std::vector<TokenKind>& edible,
                           const std::vector<TokenKind>& inedible)
    -> PResult<bool> {
  if (contains(edible, m_token.kind)) {
    bump();
    return false;
  } else if (contains(inedible, m_token.kind)) {
    return false;
  } else {
    return struct_span_err(
        m_token.span,
        Message(fmt::format("expected one of {} but got {}", 0, m_token.kind),
                Style::NoStyle));
  }
}

auto Parser::check(TokenKind kind) -> bool {
  auto is_present = m_token.kind == kind;
  if (not is_present) {
    m_expected_tokens.push_back(kind);
  }
  return is_present;
}

auto Parser::eat(TokenKind kind) -> bool {
  auto is_present = check(kind);
  if (is_present) {
    bump();
  }
  return is_present;
}

auto Parser::peek() -> Token {
  auto tmp_lexer = *m_lexer.get();
  auto tok = tmp_lexer.next();
  return tok;
}

auto Parser::parse_crate_mod() -> PResult<Box<Crate>> {
  auto lo = m_token.span;
  auto attrs = TRY(parse_inner_attributes());
  auto items = TRY(parse_mod());
  ASSERT(m_token.kind == TokenKind::Eof);
  return std::make_unique<Crate>(std::move(attrs), std::move(items),
                                 m_token.span.to(lo));
}

auto Parser::parse_mod() -> PResult<std::vector<Item>> {
  auto items = std::vector<Item>();
  while (true) {
    auto item = TRY(parse_item());
    if (not item.has_value())
      break;
    items.push_back(item.value());
    // TODO: consume semi colon if present
  }
  return items;
}

auto Parser::parse_item() -> PResult<Option<Item>> {
  auto attrs = TRY(parse_outer_attributes());
  return struct_span_err(m_token.span,
                         Message("not implemented", Style::NoStyle));
}

auto parser_from_file(ParseSess& sess, const fs::path& path) -> Parser {
  auto source_file = sess.source_map->new_file(path);
  return Parser(sess, source_file);
}

} // namespace crust
