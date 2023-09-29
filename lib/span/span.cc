#include <span/span.hh>

namespace crust {

auto Span::to(Span end) -> Span {
  return Span(std::min(lo, end.lo), std::max(hi, end.hi));
}

auto Span::is_dummy() -> bool { return lo == 0 and hi == 0; }

} // namespace crust
