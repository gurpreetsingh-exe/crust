#include <errors/styled_buffer.hh>

namespace crust {

StyledChar StyledChar::SPACE = StyledChar { ' ', Style::NoStyle };

auto StyledBuffer::ensure_lines(usize line) -> void {
  if (line >= lines.size()) {
    lines.resize(line + 1, std::vector<StyledChar>());
  }
}

auto StyledBuffer::putc(usize line, usize col, char chr, Style style) -> void {
  ensure_lines(line);
  if (col >= lines[line].size()) {
    lines[line].resize(col + 1, StyledChar::SPACE);
  }
  lines[line][col] = StyledChar(chr, style);
}

auto StyledBuffer::puts(usize line, usize col, const std::string_view& str,
                        Style style) -> void {
  auto n = col;
  for (auto c : str) {
    putc(line, n, c, style);
    n += 1;
  }
}

auto StyledBuffer::prepend(usize line, const std::string_view& str, Style style)
    -> void {
  ensure_lines(line);
  auto len = str.size();
  if (not lines[line].empty()) {
    for (usize i = 0; i < len; ++i) {
      lines[line].insert(lines[line].begin(), StyledChar::SPACE);
    }
  }

  puts(line, 0, str, style);
}

auto StyledBuffer::append(usize line, const std::string_view& str, Style style)
    -> void {
  if (line >= lines.size()) {
    puts(line, 0, str, style);
  } else {
    auto col = lines[line].size();
    puts(line, col, str, style);
  }
}

auto StyledBuffer::render() -> std::vector<std::vector<StyledString>> {
  auto output = std::vector<std::vector<StyledString>>();
  auto styled_vec = std::vector<StyledString>();

  for (auto& styled_line : lines) {
    auto current_style = Style::NoStyle;
    auto current_text = std::string();

    for (auto sc : styled_line) {
      if (sc.style != current_style) {
        if (not current_text.empty()) {
          styled_vec.push_back(
              StyledString(std::move(current_text), current_style));
        }
        current_style = sc.style;
        current_text = std::string();
      }
      current_text.push_back(sc.chr);
    }
    if (not current_text.empty()) {
      styled_vec.push_back(
          StyledString(std::move(current_text), current_style));
    }

    output.push_back(std::move(styled_vec));
    styled_vec = std::vector<StyledString>();
  }

  return output;
}

} // namespace crust
