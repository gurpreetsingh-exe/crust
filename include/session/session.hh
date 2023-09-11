#ifndef SESSION_H
#define SESSION_H

#include <session/config.hh>
#include <session/parse.hh>

/// Represents the data associated with a compilation session
struct Session {
  /// Command line options
  Config config;
  /// Parsing session
  ParseSess parse_sess;

  Session(Config cfg) : config(cfg) {}
  auto run_compiler() -> void;
};

#endif // !SESSION_H
