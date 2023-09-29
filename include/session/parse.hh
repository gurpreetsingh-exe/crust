#ifndef PARSE_SESS_H
#define PARSE_SESS_H

#include <errors/handler.hh>
#include <span/source_map.hh>

namespace crust {

struct ParseSess {
  /// All source files used in this session
  Rc<SourceMap> source_map;
  /// Error handler
  Handler span_diagnostic;

  ParseSess()
      : source_map(std::make_shared<SourceMap>()),
        span_diagnostic(Handler(source_map)) {}
};

} // namespace crust

#endif // !PARSE_SESS_H
