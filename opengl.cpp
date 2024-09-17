#include"opengl.h"

std::string opengl::parse_file_from_fpath_ptr(const char* fname) {
	std::ifstream in(fname, std::ios::in | std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	} throw(errno);
}
std::string opengl::parse_file_from_fpath_str(std::string fname) {
	std::ifstream in(fname.c_str(), std::ios::in | std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	} throw(errno);
}

void opengl::shader_activate(GLuint id) {
	glUseProgram(id);
}
void opengl::shader_terminate() {
	glUseProgram(0);
}
void opengl::shader_destroy(GLuint id) {
	glDeleteProgram(id);
}
void opengl::uniform_load_texture_cubemap(const char* u_name, GLuint target, GLuint shader) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, target);
	GLuint loc = glGetUniformLocation(shader, u_name);
	glUniform1i(loc, GL_TEXTURE0);
}
void opengl::uniform_load_texture_2D(const char* u_name, GLuint target, GLuint shader, GLenum bank) {
	glActiveTexture(bank);
	glBindTexture(GL_TEXTURE_2D, target);
	GLuint loc = glGetUniformLocation(shader, u_name);
	glUniform1i(loc, bank);
}
void opengl::uniform_load_bool(const char* u_name, bool target, GLuint shader) {
	GLuint loc = glGetUniformLocation(shader, u_name);
	glUniform1i(loc, target);
}
void opengl::uniform_load_int(const char* u_name, int target, GLuint shader) {
	GLuint loc = glGetUniformLocation(shader, u_name);
	glUniform1i(loc, target);
}
void opengl::uniform_load_float(const char* u_name, float target, GLuint shader) {
	GLuint loc = glGetUniformLocation(shader, u_name);
	glUniform1f(loc, target);
}
void opengl::uniform_load_vec2(const char* u_name, glm::vec2 target, GLuint shader) {
	GLuint loc = glGetUniformLocation(shader, u_name);
	glUniform2f(loc, target.x, target.y);
}
void opengl::uniform_load_vec3(const char* u_name, glm::vec3 target, GLuint shader) {
	GLuint loc = glGetUniformLocation(shader, u_name);
	glUniform3f(loc, target.x, target.y, target.z);
}
void opengl::uniform_load_mat4(const char* u_name, glm::mat4 target, GLuint shader) {
	GLuint loc = glGetUniformLocation(shader, u_name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(target));
}
void opengl::ex::compile_shader_err(GLuint id, const char* type) {
	GLint hasCompiled; char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(id, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(id, 1024, NULL, infoLog);
			std::cout << "ERR:[SHADER COMPILATION ERROR: " << type << "]\n" << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(id, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(id, 1024, NULL, infoLog);
			std::cout << "SHADER LINKING ERROR: " << type << "]\n" << infoLog << std::endl;
		}
	}
}
GLuint opengl::create_shader(const char* v_shader, const char* f_shader) {
	GLuint id;
	const char* f_source;
	const char* v_source;
	std::string vs = parse_file_from_fpath_ptr(v_shader).c_str();
	std::string fs = parse_file_from_fpath_ptr(f_shader).c_str();
	v_source = vs.c_str();
	f_source = fs.c_str();
	if (vs != "") std::cout << "MSG:[SHADER_FILES_LOADED]\n";
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &v_source, NULL);
	glCompileShader(vertexShader);
	ex::compile_shader_err(vertexShader, "VERTEX");
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &f_source, NULL);
	glCompileShader(fragmentShader);
	ex::compile_shader_err(fragmentShader, "FRAGMENT");
	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);
	ex::compile_shader_err(id, "PROGRAM");
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return id; std::cout << "MSG:[SHADER_CREATED]\n";
}

