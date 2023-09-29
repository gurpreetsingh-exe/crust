#ifndef AST_H
#define AST_H

#include <ast/token.hh>
#include <span/span.hh>
#include <span/symbol.hh>

namespace crust {

struct Expr {};

struct GenericArgs {};

struct PathSegment {
  Ident ident;
  Option<GenericArgs> args;

  PathSegment(Ident i, Option<GenericArgs> a) : ident(i), args(a) {}

  static auto from_root(Span span) -> PathSegment {
    return PathSegment(Ident(kw::PathRoot, span), None);
  }
};

struct Path {
  std::vector<PathSegment> segments;
  Span span;

  Path(std::vector<PathSegment> s, Span sp)
      : segments(std::move(s)), span(sp) {}
};

enum class CommentKind {
  /// `/* ... */`
  Block,
  /// `/// ...`
  Line,
};

struct DocComment {
  CommentKind kind;
  Symbol sym;

  DocComment(CommentKind k, Symbol s) : kind(k), sym(s) {}
};

enum class AttrStyle {
  /// Inner attribute (e.g. `#![...]`, `//! ...`, `/*! ... */`)
  Inner,
  /// Outer attribute (e.g. `#[...]`, `/// ...`, `/** ... */`)
  Outer,
};

/// No arguments: `#[attr]`
struct Empty {};

/// Delimited arguments, as used in `#[attr()/[]/{}]` or `mac!()/[]/{}`.
struct Delimited {
  Span open;
  Span close;
  TokenKind delim;

  Delimited(Span o, Span c, TokenKind d) : open(o), close(c), delim(d) {}
};

/// Arguments of a key-value attribute: `#[attr = "value"]`.
struct AttrEq {
  Span span;
  Expr value;

  AttrEq(Span sp, Expr v) : span(sp), value(v) {}
};

using AttrArgs = std::variant<Empty, Delimited, AttrEq>;

struct AttrItem {
  Path path;
  AttrArgs args;

  AttrItem(Path p, AttrArgs a) : path(std::move(p)), args(a) {}
};

/// An attribute
struct Attribute {
  using Kind = std::variant<
      /// Normal attribute (e.g. `#[...]`, `#![...]`)
      AttrItem,
      /// A doc comment (e.g. `/// ...`, `//! ...`, `/** ... */`, `/*! ... */`)
      DocComment>;

  AttrStyle style;
  Kind kind;
  Span span;

  Attribute(AttrStyle s, Kind k, Span sp) : style(s), kind(k), span(sp) {}
};

struct Item {};

struct Crate {
  std::vector<Attribute> attrs;
  std::vector<Item> items;
  Span span;

  Crate() = delete;
  Crate(std::vector<Attribute> a, std::vector<Item> i, Span s)
      : attrs(std::move(a)), items(std::move(i)), span(s) {}
};

} // namespace crust

#endif // !AST_H
