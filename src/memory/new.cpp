#include "new.hpp"
#include "memory.hpp"

void *operator new(size_t s) { return los::memory::alloc(s); }

void *operator new[](size_t s) { return los::memory::alloc(s); }

void operator delete(void *ptr, size_t) noexcept { los::memory::free(ptr); }

void operator delete[](void *ptr, size_t) noexcept { los::memory::free(ptr); }

void operator delete(void *ptr) noexcept { los::memory::free(ptr); }

void operator delete[](void *ptr) noexcept { los::memory::free(ptr); }
