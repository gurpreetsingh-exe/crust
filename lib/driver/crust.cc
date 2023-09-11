#include <session/args.hh>
#include <session/session.hh>

int main(int argc, char** argv) {
  auto config = config_from_args(argc, argv);
  auto sess = Session(config);
  sess.run_compiler();
  return 0;
}
