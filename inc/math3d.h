#ifndef MATH3D_H
# define MATH3D_H

typedef struct vec3 vec3;
struct vec3 {
 	float x,y,z;
};

typedef struct mat4 mat4;
struct mat4 {
	float matrix[4][4];
};

mat4 mat_identity();
mat4 translate(vec3 offset);
mat4 scale(vec3 factors);
mat4 rotateX(float angle);
mat4 rotateY(float angle);
mat4 rotateZ(float angle);
mat4 perspective(float fov, float aspect, float near, float far);

#endif
