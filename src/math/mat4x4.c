#include "mat4x4.h"

const Mat4x4 ZERO_MATRIX = {
    {{0.0f, 0.0f, 0.0f, 0.0f},
     {0.0f, 0.0f, 0.0f, 0.0f},
     {0.0f, 0.0f, 0.0f, 0.0f},
     {0.0f, 0.0f, 0.0f, 0.0f}}
};

const Mat4x4 IDENTITY = {
    {{1.0f, 0.0f, 0.0f, 0.0f},
     {0.0f, 1.0f, 0.0f, 0.0f},
     {0.0f, 0.0f, 1.0f, 0.0f},
     {0.0f, 0.0f, 0.0f, 1.0f}}
};

// Right to left
Mat4x4 mat_mul(const Mat4x4 a, const Mat4x4 b) {
    Mat4x4 result = ZERO_MATRIX;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result.elem[i][j] += a.elem[i][k] * b.elem[k][j];
            }
        }
    }
    return result;
}

Mat4x4 mat_scale(Mat4x4 a, vec3 scale) {
    Mat4x4 res = a;
    res.elem[0][0] *= scale.x;
    res.elem[1][1] *= scale.y;
    res.elem[2][2] *= scale.z;
    return res;
}

Mat4x4 translate(Mat4x4 a, vec3 v) {
    Mat4x4 res = a;
    res.elem[0][3] += v.x;
    res.elem[1][3] += v.y;
    res.elem[2][3] += v.z;
    return res;
}

Mat4x4 transpose(Mat4x4 a) {
    Mat4x4 result;

    result.elem[0][0] = a.elem[0][0];
    result.elem[0][1] = a.elem[1][0];
    result.elem[0][2] = a.elem[2][0];
    result.elem[0][3] = a.elem[3][0];

    result.elem[1][0] = a.elem[0][1];
    result.elem[1][1] = a.elem[1][1];
    result.elem[1][2] = a.elem[2][1];
    result.elem[1][3] = a.elem[3][1];

    result.elem[2][0] = a.elem[0][2];
    result.elem[2][1] = a.elem[1][2];
    result.elem[2][2] = a.elem[2][2];
    result.elem[2][3] = a.elem[3][2];

    result.elem[3][0] = a.elem[0][3];
    result.elem[3][1] = a.elem[1][3];
    result.elem[3][2] = a.elem[2][3];
    result.elem[3][3] = a.elem[3][3];

    return result;
}

Mat4x4 mat_invert(Mat4x4 const *m)
{
  float n11 = m->elem[0][0], n21 = m->elem[1][0], n31 = m->elem[2][0], n41 = m->elem[3][0];
  float n12 = m->elem[0][1], n22 = m->elem[1][1], n32 = m->elem[2][1], n42 = m->elem[3][1];
  float n13 = m->elem[0][2], n23 = m->elem[1][2], n33 = m->elem[2][2], n43 = m->elem[3][2];
  float n14 = m->elem[0][3], n24 = m->elem[1][3], n34 = m->elem[2][3], n44 = m->elem[3][3];

  // compute sub mat3 determinants
  float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
  float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
  float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
  float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

  float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
  if (fabsf(det) < EPSILON) {
    return ZERO_MATRIX;
  }

  float det_inv = 1.0f / det;
  Mat4x4 res;
  res.elem[0][0] = t11 * det_inv;
  res.elem[1][0] = ( n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44 ) * det_inv;
  res.elem[2][0] = ( n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44 ) * det_inv;
  res.elem[3][0] = ( n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43 ) * det_inv;

  res.elem[0][1] = t12 * det_inv;
  res.elem[1][1] = ( n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44 ) * det_inv;
  res.elem[2][1] = ( n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44 ) * det_inv;
  res.elem[3][1] = ( n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43 ) * det_inv;

  res.elem[0][2] = t13 * det_inv;
  res.elem[1][2] = ( n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44 ) * det_inv;
  res.elem[2][2] = ( n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44 ) * det_inv;
  res.elem[3][2] = ( n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43 ) * det_inv;

  res.elem[0][3] = t14 * det_inv;
  res.elem[1][3] = ( n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34 ) * det_inv;
  res.elem[2][3] = ( n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34 ) * det_inv;
  res.elem[3][3] = ( n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33 ) * det_inv;

  return res;
}

void print_matrix(Mat4x4 m) {
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
