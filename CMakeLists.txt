cmake_minimum_required(VERSION 3.24)
project(LOS)

enable_language(ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i386)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_C_FLAGS "-target i386-elf -m32 -ffreestanding -fno-builtin")
set(CMAKE_CXX_FLAGS "-target i386-elf -m32 -ffreestanding -fno-builtin -fno-exceptions -fno-rtti")
set(CMAKE_ASM_FLAGS "-target i386-elf -m32")

add_library(kernel STATIC src/kernel.cpp src/boot.s)

set(K_LINKER ld.lld)
set(K_LINK_FLAGS "-m elf_i386 -nostdlib -T ${CMAKE_SOURCE_DIR}/src/kernel.ld")
set(K_OUTPUT "kernel.bin")

separate_arguments(K_LINK_FLAGS_LIST UNIX_COMMAND "${K_LINK_FLAGS}")

# 添加自定义链接命令
add_custom_command(
  OUTPUT ${K_OUTPUT}
  COMMAND ${K_LINKER} ${K_LINK_FLAGS_LIST} -o ${K_OUTPUT} $<TARGET_FILE:kernel>
  DEPENDS kernel
  COMMENT "Linking ${K_OUTPUT}"
)

# 添加自定义目标
add_custom_target(
  kernel_image ALL
  DEPENDS ${K_OUTPUT}
  COMMENT "Building kernel"
)
