#include <parser/parser.hh>
#include <session/session.hh>

namespace crust {

auto Session::run_compiler() -> void {
  interner.fresh();
  auto parser = parser_from_file(parse_sess, config.input);
  parser.parse();
}

} // namespace crust
