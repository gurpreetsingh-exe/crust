#ifndef CONFIG_H
#define CONFIG_H

#include <alloc/str.h>

/// Command line options
typedef struct {
  /// Input filename
  str input;
  /// Output filename
  str output;
} Config;

/// Drops the contents of a config
void config_drop(Config* config);

#endif // !CONFIG_H
