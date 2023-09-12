#ifndef LOC_H
#define LOC_H

struct SourceFile;

using CharPos = usize;

struct Loc {
  std::shared_ptr<SourceFile> file;
  usize line;
  CharPos col;

  Loc(std::shared_ptr<SourceFile> f, usize l, CharPos c)
      : file(f), line(l), col(c) {}
};

#endif // !LOC_H
