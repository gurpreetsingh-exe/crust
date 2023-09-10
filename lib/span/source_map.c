#include <alloc/alloc.h>
#include <errno.h>
#include <span/source_map.h>

void file_drop(const SourceFile* file) {
  drop(file->src);
  drop(file);
}

SourceFile* source_map_load_file(SourceMap* sm, const str* path) {
  (void)sm;
  FILE* f = fopen(path->buf, "r");

  if (!f) {
    eprint("cannot open \"%s\"\n  %s\n", path->buf, strerror(errno));
  }

  if (fseek(f, 0L, SEEK_END)) {
    eprint("%s\n", strerror(errno));
  }

  i64 file_size = ftell(f);
  if (file_size < 0) {
    eprint("cannot get file size\n  %s\n", strerror(errno));
  }

  rewind(f);

  usize size = (usize)file_size;
  char* content = alloc_zeroed(char, size + 1);
  usize read_bytes = fread(content, 1, size, f);

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

  SourceFile* file = alloc(SourceFile);
  file->path = path;
  file->src = content;
  file->size = size;
  return file;
}
