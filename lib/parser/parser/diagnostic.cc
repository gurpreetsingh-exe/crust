#include <parser/parser.hh>

namespace crust {

auto Parser::struct_span_err(Span sp, Message msg, Location loc) -> Diagnostic {
  return m_sess.span_diagnostic.struct_span_err(sp, std::move(msg),
                                                std::move(loc));
}

auto Parser::unexpected_try_recover(TokenKind kind) -> PResult<bool> {
  auto tok_str = fmt::format("{}", kind);
  auto descr = std::make_optional(std::string());
  if (m_token.is_special_ident()) {
    descr = "reserved identifier";
  } else if (m_token.is_used_keyword()) {
    descr = "keyword";
  } else if (m_token.is_unused_keyword()) {
    descr = "reserved keyword";
  } else if (m_token.kind == TokenKind::DocCommentInner or
             m_token.kind == TokenKind::DocCommentOuter) {
    descr = "doc comment";
  }

  auto token_descr = std::string();
  if (descr.has_value()) {
    token_descr = fmt::format("{} `{}`", descr.value(), tok_str);
  } else {
    token_descr = fmt::format("`{}`", tok_str);
  }

  auto msg = std::string();
  if (m_token.kind == TokenKind::Eof) {
    msg = fmt::format("expected `{}`, found end of file", tok_str);
  } else {
    msg = fmt::format("expected `{}`, found `{}`", tok_str, token_descr);
  }

  return struct_span_err(m_token.span, Message(std::move(msg), Style::NoStyle));
}

} // namespace crust
