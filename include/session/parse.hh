#ifndef PARSE_SESS_H
#define PARSE_SESS_H

#include <span/source_map.hh>

struct ParseSess {
  /// All source files used in this session
  SourceMap source_map;

  ParseSess() : source_map(SourceMap()) {}
};

#endif // !PARSE_SESS_H
