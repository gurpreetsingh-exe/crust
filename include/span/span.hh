#ifndef SPAN_H
#define SPAN_H

struct Span {
  u32 lo;
  u32 hi;

  Span() {}
  [[nodiscard]] Span(u32 s, u32 e) : lo(s), hi(e) {}
};

#endif // !SPAN_H
