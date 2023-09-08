#include <alloc/alloc.h>
#include <session/args.h>
#include <session/session.h>

int main(int argc, char** argv) {
  Config config = {};
  config_from_args(argc, argv, &config);
  Session* sess = session_new(config);

  printf("%s\n", config.input.buf);
  session_drop(sess);
  drop(sess);

  return 0;
}
