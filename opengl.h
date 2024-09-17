#ifndef OPENGL_H
#define OPENGL_H

#include"std_data.h"

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<stb/stb_image.h>
#include<json/json.h>

#include<algorithm>
#include<unordered_map>
#include<map>
#include<array>
#include<vector>

#include<iostream>
#include<fstream>
#include<string>
#include<strstream>
#include<string>
#include<cstdio>
#include<cerrno>

using json = nlohmann::json;

namespace opengl {
	__declspec(selectany) struct RENDER_SETTINGS {
		float render_scale = 1500.0f;
		float brightness = 1.65f;
		float bloom = 0.35f;
		float fog_amount = 0.05f;
		glm::vec3 fog_color = glm::vec3(0.2f);
		float ambient_light_power = 0.85f;
	} RENDER_SETTINGS;
	namespace skybox {
		__declspec(selectany) struct RENDER_SETTINGS {
			float fog_height = 1.35f;
			float fog_depth = 0.0f;
		} RENDER_SETTINGS;
	}

	namespace registry {
		__declspec(selectany) std::vector<GLuint> vaos;
		__declspec(selectany) std::vector<GLuint> textures;
		__declspec(selectany) std::vector<GLuint> shaders;
	}
	
	namespace ex {
		void compile_shader_err(GLuint id, const char* type);

			void activate_vertex(GLuint id);
			void terminate_vertex(GLuint id);
			void destroy_vertex(GLuint id);
			void activate_index(GLuint id);
			void terminate_index(GLuint id);
			void destroy_index(GLuint id);
		GLuint create_vbo_attrib(std::vector<GLfloat>& attrib);
		GLuint create_vbo_index(std::vector<GLuint>& index);
			void Link_attrib(GLuint vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
			void vao_activate(GLuint id);
			void vao_terminate();
			void vao_destroy(GLuint* id);
		GLuint create_vao();
	}
	std::string parse_file_from_fpath_ptr(const char* fpath);
	std::string parse_file_from_fpath_str(std::string fpath);

	void shader_activate(GLuint id);
	void shader_terminate();
	void shader_destroy(GLuint id);
		void uniform_load_texture_cubemap(const char* u_name, GLuint target, GLuint shader);
		void uniform_load_texture_2D(const char* u_name, GLuint target, GLuint shader, GLenum bank);
		void uniform_load_bool(const char* u_name, bool target, GLuint shader);
		void uniform_load_int(const char* u_name, int target, GLuint shader);
		void uniform_load_float(const char* u_name, float target, GLuint shader);
		void uniform_load_vec2(const char* u_name, glm::vec2 target, GLuint shader);
		void uniform_load_vec3(const char* u_name, glm::vec3 target, GLuint shader);
		void uniform_load_mat4(const char* u_name, glm::mat4 target, GLuint shader);
	GLuint create_shader(const char* v_shader, const char* f_shader);

	void load_vao_id(std::vector<GLfloat>* positions, std::vector<GLuint>* indices, std::vector<GLfloat>* uvs, std::vector<GLfloat>* normals, GLuint* _address);
	void load_tex_id(GLenum BANK, GLenum filter_t, const char* fpath, GLuint* _address);
	void load_shader_id(const char* v_shader, const char* f_shader, GLuint* _address);

	GLuint load_vao_id(std::vector<GLfloat>* positions, std::vector<GLuint>* indices, std::vector<GLfloat>* uvs, std::vector<GLfloat>* normals);
	GLuint load_tex_id(GLenum BANK, GLenum filter_t, const char* fpath);
	GLuint load_shader_id(const char* v_shader, const char* f_shader);

	eng::mesh load_mesh_from_fpath_ptr(const char* fpath);
	eng::mesh load_mesh_from_fpath_str(std::string fpath);

	namespace window {
		_declspec(selectany) struct {
			GLFWwindow* main_window;
			uint16_t width, height;
			const char* title;
			glm::vec3 clear_color = glm::vec3(0.0f);
		} data;
		void start_window(uint16_t width, uint16_t height, const char* title, bool fullscreen);
		void start_window(uint16_t width, uint16_t height, const char* title, bool fullscreen, glm::vec3 clear_color);
		void end_window();
		bool window_closed();
		GLFWwindow* get_window();
			void bind_window();
			void unbind_window();
	}

	void free();
}

#endif // !OPENGL_H

