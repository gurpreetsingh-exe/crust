#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <cerrno>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <experimental/source_location>
#include <filesystem>
#include <fmt/core.h>
#include <format>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace fs = std::filesystem;

namespace crust {

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

template <typename T>
using Box = std::unique_ptr<T>;

template <typename T>
using Rc = std::shared_ptr<T>;

template <typename T>
using Option = std::optional<T>;

#define None std::nullopt

using location = std::experimental::source_location;

template <typename T, typename E>
class [[nodiscard]] Result {
public:
  Result(std::nullptr_t) = delete;
  Result(T t) : inner(std::move(t)) {}
  Result(E e) : inner(std::move(e)) {}

public:
  [[nodiscard]]
  auto is_ok() -> bool {
    return std::holds_alternative<T>(inner);
  }

  [[nodiscard]]
  auto is_err() -> bool {
    return std::holds_alternative<E>(inner);
  }

  [[nodiscard]]
  auto err() -> E&& {
    return std::move(std::get<E>(inner));
  }

  [[nodiscard]]
  auto value() -> T& {
    return std::get<T>(inner);
  }

  template <typename Callable>
  [[nodiscard]] auto bind(Callable&& cb)
      -> Result<std::invoke_result_t<Callable, T&>, E> {
    if (is_err())
      return err();
    return Result { std::invoke(std::forward<Callable>(cb), value()) };
  }

  template <typename Callable>
  [[nodiscard]] auto operator>>=(Callable&& cb)
      -> Result<std::invoke_result_t<Callable, T&>, E> {
    if (is_err())
      return err();
    return Result { std::invoke(std::forward<Callable>(cb), value()) };
  }

private:
  std::variant<T, E> inner;
};

/// Helper for matching variant types
///
/// Usage:
/// ```
/// std::visit(match {
///     [](double arg) { ... },
///     [](const std::string& arg) { ... },
///     // default case
///     [](auto arg) { ... },
///   }
/// }, v);
/// ```
template <class... Ts>
struct match : Ts... {
  using Ts::operator()...;
};

template <class... Ts>
match(Ts...) -> match<Ts...>;

template <class T>
auto contains(const std::vector<T>& v, const T& item) -> bool {
  return std::find(v.cbegin(), v.cend(), item) != v.cend();
}

#define CAT(a, b) #a #b
#define CAT2(a, b) a##b
#define STR(a) #a

#define UNREACHABLE() __builtin_unreachable()
#define PRAGMA(S) _Pragma(#S)
#define PUSH_IGNORE_WARNING(W)                                                 \
  PRAGMA(GCC diagnostic push) PRAGMA(GCC diagnostic ignored W)
#define POP_IGNORE_WARNING() PRAGMA(GCC diagnostic pop)

#define eprint(...)                                                            \
  (fprintf(stderr, "\033[1;31merror\033[0m: " __VA_ARGS__), exit(1))

#define PANIC(...)                                                             \
  (::crust::__crust_panic(__FILE__, __LINE__, "\n    " __VA_ARGS__))

#define TODO(...) PANIC(__VA_ARGS__)

#define ASSERT(cond, ...)                                                      \
  (((cond) ? static_cast<void>(0)                                              \
           : PANIC("assertion failed \"" #cond "\"\n    " __VA_ARGS__)))

#define TRY(res)                                                               \
  ({                                                                           \
    auto v = res;                                                              \
    if (v.is_err())                                                            \
      return v.err();                                                          \
    v.value();                                                                 \
  })

[[noreturn]] [[gnu::format(printf, 3, 4)]]
void __crust_panic(const char* file, int line, const char*, ...);

void print_backtrace();

/// True if `c` is considered a whitespace according to Rust language
/// definition. See https://doc.rust-lang.org/reference/whitespace.html for
/// definitions of these classes.
bool is_whitespace(char c);

/// Returns a new vector of strings split by newlines.
auto get_lines(const std::string& s) -> std::vector<std::string>;

} // namespace crust

#endif // !UTILS_H
