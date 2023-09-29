#include <errors/emitter.hh>
#include <fmt/color.h>

namespace crust {

auto Emitter::get_max_line_num(const MultiSpan& span) -> usize {
  return get_multispan_max_line_num(span);
}

auto Emitter::get_multispan_max_line_num(const MultiSpan& span) -> usize {
  if (not sm.has_value()) {
    return 0;
  }

  auto will_be_emitted = [](Span sp) { return not sp.is_dummy(); };

  auto max = 0UL;
  for (auto& prim_sp : span.primary_spans) {
    if (will_be_emitted(prim_sp)) {
      auto hi = sm.value()->lookup_char_pos(prim_sp.hi);
      max = std::max(hi.line, max);
    }
  }

  return max;
}

auto Emitter::emit_diagnostic(const Diagnostic& diagnostic) -> void {
  emit_messages_default(diagnostic.span, diagnostic.message, diagnostic.level);
  auto& loc = diagnostic.location;
  fmt::println("created at: {}:{}:{}", loc.file, loc.line, loc.column);
}

static auto num_decimal_digits(usize num) -> usize {
  constexpr usize MAX_DIGITS = 20;

  auto lim = 10UL;
  for (auto num_digits = 1UL; num_digits < MAX_DIGITS; ++num_digits) {
    if (num < lim) {
      return num_digits;
    }
    lim *= 10;
  }
  return MAX_DIGITS;
}

auto Emitter::emit_messages_default(const MultiSpan& span,
                                    const std::vector<Message>& msg,
                                    Level level) -> void {
  auto max_line_num_len = 0UL;
  if (ui_testing) {
    max_line_num_len = 2;
  } else {
    auto n = get_max_line_num(span);
    max_line_num_len = num_decimal_digits(n);
  }

  emit_message_default(span, msg, level, max_line_num_len, false);
}

auto Emitter::emit_message_default(const MultiSpan& span,
                                   const std::vector<Message>& msg, Level level,
                                   usize max_line_num_len, bool is_secondary)
    -> void {
  auto buffer = StyledBuffer();
  if (not span.has_primary_spans() and not span.has_span_labels() and
      is_secondary and not short_message) {
    for (usize i = 0; i < max_line_num_len; ++i) {
      buffer.prepend(0, " ", Style::NoStyle);
    }

    draw_note_separator(buffer, 0, max_line_num_len + 1);
    if (level != Level::FailureNote) {
      buffer.append(0, fmt::format("{}", level), Style::MainHeaderMsg);
      buffer.append(0, ": ", Style::NoStyle);
    }
    msg_to_buffer(buffer, msg, max_line_num_len, "note", {});
  } else {
    auto label_width = 0UL;
    if (level != Level::FailureNote) {
      auto level_str = fmt::format("{}", level);
      buffer.append(0, level_str, Style::Level);
      label_width += level_str.size();
    }
    auto header_style = is_secondary ? Style::HeaderMsg : Style::MainHeaderMsg;
    if (level != Level::FailureNote) {
      buffer.append(0, ": ", header_style);
      label_width += 2;
    }

    for (auto& m : msg) {
      auto lines = get_lines(m.msg);
      for (auto i = 0UL; i < lines.size(); ++i) {
        buffer.append(i,
                      fmt::format("{}{}",
                                  i == 0 ? "" : std::string(label_width, ' '),
                                  lines[i]),
                      header_style);
      }
    }
  }

  auto primary_span = span.primary_span().value_or(Span());
  if (primary_span.is_dummy() or not sm.has_value()) {
    emit_to_destination(level, buffer.render());
    return;
  }

  auto src_map = sm.value();
  auto primary_lo = src_map->lookup_char_pos(primary_span.lo);
  emit_to_destination(level, buffer.render());
}

auto Emitter::msg_to_buffer(StyledBuffer& buffer,
                            const std::vector<Message>& msg, usize pad,
                            const std::string_view& label,
                            Option<Style> override_style) -> void {
  auto padding = std::string(pad + label.size() + 5, ' ');
  auto style_or_override = [](Style style, Option<Style> override) {
    return style == Style::NoStyle and override.has_value() ? override.value()
                                                            : style;
  };

  auto line_no = 0UL;
  for (auto& ms : msg) {
    auto lines = get_lines(ms.msg);
    if (lines.size() > 1) {
      for (auto i = 0UL; i < lines.size(); ++i) {
        auto line = lines[i];
        if (i != 0) {
          line_no += 1;
          buffer.append(line_no, padding, Style::NoStyle);
        }
        buffer.append(line_no, line,
                      style_or_override(ms.style, override_style));
      }
    } else {
      buffer.append(line_no, ms.msg,
                    style_or_override(ms.style, override_style));
    }
  }
}

static auto level_color(Level level) -> fmt::terminal_color {
  switch (level) {
    case Level::Error:
    case Level::Bug:
    case Level::FailureNote:
      return fmt::terminal_color::bright_red;
    case Level::Warning:
      return fmt::terminal_color::bright_yellow;
    case Level::Note:
      return fmt::terminal_color::bright_cyan;
  }
  UNREACHABLE();
}

static auto set_color(Level level, Style style) -> fmt::text_style {
  switch (style) {
    case Style::MainHeaderMsg: {
      return fmt::emphasis::bold | fmt::fg(fmt::color::white);
    } break;
    case Style::NoStyle:
    case Style::Quotation:
    case Style::LineAndColumn:
    case Style::HeaderMsg: {
      return fmt::fg(fmt::color::white);
    } break;
    case Style::LineNumber: {
      return fmt::emphasis::bold | fmt::fg(fmt::color::blue);
    } break;
    case Style::UnderlinePrimary:
    case Style::LabelPrimary: {
      return fmt::emphasis::bold | fmt::emphasis::strikethrough |
             fmt::fg(level_color(level));
    } break;
    case Style::UnderlineSecondary:
    case Style::LabelSecondary: {
      return fmt::emphasis::bold | fmt::fg(fmt::color::blue);
    } break;
    case Style::Level: {
      return fmt::emphasis::bold | fmt::fg(level_color(level));
    } break;
    case Style::Highlight: {
      return fmt::emphasis::bold | fmt::fg(fmt::color::white);
    } break;
    case Style::Addition: {
      return fmt::emphasis::bold | fmt::fg(fmt::color::green);
    } break;
    case Style::Removal: {
      return fmt::emphasis::bold | fmt::fg(fmt::color::red);
    } break;
  }
  UNREACHABLE();
}

auto emit_to_destination(Level level,
                         const std::vector<std::vector<StyledString>>& buffer)
    -> void {
  for (auto line : buffer) {
    for (auto part : line) {
      fmt::print(set_color(level, part.style), "{}", part.text);
    }
    fmt::println("");
  }
}

auto draw_note_separator(StyledBuffer& buffer, usize line, usize col) -> void {
  buffer.puts(line, col, "= ", Style::LineNumber);
}

} // namespace crust
