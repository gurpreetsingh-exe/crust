#ifndef LEXER_H
#define LEXER_H

#include <ast/token.hh>
#include <span/source_map.hh>

class Lexer {
public:
  Lexer(const std::string& src) : m_src(src) {
    if (not m_src.empty()) {
      m_c = m_src[m_index];
    }
  }

public:
  [[nodiscard]] auto next() -> Token;

private:
  auto bump() -> void;
  auto first() -> char;
  auto symbol(usize start) -> Symbol;
  auto number(Base* base) -> TokenKind;
  auto line_comment() -> std::optional<TokenKind>;
  auto block_comment() -> std::optional<TokenKind>;
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
  u32 m_line = 1;
  u32 m_col = 1;
  bool m_preceded_by_whitespace = false;
};

#endif // !LEXER_H
