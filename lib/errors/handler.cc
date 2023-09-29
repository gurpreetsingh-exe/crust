#include <errors/handler.hh>

namespace crust {

auto Handler::struct_span_err(Span sp, Message msg, Location loc)
    -> Diagnostic {
  std::vector<Message> vec = { msg };
  return Diagnostic(Level::Error, std::move(vec), MultiSpan::from_span(sp),
                    std::move(loc));
}

} // namespace crust
