#ifndef ALLOC_H
#define ALLOC_H

#define alloc(T) ((T*)malloc(sizeof(T)))

#define alloc_zeroed(T, n) ((T*)calloc((n), sizeof(T)))

#define drop(v)                                                                \
  do { free((void*)v); } while (0)

#endif // !ALLOC_H
