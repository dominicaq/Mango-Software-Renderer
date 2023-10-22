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