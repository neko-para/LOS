#pragma once

#include "gdt.hpp"
#include "tss.hpp"

extern "C" void setGdt(void *pgdt, uint16_t size);
extern "C" void reloadSegments();

namespace los::gdt {

constexpr int entry_count = 6;
inline uint8_t gdt_data[sizeof(gdt_t) * entry_count];

inline void init() {
  sys_tss.ss0 = 0x10;

  gdt_t *gdt = reinterpret_cast<gdt_t *>(gdt_data);
  gdt[0].set(0, 0, 0, 0);
  gdt[1].set(0, 0x000FFFFF,
             present | dpl0 | not_system | executable | read_write,
             granularity | size);
  gdt[2].set(0, 0x000FFFFF, present | dpl0 | not_system | read_write,
             granularity | size);
  gdt[3].set(0, 0x000FFFFF,
             present | dpl3 | not_system | executable | read_write,
             granularity | size);
  gdt[4].set(0, 0x000FFFFF, present | dpl3 | not_system | read_write,
             granularity | size);
  gdt[5].set(reinterpret_cast<uint32_t>(&sys_tss), sizeof(tss_t),
             present | executable | access, 0);
  setGdt(gdt, sizeof(gdt_data));
  reloadSegments();
  asm volatile("movw $0x28, %ax; ltr %ax;");
}

} // namespace los::gdt