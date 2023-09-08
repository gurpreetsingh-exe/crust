#ifndef SOURCE_MAP_H
#define SOURCE_MAP_H

#include <alloc/str.h>
#include <alloc/vector.h>

typedef struct {
  const str* path;
  char* src;
  usize size;
} SourceFile;

typedef struct SourceMap {
  vector(SourceFile) files;
} SourceMap;

NODISCARD SourceFile source_map_load_file(SourceMap* sm, const str* path);

#endif // !SOURCE_MAP_H
