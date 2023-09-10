#ifndef SPAN_H
#define SPAN_H

// #define span(s, e) span_new((u32)(s), (u32)(e))

typedef struct {
  u32 lo;
  u32 hi;
} Span;

[[nodiscard]] Span span_new(u32 start, u32 end);

#endif // !SPAN_H
