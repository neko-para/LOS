#pragma once

#include <stdint.h>

extern "C" uint32_t image_size; // declare in kernel.ld

namespace los::boot {

struct memory_map_entry_t {
  uint32_t size;
  uint32_t addr_lo;
  uint32_t addr_hi;
  uint32_t len_lo;
  uint32_t len_hi;
  uint32_t type;
};

struct boot_info_t {
  uint32_t flags;

  uint32_t mem_lower;
  uint32_t mem_upper;

  uint32_t boot_device;

  uint32_t cmdline;

  uint32_t mods_count;
  uint32_t mods_addr;

  uint32_t syms[4];

  uint32_t mmap_length;
  memory_map_entry_t *mmap_addr;

  uint32_t drives_length;
  uint32_t drives_addr;

  uint32_t config_table;

  char *boot_loader_name;

  uint32_t apm_table;

  uint32_t vbe_control_info;
  uint32_t vbe_mode_info;
  uint16_t vbe_mode;
  uint16_t vbe_interface_seg;
  uint16_t vbe_interface_off;
  uint16_t vbe_interface_len;

  uint32_t framebuffer_addr_lo;
  uint32_t framebuffer_addr_hi;
  uint32_t framebuffer_pitch;
  uint32_t framebuffer_width;
  uint32_t framebuffer_height;
  uint8_t framebuffer_bpp;
  uint8_t framebuffer_type;
  uint8_t color_info[6];
};

} // namespace los::boot