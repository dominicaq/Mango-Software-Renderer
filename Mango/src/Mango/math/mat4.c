#include "mat4.h"

#include <math.h>

#ifndef EPSILON
#define EPSILON 1e-6f
#endif

/* ---- platform SIMD detection (compile-time, no runtime CPUID needed) ----
 *
 * x86/x64: SSE2 is part of the baseline ABI on x86-64 for every mainstream
 * compiler (MSVC, GCC, Clang), and has been the de-facto default on 32-bit
 * builds for a long time too, so we use it unconditionally rather than
 * runtime-dispatching.
 *
 * ARM64 (Apple Silicon Macs, AArch64 Linux/Windows): NEON is baseline and
 * always available, same reasoning.
 *
 * Anything else falls back to portable scalar code.
 */
#if defined(__SSE2__) || defined(_M_X64) || defined(_M_AMD64) || \
    (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#define MAT4_SIMD_SSE 1
#include <emmintrin.h>
#elif defined(__ARM_NEON) || defined(__ARM_NEON__) || defined(__aarch64__) || defined(_M_ARM64)
#define MAT4_SIMD_NEON 1
#include <arm_neon.h>
#endif

const Mat4 ZERO_MATRIX = {{{0.0f, 0.0f, 0.0f, 0.0f},
                           {0.0f, 0.0f, 0.0f, 0.0f},
                           {0.0f, 0.0f, 0.0f, 0.0f},
                           {0.0f, 0.0f, 0.0f, 0.0f}}};

const Mat4 IDENTITY = {{{1.0f, 0.0f, 0.0f, 0.0f},
                        {0.0f, 1.0f, 0.0f, 0.0f},
                        {0.0f, 0.0f, 1.0f, 0.0f},
                        {0.0f, 0.0f, 0.0f, 1.0f}}};

/* ---------------------------- mat4_mul ---------------------------- */
/* result row i = a[i][0]*b_row0 + a[i][1]*b_row1 + a[i][2]*b_row2 + a[i][3]*b_row3
 * (this row-major layout falls straight out of the original triple loop -
 * it's what makes the "broadcast + fused multiply-add" SIMD form apply
 * with zero data reshuffling). */

#if defined(MAT4_SIMD_SSE)

Mat4 mat4_mul(const Mat4 a, const Mat4 b) {
    Mat4 result;
    __m128 b_row0 = _mm_loadu_ps(b.elem[0]);
    __m128 b_row1 = _mm_loadu_ps(b.elem[1]);
    __m128 b_row2 = _mm_loadu_ps(b.elem[2]);
    __m128 b_row3 = _mm_loadu_ps(b.elem[3]);

    for (int i = 0; i < 4; i++) {
        __m128 r = _mm_mul_ps(_mm_set1_ps(a.elem[i][0]), b_row0);
        r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(a.elem[i][1]), b_row1));
        r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(a.elem[i][2]), b_row2));
        r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(a.elem[i][3]), b_row3));
        _mm_storeu_ps(result.elem[i], r);
    }
    return result;
}

#elif defined(MAT4_SIMD_NEON)

Mat4 mat4_mul(const Mat4 a, const Mat4 b) {
    Mat4 result;
    float32x4_t b_row0 = vld1q_f32(b.elem[0]);
    float32x4_t b_row1 = vld1q_f32(b.elem[1]);
    float32x4_t b_row2 = vld1q_f32(b.elem[2]);
    float32x4_t b_row3 = vld1q_f32(b.elem[3]);

    for (int i = 0; i < 4; i++) {
        float32x4_t r = vmulq_n_f32(b_row0, a.elem[i][0]);
        r = vmlaq_n_f32(r, b_row1, a.elem[i][1]);
        r = vmlaq_n_f32(r, b_row2, a.elem[i][2]);
        r = vmlaq_n_f32(r, b_row3, a.elem[i][3]);
        vst1q_f32(result.elem[i], r);
    }
    return result;
}

#else /* scalar fallback */

Mat4 mat4_mul(const Mat4 a, const Mat4 b) {
    Mat4 result = ZERO_MATRIX;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result.elem[i][j] += a.elem[i][k] * b.elem[k][j];
            }
        }
    }
    return result;
}

#endif

/* -------------------------- mat4_transpose -------------------------- */

#if defined(MAT4_SIMD_SSE)

