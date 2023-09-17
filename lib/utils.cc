#include <execinfo.h>
#include <utils.hh>

namespace crust {

void __crust_panic(const char* file, int line, const char* format, ...) {
  fprintf(stderr, "panic at %s:%d", file, line);
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
      [[fallthrough]];
    case '\n':
      [[fallthrough]];
    case '\x0b': // vertical tab
      [[fallthrough]];
    case '\x0c': // form feed
      [[fallthrough]];
    case '\r':
      [[fallthrough]];
    case ' ':
      [[fallthrough]];

      // NEXT LINE from latin1
    case '\x85':
      /**
      [[fallthrough]];

        // Bidi markers
      case L'\u200E':
        [[fallthrough]]; // LEFT-TO-RIGHT MARK
      case L'\u200F':
        [[fallthrough]]; // RIGHT-TO-LEFT MARK

        // Dedicated whitespace characters from Unicode
      case L'\u2028':
        [[fallthrough]]; // LINE SEPARATOR
      case L'\u2029':    // PARAGRAPH SEPARATOR
     **/
      return true;
  }
  return false;
}

} // namespace crust
