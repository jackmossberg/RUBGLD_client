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
		const std::vector<GLfloat> positions;
		const std::vector<GLuint> indices;
		const std::vector<GLfloat> uvs;
		const std::vector<GLfloat> normals;
	};

	typedef struct transform {
			const transform* parent;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	typedef struct framebuffer_rgb {
		GLuint id, vao, output, shader;
		bool rasturized = false;
		uint16_t width, height;
	};
}

#endif // !STANDARD_DATA_STRUCTURES_H

