#include <parser/parser.hh>

namespace crust {

auto Parser::parse_path() -> PResult<Path> {
  auto lo = m_token.span;
  auto segments = std::vector<PathSegment>();
  if (eat(TokenKind::ModSep)) {
    segments.push_back(PathSegment::from_root(lo));
  }

  TRY(parse_path_segments(segments));
  return Path(std::move(segments), lo.to(m_prev_token.span));
}

auto Parser::parse_path_segments(std::vector<PathSegment>& segments)
    -> PResult<bool> {
  while (true) {
    auto segment = TRY(parse_path_segment());
    segments.push_back(segment);
    if (eat(TokenKind::ModSep)) {
      continue;
    }
    return true;
  }
}

auto Parser::parse_path_segment() -> PResult<PathSegment> {
  auto ident = TRY(parse_path_segment_ident());
  return PathSegment(ident, None);
}

auto Parser::parse_path_segment_ident() -> PResult<Ident> {
  auto ident_ = m_token.ident();
  if (ident_.has_value()) {
    auto [ident, is_raw] = ident_.value();
    if (not is_raw and ident.is_path_segment_keyword()) {
      bump();
      return ident;
    } else {
      return parse_ident();
    }
  }
  return parse_ident();
}

auto Parser::parse_ident() -> PResult<Ident> {
  auto ident_ = m_token.ident();
  if (ident_.has_value()) {
    bump();
    auto [ident, is_raw] = ident_.value();
    return ident;
  }

  return struct_span_err(m_token.span,
                         Message("expected identifier", Style::NoStyle));
}

} // namespace crust
