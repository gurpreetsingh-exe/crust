#ifndef LEXER_H
#define LEXER_H

#include <ast/token.hh>
#include <span/source_map.hh>

namespace crust {

class Lexer {
public:
  Lexer(const std::string& src) : m_src(src) {
    if (not m_src.empty()) {
      m_c = m_src[m_index];
    }
  }

public:
  [[nodiscard]] auto next() -> Token;
  [[nodiscard]] auto preceded_by_whitespace() -> bool {
    return m_preceded_by_whitespace;
  }

private:
  auto bump() -> void;
  auto first() -> char;
  auto symbol(usize start) -> Symbol;
  auto number(Base* base) -> TokenKind;
  auto line_comment() -> Option<TokenKind>;
  auto block_comment() -> Option<TokenKind>;
  auto lifetime_or_char(Symbol* sym, usize start) -> TokenKind;
  auto single_quoted_string() -> bool;
  auto double_quoted_string() -> bool;
  auto eat_decimal_digits() -> bool;
  auto eat_literal_suffix() -> void;
  auto eat_identifier() -> void;

private:
  const std::string& m_src;
  char m_c = 0;
  char m_prev = 0;
  usize m_index = 0;
  bool m_preceded_by_whitespace = false;
};

} // namespace crust

#endif // !LEXER_H
