#ifndef CONFIG_H
#define CONFIG_H

/// Command line options
struct Config {
  /// Input filename
  fs::path input;
  /// Output filename
  std::optional<fs::path> output;
};

#endif // !CONFIG_H