void opengl::ex::activate_vertex(GLuint id) {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}
void opengl::ex::terminate_vertex(GLuint id) {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}
void opengl::ex::destroy_vertex(GLuint id) {
	glDeleteBuffers(1, &id);
}
void opengl::ex::activate_index(GLuint id) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}
void opengl::ex::terminate_index(GLuint id) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void opengl::ex::destroy_index(GLuint id) {
	glDeleteBuffers(1, &id);
}
GLuint opengl::ex::create_vbo_attrib(std::vector<GLfloat>& attrib) {
	GLuint id;
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, attrib.size() * sizeof(GLfloat), attrib.data(), GL_STATIC_DRAW);
	terminate_vertex(id); return id;
}
GLuint opengl::ex::create_vbo_index(std::vector<GLuint>& index) {
	GLuint id;
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);
	terminate_index(id); return id;
}
void opengl::ex::Link_attrib(GLuint vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	activate_vertex(vbo);
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	terminate_vertex(vbo);
}
void opengl::ex::vao_activate(GLuint id) {
	glBindVertexArray(id);
}
void opengl::ex::vao_terminate() {
	glBindVertexArray(0);
}
void opengl::ex::vao_destroy(GLuint* id) {
	glDeleteVertexArrays(1, id);
}
GLuint opengl::ex::create_vao() {
	GLuint id; glGenVertexArrays(1, &id); return id;
}

