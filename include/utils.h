#ifndef UTILS_H
#define UTILS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef size_t usize;
typedef float f32;
typedef double f64;

#define CAT(a, b) #a #b
#define STR(a) #a

#define UNREACHABLE() __builtin_unreachable()
#define PRAGMA(S) _Pragma(#S)
#define PUSH_IGNORE_WARNING(W)                                                 \
  PRAGMA(GCC diagnostic push) PRAGMA(GCC diagnostic ignored W)
#define POP_IGNORE_WARNING() PRAGMA(GCC diagnostic pop)

#define eprint(...)                                                            \
  (fprintf(stderr, "\033[1;31merror\033[0m: " __VA_ARGS__), exit(1))

#define PANIC(...) (__crust_panic(__FILE__, __LINE__, "\n    " __VA_ARGS__))

#define TODO(...) PANIC(__VA_ARGS__)

#define ASSERT(cond, ...)                                                      \
  (((cond) ? (void)0                                                           \
           : PANIC("assertion failed \"" #cond "\"\n    " __VA_ARGS__)))

#define STREQ(str1, str2)                                                      \
  ({                                                                           \
    usize len1 = strlen(str1);                                                 \
    usize len2 = strlen(str2);                                                 \
    len1 == len2 && !strncmp(str1, str2, len1);                                \
  })

[[noreturn]] [[gnu::format(printf, 3, 4)]]
void __crust_panic(const char* file, int line, const char*, ...);

void print_backtrace();

/// True if `c` is considered a whitespace according to Rust language
/// definition. See https://doc.rust-lang.org/reference/whitespace.html for
/// definitions of these classes.
bool is_whitespace(char c);

#endif // !UTILS_H
