#ifndef HANDLER_H
#define HANDLER_H

#include <errors/emitter.hh>
#include <span/source_map.hh>

namespace crust {

class Handler {
public:
  Handler(Option<Rc<SourceMap>> sm)
      : emitter(std::make_unique<Emitter>(sm, false, false)) {}

public:
  auto emit_diagnostic(const Diagnostic& diagnostic) -> void {
    emitter->emit_diagnostic(diagnostic);
  }

  [[nodiscard]]
  auto struct_span_err(Span sp, Message msg, Location loc = Location())
      -> Diagnostic;

private:
  usize err_count = 0;
  usize warn_count = 0;
  Box<Emitter> emitter;
};

} // namespace crust

#endif // !HANDLER_H