GLuint opengl::load_vao_id(std::vector<GLfloat>* positions, std::vector<GLuint>* indices, std::vector<GLfloat>* uvs, std::vector<GLfloat>* normals) {
	GLuint
		vao_id,
		vao_positions_buffer,
		vao_indices_buffer,
		vao_uvs_buffer,
		vao_normals_buffer;
	vao_id = ex::create_vao();
	ex::vao_activate(vao_id);
	vao_positions_buffer = ex::create_vbo_attrib(*positions);
	vao_indices_buffer = ex::create_vbo_index(*indices);
	vao_uvs_buffer = ex::create_vbo_attrib(*uvs);
	vao_normals_buffer = ex::create_vbo_attrib(*normals);
	ex::activate_vertex(vao_positions_buffer);
	ex::activate_vertex(vao_uvs_buffer);
	ex::activate_vertex(vao_normals_buffer);
	ex::activate_index(vao_indices_buffer);
	ex::Link_attrib(vao_positions_buffer, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	ex::Link_attrib(vao_uvs_buffer, 1, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);
	ex::Link_attrib(vao_normals_buffer, 2, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	ex::vao_terminate();
	ex::terminate_vertex(vao_positions_buffer);
	ex::terminate_vertex(vao_uvs_buffer);
	ex::terminate_vertex(vao_normals_buffer);
	ex::terminate_index(vao_indices_buffer);
	registry::vaos.push_back(vao_id);
	return vao_id;
}
GLuint opengl::load_tex_id(GLenum BANK, GLenum filter_t, const char* fpath) {
	GLuint tex_id;
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(fpath, &widthImg, &heightImg, &numColCh, 0);
	glGenTextures(1, &tex_id);
	glActiveTexture(BANK);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	stbi_image_free(bytes);
	registry::vaos.push_back(tex_id);
	return tex_id;
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glActiveTexture(0);
}
GLuint opengl::load_shader_id(const char* v_shader, const char* f_shader) {
	GLuint shader_id = create_shader(v_shader, f_shader);
	shader_activate(shader_id); shader_terminate();
	registry::vaos.push_back(shader_id);
	return shader_id;
}

std::vector<GLfloat> load_GLfloat_list_from_json(json accessor, json JSON, std::vector<unsigned char> data) {
	std::vector<GLfloat> positions;
	unsigned int buff_view_ind = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int acc_byte_offset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	json buffer_view = JSON["bufferViews"][buff_view_ind];
	unsigned int byte_offset = buffer_view["byteOffset"];

	unsigned int num_per_vert;
	if (type == "SCALAR") { num_per_vert = 1; }
	else if (type == "VEC2") { num_per_vert = 2; }
	else if (type == "VEC3") { num_per_vert = 3; }
	else if (type == "VEC4") { num_per_vert = 4; }
	else throw std::invalid_argument("ERR::TRIED_TO_LOAD_INVALID_ATTRIBUTE_TYPE");

	unsigned int beginning_of_data = byte_offset + acc_byte_offset;
	unsigned int length_of_data = count * 4 * num_per_vert;
	for (unsigned int i = beginning_of_data; i < beginning_of_data + length_of_data; i) {
		unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		positions.push_back((GLfloat)value);
	}
	return positions;
}

std::vector<GLuint> load_GLuint_list_from_json(json accessor, json JSON, std::vector<unsigned char> data) {
	std::vector<GLuint> indices;

	unsigned int buff_view_ind = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int acc_byte_offset = accessor.value("byteOffset", 0);
	unsigned int component_type = accessor["componentType"];

	json buffer_view = JSON["bufferViews"][buff_view_ind];
	unsigned int byte_offset = buffer_view.value("byteOffset", 0);

	unsigned int beginning_of_data = byte_offset + acc_byte_offset;
	if (component_type == 5125) {
		for (unsigned int i = beginning_of_data; i < byte_offset + acc_byte_offset + count * 4; i) {
			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);
		}
	}
	else if (component_type == 5123) {
		for (unsigned int i = beginning_of_data; i < byte_offset + acc_byte_offset + count * 2; i) {
			unsigned char bytes[] = { data[i++], data[i++] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	}
	else if (component_type == 5122) {
		for (unsigned int i = beginning_of_data; i < byte_offset + acc_byte_offset + count * 2; i) {
			unsigned char bytes[] = { data[i++], data[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}
	return indices;
}

eng::mesh opengl::load_mesh_from_fpath_ptr(const char* fpath) {
	std::string text = opengl::parse_file_from_fpath_ptr(fpath);
	json JSON = json::parse(text);

	std::string bytes_text;
	std::string uri = JSON["buffers"][0]["uri"];
	std::string file_directory = std::string(fpath).substr(0, std::string(fpath).find_last_of('/') + 1);
	bytes_text = opengl::parse_file_from_fpath_str((file_directory + uri).c_str());
	std::vector<unsigned char> data(bytes_text.begin(), bytes_text.end());

	unsigned int pos_acc_ind = JSON["meshes"][0]["primitives"][0]["attributes"]["POSITION"];
	unsigned int ind_acc_ind = JSON["meshes"][0]["primitives"][0]["indices"];
	unsigned int uv_acc_ind = JSON["meshes"][0]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int normal_acc_ind = JSON["meshes"][0]["primitives"][0]["attributes"]["NORMAL"];

	std::vector<GLfloat> positions = load_GLfloat_list_from_json(JSON["accessors"][pos_acc_ind], JSON, data);
	std::vector<GLuint> indices = load_GLuint_list_from_json(JSON["accessors"][ind_acc_ind], JSON, data);
	std::vector<GLfloat> uvs = load_GLfloat_list_from_json(JSON["accessors"][uv_acc_ind], JSON, data);
	std::vector<GLfloat> normals = load_GLfloat_list_from_json(JSON["accessors"][normal_acc_ind], JSON, data);

	eng::mesh result = eng::mesh{
		positions,
		indices,
		uvs,
		normals
	};
	return result;
}

eng::mesh opengl::load_mesh_from_fpath_str(std::string fpath) {
	std::string text = parse_file_from_fpath_str(fpath);
	json JSON = json::parse(text);

	std::string bytes_text;
	std::string uri = JSON["buffers"][0]["uri"];
	std::string file_directory = fpath.substr(0, fpath.find_last_of('/') + 1);
	bytes_text = parse_file_from_fpath_str((file_directory + uri).c_str());
	std::vector<unsigned char> data(bytes_text.begin(), bytes_text.end());

	unsigned int pos_acc_ind = JSON["meshes"][0]["primitives"][0]["attributes"]["POSITION"];
	unsigned int ind_acc_ind = JSON["meshes"][0]["primitives"][0]["indices"];
	unsigned int uv_acc_ind = JSON["meshes"][0]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int normal_acc_ind = JSON["meshes"][0]["primitives"][0]["attributes"]["NORMAL"];

	std::vector<GLfloat> positions = load_GLfloat_list_from_json(JSON["accessors"][pos_acc_ind], JSON, data);
	std::vector<GLuint> indices = load_GLuint_list_from_json(JSON["accessors"][ind_acc_ind], JSON, data);
	std::vector<GLfloat> uvs = load_GLfloat_list_from_json(JSON["accessors"][uv_acc_ind], JSON, data);;
	std::vector<GLfloat> normals = load_GLfloat_list_from_json(JSON["accessors"][normal_acc_ind], JSON, data);;

	eng::mesh result = eng::mesh{
		positions,
		indices,
		uvs,
		normals
	};
	return result;
}

void opengl::free() {
	for (GLuint shader : registry::shaders) glDeleteProgram(shader);
	for (GLuint vao : registry::vaos) glDeleteVertexArrays(1, &vao);
	for (GLuint texture : registry::textures) glDeleteTextures(1, &texture);
}