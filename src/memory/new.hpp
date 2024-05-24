#pragma once

#include <stddef.h>

void *operator new(size_t s);
void *operator new[](size_t s);
void operator delete(void *ptr, size_t) noexcept;
void operator delete[](void *ptr, size_t) noexcept;
void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;

inline void *operator new(size_t, void *p) noexcept { return p; }

inline void *operator new[](size_t, void *p) noexcept { return p; }

inline void operator delete(void *, void *) noexcept {};

inline void operator delete[](void *, void *) noexcept {};
