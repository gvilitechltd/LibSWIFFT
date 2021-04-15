/*
 * This source file is licensed under the CC BY-SA 2.5 license, available at
 * https://creativecommons.org/licenses/by-sa/2.5/
 *
 * The code in the transpose_8x8_16_sse2 function within this source file is
 * adopted from a Stack Overflow answer attributed to Stephen Thomas dated
 * February 9th 2011 and available at https://stackoverflow.com/a/4951060
 */

#include <xmmintrin.h>

static inline void transpose_8x8_16_sse2(__m128i * array)
{
	__m128i *p_input  = (__m128i*)array;
	__m128i *p_output = (__m128i*)array;
	__m128i a = _mm_load_si128(p_input++);
	__m128i b = _mm_load_si128(p_input++);
	__m128i c = _mm_load_si128(p_input++);
	__m128i d = _mm_load_si128(p_input++);
	__m128i e = _mm_load_si128(p_input++);
	__m128i f = _mm_load_si128(p_input++);
	__m128i g = _mm_load_si128(p_input++);
	__m128i h = _mm_load_si128(p_input);
	
	__m128i a03b03 = _mm_unpacklo_epi16(a, b);
	__m128i c03d03 = _mm_unpacklo_epi16(c, d);
	__m128i e03f03 = _mm_unpacklo_epi16(e, f);
	__m128i g03h03 = _mm_unpacklo_epi16(g, h);
	__m128i a47b47 = _mm_unpackhi_epi16(a, b);
	__m128i c47d47 = _mm_unpackhi_epi16(c, d);
	__m128i e47f47 = _mm_unpackhi_epi16(e, f);
	__m128i g47h47 = _mm_unpackhi_epi16(g, h);
	
	__m128i a01b01c01d01 = _mm_unpacklo_epi32(a03b03, c03d03);
	__m128i a23b23c23d23 = _mm_unpackhi_epi32(a03b03, c03d03);
	__m128i e01f01g01h01 = _mm_unpacklo_epi32(e03f03, g03h03);
	__m128i e23f23g23h23 = _mm_unpackhi_epi32(e03f03, g03h03);
	__m128i a45b45c45d45 = _mm_unpacklo_epi32(a47b47, c47d47);
	__m128i a67b67c67d67 = _mm_unpackhi_epi32(a47b47, c47d47);
	__m128i e45f45g45h45 = _mm_unpacklo_epi32(e47f47, g47h47);
	__m128i e67f67g67h67 = _mm_unpackhi_epi32(e47f47, g47h47);
	
	__m128i a0b0c0d0e0f0g0h0 = _mm_unpacklo_epi64(a01b01c01d01, e01f01g01h01);
	__m128i a1b1c1d1e1f1g1h1 = _mm_unpackhi_epi64(a01b01c01d01, e01f01g01h01);
	__m128i a2b2c2d2e2f2g2h2 = _mm_unpacklo_epi64(a23b23c23d23, e23f23g23h23);
	__m128i a3b3c3d3e3f3g3h3 = _mm_unpackhi_epi64(a23b23c23d23, e23f23g23h23);
	__m128i a4b4c4d4e4f4g4h4 = _mm_unpacklo_epi64(a45b45c45d45, e45f45g45h45);
	__m128i a5b5c5d5e5f5g5h5 = _mm_unpackhi_epi64(a45b45c45d45, e45f45g45h45);
	__m128i a6b6c6d6e6f6g6h6 = _mm_unpacklo_epi64(a67b67c67d67, e67f67g67h67);
	__m128i a7b7c7d7e7f7g7h7 = _mm_unpackhi_epi64(a67b67c67d67, e67f67g67h67);
	
	_mm_store_si128(p_output++, a0b0c0d0e0f0g0h0);
	_mm_store_si128(p_output++, a1b1c1d1e1f1g1h1);
	_mm_store_si128(p_output++, a2b2c2d2e2f2g2h2);
	_mm_store_si128(p_output++, a3b3c3d3e3f3g3h3);
	_mm_store_si128(p_output++, a4b4c4d4e4f4g4h4);
	_mm_store_si128(p_output++, a5b5c5d5e5f5g5h5);
	_mm_store_si128(p_output++, a6b6c6d6e6f6g6h6);
	_mm_store_si128(p_output, a7b7c7d7e7f7g7h7);
}
