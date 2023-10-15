#ifndef MAX4x4_H
#define MAX4x4_H

// Define the 4x4 matrix structure
typedef struct {
    float data[4][4];
} mat4x4;

// Function to add two mat4x4 matrices
static inline mat4x4 mat4x4_add(const mat4x4 a, const mat4x4 b) {
    mat4x4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }
    return result;
}

// Function to subtract two mat4x4 matrices
static inline mat4x4 mat4x4_sub(const mat4x4 a, const mat4x4 b) {
    mat4x4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = a.data[i][j] - b.data[i][j];
        }
    }
    return result;
}

// Function to multiply two mat4x4 matrices
static inline mat4x4 mat4x4_mul(const mat4x4 a, const mat4x4 b) {
    mat4x4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                result.data[i][j] += a.data[i][k] * b.data[k][j];
            }
        }
    }
    return result;
}

// Function to transpose a mat4x4 matrix
static inline mat4x4 mat4x4_transpose(const mat4x4 mat) {
    mat4x4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = mat.data[j][i];
        }
    }
    return result;
}

// Function to scale a mat4x4 matrix by a scalar value
static inline mat4x4 mat4x4_scale(const mat4x4 mat, float scale) {
    mat4x4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = scale * mat.data[i][j];
        }
    }
    return result;
}

// Function to create an identity mat4x4 matrix
static inline mat4x4 mat4x4_identity() {
    mat4x4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    return result;
}

#endif // MAX4x4_H