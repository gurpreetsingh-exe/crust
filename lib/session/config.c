#include <alloc/alloc.h>
#include <session/config.h>

Config* config_new() { return alloc_zeroed(Config, 1); }

void config_drop(Config* config) {
  str_drop(&config->input);
  str_drop(&config->output);
}
