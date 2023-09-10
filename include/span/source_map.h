#ifndef SOURCE_MAP_H
#define SOURCE_MAP_H

#include <alloc/str.h>
#include <alloc/vector.h>

typedef struct {
  const str* path;
  char* src;
  usize size;
} SourceFile;

void file_drop(const SourceFile* file);

typedef struct SourceMap {
  vector(SourceFile*) files;
} SourceMap;

[[nodiscard]] SourceFile* source_map_load_file(SourceMap* sm, const str* path);

#endif // !SOURCE_MAP_H