Mat4 mat4_transpose(const Mat4 a) {
    Mat4 result;
    __m128 r0 = _mm_loadu_ps(a.elem[0]);
    __m128 r1 = _mm_loadu_ps(a.elem[1]);
    __m128 r2 = _mm_loadu_ps(a.elem[2]);
    __m128 r3 = _mm_loadu_ps(a.elem[3]);
    _MM_TRANSPOSE4_PS(r0, r1, r2, r3); /* standard SSE shuffle transpose */
    _mm_storeu_ps(result.elem[0], r0);
    _mm_storeu_ps(result.elem[1], r1);
    _mm_storeu_ps(result.elem[2], r2);
    _mm_storeu_ps(result.elem[3], r3);
    return result;
}

#elif defined(MAT4_SIMD_NEON)

Mat4 mat4_transpose(const Mat4 a) {
    Mat4 result;
    /* vld4q_f32 loads 16 contiguous floats de-interleaved into 4 lanes,
     * i.e. val[j] = {a[0][j], a[1][j], a[2][j], a[3][j]} - that's the
     * transpose falling directly out of the load, no shuffles needed. */
    float32x4x4_t t = vld4q_f32(&a.elem[0][0]);
    vst1q_f32(result.elem[0], t.val[0]);
    vst1q_f32(result.elem[1], t.val[1]);
    vst1q_f32(result.elem[2], t.val[2]);
    vst1q_f32(result.elem[3], t.val[3]);
    return result;
}

#else /* scalar fallback */

Mat4 mat4_transpose(const Mat4 a) {
    Mat4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.elem[i][j] = a.elem[j][i];
        }
    }
    return result;
}

#endif

/* ---------------------- mat4_scale / mat4_translation ---------------------- *
 * Constructors, not hot loops - not worth vectorizing. */

Mat4 mat4_scale(const Vec3 scale) {
    Mat4 res = {{{scale.x, 0.0f, 0.0f, 0.0f},
                 {0.0f, scale.y, 0.0f, 0.0f},
                 {0.0f, 0.0f, scale.z, 0.0f},
                 {0.0f, 0.0f, 0.0f, 1.0f}}};
    return res;
}

Mat4 mat4_translation(const Vec3 v) {
    Mat4 res = {{{1.0f, 0.0f, 0.0f, v.x},
                 {0.0f, 1.0f, 0.0f, v.y},
                 {0.0f, 0.0f, 1.0f, v.z},
                 {0.0f, 0.0f, 0.0f, 1.0f}}};
    return res;
}

/* --------------------- mat4_mul_point3 / mat4_mul_dir3 --------------------- *
 * These are the likely actual per-pixel hot path in a renderer: transform
 * one point/direction by one matrix. row i of output = dot(m_row_i, p4). */

#if defined(MAT4_SIMD_SSE)

static Vec3 mat4_mul_vec4_sse(const Mat4 m, float x, float y, float z, float w) {
    __m128 p = _mm_setr_ps(x, y, z, w);
    __m128 r0 = _mm_mul_ps(_mm_loadu_ps(m.elem[0]), p);
    __m128 r1 = _mm_mul_ps(_mm_loadu_ps(m.elem[1]), p);
    __m128 r2 = _mm_mul_ps(_mm_loadu_ps(m.elem[2]), p);

    /* horizontal add each row with plain SSE2 shuffles (no SSE3 haddps
     * dependency, keeps this portable to the widest baseline) */
    __m128 sh0 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 0, 1, 0));
    __m128 sh1 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(3, 2, 3, 2));
    __m128 sums01 = _mm_add_ps(sh0, sh1); /* [x0+z0, y0+w0, x1+z1, y1+w1] */
    sums01 = _mm_add_ps(sums01, _mm_shuffle_ps(sums01, sums01, _MM_SHUFFLE(2, 3, 0, 1)));
    /* lane 0 = sum(r0), lane 2 = sum(r1) */

    __m128 r2_dup = _mm_add_ps(r2, _mm_movehl_ps(r2, r2));
    r2_dup = _mm_add_ss(r2_dup, _mm_shuffle_ps(r2_dup, r2_dup, _MM_SHUFFLE(1, 1, 1, 1)));

    float result[4];
    _mm_store_ss(&result[0], sums01);
    __m128 lane2 = _mm_shuffle_ps(sums01, sums01, _MM_SHUFFLE(2, 2, 2, 2));
    _mm_store_ss(&result[1], lane2);
    _mm_store_ss(&result[2], r2_dup);

    Vec3 out = {result[0], result[1], result[2]};
    return out;
}

Vec3 mat4_mul_point3(const Mat4 m, const Vec3 p) {
    return mat4_mul_vec4_sse(m, p.x, p.y, p.z, 1.0f);
}

