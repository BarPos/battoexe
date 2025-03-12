const char *exeCode = "#define _CRT_SECURE_NO_WARNINGS  \n\
 \n\
#pragma region b64 \n\
#ifndef BASE64_HPP_ \n\
#define BASE64_HPP_ \n\
 \n\
#include <algorithm> \n\
#include <array> \n\
#include <cassert> \n\
#include <cstdint> \n\
#include <cstring> \n\
#include <stdexcept> \n\
#include <string> \n\
#include <string_view> \n\
#include <type_traits> \n\
 \n\
#if defined(__cpp_lib_bit_cast) \n\
#include <bit>  // For std::bit_cast. \n\
#endif \n\
 \n\
namespace base64 { \n\
 \n\
namespace detail { \n\
 \n\
#if defined(__cpp_lib_bit_cast) \n\
using std::bit_cast; \n\
#else \n\
template <class To, class From> \n\
std::enable_if_t<sizeof(To) == sizeof(From) && \n\
                     std::is_trivially_copyable_v<From> && \n\
                     std::is_trivially_copyable_v<To>, \n\
                 To> \n\
bit_cast(const From& src) noexcept { \n\
  static_assert(std::is_trivially_constructible_v<To>, \n\
                \"This implementation additionally requires \" \n\
                \"destination type to be trivially constructible\"); \n\
 \n\
  To dst; \n\
  std::memcpy(&dst, &src, sizeof(To)); \n\
  return dst; \n\
} \n\
#endif \n\
 \n\
inline constexpr char padding_char{'='}; \n\
inline constexpr uint32_t bad_char{0x01FFFFFF}; \n\
 \n\
#if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__) \n\
#if (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) ||  \ \n\
    (defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN) ||              \ \n\
    (defined(_BYTE_ORDER) && _BYTE_ORDER == _BIG_ENDIAN) ||                 \ \n\
    (defined(BYTE_ORDER) && BYTE_ORDER == BIG_ENDIAN) ||                    \ \n\
    (defined(__sun) && defined(__SVR4) && defined(_BIG_ENDIAN)) ||          \ \n\
    defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || \ \n\
    defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__) ||         \ \n\
    defined(_M_PPC) \n\
#define __BIG_ENDIAN__ \n\
#elif (defined(__BYTE_ORDER__) &&                                              \ \n\
       __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || /* gcc */                 \ \n\
    (defined(__BYTE_ORDER) &&                                                  \ \n\
     __BYTE_ORDER == __LITTLE_ENDIAN) /* linux header */                       \ \n\
    || (defined(_BYTE_ORDER) && _BYTE_ORDER == _LITTLE_ENDIAN) ||              \ \n\
    (defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN) /* mingw header */ || \ \n\
    (defined(__sun) && defined(__SVR4) &&                                      \ \n\
     defined(_LITTLE_ENDIAN)) || /* solaris */                                 \ \n\
    defined(__ARMEL__) ||                                                      \ \n\
    defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) ||      \ \n\
    defined(__MIPSEL) || defined(__MIPSEL__) || defined(_M_IX86) ||            \ \n\
    defined(_M_X64) || defined(_M_IA64) || /* msvc for intel processors */     \ \n\
    defined(_M_ARM) /* msvc code on arm executes in little endian mode */ \n\
#define __LITTLE_ENDIAN__ \n\
#endif \n\
#endif \n\
 \n\
#if !defined(__LITTLE_ENDIAN__) & !defined(__BIG_ENDIAN__) \n\
#error \"UNKNOWN Platform / endianness. Configure endianness explicitly.\" \n\
#endif \n\
 \n\
#if defined(__LITTLE_ENDIAN__) \n\
std::array<std::uint32_t, 256> constexpr decode_table_0 = { \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x000000f8, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x000000fc, \n\
    0x000000d0, 0x000000d4, 0x000000d8, 0x000000dc, 0x000000e0, 0x000000e4, \n\
    0x000000e8, 0x000000ec, 0x000000f0, 0x000000f4, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x00000000, \n\
    0x00000004, 0x00000008, 0x0000000c, 0x00000010, 0x00000014, 0x00000018, \n\
    0x0000001c, 0x00000020, 0x00000024, 0x00000028, 0x0000002c, 0x00000030, \n\
    0x00000034, 0x00000038, 0x0000003c, 0x00000040, 0x00000044, 0x00000048, \n\
    0x0000004c, 0x00000050, 0x00000054, 0x00000058, 0x0000005c, 0x00000060, \n\
    0x00000064, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x00000068, 0x0000006c, 0x00000070, 0x00000074, 0x00000078, \n\
    0x0000007c, 0x00000080, 0x00000084, 0x00000088, 0x0000008c, 0x00000090, \n\
    0x00000094, 0x00000098, 0x0000009c, 0x000000a0, 0x000000a4, 0x000000a8, \n\
    0x000000ac, 0x000000b0, 0x000000b4, 0x000000b8, 0x000000bc, 0x000000c0, \n\
    0x000000c4, 0x000000c8, 0x000000cc, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff}; \n\
 \n\
std::array<std::uint32_t, 256> constexpr decode_table_1 = { \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x0000e003, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x0000f003, \n\
    0x00004003, 0x00005003, 0x00006003, 0x00007003, 0x00008003, 0x00009003, \n\
    0x0000a003, 0x0000b003, 0x0000c003, 0x0000d003, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x00000000, \n\
    0x00001000, 0x00002000, 0x00003000, 0x00004000, 0x00005000, 0x00006000, \n\
    0x00007000, 0x00008000, 0x00009000, 0x0000a000, 0x0000b000, 0x0000c000, \n\
    0x0000d000, 0x0000e000, 0x0000f000, 0x00000001, 0x00001001, 0x00002001, \n\
    0x00003001, 0x00004001, 0x00005001, 0x00006001, 0x00007001, 0x00008001, \n\
    0x00009001, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x0000a001, 0x0000b001, 0x0000c001, 0x0000d001, 0x0000e001, \n\
    0x0000f001, 0x00000002, 0x00001002, 0x00002002, 0x00003002, 0x00004002, \n\
    0x00005002, 0x00006002, 0x00007002, 0x00008002, 0x00009002, 0x0000a002, \n\
    0x0000b002, 0x0000c002, 0x0000d002, 0x0000e002, 0x0000f002, 0x00000003, \n\
    0x00001003, 0x00002003, 0x00003003, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff}; \n\
 \n\
std::array<std::uint32_t, 256> constexpr decode_table_2 = { \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x00800f00, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x00c00f00, \n\
    0x00000d00, 0x00400d00, 0x00800d00, 0x00c00d00, 0x00000e00, 0x00400e00, \n\
    0x00800e00, 0x00c00e00, 0x00000f00, 0x00400f00, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x00000000, \n\
    0x00400000, 0x00800000, 0x00c00000, 0x00000100, 0x00400100, 0x00800100, \n\
    0x00c00100, 0x00000200, 0x00400200, 0x00800200, 0x00c00200, 0x00000300, \n\
    0x00400300, 0x00800300, 0x00c00300, 0x00000400, 0x00400400, 0x00800400, \n\
    0x00c00400, 0x00000500, 0x00400500, 0x00800500, 0x00c00500, 0x00000600, \n\
    0x00400600, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x00800600, 0x00c00600, 0x00000700, 0x00400700, 0x00800700, \n\
    0x00c00700, 0x00000800, 0x00400800, 0x00800800, 0x00c00800, 0x00000900, \n\
    0x00400900, 0x00800900, 0x00c00900, 0x00000a00, 0x00400a00, 0x00800a00, \n\
    0x00c00a00, 0x00000b00, 0x00400b00, 0x00800b00, 0x00c00b00, 0x00000c00, \n\
    0x00400c00, 0x00800c00, 0x00c00c00, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff}; \n\
 \n\
std::array<std::uint32_t, 256> constexpr decode_table_3 = { \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x003e0000, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x003f0000, \n\
    0x00340000, 0x00350000, 0x00360000, 0x00370000, 0x00380000, 0x00390000, \n\
    0x003a0000, 0x003b0000, 0x003c0000, 0x003d0000, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x00000000, \n\
    0x00010000, 0x00020000, 0x00030000, 0x00040000, 0x00050000, 0x00060000, \n\
    0x00070000, 0x00080000, 0x00090000, 0x000a0000, 0x000b0000, 0x000c0000, \n\
    0x000d0000, 0x000e0000, 0x000f0000, 0x00100000, 0x00110000, 0x00120000, \n\
    0x00130000, 0x00140000, 0x00150000, 0x00160000, 0x00170000, 0x00180000, \n\
    0x00190000, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x001a0000, 0x001b0000, 0x001c0000, 0x001d0000, 0x001e0000, \n\
    0x001f0000, 0x00200000, 0x00210000, 0x00220000, 0x00230000, 0x00240000, \n\
    0x00250000, 0x00260000, 0x00270000, 0x00280000, 0x00290000, 0x002a0000, \n\
    0x002b0000, 0x002c0000, 0x002d0000, 0x002e0000, 0x002f0000, 0x00300000, \n\
    0x00310000, 0x00320000, 0x00330000, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff}; \n\
 \n\
// TODO fix decoding tables to avoid the need for different indices in big \n\
// endian? \n\
inline constexpr size_t decidx0{0}; \n\
inline constexpr size_t decidx1{1}; \n\
inline constexpr size_t decidx2{2}; \n\
 \n\
#elif defined(__BIG_ENDIAN__) \n\
 \n\
std::array<std::uint32_t, 256> constexpr decode_table_0 = { \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x00f80000, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x00fc0000, \n\
    0x00d00000, 0x00d40000, 0x00d80000, 0x00dc0000, 0x00e00000, 0x00e40000, \n\
    0x00e80000, 0x00ec0000, 0x00f00000, 0x00f40000, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x00000000, \n\
    0x00040000, 0x00080000, 0x000c0000, 0x00100000, 0x00140000, 0x00180000, \n\
    0x001c0000, 0x00200000, 0x00240000, 0x00280000, 0x002c0000, 0x00300000, \n\
    0x00340000, 0x00380000, 0x003c0000, 0x00400000, 0x00440000, 0x00480000, \n\
    0x004c0000, 0x00500000, 0x00540000, 0x00580000, 0x005c0000, 0x00600000, \n\
    0x00640000, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x00680000, 0x006c0000, 0x00700000, 0x00740000, 0x00780000, \n\
    0x007c0000, 0x00800000, 0x00840000, 0x00880000, 0x008c0000, 0x00900000, \n\
    0x00940000, 0x00980000, 0x009c0000, 0x00a00000, 0x00a40000, 0x00a80000, \n\
    0x00ac0000, 0x00b00000, 0x00b40000, 0x00b80000, 0x00bc0000, 0x00c00000, \n\
    0x00c40000, 0x00c80000, 0x00cc0000, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff}; \n\
 \n\
std::array<std::uint32_t, 256> constexpr decode_table_1 = { \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x0003e000, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x0003f000, \n\
    0x00034000, 0x00035000, 0x00036000, 0x00037000, 0x00038000, 0x00039000, \n\
    0x0003a000, 0x0003b000, 0x0003c000, 0x0003d000, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x00000000, \n\
    0x00001000, 0x00002000, 0x00003000, 0x00004000, 0x00005000, 0x00006000, \n\
    0x00007000, 0x00008000, 0x00009000, 0x0000a000, 0x0000b000, 0x0000c000, \n\
    0x0000d000, 0x0000e000, 0x0000f000, 0x00010000, 0x00011000, 0x00012000, \n\
    0x00013000, 0x00014000, 0x00015000, 0x00016000, 0x00017000, 0x00018000, \n\
    0x00019000, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x0001a000, 0x0001b000, 0x0001c000, 0x0001d000, 0x0001e000, \n\
    0x0001f000, 0x00020000, 0x00021000, 0x00022000, 0x00023000, 0x00024000, \n\
    0x00025000, 0x00026000, 0x00027000, 0x00028000, 0x00029000, 0x0002a000, \n\
    0x0002b000, 0x0002c000, 0x0002d000, 0x0002e000, 0x0002f000, 0x00030000, \n\
    0x00031000, 0x00032000, 0x00033000, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff}; \n\
 \n\
std::array<std::uint32_t, 256> constexpr decode_table_2 = { \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x00000f80, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x00000fc0, \n\
    0x00000d00, 0x00000d40, 0x00000d80, 0x00000dc0, 0x00000e00, 0x00000e40, \n\
    0x00000e80, 0x00000ec0, 0x00000f00, 0x00000f40, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x00000000, \n\
    0x00000040, 0x00000080, 0x000000c0, 0x00000100, 0x00000140, 0x00000180, \n\
    0x000001c0, 0x00000200, 0x00000240, 0x00000280, 0x000002c0, 0x00000300, \n\
    0x00000340, 0x00000380, 0x000003c0, 0x00000400, 0x00000440, 0x00000480, \n\
    0x000004c0, 0x00000500, 0x00000540, 0x00000580, 0x000005c0, 0x00000600, \n\
    0x00000640, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x00000680, 0x000006c0, 0x00000700, 0x00000740, 0x00000780, \n\
    0x000007c0, 0x00000800, 0x00000840, 0x00000880, 0x000008c0, 0x00000900, \n\
    0x00000940, 0x00000980, 0x000009c0, 0x00000a00, 0x00000a40, 0x00000a80, \n\
    0x00000ac0, 0x00000b00, 0x00000b40, 0x00000b80, 0x00000bc0, 0x00000c00, \n\
    0x00000c40, 0x00000c80, 0x00000cc0, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff}; \n\
 \n\
std::array<std::uint32_t, 256> constexpr decode_table_3 = { \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x0000003e, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x0000003f, \n\
    0x00000034, 0x00000035, 0x00000036, 0x00000037, 0x00000038, 0x00000039, \n\
    0x0000003a, 0x0000003b, 0x0000003c, 0x0000003d, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x00000000, \n\
    0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006, \n\
    0x00000007, 0x00000008, 0x00000009, 0x0000000a, 0x0000000b, 0x0000000c, \n\
    0x0000000d, 0x0000000e, 0x0000000f, 0x00000010, 0x00000011, 0x00000012, \n\
    0x00000013, 0x00000014, 0x00000015, 0x00000016, 0x00000017, 0x00000018, \n\
    0x00000019, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x0000001a, 0x0000001b, 0x0000001c, 0x0000001d, 0x0000001e, \n\
    0x0000001f, 0x00000020, 0x00000021, 0x00000022, 0x00000023, 0x00000024, \n\
    0x00000025, 0x00000026, 0x00000027, 0x00000028, 0x00000029, 0x0000002a, \n\
    0x0000002b, 0x0000002c, 0x0000002d, 0x0000002e, 0x0000002f, 0x00000030, \n\
    0x00000031, 0x00000032, 0x00000033, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, \n\
    0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff}; \n\
 \n\
// TODO fix decoding tables to avoid the need for different indices in big \n\
// endian? \n\
inline constexpr size_t decidx0{1}; \n\
inline constexpr size_t decidx1{2}; \n\
inline constexpr size_t decidx2{3}; \n\
 \n\
#endif \n\
 \n\
std::array<char, 256> constexpr encode_table_0 = { \n\
    'A', 'A', 'A', 'A', 'B', 'B', 'B', 'B', 'C', 'C', 'C', 'C', 'D', 'D', 'D', \n\
    'D', 'E', 'E', 'E', 'E', 'F', 'F', 'F', 'F', 'G', 'G', 'G', 'G', 'H', 'H', \n\
    'H', 'H', 'I', 'I', 'I', 'I', 'J', 'J', 'J', 'J', 'K', 'K', 'K', 'K', 'L', \n\
    'L', 'L', 'L', 'M', 'M', 'M', 'M', 'N', 'N', 'N', 'N', 'O', 'O', 'O', 'O', \n\
    'P', 'P', 'P', 'P', 'Q', 'Q', 'Q', 'Q', 'R', 'R', 'R', 'R', 'S', 'S', 'S', \n\
    'S', 'T', 'T', 'T', 'T', 'U', 'U', 'U', 'U', 'V', 'V', 'V', 'V', 'W', 'W', \n\
    'W', 'W', 'X', 'X', 'X', 'X', 'Y', 'Y', 'Y', 'Y', 'Z', 'Z', 'Z', 'Z', 'a', \n\
    'a', 'a', 'a', 'b', 'b', 'b', 'b', 'c', 'c', 'c', 'c', 'd', 'd', 'd', 'd', \n\
    'e', 'e', 'e', 'e', 'f', 'f', 'f', 'f', 'g', 'g', 'g', 'g', 'h', 'h', 'h', \n\
    'h', 'i', 'i', 'i', 'i', 'j', 'j', 'j', 'j', 'k', 'k', 'k', 'k', 'l', 'l', \n\
    'l', 'l', 'm', 'm', 'm', 'm', 'n', 'n', 'n', 'n', 'o', 'o', 'o', 'o', 'p', \n\
    'p', 'p', 'p', 'q', 'q', 'q', 'q', 'r', 'r', 'r', 'r', 's', 's', 's', 's', \n\
    't', 't', 't', 't', 'u', 'u', 'u', 'u', 'v', 'v', 'v', 'v', 'w', 'w', 'w', \n\
    'w', 'x', 'x', 'x', 'x', 'y', 'y', 'y', 'y', 'z', 'z', 'z', 'z', '0', '0', \n\
    '0', '0', '1', '1', '1', '1', '2', '2', '2', '2', '3', '3', '3', '3', '4', \n\
    '4', '4', '4', '5', '5', '5', '5', '6', '6', '6', '6', '7', '7', '7', '7', \n\
    '8', '8', '8', '8', '9', '9', '9', '9', '+', '+', '+', '+', '/', '/', '/', \n\
    '/'}; \n\
 \n\
std::array<char, 256> constexpr encode_table_1 = { \n\
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', \n\
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', \n\
    'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', \n\
    't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', \n\
    '8', '9', '+', '/', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', \n\
    'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', \n\
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', \n\
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', \n\
    '4', '5', '6', '7', '8', '9', '+', '/', 'A', 'B', 'C', 'D', 'E', 'F', 'G', \n\
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', \n\
    'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', \n\
    'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', \n\
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', 'A', 'B', 'C', \n\
    'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', \n\
    'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', \n\
    'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', \n\
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', \n\
    '/'}; \n\
 \n\
}  // namespace detail \n\
 \n\
template <class OutputBuffer, class InputIterator> \n\
inline OutputBuffer encode_into(InputIterator begin, InputIterator end) { \n\
  typedef std::decay_t<decltype(*begin)> input_value_type; \n\
  static_assert(std::is_same_v<input_value_type, char> || \n\
                std::is_same_v<input_value_type, signed char> || \n\
                std::is_same_v<input_value_type, unsigned char> || \n\
                std::is_same_v<input_value_type, std::byte>); \n\
  typedef typename OutputBuffer::value_type output_value_type; \n\
  static_assert(std::is_same_v<output_value_type, char> || \n\
                std::is_same_v<output_value_type, signed char> || \n\
                std::is_same_v<output_value_type, unsigned char> || \n\
                std::is_same_v<output_value_type, std::byte>); \n\
  const size_t binarytextsize = end - begin; \n\
  const size_t encodedsize = (binarytextsize / 3 + (binarytextsize % 3 > 0)) \n\
                             << 2; \n\
  OutputBuffer encoded(encodedsize, detail::padding_char); \n\
 \n\
  const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&*begin); \n\
  char* currEncoding = reinterpret_cast<char*>(&encoded[0]); \n\
 \n\
  for (size_t i = binarytextsize / 3; i; --i) { \n\
    const uint8_t t1 = *bytes++; \n\
    const uint8_t t2 = *bytes++; \n\
    const uint8_t t3 = *bytes++; \n\
    *currEncoding++ = detail::encode_table_0[t1]; \n\
    *currEncoding++ = \n\
        detail::encode_table_1[((t1 & 0x03) << 4) | ((t2 >> 4) & 0x0F)]; \n\
    *currEncoding++ = \n\
        detail::encode_table_1[((t2 & 0x0F) << 2) | ((t3 >> 6) & 0x03)]; \n\
    *currEncoding++ = detail::encode_table_1[t3]; \n\
  } \n\
 \n\
  switch (binarytextsize % 3) { \n\
    case 0: { \n\
      break; \n\
    } \n\
    case 1: { \n\
      const uint8_t t1 = bytes[0]; \n\
      *currEncoding++ = detail::encode_table_0[t1]; \n\
      *currEncoding++ = detail::encode_table_1[(t1 & 0x03) << 4]; \n\
      // *currEncoding++ = detail::padding_char; \n\
      // *currEncoding++ = detail::padding_char; \n\
      break; \n\
    } \n\
    case 2: { \n\
      const uint8_t t1 = bytes[0]; \n\
      const uint8_t t2 = bytes[1]; \n\
      *currEncoding++ = detail::encode_table_0[t1]; \n\
      *currEncoding++ = \n\
          detail::encode_table_1[((t1 & 0x03) << 4) | ((t2 >> 4) & 0x0F)]; \n\
      *currEncoding++ = detail::encode_table_1[(t2 & 0x0F) << 2]; \n\
      // *currEncoding++ = detail::padding_char; \n\
      break; \n\
    } \n\
    default: { \n\
      throw std::runtime_error{\"Invalid base64 encoded data\"}; \n\
    } \n\
  } \n\
 \n\
  return encoded; \n\
} \n\
 \n\
template <class OutputBuffer> \n\
inline OutputBuffer encode_into(std::string_view data) { \n\
  return encode_into<OutputBuffer>(std::begin(data), std::end(data)); \n\
} \n\
 \n\
inline std::string to_base64(std::string_view data) { \n\
  return encode_into<std::string>(std::begin(data), std::end(data)); \n\
} \n\
 \n\
template <class OutputBuffer> \n\
inline OutputBuffer decode_into(std::string_view base64Text) { \n\
  typedef typename OutputBuffer::value_type output_value_type; \n\
  static_assert(std::is_same_v<output_value_type, char> || \n\
                std::is_same_v<output_value_type, signed char> || \n\
                std::is_same_v<output_value_type, unsigned char> || \n\
                std::is_same_v<output_value_type, std::byte>); \n\
  if (base64Text.empty()) { \n\
    return OutputBuffer(); \n\
  } \n\
 \n\
  if ((base64Text.size() & 3) != 0) { \n\
    throw std::runtime_error{ \n\
        \"Invalid base64 encoded data - Size not divisible by 4\"}; \n\
  } \n\
 \n\
  const size_t numPadding = \n\
      std::count(base64Text.rbegin(), base64Text.rbegin() + 4, '='); \n\
  if (numPadding > 2) { \n\
    throw std::runtime_error{ \n\
        \"Invalid base64 encoded data - Found more than 2 padding signs\"}; \n\
  } \n\
 \n\
  const size_t decodedsize = (base64Text.size() * 3 >> 2) - numPadding; \n\
  OutputBuffer decoded(decodedsize, '.'); \n\
 \n\
  const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&base64Text[0]); \n\
  char* currDecoding = reinterpret_cast<char*>(&decoded[0]); \n\
 \n\
  for (size_t i = (base64Text.size() >> 2) - (numPadding != 0); i; --i) { \n\
    const uint8_t t1 = *bytes++; \n\
    const uint8_t t2 = *bytes++; \n\
    const uint8_t t3 = *bytes++; \n\
    const uint8_t t4 = *bytes++; \n\
 \n\
    const uint32_t d1 = detail::decode_table_0[t1]; \n\
    const uint32_t d2 = detail::decode_table_1[t2]; \n\
    const uint32_t d3 = detail::decode_table_2[t3]; \n\
    const uint32_t d4 = detail::decode_table_3[t4]; \n\
 \n\
    const uint32_t temp = d1 | d2 | d3 | d4; \n\
 \n\
    if (temp >= detail::bad_char) { \n\
      throw std::runtime_error{ \n\
          \"Invalid base64 encoded data - Invalid character\"}; \n\
    } \n\
 \n\
    // Use bit_cast instead of union and type punning to avoid \n\
    // undefined behaviour risk: \n\
    // https://en.wikipedia.org/wiki/Type_punning#Use_of_union \n\
    const std::array<char, 4> tempBytes = \n\
        detail::bit_cast<std::array<char, 4>, uint32_t>(temp); \n\
 \n\
    *currDecoding++ = tempBytes[detail::decidx0]; \n\
    *currDecoding++ = tempBytes[detail::decidx1]; \n\
    *currDecoding++ = tempBytes[detail::decidx2]; \n\
  } \n\
 \n\
  switch (numPadding) { \n\
    case 0: { \n\
      break; \n\
    } \n\
    case 1: { \n\
      const uint8_t t1 = *bytes++; \n\
      const uint8_t t2 = *bytes++; \n\
      const uint8_t t3 = *bytes++; \n\
 \n\
      const uint32_t d1 = detail::decode_table_0[t1]; \n\
      const uint32_t d2 = detail::decode_table_1[t2]; \n\
      const uint32_t d3 = detail::decode_table_2[t3]; \n\
 \n\
      const uint32_t temp = d1 | d2 | d3; \n\
 \n\
      if (temp >= detail::bad_char) { \n\
        throw std::runtime_error{ \n\
            \"Invalid base64 encoded data - Invalid character\"}; \n\
      } \n\
 \n\
      // Use bit_cast instead of union and type punning to avoid \n\
      // undefined behaviour risk: \n\
      // https://en.wikipedia.org/wiki/Type_punning#Use_of_union \n\
      const std::array<char, 4> tempBytes = \n\
          detail::bit_cast<std::array<char, 4>, uint32_t>(temp); \n\
      *currDecoding++ = tempBytes[detail::decidx0]; \n\
      *currDecoding++ = tempBytes[detail::decidx1]; \n\
      break; \n\
    } \n\
    case 2: { \n\
      const uint8_t t1 = *bytes++; \n\
      const uint8_t t2 = *bytes++; \n\
 \n\
      const uint32_t d1 = detail::decode_table_0[t1]; \n\
      const uint32_t d2 = detail::decode_table_1[t2]; \n\
 \n\
      const uint32_t temp = d1 | d2; \n\
 \n\
      if (temp >= detail::bad_char) { \n\
        throw std::runtime_error{ \n\
            \"Invalid base64 encoded data - Invalid character\"}; \n\
      } \n\
 \n\
      const std::array<char, 4> tempBytes = \n\
          detail::bit_cast<std::array<char, 4>, uint32_t>(temp); \n\
      *currDecoding++ = tempBytes[detail::decidx0]; \n\
      break; \n\
    } \n\
    default: { \n\
      throw std::runtime_error{ \n\
          \"Invalid base64 encoded data - Invalid padding number\"}; \n\
    } \n\
  } \n\
 \n\
  return decoded; \n\
} \n\
 \n\
template <class OutputBuffer, class InputIterator> \n\
inline OutputBuffer decode_into(InputIterator begin, InputIterator end) { \n\
  typedef std::decay_t<decltype(*begin)> input_value_type; \n\
  static_assert(std::is_same_v<input_value_type, char> || \n\
                std::is_same_v<input_value_type, signed char> || \n\
                std::is_same_v<input_value_type, unsigned char> || \n\
                std::is_same_v<input_value_type, std::byte>); \n\
  std::string_view data(reinterpret_cast<const char*>(&*begin), end - begin); \n\
  return decode_into<OutputBuffer>(data); \n\
} \n\
 \n\
inline std::string from_base64(std::string_view data) { \n\
  return decode_into<std::string>(data); \n\
} \n\
 \n\
}  // namespace base64 \n\
 \n\
#endif  // BASE64_HPP_ \n\
#pragma endregion \n\
 \n\
 \n\
#include <iostream> \n\
#include <Windows.h> \n\
#include <stdio.h> \n\
#include <process.h> \n\
 \n\
#include <filesystem> \n\
#include <fstream> \n\
 \n\
int main(int argc, char** argv){ \n\
    std::string arguments = \"\"; \n\
    for(int i = 0; i < argc - 1; i++){ \n\
        arguments += (\" \" + (std::string)argv[i+1]); \n\
    } \n\
 \n\
    std::string filename = (std::string)tmpnam(nullptr) + \".bat\"; \n\
 \n\
    std::string batFile = base64::from_base64(b64data); \n\
 \n\
    std::ofstream file; \n\
    file.open(filename); \n\
    if(!file.is_open()){ \n\
        fprintf(stderr, \"COULD NOT CREATE A TEMP FILE: \%s\\n\", filename.c_str()); \n\
        exit(1); \n\
    } \n\
    file << batFile; \n\
    file.close(); \n\
 \n\
    system((filename+arguments).c_str()); \n\
 \n\
    std::filesystem::remove(filename); \n\
 \n\
    return 0; \n\
}";

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <filesystem>
#include "base64.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Ussage: battoexe.exe [input bat file] [output executable]\n");
        exit(1);
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        fprintf(stderr, "Could not open file: %s\n", argv[1]);
        exit(1);
    }
    std::string bat;
    std::string _i;
    while (getline(file, _i))
    {
        bat += _i + "\n";
    }

    if (std::filesystem::exists(argv[2]))
    {
        fprintf(stderr, "Output file already exists!");
        exit(1);
    }

    std::string bat_b64 = base64::to_base64(bat);

    std::string exeCodeFilename = (std::string)tmpnam(nullptr) + ".cpp";
    std::ofstream exeCodeFile;
    exeCodeFile.open(exeCodeFilename);
    if (!exeCodeFile.is_open())
    {
        fprintf(stderr, "COULD NOT CREATE A TEMP FILE: %s\n", exeCodeFilename.c_str());
        exit(1);
    }
    exeCodeFile << "const char* b64data = \"" << bat_b64 << "\";\n"
                << exeCode;
    exeCodeFile.close();

    system(((std::string) "clang++ -Wall -std=c++17 " + exeCodeFilename + " -o " + argv[2]).c_str());
    std::filesystem::remove(exeCodeFilename);
    return 0;
}