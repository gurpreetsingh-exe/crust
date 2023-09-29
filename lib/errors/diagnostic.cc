#include <errors/diagnostic.hh>

namespace crust {

auto MultiSpan::is_dummy() const -> bool {
  return std::all_of(primary_spans.cbegin(), primary_spans.cend(),
                     [](Span sp) { return sp.is_dummy(); });
}

auto MultiSpan::has_span_labels() const -> bool {
  return std::any_of(span_labels.cbegin(), span_labels.cend(), [](auto sl) {
    auto [sp, _] = sl;
    return not sp.is_dummy();
  });
}

auto MultiSpan::primary_span() const -> Option<Span> {
  return primary_spans.size() == 0 ? std::nullopt
                                   : std::optional(primary_spans[0]);
}

auto MultiSpan::get_span_labels() const -> std::vector<SpanLabel> {
  auto is_primary = [&](Span span) {
    return std::find(primary_spans.cbegin(), primary_spans.cend(), span) !=
           primary_spans.cend();
  };
  auto labels = std::vector<SpanLabel>();
  for (auto& [span, label] : span_labels) {
    labels.push_back(SpanLabel(span, is_primary(span), label));
  }
  for (auto& span : primary_spans) {
    if (not std::any_of(labels.cbegin(), labels.cend(),
                        [&](SpanLabel sl) { return sl.span == span; })) {
      labels.push_back(SpanLabel(span, true, {}));
    }
  }
  return labels;
}

} // namespace crust
