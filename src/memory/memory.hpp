#pragma once

#include <stdint.h>

#include "../container/prealloc_list_pool.hpp"

namespace los::memory {

namespace details {

constexpr uint32_t count = 4096;
constexpr uint32_t magic = 0x317839FE;

struct mem_slice_t {
  uint32_t base;
  uint32_t size;

  uint32_t lower() const { return base; }
  uint32_t upper() const { return base + size; }

  void merge_lower(size_t s) {
    base -= s;
    size += s;
  }
  void merge_upper(size_t s) { size += s; }

  uint32_t alloc_lower(size_t s) {
    base += s;
    size -= s;
    return base - s;
  }
  uint32_t alloc_upper(size_t s) {
    size -= s;
    return base + size;
  }
};

using mem_pool_t = container::prealloc_list_pool_t<mem_slice_t>;
using mem_pool_node_t = mem_pool_t::node_t;

inline mem_pool_node_t node_pool[count];
inline mem_pool_node_t *rest;
inline mem_pool_t pool;

inline void init() {
  pool.init(node_pool, count);
  rest = nullptr;
}

inline mem_pool_node_t *make(size_t base, size_t size,
                             mem_pool_node_t *next = nullptr) {
  auto node = pool.alloc();
  node->base = base;
  node->size = size;
  node->next = next;
  return node;
}

inline void insert(size_t base, size_t size) {
  if (!rest) {
    rest = make(base, size);
  } else if (base < rest->base) {
    if (base + size == rest->base) {
      rest->merge_lower(size);
    } else {
      rest = make(base, size, rest);
    }
  } else if (rest->next == nullptr) {
    if (rest->upper() == base) {
      rest->merge_upper(size);
    } else {
      rest->next = make(base, size);
    }
  } else {
    auto prev = rest;
    auto curr = rest->next;
    while (curr) {
      if (prev->upper() < base && base < curr->lower()) {
        if (prev->upper() == base) {
          if (base + size == curr->lower()) {
            prev->merge_upper(size + curr->size);
            prev->next = curr->next;
            pool.release(curr);
          } else {
            prev->merge_upper(size);
          }
        } else if (base + size == curr->lower()) {
          curr->merge_lower(size);
        } else {
          prev->next = make(base, size, curr);
        }
        return;
      }
      prev = curr;
      curr = curr->next;
    }
    if (prev->upper() == base) {
      prev->merge_upper(size);
    } else {
      prev->next = make(base, size);
    }
  }
}

inline uint32_t alloc(size_t size) {
  if (!rest) {
    return 0;
  } else if (!rest->next) {
    if (rest->size >= size) {
      return rest->alloc_lower(size);
    } else {
      return 0;
    }
  } else {
    auto prev_next = &rest;
    auto curr = rest;
    mem_pool_node_t *find = nullptr;
    while (curr) {
      if (curr->size == size) {
        auto ret = curr->base;
        *prev_next = curr->next;
        pool.release(curr);
        return ret;
      }
      if (curr->size > size && (!find || curr->size < find->size)) {
        find = curr;
      }
      prev_next = &curr->next;
      curr = curr->next;
    }
    if (!find) {
      return 0;
    }
    return find->alloc_lower(size);
  }
}

} // namespace details

inline void init() { details::init(); }

inline void add(uint32_t base, uint32_t size) {
  base = (base + 3) & (~3);
  size &= ~3;
  details::insert(base, size);
}

inline void *alloc(uint32_t size) {
  auto real_size = (size + 8 + 3) & (~3);
  auto addr = details::alloc(real_size);
  uint32_t *ptr = reinterpret_cast<uint32_t *>(addr);
  *ptr = real_size;
  ptr[real_size / 4 - 1] = details::magic ^ real_size;
  return &ptr[1];
}

inline void free(void *ptr) {
  uint32_t *uptr = static_cast<uint32_t *>(ptr);
  auto size = uptr[-1];
  if ((uptr[size / 4 - 2] ^ size) != details::magic) {
    return;
  }
  details::insert(reinterpret_cast<uint32_t>(&uptr[-1]), size);
}

} // namespace los::memory