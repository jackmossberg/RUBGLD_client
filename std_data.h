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
		GLuint id, output;
		struct config {
			bool rasturized = false;
			uint16_t width, height;
		};
		struct data {
			GLuint vao;
			eng::mesh mesh = eng::mesh();
		};
		struct vertices {
			GLfloat positions[18] = {
				1.0f, -1.0f, 0.0f,
				-1.0f, -1.0f, 0.0f,
				-1.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 0.0f,
				1.0f, -1.0f, 0.0f,
				-1.0f, 1.0f, 0.0f,
			};
			GLfloat uvs[12] = {
				1.0f, 0.0f,
				0.0f, 0.0f,
				0.0f, 1.0f,
				1.0f, 1.0f,
				1.0f, 0.0f,
				0.0f, 1.0f,
			};
		};
	};
}

#endif // !STANDARD_DATA_STRUCTURES_H

