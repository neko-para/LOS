#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "io/vga.hpp"
#include "lib/string.hpp"

extern "C" void kernel_main(void) {
  using namespace los;
  vga::fill(vga::make(' ', vga::attr));

  vga::write("Hello world!\nLOS");
}