// Force-included into every C++ translation unit of the LRPS2 build (see
// ps5-toolchain.cmake).
//
// The SDK compiles for x86_64-sie-ps5, which defines __SCE__. On that target
// clang's <immintrin.h> declares only the intrinsics of the ISA extensions the
// build enables, where every other target declares all of them. The console
// is a Zen 2 (-march=znver2: AVX2, no AVX-512), so the AVX-512BW/VL variable
// 16-bit shifts are never declared, and pcsx2/GS/GSVector8i.h, which names
// them in two inline members (srav16, sllv16), fails to parse. Nothing in the
// core calls those members, but they must still compile; the definitions below
// are exact AVX2 equivalents, and they step aside whenever the compiler
// declares the real intrinsics.
#pragma once

#if defined(__cplusplus) && defined(__x86_64__)
#include <immintrin.h>

#if defined(__SCE__) && defined(__AVX2__) && \
    !(defined(__AVX512BW__) && defined(__AVX512VL__))

// Arithmetic right shift of each 16-bit lane by the matching 16-bit count; a
// count above 15 fills the lane with its sign bit.
static __inline__ __m256i __attribute__((__always_inline__, __nodebug__))
_mm256_srav_epi16(__m256i a, __m256i count)
{
    const __m256i low16 = _mm256_set1_epi32(0xFFFF);
    // Even lanes, sign-extended to 32 bits; a count of 16..65535 still
    // produces the sign fill.
    __m256i even = _mm256_srav_epi32(_mm256_srai_epi32(_mm256_slli_epi32(a, 16), 16),
                                     _mm256_and_si256(count, low16));
    // Odd lanes, sign-extended to 32 bits.
    __m256i odd = _mm256_srav_epi32(_mm256_srai_epi32(a, 16),
                                    _mm256_srli_epi32(count, 16));
    return _mm256_blend_epi16(even, _mm256_slli_epi32(odd, 16), 0xAA);
}

// Logical left shift of each 16-bit lane by the matching 16-bit count; a count
// above 15 clears the lane.
static __inline__ __m256i __attribute__((__always_inline__, __nodebug__))
_mm256_sllv_epi16(__m256i a, __m256i count)
{
    const __m256i low16 = _mm256_set1_epi32(0xFFFF);
    __m256i even = _mm256_sllv_epi32(a, _mm256_and_si256(count, low16));
    __m256i odd = _mm256_sllv_epi32(_mm256_andnot_si256(low16, a),
                                    _mm256_srli_epi32(count, 16));
    return _mm256_blend_epi16(even, odd, 0xAA);
}

#endif
#endif
