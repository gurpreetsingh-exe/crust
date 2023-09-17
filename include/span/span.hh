#ifndef SPAN_H
#define SPAN_H

namespace crust {

struct Span {
  u32 lo;
  u32 hi;

  Span() {}
  [[nodiscard]] Span(u32 s, u32 e) : lo(s), hi(e) {}
};

} // namespace crust

#endif // !SPAN_H
