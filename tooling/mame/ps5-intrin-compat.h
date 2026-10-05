/* Force-included into every C translation unit of the MAME build (ARCHOPTS_C
 * in tools/build-mame.sh).
 *
 * The SDK compiles for x86_64-sie-ps5, which defines __SCE__. On that target
 * clang's <immintrin.h> declares only the intrinsics of the ISA extensions the
 * build enables, where every other target declares all of them. The console is
 * a Zen 2, without VAES, so the 256-bit AES intrinsics are never declared and
 * 3rdparty/lzma/C/AesOpt.c, whose VAES paths are compiled under a
 * __target__("vaes") attribute and chosen by CPUID at run time, fails to
 * compile. Declaring them is what every other target does; it enables no
 * instruction outside those attributed functions, and the console's CPUID
 * keeps them from being called.
 *
 * <vaesintrin.h> may be included only after <immintrin.h>, as AesOpt.c itself
 * does for clang-cl. */
#ifndef PS5_MAME_INTRIN_COMPAT_H
#define PS5_MAME_INTRIN_COMPAT_H

#if defined(__SCE__) && defined(__x86_64__)
#include <immintrin.h>
#if !defined(__VAES__)
#include <vaesintrin.h>
#endif
#endif

#endif