Vec3 mat4_mul_dir3(const Mat4 m, const Vec3 d) {
    return mat4_mul_vec4_sse(m, d.x, d.y, d.z, 0.0f);
}

#elif defined(MAT4_SIMD_NEON)

static Vec3 mat4_mul_vec4_neon(const Mat4 m, float x, float y, float z, float w) {
    float pv[4] = {x, y, z, w};
    float32x4_t p = vld1q_f32(pv);
    float32x4_t r0 = vmulq_f32(vld1q_f32(m.elem[0]), p);
    float32x4_t r1 = vmulq_f32(vld1q_f32(m.elem[1]), p);
    float32x4_t r2 = vmulq_f32(vld1q_f32(m.elem[2]), p);

    Vec3 out;
    out.x = vaddvq_f32(r0);
    out.y = vaddvq_f32(r1);
    out.z = vaddvq_f32(r2);
    return out;
}

Vec3 mat4_mul_point3(const Mat4 m, const Vec3 p) {
    return mat4_mul_vec4_neon(m, p.x, p.y, p.z, 1.0f);
}

Vec3 mat4_mul_dir3(const Mat4 m, const Vec3 d) {
    return mat4_mul_vec4_neon(m, d.x, d.y, d.z, 0.0f);
}

#else /* scalar fallback */

static Vec3 mat4_mul_vec4_scalar(const Mat4 m, float x, float y, float z, float w) {
    Vec3 out;
    out.x = m.elem[0][0] * x + m.elem[0][1] * y + m.elem[0][2] * z + m.elem[0][3] * w;
    out.y = m.elem[1][0] * x + m.elem[1][1] * y + m.elem[1][2] * z + m.elem[1][3] * w;
    out.z = m.elem[2][0] * x + m.elem[2][1] * y + m.elem[2][2] * z + m.elem[2][3] * w;
    return out;
}

Vec3 mat4_mul_point3(const Mat4 m, const Vec3 p) {
    return mat4_mul_vec4_scalar(m, p.x, p.y, p.z, 1.0f);
}

Vec3 mat4_mul_dir3(const Mat4 m, const Vec3 d) {
    return mat4_mul_vec4_scalar(m, d.x, d.y, d.z, 0.0f);
}

#endif

/* --------------------------- mat4_inverse --------------------------- *
 * Deliberately scalar - see the doc comment in mat4.h for why. */

Mat4 mat4_inverse(const Mat4 m) {
    float n11 = m.elem[0][0], n21 = m.elem[1][0], n31 = m.elem[2][0],
          n41 = m.elem[3][0];
    float n12 = m.elem[0][1], n22 = m.elem[1][1], n32 = m.elem[2][1],
          n42 = m.elem[3][1];
    float n13 = m.elem[0][2], n23 = m.elem[1][2], n33 = m.elem[2][2],
          n43 = m.elem[3][2];
    float n14 = m.elem[0][3], n24 = m.elem[1][3], n34 = m.elem[2][3],
          n44 = m.elem[3][3];

    float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 -
                n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 +
                n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 -
                n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 +
                n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    if (fabsf(det) < EPSILON) {
        return ZERO_MATRIX;
    }

    float det_inv = 1.0f / det;
    Mat4 res;
    res.elem[0][0] = t11 * det_inv;
    res.elem[1][0] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 +
                      n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) *
                     det_inv;
    res.elem[2][0] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 -
                      n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) *
                     det_inv;
    res.elem[3][0] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 +
                      n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) *
                     det_inv;

    res.elem[0][1] = t12 * det_inv;
    res.elem[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 -
                      n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) *
                     det_inv;
    res.elem[2][1] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 +
                      n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) *
                     det_inv;
    res.elem[3][1] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 -
                      n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) *
                     det_inv;

    res.elem[0][2] = t13 * det_inv;
    res.elem[1][2] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 +
                      n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) *
                     det_inv;
    res.elem[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 -
                      n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) *
                     det_inv;
    res.elem[3][2] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 +
                      n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) *
                     det_inv;

    res.elem[0][3] = t14 * det_inv;
    res.elem[1][3] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 -
                      n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) *
                     det_inv;
    res.elem[2][3] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 +
                      n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) *
                     det_inv;
    res.elem[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 -
                      n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) *
                     det_inv;

    return res;
}

void mat4_print(Mat4 m) {
    for (int i = 0; i < 4; i++) {
        printf("[");
        for (int j = 0; j < 4; j++) {
            printf("%f", m.elem[i][j]);
            if (j != 3) {
                printf(", ");
            }
        }
        printf("]\n");
    }
    printf("\n");
}