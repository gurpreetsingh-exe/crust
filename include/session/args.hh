#ifndef ARGS_H
#define ARGS_H

#include <session/config.hh>

namespace crust {

auto config_from_args(i32 argc, char** argv) -> Config;

} // namespace crust

#endif // !ARGS_H
