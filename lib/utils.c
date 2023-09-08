#include <execinfo.h>
#include <utils.h>

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
