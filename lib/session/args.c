#include <session/args.h>

NORETURN void usage(const char* arg0) {
  printf("Usage: %s [options] input...\n"
         "\nOptions:\n"
         "    -h, --help           Display help information\n"
         "\n",
         arg0);
  exit(1);
}

void config_from_args(i32 argc, char** argv, Config* r_config) {
  char* arg0 = argv[0];
  if (argc < 2) {
    usage(arg0);
  }
  ++argv;
  --argc;

  for (usize i = 0; i < (usize)argc; ++i) {
    char* arg = argv[i];
    if (arg[0] == '-') {
      switch (arg[1]) {
        case '-': {
          if (STREQ(arg, "--help")) {
            usage(arg0);
          } else {
            eprint("Unknown option `%s`\n", arg);
          }
          break;
        }
        case 'h': usage(arg0);
        default: eprint("Unknown option `%s`\n", arg);
      }
    } else {
      str s = str_new(arg);
      if (str_is_empty(s))
        continue;
      if (str_is_empty(r_config->input)) {
        r_config->input = s;
      } else {
        eprint("multiple input files provided\n");
      }
    }
  }

  if (str_is_empty(r_config->input)) {
    eprint("no input file provided\n");
  }
}
