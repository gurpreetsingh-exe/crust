#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <cerrno>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fmt/core.h>
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace fs = std::filesystem;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using usize = size_t;
using f32 = float;
using f64 = double;

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
  (((cond) ? static_cast<void>(0)                                              \
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
