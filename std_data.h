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
	typedef struct camera {
		glm::vec3 position;
		glm::vec3 rotation;
			float fov;
			float near_plane;
			float far_plane;
	};

	typedef struct mesh {
		std::vector<GLfloat> positions;
		std::vector<GLuint> indices;
		std::vector<GLfloat> uvs;
		std::vector<GLfloat> normals;
	};

	typedef struct transform {
			transform* parent = NULL;
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);
	};

	typedef struct model {
		GLuint vao, texture, shader;
			transform transform;
			mesh mesh;
				float texture_scale;
	};

	typedef struct framebuffer_rgb {
		GLuint id, vao, output, shader;
		bool rasturized = false;
		uint16_t width, height;
	};
}

#endif // !STANDARD_DATA_STRUCTURES_H

