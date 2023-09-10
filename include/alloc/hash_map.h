#ifndef HASH_MAP_H
#define HASH_MAP_H

#define TBL_SIZE 1000
#define K 0x517cc1b727220a95

#include <alloc/str.h>
#include <time.h>

typedef struct hash_map_entry {
  str key;
  u32 value;
  struct hash_map_entry* next;
} hash_map_entry;

#if defined(hash)
#define hash_not_found() PANIC("hash macro not found")
#else
#define hash_not_found()
#endif

#define hash_map(K, V)                                                         \
  struct {                                                                     \
    FxHasher* hasher;                                                          \
    vector(hash_map_entry) entries;                                            \
    usize size;                                                                \
  }

#define hash_map_init(K, V, map)                                               \
  do {                                                                         \
    (map).hasher = (FxHasher*)malloc(sizeof(FxHasher));                        \
    srand((u32)time(0));                                                       \
    (map).hasher->hash = (usize)rand();                                        \
    vector_reserve(hash_map_entry, (map).entries, TBL_SIZE);                   \
    (map).size = 0;                                                            \
  } while (0)

#define hash_map_insert(map, k, v)                                             \
  do {                                                                         \
    hash_not_found();                                                          \
    usize h = hash((map), (k));                                                \
    hash_map_entry* entry = &(map).entries.ptr[h];                             \
    if (str_is_null(entry->key)) {                                             \
      entry->key = (k);                                                        \
      entry->value = (v);                                                      \
      (map).size++;                                                            \
    } else {                                                                   \
    }                                                                          \
  } while (0);

#define hash_map_get(map, k, v)                                                \
  ({                                                                           \
    hash_not_found();                                                          \
    usize h = hash((map), (k));                                                \
    hash_map_entry* entry = &(map).entries.ptr[h];                             \
    if (str_is_null(entry->key)) {                                             \
      (v) = (u32)-1;                                                           \
    } else {                                                                   \
      if (!str_eq(entry->key, k)) {                                            \
        (v) = (u32)-1;                                                         \
      } else {                                                                 \
        (v) = entry->value;                                                    \
      }                                                                        \
    }                                                                          \
  })

#define hash_map_iter(map)                                                     \
  for (usize i = 0; i < (map).entries.cap; ++i) {                              \
    hash_map_entry entry = (map).entries.ptr[i];                               \
    if (str_is_null(entry.key))                                                \
      continue;                                                                \
    printf("%s - %ld\n", ((str*)entry.key)->buf, ((usize)entry.value));        \
  }

#define hash_map_drop(map)                                                     \
  do {                                                                         \
    vector_drop((map).entries);                                                \
    free((map).hasher);                                                        \
  } while (0)

typedef struct {
  usize hash;
} FxHasher;

void fx_add_to_hash(FxHasher* fx, usize i);
void fx_write_u8(FxHasher* fx, u8 i);
void fx_write(FxHasher* fx, str bytes);
void fx_write_str(FxHasher* fx, str s);
u64 fx_finish(FxHasher* fx);

#endif // !HASH_MAP_H
