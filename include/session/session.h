#ifndef SESSION_H
#define SESSION_H

#include <parser/parser.h>
#include <session/config.h>

/// Represents the data associated with a compilation session
typedef struct {
  /// Command line options
  Config config;
  /// All source files used in this session
  SourceMap source_map;
  /// Parsing context
  ParseCtx* pcx;
} Session;

[[nodiscard]] Session* session_new(Config config);
void session_drop(Session* sess);
void session_run_compiler(Session* sess);

#endif // !SESSION_H
