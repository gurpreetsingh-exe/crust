#ifndef PARSER_H
#define PARSER_H

#include <ast/ast.hh>
#include <ast/token.hh>
#include <errors/diagnostic.hh>
#include <parser/lexer.hh>
#include <session/parse.hh>

namespace crust {

template <typename T>
using PResult = Result<T, Diagnostic>;

class Parser {
public:
  Parser(ParseSess& sess, Rc<SourceFile> file) : m_sess(sess) {
    m_lexer = std::make_unique<Lexer>(file->content);
    bump();
  }

public:
  [[nodiscard]] auto parse_crate_mod() -> PResult<Box<Crate>>;
  auto emit_diagnostic(const Diagnostic& diagnostic) -> void {
    m_sess.span_diagnostic.emit_diagnostic(diagnostic);
  }

private:
  auto bump() -> void;
  auto expect(TokenKind kind) -> PResult<bool>;
  auto expect_one_of(const std::vector<TokenKind>& edible,
                     const std::vector<TokenKind>& inedible) -> PResult<bool>;
  auto check(TokenKind kind) -> bool;
  auto eat(TokenKind kind) -> bool;
  auto peek() -> Token;

  auto struct_span_err(Span sp, Message msg, Location loc = Location())
      -> Diagnostic;

  auto unexpected_try_recover(TokenKind kind) -> PResult<bool>;

  auto parse_mod() -> PResult<std::vector<Item>>;
  auto parse_item() -> PResult<Option<Item>>;

  auto parse_attribute() -> PResult<Attribute>;
  auto parse_attr_item() -> PResult<AttrItem>;
  auto parse_attr_args() -> PResult<AttrArgs>;
  auto parse_delimited_args() -> Option<Delimited>;
  auto parse_inner_attributes() -> PResult<std::vector<Attribute>>;
  auto parse_outer_attributes() -> PResult<std::vector<Attribute>>;

  /// Parses simple paths.
  ///
  /// `path = [::] segment+`
  /// `segment = ident | ident[::]<args> | ident[::](args) [-> type]`
  ///
  /// # Examples
  /// `a::b::C<D>` (without disambiguator)
  /// `a::b::C::<D>` (with disambiguator)
  /// `Fn(Args)` (without disambiguator)
  /// `Fn::(Args)` (with disambiguator)
  auto parse_path() -> PResult<Path>;
  auto parse_path_segments(std::vector<PathSegment>& segments) -> PResult<bool>;
  auto parse_path_segment() -> PResult<PathSegment>;
  auto parse_path_segment_ident() -> PResult<Ident>;
  auto parse_ident() -> PResult<Ident>;

  auto parse_expr() -> PResult<Expr>;

private:
  ParseSess& m_sess;
  Box<Lexer> m_lexer;
  Token m_token;
  Token m_prev_token;
  std::vector<TokenKind> m_expected_tokens = {};
  usize m_num_bump_calls = 0;
};

[[nodiscard]]
auto parser_from_file(ParseSess& sess, const fs::path& path) -> Parser;

} // namespace crust

#endif // !PARSER_H
