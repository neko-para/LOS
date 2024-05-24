#pragma once

#include <stdint.h>

namespace los::gdt {

enum {
  access = 1 << 0,
  read_write = 1 << 1,
  direction_confroming = 1 << 2,
  executable = 1 << 3,
  not_system = 1 << 4,
  dpl0 = 0,
  dpl1 = 1 << 5,
  dpl2 = 1 << 6,
  dpl3 = 3 << 5,
  present = 1 << 7,

  long_ = 1 << 1,
  size = 1 << 2,
  granularity = 1 << 3
};

struct gdt_t {
  uint16_t limit_lo;
  uint16_t base_lo;
  uint8_t base_mi;
  uint8_t access;
  uint8_t limit_hi : 4;
  uint8_t flag : 4;
  uint8_t base_hi;

  static void init();

  void setbase(uint32_t base) {
    base_lo = base & 0xFFFF;
    base_mi = (base >> 16) & 0xFF;
    base_hi = (base >> 24) & 0xFF;
  }

  void setlimit(uint32_t limit) {
    limit_lo = limit & 0xFFFF;
    limit_hi = (limit >> 16) & 0xF;
  }

  void set(uint32_t base, uint32_t limit, uint8_t access, uint8_t flag) {
    setbase(base);
    setlimit(limit);
    this->access = access;
    this->flag = flag;
  }
};

} // namespace los::gdt