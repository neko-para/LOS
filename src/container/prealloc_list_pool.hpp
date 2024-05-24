#pragma once

#include <stddef.h>
#include <stdint.h>

namespace los::container {

template <typename data_t> struct prealloc_list_pool_t {
  struct node_t : public data_t {
    node_t *next;
  };

  node_t *data;
  node_t *free;

  void init(node_t *base, size_t size) {
    data = base;
    free = nullptr;
    for (size_t i = 1; i < size; i++) {
      data[i - 1].next = &data[i];
    }
    data[size - 1].next = nullptr;
  }

  node_t *alloc() {
    auto node = free;
    free = node->next;
    return node;
  }

  void release(node_t *node) {
    node->next = free;
    free = node;
  }
};

} // namespace los::container