#ifndef STYLED_BUFFER_H
#define STYLED_BUFFER_H

#include <errors/diagnostic.hh>

namespace crust {

struct StyledString {
  std::string text;
  Style style;

  StyledString(std::string t, Style s) : text(std::move(t)), style(s) {}
};

struct StyledChar {
  static StyledChar SPACE;
  char chr;
  Style style;

  StyledChar(char c, Style s) : chr(c), style(s) {}
};

class StyledBuffer {
public:
  StyledBuffer() {}

public:
  auto putc(usize line, usize col, char chr, Style style) -> void;
  auto puts(usize line, usize col, const std::string_view& str, Style style)
      -> void;
  auto prepend(usize line, const std::string_view& str, Style style) -> void;
  auto append(usize line, const std::string_view& str, Style style) -> void;
  auto render() -> std::vector<std::vector<StyledString>>;

private:
  auto ensure_lines(usize line) -> void;

private:
  std::vector<std::vector<StyledChar>> lines;
};

} // namespace crust

#endif // !STYLED_BUFFER_H
