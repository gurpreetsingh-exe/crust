#include <alloc/hash_map.h>

/// https://github.com/rust-lang/rust/blob/5ede9408945b46ab183dd228253297bdf62304f7/library/proc_macro/src/bridge/fxhash.rs#L50

usize rotl64c(usize x, usize n) {
  ASSERT(n < 64);
  return (x << n) | (x >> (-n & 63));
}

void fx_add_to_hash(FxHasher* fx, usize i) {
  fx->hash = ((rotl64c(fx->hash, 5) ^ i) * K) % SIZE_MAX;
}

#define READ(T, bytes)                                                         \
  ({                                                                           \
    T sz = 0;                                                                  \
    memcpy(&sz, bytes.buf, sizeof(T));                                         \
    sz;                                                                        \
  })

void fx_write_u8(FxHasher* fx, u8 i) { fx_add_to_hash(fx, i); }

void fx_write(FxHasher* fx, str bytes) {
  FxHasher hash = { .hash = fx->hash };
  while (bytes.len >= sizeof(usize)) {
    fx_add_to_hash(&hash, READ(usize, bytes));
    bytes.buf += sizeof(usize);
    bytes.len -= sizeof(usize);
  }
  if (sizeof(usize) > 4 && bytes.len >= 4) {
    fx_add_to_hash(&hash, READ(u32, bytes));
    bytes.buf += sizeof(u32);
    bytes.len -= sizeof(u32);
  }
  if (sizeof(usize) > 2 && bytes.len >= 2) {
    fx_add_to_hash(&hash, READ(u16, bytes));
    bytes.buf += sizeof(u16);
    bytes.len -= sizeof(u16);
  }
  if (sizeof(usize) > 1 && bytes.len >= 1) {
    fx_add_to_hash(&hash, (usize)bytes.buf[0]);
  }
  fx->hash = hash.hash;
}

void fx_write_str(FxHasher* fx, str s) {
  fx_write(fx, s);
  fx_write_u8(fx, 0xff);
}

u64 fx_finish(FxHasher* fx) { return fx->hash; }
