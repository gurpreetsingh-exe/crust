#ifndef SESSION_H
#define SESSION_H

#include "session/config.h"

/// Represents the data associated with a compilation session
typedef struct {
  /// Command line options
  Config config;
} Session;

Session* session_new(Config config);
void session_drop(Session* sess);

#endif // !SESSION_H
