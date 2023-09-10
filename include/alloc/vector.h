#ifndef VECTOR_H
#define VECTOR_H

#include <alloc/alloc.h>

#define vector(T)                                                              \
  struct {                                                                     \
    T* ptr;                                                                    \
    usize len;                                                                 \
    usize cap;                                                                 \
  }

#define vector_init(T, vec)                                                    \
  do {                                                                         \
    (vec).ptr = (T*)malloc(sizeof(T) * 4);                                     \
    (vec).len = 0;                                                             \
    (vec).cap = 4;                                                             \
  } while (0)

#define vector_reserve(T, vec, capacity)                                       \
  do {                                                                         \
    (vec).ptr = (T*)calloc((capacity), sizeof(T));                             \
    (vec).len = 0;                                                             \
    (vec).cap = (capacity);                                                    \
  } while (0)

#define vector_push(T, vec, item)                                              \
  do {                                                                         \
    if ((vec).len >= (vec).cap) {                                              \
      (vec).cap *= 2;                                                          \
      (vec).ptr = (T*)realloc((vec).ptr, (vec).cap * sizeof(T));               \
    }                                                                          \
    (vec).ptr[(vec).len] = (item);                                             \
    (vec).len++;                                                               \
  } while (0);

#define vector_map(T, vec, name, ...)                                          \
  do {                                                                         \
    for (usize i = 0; i < (vec).len; ++i) {                                    \
      T* name = &(vec).ptr[i];                                                 \
      __VA_ARGS__;                                                             \
    }                                                                          \
  } while (0)

#define vector_drop(vec) free((vec).ptr)

#endif // !VECTOR_H
