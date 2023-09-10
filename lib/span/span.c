#include <span/span.h>

Span span_new(u32 start, u32 end) {
  return (Span) {
    .lo = start,
    .hi = end,
  };
}
