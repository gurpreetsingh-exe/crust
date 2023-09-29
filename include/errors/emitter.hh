#ifndef EMITTER_H
#define EMITTER_H

#include <errors/diagnostic.hh>
#include <errors/styled_buffer.hh>
#include <span/source_map.hh>

namespace crust {

class Emitter {
public:
  Emitter(Option<Rc<SourceMap>> s, bool short_msg, bool testing)
      : sm(s), short_message(short_msg), ui_testing(testing) {}

public:
  auto emit_diagnostic(const Diagnostic& diagnostic) -> void;

private:
  auto get_max_line_num(const MultiSpan& span) -> usize;
  auto get_multispan_max_line_num(const MultiSpan& span) -> usize;

  auto emit_messages_default(const MultiSpan& span,
                             const std::vector<Message>& msg, Level level)
      -> void;
  auto emit_message_default(const MultiSpan& span,
                            const std::vector<Message>& msg, Level level,
                            usize max_line_num_len, bool is_secondary) -> void;
  auto msg_to_buffer(StyledBuffer& buffer, const std::vector<Message>& msg,
                     usize padding, const std::string_view& label,
                     Option<Style> override_style) -> void;

private:
  Option<Rc<SourceMap>> sm;
  bool short_message = false;
  bool ui_testing = false;
};

auto emit_to_destination(Level level,
                         const std::vector<std::vector<StyledString>>& buffer)
    -> void;
auto draw_note_separator(StyledBuffer& buffer, usize line, usize col) -> void;

} // namespace crust

#endif // !EMITTER_H
