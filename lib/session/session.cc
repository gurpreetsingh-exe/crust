#include <parser/parser.hh>
#include <session/session.hh>

auto Session::run_compiler() -> void {
  interner.fresh();
  auto parser = parser_from_file(parse_sess, config.input);
  parser.parse();
}
