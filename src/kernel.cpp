#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "boot/info.hpp"
#include "gdt/init.hpp"
#include "io/vga.hpp"
#include "lib/string.hpp"
#include "memory/memory.hpp"

void setup_memory(los::boot::boot_info_t *info) {
  for (uint32_t i = 0;
       i < info->mmap_length / sizeof(los::boot::memory_map_entry_t); i++) {
    los::boot::memory_map_entry_t *me = info->mmap_addr + i;
    if (me->addr_hi || me->len_hi) {
      continue;
    }
    if (me->type == 1 && me->addr_lo > 0) { // skip first
      if (me->addr_lo == 0x100000) {
        // acually always one hole
        // use &image_size ~ 16M as kernel heap, 16M ~ 1G as pages.
        uint32_t size = (uint32_t)&image_size;
        me->addr_lo = (size + 0xFFF) & (~0xFFF);
        los::memory::add(me->addr_lo, 0x1000000 - me->addr_lo);
        // Frame::init(0x1000000, (me->len_lo - 0xF00000) >> 12);
        // Page::init(me->len_lo + 0x100000);
      }
    }
  }
}

extern "C" void kernel_main(los::boot::boot_info_t *boot_info) {
  using namespace los;

  gdt::init();

  memory::init();
  setup_memory(boot_info);

  vga::fill(vga::make(' ', vga::attr));

  vga::write("Hello world!\nLOS");
}