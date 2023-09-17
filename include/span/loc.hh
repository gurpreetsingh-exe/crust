#ifndef LOC_H
#define LOC_H

namespace crust {

struct SourceFile;

using CharPos = usize;

struct Loc {
  Rc<SourceFile> file;
  usize line;
  CharPos col;

  Loc(Rc<SourceFile> f, usize l, CharPos c) : file(f), line(l), col(c) {}
};

} // namespace crust

#endif // !LOC_H
