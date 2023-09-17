#ifndef SOURCE_MAP_H
#define SOURCE_MAP_H

#include <span/loc.hh>
#include <span/span.hh>

namespace crust {

using RelativeBytePos = u32;
using BytePos = u32;
using CharPos = usize;

/// A source file
struct SourceFile {
  const fs::path path;
  const std::string content;
  BytePos start_pos;
  RelativeBytePos source_len;
  std::vector<RelativeBytePos> lines;

  SourceFile(const fs::path filepath, const std::string source);

  [[nodiscard]] [[gnu::always_inline]] auto operator[](usize i) const -> char {
    return content[i];
  }

  [[nodiscard]] [[gnu::always_inline]]
  auto len() const -> usize {
    return content.size();
  }

  [[nodiscard]] [[gnu::always_inline]]
  auto relative_position(BytePos pos) -> RelativeBytePos {
    return pos - start_pos;
  }

  [[nodiscard]] [[gnu::always_inline]]
  auto absolute_position(RelativeBytePos pos) -> BytePos {
    return pos + start_pos;
  }

  [[nodiscard]] [[gnu::always_inline]]
  auto end_position() -> BytePos {
    return absolute_position(source_len);
  }

  [[nodiscard]]
  auto lookup_line(RelativeBytePos pos) -> Option<usize>;

  [[nodiscard]]
  auto lookup_file_pos(RelativeBytePos pos) -> std::tuple<usize, CharPos>;

  [[nodiscard]]
  auto lookup_file_pos_with_col_display(BytePos pos)
      -> std::tuple<usize, CharPos>;
};

/// Source Map
/// contains all the files used in one compile session
class SourceMap {
public:
  SourceMap() {}

public:
  /// checks if the given `path` already exists in the map and returns that file
  /// otherwise opens and reads the file and adds it to the map
  [[nodiscard]] auto new_file(fs::path path) -> Rc<SourceFile>;

  /// loads the file
  [[nodiscard]]
  auto load_file(const fs::path& path) -> std::string;

  [[nodiscard]]
  auto register_file(fs::path path, SourceFile file) -> Rc<SourceFile>;

  auto lookup_source_file_idx(BytePos pos) -> usize;

  [[nodiscard]]
  auto lookup_source_file(BytePos pos) -> Rc<SourceFile>;

  auto lookup_char_pos(BytePos pos) -> Loc;

  auto span_to_location_info(Span span)
      -> std::tuple<Option<Rc<SourceFile>>, usize, usize, usize, usize>;

  [[nodiscard]]
  auto span_to_string(Span span) -> std::string;

private:
  std::unordered_map<fs::path, Rc<SourceFile>> path_to_source_file;
  std::vector<Rc<SourceFile>> source_files;
};

} // namespace crust

#endif // !SOURCE_MAP_H
