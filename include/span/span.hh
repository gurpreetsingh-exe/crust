#ifndef SPAN_H
#define SPAN_H

namespace crust {

struct [[nodiscard]] Span {
  u32 lo;
  u32 hi;

  Span() : lo(0), hi(0) {}
  Span(u32 s, u32 e) : lo(s), hi(e) {}
  auto operator==(Span sp) const -> bool { return sp.hi == hi and sp.lo == lo; }

  auto to(Span end) -> Span;
  auto is_dummy() -> bool;
};

} // namespace crust

#endif // !SPAN_H
