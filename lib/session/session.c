#include <alloc/alloc.h>
#include <session/session.h>

Session* session_new(Config config) {
  Session* sess = alloc(Session);
  sess->config = config;
  SourceFile* file =
      source_map_load_file(&sess->source_map, &sess->config.input);
  interner_fresh();
  sess->pcx = parser_from_file(file);
  return sess;
}

void session_drop(Session* sess) {
  config_drop(&sess->config);
  file_drop(sess->pcx->file);
  interner_drop();
  drop(sess->pcx);
}

void session_run_compiler(Session* sess) {
  // hash_map_iter(interner.names);
  parse(sess->pcx);
}
