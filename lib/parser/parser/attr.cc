#include <parser/parser.hh>

namespace crust {

auto Parser::parse_attribute() -> PResult<Attribute> {
  ASSERT(eat(TokenKind::Pound),
         "parse_attribute called in non-attribute position");

  auto lo = m_token.span;
  auto style = eat(TokenKind::Not) ? AttrStyle::Inner : AttrStyle::Outer;
  TRY(expect(TokenKind::OpenBracket));
  auto item = TRY(parse_attr_item());
  TRY(expect(TokenKind::CloseBracket));
  auto sp = lo.to(m_token.span);

  if (style == AttrStyle::Inner) {
    // TODO: emit error if inner attribute is found and forbidden
  }
  return Attribute(style, item, sp);
}

auto Parser::parse_attr_item() -> PResult<AttrItem> {
  auto path = TRY(parse_path());
  auto args = TRY(parse_attr_args());
  return AttrItem(path, args);
}

auto Parser::parse_delimited_args() -> Option<Delimited> {
  if (not(check(TokenKind::OpenParenthesis) or check(TokenKind::OpenBracket) or
          check(TokenKind::OpenBrace))) {
    return {};
  }

  switch (m_token.kind) {
    case TokenKind::OpenParenthesis:
    case TokenKind::OpenBracket:
    case TokenKind::OpenBrace: {
      auto open = m_token.span;
      auto delim = m_token.kind;
      auto stack = 1;
      while (true) {
        bump();
        if (stack == 0) {
          ASSERT(m_token.kind == TokenKind::CloseParenthesis ||
                 m_token.kind == TokenKind::CloseBracket ||
                 m_token.kind == TokenKind::CloseBrace);
          break;
        }
      }
      bump();
      return Delimited(open, m_prev_token.span, delim);
    } break;
    case TokenKind::CloseParenthesis:
    case TokenKind::CloseBracket:
    case TokenKind::CloseBrace: {
      UNREACHABLE();
    } break;
    default: {
      bump();
      UNREACHABLE();
    } break;
  }
}

auto Parser::parse_attr_args() -> PResult<AttrArgs> {
  auto args = parse_delimited_args();
  if (args.has_value()) {
    return AttrArgs(args.value());
  }
  if (eat(TokenKind::Eq)) {
    auto eq_span = m_prev_token.span;
    auto expr = TRY(parse_expr());
    return AttrArgs(AttrEq(eq_span, expr));
  } else {
    return AttrArgs(Empty {});
  }
}

auto Parser::parse_inner_attributes() -> PResult<std::vector<Attribute>> {
  auto attrs = std::vector<Attribute>();
  while (true) {
    if (check(TokenKind::Pound) and peek().kind == TokenKind::Not) {
      auto attr = TRY(parse_attribute());
      attrs.push_back(attr);
    } else if (m_token.kind == TokenKind::DocCommentInner) {
      bump();
      auto attr = Attribute(
          AttrStyle::Inner,
          DocComment(CommentKind::Line, std::get<Symbol>(m_token.extra)),
          m_prev_token.span);
      attrs.push_back(attr);
    } else {
      break;
    }
  }

  return attrs;
}

auto Parser::parse_outer_attributes() -> PResult<std::vector<Attribute>> {
  auto attrs = std::vector<Attribute>();
  while (true) {
    if (check(TokenKind::Pound)) {
      fmt::println("attr found");
    } else {
      break;
    }
  }

  return attrs;
}

} // namespace crust
