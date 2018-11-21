/* MIT License
 *
 * Copyright (c) 2018 INRIA 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Hacl_AesGCM_NI.h"

static void Hacl_Impl_Gf128_FieldNI_fadd(Lib_Vec128_vec128 *x, Lib_Vec128_vec128 *y)
{
  x[0U] = Lib_Vec128_vec128_xor(x[0U], y[0U]);
}

static void Hacl_Impl_Gf128_FieldNI_fmul(Lib_Vec128_vec128 *x, Lib_Vec128_vec128 *y)
{
  Lib_Vec128_vec128 xe = x[0U];
  Lib_Vec128_vec128 ye = y[0U];
  Lib_Vec128_vec128 lo0 = Lib_Vec128_ni_clmul(xe, ye, (uint8_t)0x00U);
  Lib_Vec128_vec128 m1 = Lib_Vec128_ni_clmul(xe, ye, (uint8_t)0x10U);
  Lib_Vec128_vec128 m2 = Lib_Vec128_ni_clmul(xe, ye, (uint8_t)0x01U);
  Lib_Vec128_vec128 hi = Lib_Vec128_ni_clmul(xe, ye, (uint8_t)0x11U);
  Lib_Vec128_vec128 m11 = Lib_Vec128_vec128_xor(m1, m2);
  Lib_Vec128_vec128 m21 = Lib_Vec128_vec128_shift_left(m11, (uint32_t)64U);
  Lib_Vec128_vec128 m12 = Lib_Vec128_vec128_shift_right(m11, (uint32_t)64U);
  Lib_Vec128_vec128 lo10 = Lib_Vec128_vec128_xor(lo0, m21);
  Lib_Vec128_vec128 hi10 = Lib_Vec128_vec128_xor(hi, m12);
  Lib_Vec128_vec128 hi0 = hi10;
  Lib_Vec128_vec128 lo = lo10;
  Lib_Vec128_vec128 lo1 = Lib_Vec128_vec128_shift_right64(lo, (uint32_t)63U);
  Lib_Vec128_vec128 lo2 = Lib_Vec128_vec128_shift_left(lo1, (uint32_t)64U);
  Lib_Vec128_vec128 lo3 = Lib_Vec128_vec128_shift_left64(lo, (uint32_t)1U);
  Lib_Vec128_vec128 lo31 = Lib_Vec128_vec128_xor(lo3, lo2);
  Lib_Vec128_vec128 hi1 = Lib_Vec128_vec128_shift_right64(hi0, (uint32_t)63U);
  Lib_Vec128_vec128 hi11 = Lib_Vec128_vec128_shift_left(hi1, (uint32_t)64U);
  Lib_Vec128_vec128 hi2 = Lib_Vec128_vec128_shift_left64(hi0, (uint32_t)1U);
  Lib_Vec128_vec128 hi21 = Lib_Vec128_vec128_xor(hi2, hi11);
  Lib_Vec128_vec128 lo11 = Lib_Vec128_vec128_shift_right64(lo, (uint32_t)63U);
  Lib_Vec128_vec128 lo12 = Lib_Vec128_vec128_shift_right(lo11, (uint32_t)64U);
  Lib_Vec128_vec128 hi22 = Lib_Vec128_vec128_xor(hi21, lo12);
  Lib_Vec128_vec128 lo4 = lo31;
  Lib_Vec128_vec128 hi3 = hi22;
  Lib_Vec128_vec128 lo13 = Lib_Vec128_vec128_shift_left64(lo4, (uint32_t)63U);
  Lib_Vec128_vec128 lo21 = Lib_Vec128_vec128_shift_left64(lo4, (uint32_t)62U);
  Lib_Vec128_vec128 lo32 = Lib_Vec128_vec128_shift_left64(lo4, (uint32_t)57U);
  Lib_Vec128_vec128 lo14 = Lib_Vec128_vec128_xor(lo13, lo21);
  Lib_Vec128_vec128 lo15 = Lib_Vec128_vec128_xor(lo14, lo32);
  Lib_Vec128_vec128 lo22 = Lib_Vec128_vec128_shift_right(lo15, (uint32_t)64U);
  Lib_Vec128_vec128 lo33 = Lib_Vec128_vec128_shift_left(lo15, (uint32_t)64U);
  Lib_Vec128_vec128 lo5 = Lib_Vec128_vec128_xor(lo4, lo33);
  Lib_Vec128_vec128 lo_ = lo22;
  Lib_Vec128_vec128 lo16 = Lib_Vec128_vec128_shift_right64(lo5, (uint32_t)1U);
  Lib_Vec128_vec128 lo23 = Lib_Vec128_vec128_shift_right64(lo5, (uint32_t)2U);
  Lib_Vec128_vec128 lo34 = Lib_Vec128_vec128_shift_right64(lo5, (uint32_t)7U);
  Lib_Vec128_vec128 lo17 = Lib_Vec128_vec128_xor(lo16, lo23);
  Lib_Vec128_vec128 lo18 = Lib_Vec128_vec128_xor(lo17, lo34);
  Lib_Vec128_vec128 lo19 = Lib_Vec128_vec128_xor(lo18, lo_);
  Lib_Vec128_vec128 lo6 = Lib_Vec128_vec128_xor(lo5, lo19);
  Lib_Vec128_vec128 lo7 = Lib_Vec128_vec128_xor(lo6, hi3);
  Lib_Vec128_vec128 lo110 = lo7;
  x[0U] = lo110;
}

static void Hacl_Impl_Gf128_FieldNI_load_precompute_r(Lib_Vec128_vec128 *pre, uint8_t *key)
{
  Lib_Vec128_vec128 *r_4 = pre;
  Lib_Vec128_vec128 *r_3 = pre + (uint32_t)1U;
  Lib_Vec128_vec128 *r_2 = pre + (uint32_t)2U;
  Lib_Vec128_vec128 *r = pre + (uint32_t)3U;
  r[0U] = Lib_Vec128_vec128_load_be(key);
  pre[0U] = r[0U];
  pre[1U] = r[0U];
  pre[2U] = r[0U];
  Hacl_Impl_Gf128_FieldNI_fmul(r_2, r);
  Hacl_Impl_Gf128_FieldNI_fmul(r_3, r_2);
  Hacl_Impl_Gf128_FieldNI_fmul(r_4, r_3);
}

static void
Hacl_Impl_Gf128_FieldNI_fadd_mul4(
  Lib_Vec128_vec128 *acc,
  Lib_Vec128_vec128 *x,
  Lib_Vec128_vec128 *pre
)
{
  Lib_Vec128_vec128 x1 = x[0U];
  Lib_Vec128_vec128 x2 = x[1U];
  Lib_Vec128_vec128 x3 = x[2U];
  Lib_Vec128_vec128 x4 = x[3U];
  Lib_Vec128_vec128 y1 = pre[0U];
  Lib_Vec128_vec128 y2 = pre[1U];
  Lib_Vec128_vec128 y3 = pre[2U];
  Lib_Vec128_vec128 y4 = pre[3U];
  Lib_Vec128_vec128 acc0 = acc[0U];
  Lib_Vec128_vec128 acc01 = Lib_Vec128_vec128_xor(acc0, x1);
  Lib_Vec128_vec128 lo10 = Lib_Vec128_ni_clmul(acc01, y1, (uint8_t)0x00U);
  Lib_Vec128_vec128 lo2 = Lib_Vec128_ni_clmul(x2, y2, (uint8_t)0x00U);
  Lib_Vec128_vec128 lo30 = Lib_Vec128_ni_clmul(x3, y3, (uint8_t)0x00U);
  Lib_Vec128_vec128 lo40 = Lib_Vec128_ni_clmul(x4, y4, (uint8_t)0x00U);
  Lib_Vec128_vec128 lo0 = Lib_Vec128_vec128_xor(lo10, lo2);
  Lib_Vec128_vec128 lo5 = Lib_Vec128_vec128_xor(lo0, lo30);
  Lib_Vec128_vec128 lo6 = Lib_Vec128_vec128_xor(lo5, lo40);
  Lib_Vec128_vec128 m1 = Lib_Vec128_ni_clmul(acc01, y1, (uint8_t)0x10U);
  Lib_Vec128_vec128 m2 = Lib_Vec128_ni_clmul(x2, y2, (uint8_t)0x10U);
  Lib_Vec128_vec128 m3 = Lib_Vec128_ni_clmul(x3, y3, (uint8_t)0x10U);
  Lib_Vec128_vec128 m4 = Lib_Vec128_ni_clmul(x4, y4, (uint8_t)0x10U);
  Lib_Vec128_vec128 m = Lib_Vec128_vec128_xor(m1, m2);
  Lib_Vec128_vec128 m5 = Lib_Vec128_vec128_xor(m, m3);
  Lib_Vec128_vec128 m6 = Lib_Vec128_vec128_xor(m5, m4);
  Lib_Vec128_vec128 m11 = Lib_Vec128_ni_clmul(acc01, y1, (uint8_t)0x01U);
  Lib_Vec128_vec128 m21 = Lib_Vec128_ni_clmul(x2, y2, (uint8_t)0x01U);
  Lib_Vec128_vec128 m31 = Lib_Vec128_ni_clmul(x3, y3, (uint8_t)0x01U);
  Lib_Vec128_vec128 m41 = Lib_Vec128_ni_clmul(x4, y4, (uint8_t)0x01U);
  Lib_Vec128_vec128 m7 = Lib_Vec128_vec128_xor(m6, m11);
  Lib_Vec128_vec128 m8 = Lib_Vec128_vec128_xor(m7, m21);
  Lib_Vec128_vec128 m9 = Lib_Vec128_vec128_xor(m8, m31);
  Lib_Vec128_vec128 m10 = Lib_Vec128_vec128_xor(m9, m41);
  Lib_Vec128_vec128 hi10 = Lib_Vec128_ni_clmul(acc01, y1, (uint8_t)0x11U);
  Lib_Vec128_vec128 hi20 = Lib_Vec128_ni_clmul(x2, y2, (uint8_t)0x11U);
  Lib_Vec128_vec128 hi30 = Lib_Vec128_ni_clmul(x3, y3, (uint8_t)0x11U);
  Lib_Vec128_vec128 hi4 = Lib_Vec128_ni_clmul(x4, y4, (uint8_t)0x11U);
  Lib_Vec128_vec128 hi = Lib_Vec128_vec128_xor(hi10, hi20);
  Lib_Vec128_vec128 hi5 = Lib_Vec128_vec128_xor(hi, hi30);
  Lib_Vec128_vec128 hi6 = Lib_Vec128_vec128_xor(hi5, hi4);
  Lib_Vec128_vec128 m12 = Lib_Vec128_vec128_shift_left(m10, (uint32_t)64U);
  Lib_Vec128_vec128 m22 = Lib_Vec128_vec128_shift_right(m10, (uint32_t)64U);
  Lib_Vec128_vec128 lo7 = Lib_Vec128_vec128_xor(lo6, m12);
  Lib_Vec128_vec128 hi7 = Lib_Vec128_vec128_xor(hi6, m22);
  Lib_Vec128_vec128 hi0 = hi7;
  Lib_Vec128_vec128 lo = lo7;
  Lib_Vec128_vec128 lo1 = Lib_Vec128_vec128_shift_right64(lo, (uint32_t)63U);
  Lib_Vec128_vec128 lo20 = Lib_Vec128_vec128_shift_left(lo1, (uint32_t)64U);
  Lib_Vec128_vec128 lo3 = Lib_Vec128_vec128_shift_left64(lo, (uint32_t)1U);
  Lib_Vec128_vec128 lo31 = Lib_Vec128_vec128_xor(lo3, lo20);
  Lib_Vec128_vec128 hi1 = Lib_Vec128_vec128_shift_right64(hi0, (uint32_t)63U);
  Lib_Vec128_vec128 hi11 = Lib_Vec128_vec128_shift_left(hi1, (uint32_t)64U);
  Lib_Vec128_vec128 hi2 = Lib_Vec128_vec128_shift_left64(hi0, (uint32_t)1U);
  Lib_Vec128_vec128 hi21 = Lib_Vec128_vec128_xor(hi2, hi11);
  Lib_Vec128_vec128 lo11 = Lib_Vec128_vec128_shift_right64(lo, (uint32_t)63U);
  Lib_Vec128_vec128 lo12 = Lib_Vec128_vec128_shift_right(lo11, (uint32_t)64U);
  Lib_Vec128_vec128 hi22 = Lib_Vec128_vec128_xor(hi21, lo12);
  Lib_Vec128_vec128 lo4 = lo31;
  Lib_Vec128_vec128 hi3 = hi22;
  Lib_Vec128_vec128 lo13 = Lib_Vec128_vec128_shift_left64(lo4, (uint32_t)63U);
  Lib_Vec128_vec128 lo21 = Lib_Vec128_vec128_shift_left64(lo4, (uint32_t)62U);
  Lib_Vec128_vec128 lo32 = Lib_Vec128_vec128_shift_left64(lo4, (uint32_t)57U);
  Lib_Vec128_vec128 lo14 = Lib_Vec128_vec128_xor(lo13, lo21);
  Lib_Vec128_vec128 lo15 = Lib_Vec128_vec128_xor(lo14, lo32);
  Lib_Vec128_vec128 lo22 = Lib_Vec128_vec128_shift_right(lo15, (uint32_t)64U);
  Lib_Vec128_vec128 lo33 = Lib_Vec128_vec128_shift_left(lo15, (uint32_t)64U);
  Lib_Vec128_vec128 lo50 = Lib_Vec128_vec128_xor(lo4, lo33);
  Lib_Vec128_vec128 lo_ = lo22;
  Lib_Vec128_vec128 lo16 = Lib_Vec128_vec128_shift_right64(lo50, (uint32_t)1U);
  Lib_Vec128_vec128 lo23 = Lib_Vec128_vec128_shift_right64(lo50, (uint32_t)2U);
  Lib_Vec128_vec128 lo34 = Lib_Vec128_vec128_shift_right64(lo50, (uint32_t)7U);
  Lib_Vec128_vec128 lo17 = Lib_Vec128_vec128_xor(lo16, lo23);
  Lib_Vec128_vec128 lo18 = Lib_Vec128_vec128_xor(lo17, lo34);
  Lib_Vec128_vec128 lo19 = Lib_Vec128_vec128_xor(lo18, lo_);
  Lib_Vec128_vec128 lo60 = Lib_Vec128_vec128_xor(lo50, lo19);
  Lib_Vec128_vec128 lo70 = Lib_Vec128_vec128_xor(lo60, hi3);
  Lib_Vec128_vec128 lo110 = lo70;
  acc[0U] = lo110;
}

static void Hacl_Gf128_NI_gcm_init(Lib_Vec128_vec128 *ctx, uint8_t *key)
{
  Lib_Vec128_vec128 *acc = ctx;
  Lib_Vec128_vec128 *pre = ctx + (uint32_t)1U;
  acc[0U] = Lib_Vec128_vec128_zero;
  Hacl_Impl_Gf128_FieldNI_load_precompute_r(pre, key);
}

static void
Hacl_Gf128_NI_gcm_update_blocks(Lib_Vec128_vec128 *ctx, uint32_t len, uint8_t *text)
{
  Lib_Vec128_vec128 *acc = ctx;
  Lib_Vec128_vec128 *pre = ctx + (uint32_t)1U;
  Lib_Vec128_vec128 b4[4U];
  for (uint32_t _i = 0U; _i < (uint32_t)4U; ++_i)
    b4[_i] = Lib_Vec128_vec128_zero;
  uint32_t blocks = len / (uint32_t)64U;
  for (uint32_t i = (uint32_t)0U; i < blocks; i = i + (uint32_t)1U)
  {
    uint8_t *uu____0 = text + i * (uint32_t)64U;
    b4[0U] = Lib_Vec128_vec128_load_be(uu____0);
    b4[1U] = Lib_Vec128_vec128_load_be(uu____0 + (uint32_t)16U);
    b4[2U] = Lib_Vec128_vec128_load_be(uu____0 + (uint32_t)32U);
    b4[3U] = Lib_Vec128_vec128_load_be(uu____0 + (uint32_t)48U);
    Hacl_Impl_Gf128_FieldNI_fadd_mul4(acc, b4, pre);
  }
  uint32_t rem1 = len % (uint32_t)64U;
  uint8_t *last1 = text + blocks * (uint32_t)64U;
  Lib_Vec128_vec128 *acc1 = ctx;
  Lib_Vec128_vec128 *r = ctx + (uint32_t)4U;
  uint32_t blocks1 = rem1 / (uint32_t)16U;
  for (uint32_t i = (uint32_t)0U; i < blocks1; i = i + (uint32_t)1U)
  {
    uint8_t *uu____1 = last1 + i * (uint32_t)16U;
    Lib_Vec128_vec128 elem = Lib_Vec128_vec128_zero;
    elem = Lib_Vec128_vec128_load_be(uu____1);
    Hacl_Impl_Gf128_FieldNI_fadd(acc1, &elem);
    Hacl_Impl_Gf128_FieldNI_fmul(acc1, r);
  }
  uint32_t rem2 = rem1 % (uint32_t)16U;
  if (rem2 > (uint32_t)0U)
  {
    uint8_t *last2 = last1 + blocks1 * (uint32_t)16U;
    Lib_Vec128_vec128 elem = Lib_Vec128_vec128_zero;
    uint8_t b[16U] = { 0U };
    memcpy(b, last2, rem2 * sizeof last2[0U]);
    elem = Lib_Vec128_vec128_load_be(b);
    Hacl_Impl_Gf128_FieldNI_fadd(acc1, &elem);
    Hacl_Impl_Gf128_FieldNI_fmul(acc1, r);
  }
}

static void
Hacl_Gf128_NI_gcm_update_padded(Lib_Vec128_vec128 *ctx, uint32_t len, uint8_t *text)
{
  Lib_Vec128_vec128 *acc = ctx;
  Lib_Vec128_vec128 *pre = ctx + (uint32_t)1U;
  Lib_Vec128_vec128 b4[4U];
  for (uint32_t _i = 0U; _i < (uint32_t)4U; ++_i)
    b4[_i] = Lib_Vec128_vec128_zero;
  uint32_t blocks = len / (uint32_t)64U;
  for (uint32_t i = (uint32_t)0U; i < blocks; i = i + (uint32_t)1U)
  {
    uint8_t *uu____0 = text + i * (uint32_t)64U;
    b4[0U] = Lib_Vec128_vec128_load_be(uu____0);
    b4[1U] = Lib_Vec128_vec128_load_be(uu____0 + (uint32_t)16U);
    b4[2U] = Lib_Vec128_vec128_load_be(uu____0 + (uint32_t)32U);
    b4[3U] = Lib_Vec128_vec128_load_be(uu____0 + (uint32_t)48U);
    Hacl_Impl_Gf128_FieldNI_fadd_mul4(acc, b4, pre);
  }
  uint32_t rem1 = len % (uint32_t)64U;
  uint8_t *last1 = text + blocks * (uint32_t)64U;
  Lib_Vec128_vec128 *acc1 = ctx;
  Lib_Vec128_vec128 *r = ctx + (uint32_t)4U;
  uint32_t blocks1 = rem1 / (uint32_t)16U;
  for (uint32_t i = (uint32_t)0U; i < blocks1; i = i + (uint32_t)1U)
  {
    uint8_t *uu____1 = last1 + i * (uint32_t)16U;
    Lib_Vec128_vec128 elem = Lib_Vec128_vec128_zero;
    elem = Lib_Vec128_vec128_load_be(uu____1);
    Hacl_Impl_Gf128_FieldNI_fadd(acc1, &elem);
    Hacl_Impl_Gf128_FieldNI_fmul(acc1, r);
  }
  uint32_t rem2 = rem1 % (uint32_t)16U;
  if (rem2 > (uint32_t)0U)
  {
    uint8_t *last2 = last1 + blocks1 * (uint32_t)16U;
    Lib_Vec128_vec128 elem = Lib_Vec128_vec128_zero;
    uint8_t b[16U] = { 0U };
    memcpy(b, last2, rem2 * sizeof last2[0U]);
    elem = Lib_Vec128_vec128_load_be(b);
    Hacl_Impl_Gf128_FieldNI_fadd(acc1, &elem);
    Hacl_Impl_Gf128_FieldNI_fmul(acc1, r);
  }
}

static void Hacl_Gf128_NI_gcm_emit(uint8_t *tag, Lib_Vec128_vec128 *ctx)
{
  Lib_Vec128_vec128 *acc = ctx;
  Lib_Vec128_vec128_store_be(tag, acc[0U]);
}

inline static void
Hacl_Aes_NI_aes128_init(Lib_Vec128_vec128 *ctx, uint8_t *key, uint8_t *nonce)
{
  Lib_Vec128_vec128 *kex = ctx + (uint32_t)1U;
  Lib_Vec128_vec128 *n1 = ctx;
  uint32_t klen1 = (uint32_t)1U;
  Lib_Vec128_vec128 *uu____0 = kex;
  uu____0[0U] = Lib_Vec128_vec128_load_le(key);
  Lib_Vec128_vec128 *prev = kex;
  Lib_Vec128_vec128 *next = kex + klen1;
  next[0U] = Lib_Vec128_ni_aes_keygen_assist(prev[0U], (uint8_t)0x01U);
  Lib_Vec128_vec128 n00 = next[0U];
  next[0U] =
    Lib_Vec128_vec128_shuffle32(n00,
      Lib_Vec128_vec128_shuffle32_spec((uint8_t)3U, (uint8_t)3U, (uint8_t)3U, (uint8_t)3U));
  Lib_Vec128_vec128 key1 = prev[0U];
  Lib_Vec128_vec128
  key2 = Lib_Vec128_vec128_xor(key1, Lib_Vec128_vec128_shift_left(key1, (uint32_t)32U));
  Lib_Vec128_vec128
  key3 = Lib_Vec128_vec128_xor(key2, Lib_Vec128_vec128_shift_left(key2, (uint32_t)32U));
  Lib_Vec128_vec128
  key4 = Lib_Vec128_vec128_xor(key3, Lib_Vec128_vec128_shift_left(key3, (uint32_t)32U));
  next[0U] = Lib_Vec128_vec128_xor(next[0U], key4);
  Lib_Vec128_vec128 *prev1 = kex + klen1;
  Lib_Vec128_vec128 *next1 = kex + (uint32_t)2U * klen1;
  next1[0U] = Lib_Vec128_ni_aes_keygen_assist(prev1[0U], (uint8_t)0x02U);
  Lib_Vec128_vec128 n01 = next1[0U];
  next1[0U] =
    Lib_Vec128_vec128_shuffle32(n01,
      Lib_Vec128_vec128_shuffle32_spec((uint8_t)3U, (uint8_t)3U, (uint8_t)3U, (uint8_t)3U));
  Lib_Vec128_vec128 key10 = prev1[0U];
  Lib_Vec128_vec128
  key20 = Lib_Vec128_vec128_xor(key10, Lib_Vec128_vec128_shift_left(key10, (uint32_t)32U));
  Lib_Vec128_vec128
  key30 = Lib_Vec128_vec128_xor(key20, Lib_Vec128_vec128_shift_left(key20, (uint32_t)32U));
  Lib_Vec128_vec128
  key40 = Lib_Vec128_vec128_xor(key30, Lib_Vec128_vec128_shift_left(key30, (uint32_t)32U));
  next1[0U] = Lib_Vec128_vec128_xor(next1[0U], key40);
  Lib_Vec128_vec128 *prev2 = kex + klen1 * (uint32_t)2U;
  Lib_Vec128_vec128 *next2 = kex + klen1 * (uint32_t)3U;
  next2[0U] = Lib_Vec128_ni_aes_keygen_assist(prev2[0U], (uint8_t)0x04U);
  Lib_Vec128_vec128 n02 = next2[0U];
  next2[0U] =
    Lib_Vec128_vec128_shuffle32(n02,
      Lib_Vec128_vec128_shuffle32_spec((uint8_t)3U, (uint8_t)3U, (uint8_t)3U, (uint8_t)3U));
  Lib_Vec128_vec128 key11 = prev2[0U];
  Lib_Vec128_vec128
  key21 = Lib_Vec128_vec128_xor(key11, Lib_Vec128_vec128_shift_left(key11, (uint32_t)32U));
  Lib_Vec128_vec128
  key31 = Lib_Vec128_vec128_xor(key21, Lib_Vec128_vec128_shift_left(key21, (uint32_t)32U));
  Lib_Vec128_vec128
  key41 = Lib_Vec128_vec128_xor(key31, Lib_Vec128_vec128_shift_left(key31, (uint32_t)32U));
  next2[0U] = Lib_Vec128_vec128_xor(next2[0U], key41);
  Lib_Vec128_vec128 *prev3 = kex + klen1 * (uint32_t)3U;
  Lib_Vec128_vec128 *next3 = kex + klen1 * (uint32_t)4U;
  next3[0U] = Lib_Vec128_ni_aes_keygen_assist(prev3[0U], (uint8_t)0x08U);
  Lib_Vec128_vec128 n03 = next3[0U];
  next3[0U] =
    Lib_Vec128_vec128_shuffle32(n03,
      Lib_Vec128_vec128_shuffle32_spec((uint8_t)3U, (uint8_t)3U, (uint8_t)3U, (uint8_t)3U));
  Lib_Vec128_vec128 key12 = prev3[0U];
  Lib_Vec128_vec128
  key22 = Lib_Vec128_vec128_xor(key12, Lib_Vec128_vec128_shift_left(key12, (uint32_t)32U));
  Lib_Vec128_vec128
  key32 = Lib_Vec128_vec128_xor(key22, Lib_Vec128_vec128_shift_left(key22, (uint32_t)32U));
  Lib_Vec128_vec128
  key42 = Lib_Vec128_vec128_xor(key32, Lib_Vec128_vec128_shift_left(key32, (uint32_t)32U));
  next3[0U] = Lib_Vec128_vec128_xor(next3[0U], key42);
  Lib_Vec128_vec128 *prev4 = kex + klen1 * (uint32_t)4U;
  Lib_Vec128_vec128 *next4 = kex + klen1 * (uint32_t)5U;
  next4[0U] = Lib_Vec128_ni_aes_keygen_assist(prev4[0U], (uint8_t)0x10U);
  Lib_Vec128_vec128 n04 = next4[0U];
  next4[0U] =
    Lib_Vec128_vec128_shuffle32(n04,
      Lib_Vec128_vec128_shuffle32_spec((uint8_t)3U, (uint8_t)3U, (uint8_t)3U, (uint8_t)3U));
  Lib_Vec128_vec128 key13 = prev4[0U];
  Lib_Vec128_vec128
  key23 = Lib_Vec128_vec128_xor(key13, Lib_Vec128_vec128_shift_left(key13, (uint32_t)32U));
  Lib_Vec128_vec128
  key33 = Lib_Vec128_vec128_xor(key23, Lib_Vec128_vec128_shift_left(key23, (uint32_t)32U));
  Lib_Vec128_vec128
  key43 = Lib_Vec128_vec128_xor(key33, Lib_Vec128_vec128_shift_left(key33, (uint32_t)32U));
  next4[0U] = Lib_Vec128_vec128_xor(next4[0U], key43);
  Lib_Vec128_vec128 *prev5 = kex + klen1 * (uint32_t)5U;
  Lib_Vec128_vec128 *next5 = kex + klen1 * (uint32_t)6U;
  next5[0U] = Lib_Vec128_ni_aes_keygen_assist(prev5[0U], (uint8_t)0x20U);
  Lib_Vec128_vec128 n05 = next5[0U];
  next5[0U] =
    Lib_Vec128_vec128_shuffle32(n05,
      Lib_Vec128_vec128_shuffle32_spec((uint8_t)3U, (uint8_t)3U, (uint8_t)3U, (uint8_t)3U));
  Lib_Vec128_vec128 key14 = prev5[0U];
  Lib_Vec128_vec128
  key24 = Lib_Vec128_vec128_xor(key14, Lib_Vec128_vec128_shift_left(key14, (uint32_t)32U));
  Lib_Vec128_vec128
  key34 = Lib_Vec128_vec128_xor(key24, Lib_Vec128_vec128_shift_left(key24, (uint32_t)32U));
  Lib_Vec128_vec128
  key44 = Lib_Vec128_vec128_xor(key34, Lib_Vec128_vec128_shift_left(key34, (uint32_t)32U));
  next5[0U] = Lib_Vec128_vec128_xor(next5[0U], key44);
  Lib_Vec128_vec128 *prev6 = kex + klen1 * (uint32_t)6U;
  Lib_Vec128_vec128 *next6 = kex + klen1 * (uint32_t)7U;
  next6[0U] = Lib_Vec128_ni_aes_keygen_assist(prev6[0U], (uint8_t)0x40U);
  Lib_Vec128_vec128 n06 = next6[0U];
  next6[0U] =
    Lib_Vec128_vec128_shuffle32(n06,
      Lib_Vec128_vec128_shuffle32_spec((uint8_t)3U, (uint8_t)3U, (uint8_t)3U, (uint8_t)3U));
  Lib_Vec128_vec128 key15 = prev6[0U];
  Lib_Vec128_vec128
  key25 = Lib_Vec128_vec128_xor(key15, Lib_Vec128_vec128_shift_left(key15, (uint32_t)32U));
  Lib_Vec128_vec128
  key35 = Lib_Vec128_vec128_xor(key25, Lib_Vec128_vec128_shift_left(key25, (uint32_t)32U));
  Lib_Vec128_vec128
  key45 = Lib_Vec128_vec128_xor(key35, Lib_Vec128_vec128_shift_left(key35, (uint32_t)32U));
  next6[0U] = Lib_Vec128_vec128_xor(next6[0U], key45);
  Lib_Vec128_vec128 *prev7 = kex + klen1 * (uint32_t)7U;
  Lib_Vec128_vec128 *next7 = kex + klen1 * (uint32_t)8U;
  next7[0U] = Lib_Vec128_ni_aes_keygen_assist(prev7[0U], (uint8_t)0x80U);
  Lib_Vec128_vec128 n07 = next7[0U];
  next7[0U] =
    Lib_Vec128_vec128_shuffle32(n07,
      Lib_Vec128_vec128_shuffle32_spec((uint8_t)3U, (uint8_t)3U, (uint8_t)3U, (uint8_t)3U));
  Lib_Vec128_vec128 key16 = prev7[0U];
  Lib_Vec128_vec128
  key26 = Lib_Vec128_vec128_xor(key16, Lib_Vec128_vec128_shift_left(key16, (uint32_t)32U));
  Lib_Vec128_vec128
  key36 = Lib_Vec128_vec128_xor(key26, Lib_Vec128_vec128_shift_left(key26, (uint32_t)32U));
  Lib_Vec128_vec128
  key46 = Lib_Vec128_vec128_xor(key36, Lib_Vec128_vec128_shift_left(key36, (uint32_t)32U));
  next7[0U] = Lib_Vec128_vec128_xor(next7[0U], key46);
  Lib_Vec128_vec128 *prev8 = kex + klen1 * (uint32_t)8U;
  Lib_Vec128_vec128 *next8 = kex + klen1 * (uint32_t)9U;
  next8[0U] = Lib_Vec128_ni_aes_keygen_assist(prev8[0U], (uint8_t)0x1bU);
  Lib_Vec128_vec128 n08 = next8[0U];
  next8[0U] =
    Lib_Vec128_vec128_shuffle32(n08,
      Lib_Vec128_vec128_shuffle32_spec((uint8_t)3U, (uint8_t)3U, (uint8_t)3U, (uint8_t)3U));
  Lib_Vec128_vec128 key17 = prev8[0U];
  Lib_Vec128_vec128
  key27 = Lib_Vec128_vec128_xor(key17, Lib_Vec128_vec128_shift_left(key17, (uint32_t)32U));
  Lib_Vec128_vec128
  key37 = Lib_Vec128_vec128_xor(key27, Lib_Vec128_vec128_shift_left(key27, (uint32_t)32U));
  Lib_Vec128_vec128
  key47 = Lib_Vec128_vec128_xor(key37, Lib_Vec128_vec128_shift_left(key37, (uint32_t)32U));
  next8[0U] = Lib_Vec128_vec128_xor(next8[0U], key47);
  Lib_Vec128_vec128 *prev9 = kex + klen1 * (uint32_t)9U;
  Lib_Vec128_vec128 *next9 = kex + klen1 * (uint32_t)10U;
  next9[0U] = Lib_Vec128_ni_aes_keygen_assist(prev9[0U], (uint8_t)0x36U);
  Lib_Vec128_vec128 n0 = next9[0U];
  next9[0U] =
    Lib_Vec128_vec128_shuffle32(n0,
      Lib_Vec128_vec128_shuffle32_spec((uint8_t)3U, (uint8_t)3U, (uint8_t)3U, (uint8_t)3U));
  Lib_Vec128_vec128 key18 = prev9[0U];
  Lib_Vec128_vec128
  key28 = Lib_Vec128_vec128_xor(key18, Lib_Vec128_vec128_shift_left(key18, (uint32_t)32U));
  Lib_Vec128_vec128
  key38 = Lib_Vec128_vec128_xor(key28, Lib_Vec128_vec128_shift_left(key28, (uint32_t)32U));
  Lib_Vec128_vec128
  key48 = Lib_Vec128_vec128_xor(key38, Lib_Vec128_vec128_shift_left(key38, (uint32_t)32U));
  next9[0U] = Lib_Vec128_vec128_xor(next9[0U], key48);
  uint8_t nb1[16U] = { 0U };
  memcpy(nb1, nonce, (uint32_t)12U * sizeof nonce[0U]);
  n1[0U] = Lib_Vec128_vec128_load_le(nb1);
}

inline static void Hacl_Aes_NI_aes128_set_nonce(Lib_Vec128_vec128 *ctx, uint8_t *nonce)
{
  Lib_Vec128_vec128 *n1 = ctx;
  uint8_t nb1[16U] = { 0U };
  memcpy(nb1, nonce, (uint32_t)12U * sizeof nonce[0U]);
  n1[0U] = Lib_Vec128_vec128_load_le(nb1);
}

inline static void
Hacl_Aes_NI_aes128_key_block(uint8_t *kb, Lib_Vec128_vec128 *ctx, uint32_t counter)
{
  Lib_Vec128_vec128 *kex = ctx + (uint32_t)1U;
  Lib_Vec128_vec128 *n1 = ctx;

  Lib_Vec128_vec128 st[4U];
  for (uint32_t _i = 0U; _i < (uint32_t)4U; ++_i)
    st[_i] = Lib_Vec128_vec128_zero;
  uint32_t counter0 = htobe32(counter);
  uint32_t counter1 = htobe32(counter + (uint32_t)1U);
  uint32_t counter2 = htobe32(counter + (uint32_t)2U);
  uint32_t counter3 = htobe32(counter + (uint32_t)3U);
  st[0U] = Lib_Vec128_vec128_insert32(n1[0U], counter0, (uint8_t)3U);
  st[1U] = Lib_Vec128_vec128_insert32(n1[0U], counter1, (uint8_t)3U);
  st[2U] = Lib_Vec128_vec128_insert32(n1[0U], counter2, (uint8_t)3U);
  st[3U] = Lib_Vec128_vec128_insert32(n1[0U], counter3, (uint8_t)3U);
  uint32_t klen1 = (uint32_t)1U;
  Lib_Vec128_vec128 *k0 = kex;
  Lib_Vec128_vec128 *kr = kex + klen1;
  Lib_Vec128_vec128 *kn = kex + (uint32_t)10U * klen1;
  st[0U] = Lib_Vec128_vec128_xor(st[0U], k0[0U]);
  st[1U] = Lib_Vec128_vec128_xor(st[1U], k0[0U]);
  st[2U] = Lib_Vec128_vec128_xor(st[2U], k0[0U]);
  st[3U] = Lib_Vec128_vec128_xor(st[3U], k0[0U]);
  for (uint32_t i = (uint32_t)0U; i < (uint32_t)10U - (uint32_t)1U; i = i + (uint32_t)1U)
  {
    Lib_Vec128_vec128 *sub_key = kr + i * (uint32_t)1U;
    st[0U] = Lib_Vec128_ni_aes_enc(st[0U], sub_key[0U]);
    st[1U] = Lib_Vec128_ni_aes_enc(st[1U], sub_key[0U]);
    st[2U] = Lib_Vec128_ni_aes_enc(st[2U], sub_key[0U]);
    st[3U] = Lib_Vec128_ni_aes_enc(st[3U], sub_key[0U]);
  }
  st[0U] = Lib_Vec128_ni_aes_enc_last(st[0U], kn[0U]);
  st[1U] = Lib_Vec128_ni_aes_enc_last(st[1U], kn[0U]);
  st[2U] = Lib_Vec128_ni_aes_enc_last(st[2U], kn[0U]);
  st[3U] = Lib_Vec128_ni_aes_enc_last(st[3U], kn[0U]);
  Lib_Vec128_vec128_store_le(kb, st[0U]);
}

inline static void
Hacl_Aes_NI_aes128_ctr(
  uint32_t len,
  uint8_t *out,
  uint8_t *inp,
  Lib_Vec128_vec128 *ctx,
  uint32_t counter
)
{
  uint32_t blocks64 = len / (uint32_t)64U;
  for (uint32_t i = (uint32_t)0U; i < blocks64; i = i + (uint32_t)1U)
  {
    uint32_t ctr = counter + i * (uint32_t)4U;
    uint8_t *ib = inp + i * (uint32_t)64U;
    uint8_t *ob = out + i * (uint32_t)64U;
    Lib_Vec128_vec128 st[4U];
    for (uint32_t _i = 0U; _i < (uint32_t)4U; ++_i)
      st[_i] = Lib_Vec128_vec128_zero;
    Lib_Vec128_vec128 *kex = ctx + (uint32_t)1U;
    Lib_Vec128_vec128 *n1 = ctx;
    uint32_t counter0 = htobe32(ctr);
    uint32_t counter1 = htobe32(ctr + (uint32_t)1U);
    uint32_t counter2 = htobe32(ctr + (uint32_t)2U);
    uint32_t counter3 = htobe32(ctr + (uint32_t)3U);
    st[0U] = Lib_Vec128_vec128_insert32(n1[0U], counter0, (uint8_t)3U);
    st[1U] = Lib_Vec128_vec128_insert32(n1[0U], counter1, (uint8_t)3U);
    st[2U] = Lib_Vec128_vec128_insert32(n1[0U], counter2, (uint8_t)3U);
    st[3U] = Lib_Vec128_vec128_insert32(n1[0U], counter3, (uint8_t)3U);
    uint32_t klen1 = (uint32_t)1U;
    Lib_Vec128_vec128 *k0 = kex;
    Lib_Vec128_vec128 *kr = kex + klen1;
    Lib_Vec128_vec128 *kn = kex + (uint32_t)10U * klen1;
    st[0U] = Lib_Vec128_vec128_xor(st[0U], k0[0U]);
    st[1U] = Lib_Vec128_vec128_xor(st[1U], k0[0U]);
    st[2U] = Lib_Vec128_vec128_xor(st[2U], k0[0U]);
    st[3U] = Lib_Vec128_vec128_xor(st[3U], k0[0U]);
    for (uint32_t i = (uint32_t)0U; i < (uint32_t)10U - (uint32_t)1U; i = i + (uint32_t)1U)
    {
      Lib_Vec128_vec128 *sub_key = kr + i * (uint32_t)1U;
      st[0U] = Lib_Vec128_ni_aes_enc(st[0U], sub_key[0U]);
      st[1U] = Lib_Vec128_ni_aes_enc(st[1U], sub_key[0U]);
      st[2U] = Lib_Vec128_ni_aes_enc(st[2U], sub_key[0U]);
      st[3U] = Lib_Vec128_ni_aes_enc(st[3U], sub_key[0U]);
    }
    st[0U] = Lib_Vec128_ni_aes_enc_last(st[0U], kn[0U]);
    st[1U] = Lib_Vec128_ni_aes_enc_last(st[1U], kn[0U]);
    st[2U] = Lib_Vec128_ni_aes_enc_last(st[2U], kn[0U]);
    st[3U] = Lib_Vec128_ni_aes_enc_last(st[3U], kn[0U]);
    Lib_Vec128_vec128 v0 = Lib_Vec128_vec128_load_le(ib);
    Lib_Vec128_vec128 v1 = Lib_Vec128_vec128_load_le(ib + (uint32_t)16U);
    Lib_Vec128_vec128 v2 = Lib_Vec128_vec128_load_le(ib + (uint32_t)32U);
    Lib_Vec128_vec128 v3 = Lib_Vec128_vec128_load_le(ib + (uint32_t)48U);
    Lib_Vec128_vec128 v01 = Lib_Vec128_vec128_xor(v0, st[0U]);
    Lib_Vec128_vec128 v11 = Lib_Vec128_vec128_xor(v1, st[1U]);
    Lib_Vec128_vec128 v21 = Lib_Vec128_vec128_xor(v2, st[2U]);
    Lib_Vec128_vec128 v31 = Lib_Vec128_vec128_xor(v3, st[3U]);
    Lib_Vec128_vec128_store_le(ob, v01);
    Lib_Vec128_vec128_store_le(ob + (uint32_t)16U, v11);
    Lib_Vec128_vec128_store_le(ob + (uint32_t)32U, v21);
    Lib_Vec128_vec128_store_le(ob + (uint32_t)48U, v31);
  }
  uint32_t rem1 = len % (uint32_t)64U;
  uint8_t last1[64U];
  if (rem1 > (uint32_t)0U)
  {
    uint32_t ctr = counter + blocks64 * (uint32_t)4U;
    uint8_t *ib = inp + blocks64 * (uint32_t)64U;
    uint8_t *ob = out + blocks64 * (uint32_t)64U;
    uint8_t init = (uint8_t)0U;
    for (uint32_t i = (uint32_t)0U; i < (uint32_t)64U; i = i + (uint32_t)1U)
    {
      last1[i] = init;
    }
    memcpy(last1, ib, rem1 * sizeof ib[0U]);
    Lib_Vec128_vec128 st[4U];
    for (uint32_t _i = 0U; _i < (uint32_t)4U; ++_i)
      st[_i] = Lib_Vec128_vec128_zero;
    Lib_Vec128_vec128 *kex = ctx + (uint32_t)1U;
    Lib_Vec128_vec128 *n1 = ctx;
    uint32_t counter0 = htobe32(ctr);
    uint32_t counter1 = htobe32(ctr + (uint32_t)1U);
    uint32_t counter2 = htobe32(ctr + (uint32_t)2U);
    uint32_t counter3 = htobe32(ctr + (uint32_t)3U);
    st[0U] = Lib_Vec128_vec128_insert32(n1[0U], counter0, (uint8_t)3U);
    st[1U] = Lib_Vec128_vec128_insert32(n1[0U], counter1, (uint8_t)3U);
    st[2U] = Lib_Vec128_vec128_insert32(n1[0U], counter2, (uint8_t)3U);
    st[3U] = Lib_Vec128_vec128_insert32(n1[0U], counter3, (uint8_t)3U);
    uint32_t klen1 = (uint32_t)1U;
    Lib_Vec128_vec128 *k0 = kex;
    Lib_Vec128_vec128 *kr = kex + klen1;
    Lib_Vec128_vec128 *kn = kex + (uint32_t)10U * klen1;
    st[0U] = Lib_Vec128_vec128_xor(st[0U], k0[0U]);
    st[1U] = Lib_Vec128_vec128_xor(st[1U], k0[0U]);
    st[2U] = Lib_Vec128_vec128_xor(st[2U], k0[0U]);
    st[3U] = Lib_Vec128_vec128_xor(st[3U], k0[0U]);
    for (uint32_t i = (uint32_t)0U; i < (uint32_t)10U - (uint32_t)1U; i = i + (uint32_t)1U)
    {
      Lib_Vec128_vec128 *sub_key = kr + i * (uint32_t)1U;
      st[0U] = Lib_Vec128_ni_aes_enc(st[0U], sub_key[0U]);
      st[1U] = Lib_Vec128_ni_aes_enc(st[1U], sub_key[0U]);
      st[2U] = Lib_Vec128_ni_aes_enc(st[2U], sub_key[0U]);
      st[3U] = Lib_Vec128_ni_aes_enc(st[3U], sub_key[0U]);
    }
    st[0U] = Lib_Vec128_ni_aes_enc_last(st[0U], kn[0U]);
    st[1U] = Lib_Vec128_ni_aes_enc_last(st[1U], kn[0U]);
    st[2U] = Lib_Vec128_ni_aes_enc_last(st[2U], kn[0U]);
    st[3U] = Lib_Vec128_ni_aes_enc_last(st[3U], kn[0U]);
    Lib_Vec128_vec128 v0 = Lib_Vec128_vec128_load_le(last1);
    Lib_Vec128_vec128 v1 = Lib_Vec128_vec128_load_le(last1 + (uint32_t)16U);
    Lib_Vec128_vec128 v2 = Lib_Vec128_vec128_load_le(last1 + (uint32_t)32U);
    Lib_Vec128_vec128 v3 = Lib_Vec128_vec128_load_le(last1 + (uint32_t)48U);
    Lib_Vec128_vec128 v01 = Lib_Vec128_vec128_xor(v0, st[0U]);
    Lib_Vec128_vec128 v11 = Lib_Vec128_vec128_xor(v1, st[1U]);
    Lib_Vec128_vec128 v21 = Lib_Vec128_vec128_xor(v2, st[2U]);
    Lib_Vec128_vec128 v31 = Lib_Vec128_vec128_xor(v3, st[3U]);
    Lib_Vec128_vec128_store_le(last1, v01);
    Lib_Vec128_vec128_store_le(last1 + (uint32_t)16U, v11);
    Lib_Vec128_vec128_store_le(last1 + (uint32_t)32U, v21);
    Lib_Vec128_vec128_store_le(last1 + (uint32_t)48U, v31);
    memcpy(ob, last1, rem1 * sizeof last1[0U]);
  }
}

void Hacl_AesGCM_NI_aes128_gcm_init(Lib_Vec128_vec128 *ctx, uint8_t *key, uint8_t *nonce)
{
  uint8_t gcm_key[16U] = { 0U };
  uint8_t tag_mix[16U] = { 0U };
  uint8_t nonce0[12U] = { 0U };
  Lib_Vec128_vec128 *aes_ctx = ctx;
  Lib_Vec128_vec128 *gcm_ctx = ctx + (uint32_t)16U;
  Hacl_Aes_NI_aes128_init(aes_ctx, key, nonce0);
  Hacl_Aes_NI_aes128_key_block(gcm_key, aes_ctx, (uint32_t)0U);
  Hacl_Aes_NI_aes128_set_nonce(aes_ctx, nonce);
  Hacl_Aes_NI_aes128_key_block(tag_mix, aes_ctx, (uint32_t)1U);
  Hacl_Gf128_NI_gcm_init(gcm_ctx, gcm_key);
  ctx[21U] = Lib_Vec128_vec128_load_le(tag_mix);
}

void
Hacl_AesGCM_NI_aes128_gcm_encrypt(
  Lib_Vec128_vec128 *ctx,
  uint32_t len,
  uint8_t *out,
  uint8_t *text,
  uint32_t aad_len,
  uint8_t *aad
)
{
  uint8_t *cip = out;
  Lib_Vec128_vec128 *aes_ctx = ctx;
  Hacl_Aes_NI_aes128_ctr(len, cip, text, aes_ctx, (uint32_t)2U);
  Lib_Vec128_vec128 *gcm_ctx = ctx + (uint32_t)16U;
  Lib_Vec128_vec128 tag_mix = ctx[21U];
  Hacl_Gf128_NI_gcm_update_padded(gcm_ctx, aad_len, aad);
  Hacl_Gf128_NI_gcm_update_padded(gcm_ctx, len, cip);
  uint8_t tmp[16U] = { 0U };
  store64_be(tmp, (uint64_t)(aad_len * (uint32_t)8U));
  store64_be(tmp + (uint32_t)8U, (uint64_t)(len * (uint32_t)8U));
  Hacl_Gf128_NI_gcm_update_blocks(gcm_ctx, (uint32_t)16U, tmp);
  Hacl_Gf128_NI_gcm_emit(tmp, gcm_ctx);
  Lib_Vec128_vec128 tmp_vec = Lib_Vec128_vec128_load_le(tmp);
  Lib_Vec128_vec128 tmp_vec1 = Lib_Vec128_vec128_xor(tmp_vec, tag_mix);
  Lib_Vec128_vec128_store_le(out + len, tmp_vec1);
}

bool
Hacl_AesGCM_NI_aes128_gcm_decrypt(
  Lib_Vec128_vec128 *ctx,
  uint32_t len,
  uint8_t *out,
  uint8_t *cipher,
  uint32_t aad_len,
  uint8_t *aad
)
{
  Lib_Vec128_vec128 *aes_ctx = ctx;
  Lib_Vec128_vec128 *gcm_ctx = ctx + (uint32_t)16U;
  Lib_Vec128_vec128 tag_mix = ctx[21U];
  Hacl_Gf128_NI_gcm_update_padded(gcm_ctx, aad_len, aad);
  Hacl_Gf128_NI_gcm_update_padded(gcm_ctx, len, cipher);
  uint8_t tmp[16U] = { 0U };
  store64_be(tmp, (uint64_t)(aad_len * (uint32_t)8U));
  store64_be(tmp + (uint32_t)8U, (uint64_t)(len * (uint32_t)8U));
  Hacl_Gf128_NI_gcm_update_blocks(gcm_ctx, (uint32_t)16U, tmp);
  Hacl_Gf128_NI_gcm_emit(tmp, gcm_ctx);
  Lib_Vec128_vec128 tmp_vec = Lib_Vec128_vec128_load_le(tmp);
  Lib_Vec128_vec128 tmp_vec1 = Lib_Vec128_vec128_xor(tmp_vec, tag_mix);
  Lib_Vec128_vec128_store_le(tmp, tmp_vec1);
  uint8_t *tag = cipher + len;
  uint8_t res = (uint8_t)0U;
  for (uint32_t i = (uint32_t)0U; i < (uint32_t)16U; i = i + (uint32_t)1U)
  {
    res = res | (tmp[i] ^ tag[i]);
  }
  uint8_t r = res;
  if (r == (uint8_t)0U)
  {
    uint8_t *cip = cipher;
    Hacl_Aes_NI_aes128_ctr(len, out, cip, aes_ctx, (uint32_t)2U);
    return true;
  }
  else
  {
    return false;
  }
}
