#include <span/source_map.hh>

SourceFile::SourceFile(const fs::path filepath, const std::string source)
    : path(filepath), content(std::move(source)), start_pos(0),
      source_len(static_cast<u32>(content.length())) {
  u32 i;
  for (i = 0; i < source_len; ++i) {
    if (content[i] == '\n') {
      lines.push_back(i + 1);
    }
  }

  if (not lines.empty()) {
    u32 last = lines[lines.size() - 1];
    ASSERT(source_len >= last);
    if (last == source_len) {
      lines.pop_back();
    }
  }
}

auto SourceFile::lookup_line(RelativeBytePos pos) -> std::optional<usize> {
  const auto pp = std::partition_point(lines.cbegin(), lines.cend(),
                                       [&](auto x) { return x <= pos; });
  auto d = static_cast<usize>(std::distance(lines.cbegin(), pp));
  if (d == 0)
    return {};
  return d - 1;
}

auto SourceFile::lookup_file_pos(RelativeBytePos pos)
    -> std::tuple<usize, CharPos> {
  auto l = lookup_line(pos);
  if (not l.has_value()) {
    return { 0, pos };
  }

  auto linepos = lines[l.value()];
  auto line = l.value() + 1;
  usize col = static_cast<CharPos>(pos - linepos);
  return { line, col };
}

auto SourceFile::lookup_file_pos_with_col_display(BytePos pos)
    -> std::tuple<usize, CharPos> {
  auto rel_pos = relative_position(pos);
  return lookup_file_pos(rel_pos);
}

auto SourceMap::new_file(fs::path path) -> std::shared_ptr<SourceFile> {
  const auto& canonical_path = fs::canonical(path);
  if (path_to_source_file.contains(canonical_path)) {
    return path_to_source_file.at(canonical_path);
  } else {
    auto content = load_file(canonical_path);
    auto file = SourceFile(canonical_path, std::move(content));
    return register_file(canonical_path, std::move(file));
  }
}

auto SourceMap::register_file(fs::path path, SourceFile file)
    -> std::shared_ptr<SourceFile> {
  file.start_pos =
      source_files.empty()
          ? 0
          : source_files[source_files.size() - 1]->end_position() + 1;
  auto source_file = std::make_shared<SourceFile>(file);
  source_files.push_back(source_file);
  path_to_source_file.insert(std::make_pair(path, source_file));
  return source_file;
}

auto SourceMap::load_file(const fs::path& path) -> std::string {
  FILE* f = std::fopen(path.c_str(), "r");

  if (not f) {
    eprint("cannot open \"%s\"\n  %s\n", path.c_str(), strerror(errno));
  }

  if (std::fseek(f, 0L, SEEK_END)) {
    eprint("%s\n", strerror(errno));
  }

  auto file_size = ftell(f);
  if (file_size < 0) {
    eprint("cannot get file size\n  %s\n", strerror(errno));
  }

  rewind(f);

  auto size = static_cast<usize>(file_size);
  auto content = std::string();
  content.resize(size);
  auto read_bytes = std::fread(content.data(), 1, size, f);

  if (read_bytes != size) {
    int e = feof(f);
    if (e) {
      eprint("unexpected end of file\n");
    } else {
      e = ferror(f);
      eprint("cannot read file\n  %s\n", strerror(e));
    }
  }

  fclose(f);

  return content;
}

auto SourceMap::lookup_source_file_idx(BytePos pos) -> usize {
  const auto pp =
      std::partition_point(source_files.cbegin(), source_files.cend(),
                           [&](auto x) { return x->start_pos <= pos; });
  auto d = std::distance(source_files.cbegin(), pp) - 1;
  return static_cast<usize>(d);
}

auto SourceMap::lookup_source_file(BytePos pos) -> std::shared_ptr<SourceFile> {
  usize idx = lookup_source_file_idx(pos);
  return source_files[idx];
}

auto SourceMap::lookup_char_pos(BytePos pos) -> Loc {
  auto file = lookup_source_file(pos);
  auto [line, col] = file->lookup_file_pos_with_col_display(pos);
  return Loc(file, line, col);
}

auto SourceMap::span_to_location_info(Span span)
    -> std::tuple<std::optional<std::shared_ptr<SourceFile>>, usize, usize,
                  usize, usize> {
  if (source_files.empty())
    return { {}, 0, 0, 0, 0 };

  auto lo = lookup_char_pos(span.lo);
  auto hi = lookup_char_pos(span.hi);
  return { lo.file, lo.line, lo.col + 1, hi.line, hi.col + 1 };
}

auto SourceMap::span_to_string(Span span) -> std::string {
  auto [source_file, lo_line, lo_col, hi_line, hi_col] =
      span_to_location_info(span);

  auto filename = source_file.has_value()
                      ? source_file.value()->path.filename().string()
                      : "no-location";

  return std::format("{}:{}:{}: {}:{}", filename, lo_line, lo_col, hi_line,
                     hi_col);
}
