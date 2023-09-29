#include <parser/parser.hh>
#include <session/session.hh>

namespace crust {

auto Session::run_compiler() -> void {
  interner.fresh();
  auto parser = parser_from_file(parse_sess, config.input);
  auto crate = parser.parse_crate_mod();
  if (crate.is_err()) {
    parser.emit_diagnostic(crate.err());
  }
}

} // namespace crust
