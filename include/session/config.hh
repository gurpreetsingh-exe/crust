#ifndef CONFIG_H
#define CONFIG_H

namespace crust {

/// Command line options
struct Config {
  /// Input filename
  fs::path input;
  /// Output filename
  Option<fs::path> output;
};

} // namespace crust

#endif // !CONFIG_H
