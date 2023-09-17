#ifndef PARSE_SESS_H
#define PARSE_SESS_H

#include <span/source_map.hh>

namespace crust {

struct ParseSess {
  /// All source files used in this session
  SourceMap source_map;

  ParseSess() : source_map(SourceMap()) {}
};

} // namespace crust

#endif // !PARSE_SESS_H
