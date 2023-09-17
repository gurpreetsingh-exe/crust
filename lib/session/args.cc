#include <session/args.hh>

namespace crust {

[[noreturn]] void usage(const char* arg0) {
  printf("Usage: %s [options] input...\n"
         "\nOptions:\n"
         "    -h, --help           Display help information\n"
         "\n",
         arg0);
  exit(1);
}

auto config_from_args(i32 argc, char** argv) -> Config {
  char* arg0 = argv[0];
  if (argc < 2) {
    usage(arg0);
  }
  ++argv;
  --argc;

  auto config = Config {};

  for (usize i = 0; i < static_cast<usize>(argc); ++i) {
    std::string_view arg = argv[i];
    if (arg[0] == '-') {
      switch (arg[1]) {
        case '-': {
          if (arg == "--help") {
            usage(arg0);
          } else {
            eprint("Unknown option `%s`\n", arg.data());
          }
          break;
        }
        case 'h':
          usage(arg0);
        default:
          eprint("Unknown option `%s`\n", arg.data());
      }
    } else {
      if (arg.empty())
        continue;
      if (config.input.empty()) {
        config.input = arg;
      } else {
        eprint("multiple input files provided\n");
      }
    }
  }

  if (config.input.empty()) {
    eprint("no input file provided\n");
  }

  return config;
}

} // namespace crust
