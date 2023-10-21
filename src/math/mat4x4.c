#include "mat4x4.h"

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
    Mat4x4 m = a;
    m.elem[0][0] *= scale.x;
    m.elem[1][1] *= scale.y;
    m.elem[2][2] *= scale.z;
    return m;
}

Mat4x4 translate(Mat4x4 a, vec3 v) {
    Mat4x4 m = a;
    m.elem[0][3] = v.x;
    m.elem[1][3] = v.y;
    m.elem[2][3] = v.z;
    return m;
}

void print_matrix(const Mat4x4 m) {
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