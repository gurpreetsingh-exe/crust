#include <alloc/alloc.h>
#include <session/session.h>

Session* session_new(Config config) {
  Session* sess = alloc(Session);
  sess->config = config;
  return sess;
}

void session_drop(Session* sess) { config_drop(&sess->config); }

void session_run_compiler(Session* sess) {
  SourceFile file =
      source_map_load_file(&sess->source_map, &sess->config.input);
  sess->pcx = parser_from_file(&file);
  parse(sess->pcx);
}
