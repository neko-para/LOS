#pragma once

#include <stdint.h>

#include "../lib/algorithm.hpp"

namespace los::vga {

enum class color_t : uint8_t {
  black = 0,
  blue = 1,
  green = 2,
  cyan = 3,
  red = 4,
  magenta = 5,
  brown = 6,
  light_grey = 7,
  dark_grey = 8,
  light_blue = 9,
  light_green = 10,
  light_cyan = 11,
  light_red = 12,
  light_magenta = 13,
  light_brown = 14,
  white = 15,
};

using attr_t = uint8_t;
using entry_t = uint16_t;

constexpr inline attr_t make(color_t fg, color_t bg) {
  return (static_cast<uint8_t>(bg) << 4 | static_cast<uint8_t>(fg));
}

constexpr inline entry_t make(char ch, attr_t a) {
  return static_cast<uint16_t>(a) << 8 | static_cast<uint8_t>(ch);
}

constexpr inline entry_t make(char ch, color_t fg, color_t bg) {
  return make(ch, make(fg, bg));
}

constexpr int width = 80;
constexpr int height = 25;

inline entry_t *const buffer = reinterpret_cast<uint16_t *>(0xB8000);

inline int row = 0;
inline int col = 0;
inline attr_t attr = make(color_t::light_grey, color_t::black);

inline void fill(entry_t e) {
  ::los::fill(buffer, buffer + width * height, e);
}

inline void put(char ch) {
  if (ch == '\n') {
    row = row + 1;
    col = 0;
  } else if (ch == '\r') {
    col = 0;
  } else if (ch == '\b') {
    col = col == 0 ? 0 : col - 1;
  } else if (ch >= ' ') {
    buffer[row * width + col] = make(ch, attr);
    col += 1;
    if (col == width) {
      col = 0;
      row += 1;
    }
  }
}

inline void write(const char* str) {
  while (*str) {
    put(*str);
    str++;
  }
}

} // namespace los::vga