#ifndef STANDARD_DATA_STRUCTURES_H
#define STANDARD_DATA_STRUCTURES_H

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<vector>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

namespace eng {
	typedef struct mesh {
		std::vector<GLfloat> positions;
		std::vector<GLuint> indices;
		std::vector<GLfloat> uvs;
		std::vector<GLfloat> normals;
	} mesh;
}

#endif // !STANDARD_DATA_STRUCTURES_H

