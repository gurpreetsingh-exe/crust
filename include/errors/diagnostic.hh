#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include <span/span.hh>

namespace crust {

enum class Level {
  Error,
  Warning,
  Note,
  Bug,
  FailureNote,
};

enum class Style {
  MainHeaderMsg,
  HeaderMsg,
  LineAndColumn,
  LineNumber,
  Quotation,
  UnderlinePrimary,
  UnderlineSecondary,
  LabelPrimary,
  LabelSecondary,
  NoStyle,
  Level,
  Highlight,
  Addition,
  Removal,
};

struct Message {
  std::string msg;
  Style style;

  Message(const std::string& m, Style s) : msg(std::move(m)), style(s) {}
};

struct SpanLabel {
  Span span;
  bool is_primary;
  Option<Message> label;

  SpanLabel(Span sp, bool prim, Option<Message> l)
      : span(sp), is_primary(prim), label(l) {}
};

struct MultiSpan {
  std::vector<Span> primary_spans;
  std::vector<std::tuple<Span, Message>> span_labels;

  MultiSpan(std::vector<Span> ps, std::vector<std::tuple<Span, Message>> sl)
      : primary_spans(std::move(ps)), span_labels(std::move(sl)) {}

  MultiSpan(Span sp) { primary_spans.push_back(sp); }

  static auto from_span(Span span) -> MultiSpan {
    auto vec = std::vector<Span>();
    vec.push_back(span);
    return MultiSpan(std::move(vec), std::vector<std::tuple<Span, Message>>());
  }

  auto is_dummy() const -> bool;
  auto has_primary_spans() const -> bool { return not is_dummy(); }
  auto has_span_labels() const -> bool;
  auto primary_span() const -> Option<Span>;
  auto get_span_labels() const -> std::vector<SpanLabel>;
};

struct Location {
  std::string file;
  u32 line;
  u32 column;

  Location(std::string f, u32 l, u32 c)
      : file(std::move(f)), line(l), column(c) {}
  Location(location loc = location::current())
      : file(loc.file_name()), line(loc.line()), column(loc.column()) {}
};

struct Diagnostic {
  Level level;
  std::vector<Message> message;
  MultiSpan span;
  Location location;

  Diagnostic(Level l, std::vector<Message> m, MultiSpan sp,
             Location loc = Location())
      : level(l), message(std::move(m)), span(std::move(sp)),
        location(std::move(loc)) {}
};

} // namespace crust

template <>
struct fmt::formatter<crust::Level> {
  constexpr auto parse(format_parse_context& ctx)
      -> format_parse_context::iterator {
    return ctx.begin();
  }

  auto format(crust::Level level, format_context& ctx)
      -> format_context::iterator {
    auto s = std::string_view();
    switch (level) {
      case crust::Level::Error:
        s = "error";
        break;
      case crust::Level::Warning:
        s = "warning";
        break;
      case crust::Level::Note:
        s = "note";
        break;
      case crust::Level::Bug:
        s = "internal compiler error";
        break;
      case crust::Level::FailureNote:
        s = "failure-note";
        break;
    }
    return fmt::format_to(ctx.out(), "{}", s);
  }
};

#endif // !DIAGNOSTIC_H
