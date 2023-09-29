#include <parser/parser.hh>

namespace crust {

auto Parser::parse_expr() -> PResult<Expr> {
  return struct_span_err(m_token.span,
                         Message("not implemented", Style::NoStyle));
}

} // namespace crust
