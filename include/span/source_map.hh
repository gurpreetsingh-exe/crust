#ifndef SOURCE_MAP_H
#define SOURCE_MAP_H

/// A source file
struct SourceFile {
  const fs::path path;
  const std::string content;

  SourceFile(const fs::path filepath, const std::string source);
  // SourceFile(const SourceFile&& sf) {
  //
  // }

  // SourceFile(const SourceFile&) = delete;
  // auto operator=(const SourceFile&) -> SourceFile& = delete;

  [[gnu::always_inline]] auto len() const -> usize { return content.size(); }
  [[gnu::always_inline]] auto operator[](usize i) const -> char {
    return content[i];
  }
};

/// Source Map
/// contains all the files used in one compile session
class SourceMap {
public:
  SourceMap() {}

public:
  /// checks if the given `path` already exists in the map and returns that file
  /// otherwise opens and reads the file and adds it to the map
  [[nodiscard]] auto new_file(const fs::path& path) -> std::shared_ptr<SourceFile>;

  /// loads the file
  [[nodiscard]] auto load_file(const fs::path& path) -> std::shared_ptr<SourceFile>;

private:
  std::unordered_map<fs::path, std::shared_ptr<SourceFile>> files;
};

#endif // !SOURCE_MAP_H
