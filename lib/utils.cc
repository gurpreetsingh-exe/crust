#include <execinfo.h>
#include <utils.hh>

#include <errors/diagnostic.hh>
#include <errors/handler.hh>

namespace crust {

void __crust_panic(const char* file, int line, const char* format, ...) {
  auto handler = Handler(std::nullopt);
  auto message = fmt::format("panic at {}:{}", file, line);
  auto diag = handler.struct_span_err(Span(), Message(message, Style::NoStyle));
  handler.emit_diagnostic(diag);
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");
  print_backtrace();
  exit(1);
}

void print_backtrace() {
  void* array[1024];
  int size = backtrace(array, 1024);
  char** strings = backtrace_symbols(array, size);
  for (usize i = 0; i < (usize)size; ++i) {
    printf("    #%-2ld %s\n", i, strings[i]);
  }
}

bool is_whitespace(char c) {
  switch (c) {
      // Usual ASCII suspects
    case '\t':
    case '\n':
    case '\x0b': // vertical tab
    case '\x0c': // form feed
    case '\r':
    case ' ':

      // NEXT LINE from latin1
      // case '\x85':
      //   // Bidi markers
      // case L'\u200E': // LEFT-TO-RIGHT MARK
      // case L'\u200F': // RIGHT-TO-LEFT MARK
      //
      //   // Dedicated whitespace characters from Unicode
      // case L'\u2028': // LINE SEPARATOR
      // case L'\u2029': // PARAGRAPH SEPARATOR

      return true;
  }
  return false;
}

auto get_lines(const std::string& str) -> std::vector<std::string> {
  auto lines = std::vector<std::string>();
  auto f = std::istringstream(str);
  auto s = std::string();
  while (std::getline(f, s, '\n')) { lines.push_back(s); }
  return lines;
}

} // namespace crust
