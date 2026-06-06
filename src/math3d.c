#include "../inc/math3d.h"
#include <math.h>

mat4 mat_identity() {
    mat4 m = {{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    }};
    return m;
}

mat4 translate(vec3 offset) {
    mat4 m = mat_identity();
    m.matrix[0][3] = offset.x;
    m.matrix[1][3] = offset.y;
    m.matrix[2][3] = offset.z;
    return m;
}

mat4 scale(vec3 factors) {
	mat4 m = mat_identity();
	m.matrix[0][0] = factors.x;
    m.matrix[1][1] = factors.y;
    m.matrix[2][2] = factors.z;
    return m;
}

mat4 rotateX(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    mat4 m = mat_identity();
    m.matrix[1][1] = c;
    m.matrix[1][2] = -s;
    m.matrix[2][1] = s;
    m.matrix[2][2] = c;
    return m;
}

mat4 rotateY(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    mat4 m = mat_identity();
    m.matrix[0][0] = c;
    m.matrix[0][2] = s;
    m.matrix[1][1] = 1;
    m.matrix[2][0] = -s;
    m.matrix[2][2] = c;
    m.matrix[3][3] = 1;
    return m;
}

mat4 rotateZ(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    mat4 m = mat_identity();
    m.matrix[0][0] = c;
    m.matrix[0][1] = -s;
    m.matrix[1][0] = s;
    m.matrix[1][1] = c;
    return m;
}

mat4 perspective(float fov, float aspect, float near, float far) {
    mat4 m = {0};
    float tan_half_fov = tan(fov / 2.0f);
    m.matrix[0][0] = 1.0f / (aspect * tan_half_fov);
    m.matrix[1][1] = 1.0f / tan_half_fov;
    m.matrix[2][2] = -(far + near) / (far - near);
    m.matrix[2][3] = -(2.0f * far * near) / (far - near);
    m.matrix[3][2] = -1.0f;
    return m;
}

mat4 mat_multiply(mat4 a, mat4 b) {
    mat4 res = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                res.matrix[i][j] += a.matrix[i][k] * b.matrix[k][j];
            }
        }
    }
    return res;
}

vec4 mat_vec_multiply(mat4 m, vec4 v) {
    vec4 res = {0};
    res.x = m.matrix[0][0]*v.x + m.matrix[0][1]*v.y + m.matrix[0][2]*v.z + m.matrix[0][3]*v.w;
    res.y = m.matrix[1][0]*v.x + m.matrix[1][1]*v.y + m.matrix[1][2]*v.z + m.matrix[1][3]*v.w;
    res.z = m.matrix[2][0]*v.x + m.matrix[2][1]*v.y + m.matrix[2][2]*v.z + m.matrix[2][3]*v.w;
    res.w = m.matrix[3][0]*v.x + m.matrix[3][1]*v.y + m.matrix[3][2]*v.z + m.matrix[3][3]*v.w;
    return res;
}
