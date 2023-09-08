#ifndef VECTOR_H
#define VECTOR_H

#define vector(T)                                                              \
  struct {                                                                     \
    T* ptr;                                                                    \
    usize len;                                                                 \
    usize cap;                                                                 \
  }

#endif // !VECTOR_H
