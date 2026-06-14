#ifndef MATH3D_H
# define MATH3D_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vec3 vec3;
struct vec3 {
 	float x,y,z;
};

typedef struct vec4 vec4;
struct vec4 {
 	float x,y,z,w;
};

typedef struct mat4 mat4;
struct mat4 {
	float matrix[4][4];
};

mat4 mat_identity();
mat4 mat_multiply(mat4 a, mat4 b);
vec4 mat_vec_multiply(mat4 m, vec4 v0);
mat4 translate(vec3 offset);
mat4 scale(vec3 factors);
mat4 rotateX(float angle);
mat4 rotateY(float angle);
mat4 rotateZ(float angle);
mat4 perspective(float fov, float aspect, float near, float far);

#ifdef __cplusplus
}
#endif
#endif
