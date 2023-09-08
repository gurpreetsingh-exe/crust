#ifndef CONFIG_H
#define CONFIG_H

#include <alloc/str.h>

/// Command line options
typedef struct {
  /// input filename
  str input;
  /// output filename
  str output;
} Config;

/// Drops the contents of a config
void config_drop(Config* config);

#endif // !CONFIG_H
