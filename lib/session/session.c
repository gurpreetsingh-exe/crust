#include <alloc/alloc.h>
#include <session/session.h>

Session* session_new(Config config) {
  Session* sess = alloc(Session);
  sess->config = config;
  return sess;
}

void session_drop(Session* sess) { config_drop(&sess->config); }
