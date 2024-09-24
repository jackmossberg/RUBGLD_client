#include"opengl.h"

glm::mat4 gen_view_matrix(eng::camera_radians camera);
glm::mat4 gen_view_matrix(eng::camera_radians* camera);
glm::mat4 gen_projection_matrix(eng::camera_radians camera);
glm::mat4 gen_projection_matrix(eng::camera_radians* camera);
glm::mat4 gen_view_matrix(eng::camera_locked camera);
glm::mat4 gen_view_matrix(eng::camera_locked* camera);
glm::mat4 gen_projection_matrix(eng::camera_locked camera);
glm::mat4 gen_projection_matrix(eng::camera_locked* camera);
glm::mat4 gen_transform_matrix(eng::transform transform);
glm::mat4 gen_transform_matrix(eng::transform* transform);
glm::mat4 gen_projection_matrix_ortho(eng::camera_radians camera);
glm::mat4 gen_projection_matrix_ortho(eng::camera_radians* camera);
glm::mat4 gen_projection_matrix_ortho(eng::camera_locked camera);
glm::mat4 gen_projection_matrix_ortho(eng::camera_locked* camera);

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
GLuint opengl::ex::create_shader(const char* v_shader, const char* f_shader) {
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

void opengl::load_vao_id(std::vector<GLfloat>* positions, std::vector<GLuint>* indices, std::vector<GLfloat>* uvs, std::vector<GLfloat>* normals, GLuint* _address) {
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
	*_address =  vao_id;
}
void opengl::load_tex_id(GLenum BANK, GLenum filter_t, const char* fpath, GLuint* _address) {
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
	registry::textures.push_back(tex_id);
	*_address = tex_id;
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glActiveTexture(0);
}
void opengl::load_shader_id(const char* v_shader, const char* f_shader, GLuint* _address) {
	GLuint shader_id = opengl::ex::create_shader(v_shader, f_shader);
	shader_activate(shader_id); shader_terminate();
	registry::shaders.push_back(shader_id);
	*_address = shader_id;
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
	registry::textures.push_back(tex_id);
	return tex_id;
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glActiveTexture(0);
}
GLuint opengl::load_shader_id(const char* v_shader, const char* f_shader) {
	GLuint shader_id = ex::create_shader(v_shader, f_shader);
	shader_activate(shader_id); shader_terminate();
	registry::shaders.push_back(shader_id);
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

eng::camera_radians opengl::create_camera_radians(glm::vec3 position, glm::vec3 rotation) {
	eng::camera_radians result = eng::camera_radians();
		result.position = position;
		result.rotation = rotation;
		result.fov = 90.0f;
		result.near_plane = 0.001f;
		result.far_plane = 1000.0f;
		result.isometric = false;
			result.data.target_position = position;
			result.data.target_rotation = rotation;
	return result;
}
void opengl::create_camera_radians(glm::vec3 position, glm::vec3 rotation, eng::camera_radians* _address) {
	eng::camera_radians result = eng::camera_radians();
		result.position = position;
		result.rotation = rotation;
		result.fov = 90.0f;
		result.near_plane = 0.001f;
		result.far_plane = 1000.0f;
		result.isometric = false;
			result.data.target_position = position;
			result.data.target_rotation = rotation;
	*_address = result;
}

eng::camera_locked opengl::create_camera_locked(glm::vec3 position, glm::vec3 look_position) {
	eng::camera_locked result = eng::camera_locked();
		result.position = position;
		result.look_target = look_position;
		result.fov = 90.0f;
		result.near_plane = 0.001f;
		result.far_plane = 1000.0f;
		result.isometric = false;
			result.data.target_position = position;
	return result;
}
void opengl::create_camera_locked(glm::vec3 position, glm::vec3 look_position, eng::camera_locked* _address) {
	eng::camera_locked result = eng::camera_locked();
		result.position = position;
		result.look_target = look_position;
		result.fov = 90.0f;
		result.near_plane = 0.001f;
		result.far_plane = 1000.0f;
		result.isometric = false;
			result.data.target_position = position;
	*_address = result;
}

eng::model opengl::generate_plane(const char* texture_fpath, float texture_scale, eng::transform transform, float noise_height, float noise_freq, uint16_t size) {
	eng::mesh t_mesh = eng::mesh();
	for (int row = 0; row < size + 1; row++) {
		for (int col = 0; col < size + 1; col++) {
			glm::vec3 crnt_pos = glm::vec3(row, eng::perlin((float)col / noise_freq, (float)row / noise_freq) * noise_height, col);
			t_mesh.positions.push_back(crnt_pos.x);
			t_mesh.positions.push_back(crnt_pos.y);
			t_mesh.positions.push_back(crnt_pos.z);
		}
	}
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			GLuint index = row * (size + 1) + col;
			t_mesh.indices.push_back(index);
			t_mesh.indices.push_back(index + (size + 1) + 1);
			t_mesh.indices.push_back(index + (size + 1));
			t_mesh.indices.push_back(index);
			t_mesh.indices.push_back(index + 1);
			t_mesh.indices.push_back(index + (size + 1) + 1);
		}
	}
	for (int row = 0; row < size + 1; row++) {
		for (int col = 0; col < size + 1; col++) {
			t_mesh.uvs.push_back((GLfloat)col / size);
			t_mesh.uvs.push_back((GLfloat)row / size);
		}
	}
	for (int row = 0; row < size + 1; row++) {
		for (int col = 0; col < size + 1; col++) {
			glm::vec3 crnt_norm = glm::vec3(0.0f, 1.0f, 0.0f);
			t_mesh.normals.push_back(crnt_norm.x);
			t_mesh.normals.push_back(crnt_norm.y);
			t_mesh.normals.push_back(crnt_norm.z);
		}
	}

	eng::model result = eng::model();
		result.texture_scale = texture_scale;
		result.mesh = t_mesh;
		result.transform = transform;
		result.shader = opengl::load_shader_id("default.vert", "default.frag");
		result.texture = opengl::load_tex_id(GL_TEXTURE0, GL_NEAREST, texture_fpath);
		result.vao = opengl::load_vao_id(&result.mesh.positions, &result.mesh.indices, &result.mesh.uvs, &result.mesh.normals);
	return result;
}
eng::model opengl::generate_plane(const char* texture_fpath, float texture_scale, GLuint* shader, eng::transform transform, float noise_height, float noise_freq, uint16_t size) {
	eng::mesh t_mesh = eng::mesh();
	for (int row = 0; row < size + 1; row++) {
		for (int col = 0; col < size + 1; col++) {
			glm::vec3 crnt_pos = glm::vec3(row, eng::perlin((float)col / noise_freq, (float)row / noise_freq) * noise_height, col);
			t_mesh.positions.push_back(crnt_pos.x);
			t_mesh.positions.push_back(crnt_pos.y);
			t_mesh.positions.push_back(crnt_pos.z);
		}
	}
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			GLuint index = row * (size + 1) + col;
			t_mesh.indices.push_back(index);
			t_mesh.indices.push_back(index + (size + 1) + 1);
			t_mesh.indices.push_back(index + (size + 1));
			t_mesh.indices.push_back(index);
			t_mesh.indices.push_back(index + 1);
			t_mesh.indices.push_back(index + (size + 1) + 1);
		}
	}
	for (int row = 0; row < size + 1; row++) {
		for (int col = 0; col < size + 1; col++) {
			t_mesh.uvs.push_back((GLfloat)col / size);
			t_mesh.uvs.push_back((GLfloat)row / size);
		}
	}
	for (int row = 0; row < size + 1; row++) {
		for (int col = 0; col < size + 1; col++) {
			glm::vec3 crnt_norm = glm::vec3(0.0f, 1.0f, 0.0f);
			t_mesh.normals.push_back(crnt_norm.x);
			t_mesh.normals.push_back(crnt_norm.y);
			t_mesh.normals.push_back(crnt_norm.z);
		}
	}

	eng::model result = eng::model();
		result.texture_scale = texture_scale;
		result.mesh = t_mesh;
		result.shader = *shader;
		result.transform = transform;
		result.texture = opengl::load_tex_id(GL_TEXTURE0, GL_NEAREST, texture_fpath);
		result.vao = opengl::load_vao_id(&result.mesh.positions, &result.mesh.indices, &result.mesh.uvs, &result.mesh.normals);
	return result;
}
void opengl::generate_plane(const char* texture_fpath, float texture_scale, eng::transform transform, eng::model* _address, float noise_height, float noise_freq, uint16_t size) {
	eng::mesh t_mesh = eng::mesh();
	for (int row = 0; row < size + 1; row++) {
		for (int col = 0; col < size + 1; col++) {
			glm::vec3 crnt_pos = glm::vec3(row, eng::perlin((float)col / noise_freq, (float)row / noise_freq) * noise_height, col);
			t_mesh.positions.push_back(crnt_pos.x);
			t_mesh.positions.push_back(crnt_pos.y);
			t_mesh.positions.push_back(crnt_pos.z);
		}
	}
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			GLuint index = row * (size + 1) + col;
			t_mesh.indices.push_back(index);
			t_mesh.indices.push_back(index + (size + 1) + 1);
			t_mesh.indices.push_back(index + (size + 1));
			t_mesh.indices.push_back(index);
			t_mesh.indices.push_back(index + 1);
			t_mesh.indices.push_back(index + (size + 1) + 1);
		}
	}
	for (int row = 0; row < size + 1; row++) {
		for (int col = 0; col < size + 1; col++) {
			t_mesh.uvs.push_back((GLfloat)col / size);
			t_mesh.uvs.push_back((GLfloat)row / size);
		}
	}
	for (int row = 0; row < size + 1; row++) {
		for (int col = 0; col < size + 1; col++) {
			glm::vec3 crnt_norm = glm::vec3(0.0f, 1.0f, 0.0f);
			t_mesh.normals.push_back(crnt_norm.x);
			t_mesh.normals.push_back(crnt_norm.y);
			t_mesh.normals.push_back(crnt_norm.z);
		}
	}

	eng::model result = eng::model();
		result.texture_scale = texture_scale;
		result.mesh = t_mesh;
		result.transform = transform;
		result.shader = opengl::load_shader_id("default.vert", "default.frag");
		result.texture = opengl::load_tex_id(GL_TEXTURE0, GL_NEAREST, texture_fpath);
		result.vao = opengl::load_vao_id(&result.mesh.positions, &result.mesh.indices, &result.mesh.uvs, &result.mesh.normals);
	*_address = result;
}
void opengl::generate_plane(const char* texture_fpath, float texture_scale, GLuint* shader, eng::transform transform, eng::model* _address, float noise_height, float noise_freq, uint16_t size) {
	eng::mesh t_mesh = eng::mesh();
	for (int row = 0; row < size + 1; row++) {
		for (int col = 0; col < size + 1; col++) {
			glm::vec3 crnt_pos = glm::vec3(row, eng::perlin((float)col / noise_freq, (float)row / noise_freq) * noise_height, col);
			t_mesh.positions.push_back(crnt_pos.x);
			t_mesh.positions.push_back(crnt_pos.y);
			t_mesh.positions.push_back(crnt_pos.z);
		}
	}
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			GLuint index = row * (size + 1) + col;
			t_mesh.indices.push_back(index);
			t_mesh.indices.push_back(index + (size + 1) + 1);
			t_mesh.indices.push_back(index + (size + 1));
			t_mesh.indices.push_back(index);
			t_mesh.indices.push_back(index + 1);
			t_mesh.indices.push_back(index + (size + 1) + 1);
		}
	}
	for (int row = 0; row < size + 1; row++) {
		for (int col = 0; col < size + 1; col++) {
			t_mesh.uvs.push_back((GLfloat)col / size);
			t_mesh.uvs.push_back((GLfloat)row / size);
		}
	}
	for (int row = 0; row < size + 1; row++) {
		for (int col = 0; col < size + 1; col++) {
			glm::vec3 crnt_norm = glm::vec3(0.0f, 1.0f, 0.0f);
			t_mesh.normals.push_back(crnt_norm.x);
			t_mesh.normals.push_back(crnt_norm.y);
			t_mesh.normals.push_back(crnt_norm.z);
		}
	}

	eng::model result = eng::model();
		result.texture_scale = texture_scale;
		result.mesh = t_mesh;
		result.shader = *shader;
		result.transform = transform;
		result.texture = opengl::load_tex_id(GL_TEXTURE0, GL_NEAREST, texture_fpath);
		result.vao = opengl::load_vao_id(&result.mesh.positions, &result.mesh.indices, &result.mesh.uvs, &result.mesh.normals);
	*_address = result;
}

eng::model opengl::create_model(const char* mesh_fpath, const char* texture_fpath, float texture_scale, eng::transform transform) {
	eng::mesh mesh = load_mesh_from_fpath_ptr(mesh_fpath);
		std::vector<GLfloat>* positions = &mesh.positions;
		std::vector<GLuint>* indices = &mesh.indices;
		std::vector<GLfloat>* uvs = &mesh.uvs;
		std::vector<GLfloat>* normals = &mesh.normals;
	eng::model result;
		result.mesh = mesh;
		result.transform = transform;
		result.texture_scale = texture_scale;
		result.vao = load_vao_id(positions, indices, uvs, normals);
		result.texture = load_tex_id(GL_TEXTURE0, GL_NEAREST, texture_fpath);
		result.shader = load_shader_id("default.vert", "default.frag");
	return result;
}
void opengl::create_model(const char* mesh_fpath, const char* texture_fpath, float texture_scale, eng::transform transform, eng::model* _address) {
	eng::mesh mesh = load_mesh_from_fpath_ptr(mesh_fpath);
		std::vector<GLfloat>* positions = &mesh.positions;
		std::vector<GLuint>* indices = &mesh.indices;
		std::vector<GLfloat>* uvs = &mesh.uvs;
		std::vector<GLfloat>* normals = &mesh.normals;
	eng::model result;
		result.mesh = mesh;
		result.transform = transform;
		result.texture_scale = texture_scale;
		result.vao = load_vao_id(positions, indices, uvs, normals);
		result.texture = load_tex_id(GL_TEXTURE0, GL_NEAREST, texture_fpath);
		result.shader = load_shader_id("default.vert", "default.frag");
	*_address = result;
}
eng::model opengl::create_model(const char* mesh_fpath, const char* texture_fpath, float texture_scale, GLuint* shader, eng::transform transform) {
	eng::mesh mesh = load_mesh_from_fpath_ptr(mesh_fpath);
		std::vector<GLfloat>* positions = &mesh.positions;
		std::vector<GLuint>* indices = &mesh.indices;
		std::vector<GLfloat>* uvs = &mesh.uvs;
		std::vector<GLfloat>* normals = &mesh.normals;
	eng::model result;
		result.mesh = mesh;
		result.transform = transform;
		result.texture_scale = texture_scale;
		result.vao = load_vao_id(positions, indices, uvs, normals);
		result.texture = load_tex_id(GL_TEXTURE0, GL_NEAREST, texture_fpath);
		result.shader = *shader;
	return result;
}
void opengl::create_model(const char* mesh_fpath, const char* texture_fpath, float texture_scale, GLuint* shader, eng::transform transform, eng::model* _address) {
	eng::mesh mesh = load_mesh_from_fpath_ptr(mesh_fpath);
		std::vector<GLfloat>* positions = &mesh.positions;
		std::vector<GLuint>* indices = &mesh.indices;
		std::vector<GLfloat>* uvs = &mesh.uvs;
		std::vector<GLfloat>* normals = &mesh.normals;
	eng::model result;
		result.mesh = mesh;
		result.transform = transform;
		result.texture_scale = texture_scale;
		result.vao = load_vao_id(positions, indices, uvs, normals);
		result.texture = load_tex_id(GL_TEXTURE0, GL_NEAREST, texture_fpath);
		result.shader = *shader;
	*_address = result;
}

void opengl::draw_model(eng::model model, eng::camera_radians camera) {
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	opengl::shader_activate(model.shader);
	opengl::uniform_load_mat4("model_matrix", gen_transform_matrix(model.transform), model.shader);
	opengl::uniform_load_mat4("view_matrix", gen_view_matrix(camera), model.shader);
	if (camera.isometric) opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix_ortho(camera), model.shader);
	else opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix(camera), model.shader);
	opengl::uniform_load_float("tex0_scale", model.texture_scale, model.shader);
	opengl::uniform_load_texture_2D("tex0", model.texture, model.shader, GL_TEXTURE0);
	opengl::uniform_load_float("fog_amount", opengl::RENDER_SETTINGS.fog_amount, model.shader);
	opengl::uniform_load_vec3("fog_color", opengl::RENDER_SETTINGS.fog_color, model.shader);
	opengl::uniform_load_float("ambient_light_power", opengl::RENDER_SETTINGS.ambient_light_power, model.shader);
	for (int i = 0; i < registry::objects::directional_lights.size(); i++) {
		if (i < RENDER_SETTINGS.max_directional_lights) {
			glm::vec3 light_dir_radians = glm::vec3(0.0f, 1.0f, 0.0f);
			light_dir_radians = glm::rotateX(light_dir_radians, glm::radians(registry::objects::directional_lights.at(i)->direction.x));
			light_dir_radians = glm::rotateY(light_dir_radians, glm::radians(registry::objects::directional_lights.at(i)->direction.y));
			light_dir_radians = glm::rotateZ(light_dir_radians, glm::radians(registry::objects::directional_lights.at(i)->direction.z));
			std::string int_str = std::to_string(i);
			std::string ptr_prefix = int_str + std::string("]");
			std::string dir_loc = "d_light_direction[" + ptr_prefix;
			std::string col_loc = "d_light_color[" + ptr_prefix;
			std::string pow_loc = "d_light_power[" + ptr_prefix;
			opengl::uniform_load_vec3(dir_loc.c_str(), light_dir_radians, model.shader);
			opengl::uniform_load_vec3(col_loc.c_str(), registry::objects::directional_lights.at(i)->color, model.shader);
			opengl::uniform_load_float(pow_loc.c_str(), registry::objects::directional_lights.at(i)->power, model.shader);
		}
	}
	for (int i = 0; i < registry::objects::point_lights.size(); i++) {
		if (i < RENDER_SETTINGS.max_point_lights) {
			std::string int_str = std::to_string(i);
			std::string ptr_prefix = int_str + std::string("]");
			std::string pos_loc = "p_light_position[" + ptr_prefix;
			std::string col_loc = "p_light_color[" + ptr_prefix;
			std::string pow_loc = "p_light_power[" + ptr_prefix;
			std::string ran_loc = "p_light_range[" + ptr_prefix;
			opengl::uniform_load_vec3(pos_loc.c_str(), registry::objects::point_lights.at(i)->position, model.shader);
			opengl::uniform_load_vec3(col_loc.c_str(), registry::objects::point_lights.at(i)->color, model.shader);
			opengl::uniform_load_float(pow_loc.c_str(), registry::objects::point_lights.at(i)->power, model.shader);
			opengl::uniform_load_float(ran_loc.c_str(), registry::objects::point_lights.at(i)->range, model.shader);
		}
	}
	opengl::ex::vao_activate(model.vao);
	glDrawElements(GL_TRIANGLES, sizeof(model.mesh.indices) * model.mesh.indices.size(), GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	opengl::ex::vao_terminate();
	opengl::shader_terminate();
}
void opengl::draw_model(eng::model* model, eng::camera_radians* camera) {
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	opengl::shader_activate(model->shader);
	opengl::uniform_load_mat4("model_matrix", gen_transform_matrix(model->transform), model->shader);
	opengl::uniform_load_mat4("view_matrix", gen_view_matrix(camera), model->shader);
	if (camera->isometric) opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix_ortho(camera), model->shader);
	else opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix(camera), model->shader);
	opengl::uniform_load_float("tex0_scale", model->texture_scale, model->shader);
	opengl::uniform_load_texture_2D("tex0", model->texture, model->shader, GL_TEXTURE0);
	opengl::uniform_load_float("fog_amount", opengl::RENDER_SETTINGS.fog_amount, model->shader);
	opengl::uniform_load_vec3("fog_color", opengl::RENDER_SETTINGS.fog_color, model->shader);
	opengl::uniform_load_float("ambient_light_power", opengl::RENDER_SETTINGS.ambient_light_power, model->shader);
	for (int i = 0; i < registry::objects::directional_lights.size(); i++) {
		if (i < RENDER_SETTINGS.max_directional_lights) {
			glm::vec3 light_dir_radians = glm::vec3(0.0f, 1.0f, 0.0f);
			light_dir_radians = glm::rotateX(light_dir_radians, glm::radians(registry::objects::directional_lights.at(i)->direction.x));
			light_dir_radians = glm::rotateY(light_dir_radians, glm::radians(registry::objects::directional_lights.at(i)->direction.y));
			light_dir_radians = glm::rotateZ(light_dir_radians, glm::radians(registry::objects::directional_lights.at(i)->direction.z));
			std::string int_str = std::to_string(i);
			std::string ptr_prefix = int_str + std::string("]");
			std::string dir_loc = "d_light_direction[" + ptr_prefix;
			std::string col_loc = "d_light_color[" + ptr_prefix;
			std::string pow_loc = "d_light_power[" + ptr_prefix;
			opengl::uniform_load_vec3(dir_loc.c_str(), light_dir_radians, model->shader);
			opengl::uniform_load_vec3(col_loc.c_str(), registry::objects::directional_lights.at(i)->color, model->shader);
			opengl::uniform_load_float(pow_loc.c_str(), registry::objects::directional_lights.at(i)->power, model->shader);
		}
	}
	for (int i = 0; i < registry::objects::point_lights.size(); i++) {
		if (i < RENDER_SETTINGS.max_point_lights) {
			std::string int_str = std::to_string(i);
			std::string ptr_prefix = int_str + std::string("]");
			std::string pos_loc = "p_light_position[" + ptr_prefix;
			std::string col_loc = "p_light_color[" + ptr_prefix;
			std::string pow_loc = "p_light_power[" + ptr_prefix;
			std::string ran_loc = "p_light_range[" + ptr_prefix;
			opengl::uniform_load_vec3(pos_loc.c_str(), registry::objects::point_lights.at(i)->position, model->shader);
			opengl::uniform_load_vec3(col_loc.c_str(), registry::objects::point_lights.at(i)->color, model->shader);
			opengl::uniform_load_float(pow_loc.c_str(), registry::objects::point_lights.at(i)->power, model->shader);
			opengl::uniform_load_float(ran_loc.c_str(), registry::objects::point_lights.at(i)->range, model->shader);
		}
	}
	opengl::ex::vao_activate(model->vao);
	glDrawElements(GL_TRIANGLES, sizeof(model->mesh.indices) * model->mesh.indices.size(), GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	opengl::ex::vao_terminate();
	opengl::shader_terminate();
}
void opengl::draw_model(eng::model model, eng::camera_locked camera) {
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	opengl::shader_activate(model.shader);
	opengl::uniform_load_mat4("model_matrix", gen_transform_matrix(model.transform), model.shader);
	opengl::uniform_load_mat4("view_matrix", gen_view_matrix(camera), model.shader);
	if (camera.isometric) opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix_ortho(camera), model.shader);
	else opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix(camera), model.shader);
	opengl::uniform_load_float("tex0_scale", model.texture_scale, model.shader);
	opengl::uniform_load_texture_2D("tex0", model.texture, model.shader, GL_TEXTURE0);
	opengl::uniform_load_float("fog_amount", opengl::RENDER_SETTINGS.fog_amount, model.shader);
	opengl::uniform_load_vec3("fog_color", opengl::RENDER_SETTINGS.fog_color, model.shader);
	opengl::uniform_load_float("ambient_light_power", opengl::RENDER_SETTINGS.ambient_light_power, model.shader);
	for (int i = 0; i < registry::objects::directional_lights.size(); i++) {
		if (i < RENDER_SETTINGS.max_directional_lights) {
			glm::vec3 light_dir_radians = glm::vec3(0.0f, 1.0f, 0.0f);
			light_dir_radians = glm::rotateX(light_dir_radians, glm::radians(registry::objects::directional_lights.at(i)->direction.x));
			light_dir_radians = glm::rotateY(light_dir_radians, glm::radians(registry::objects::directional_lights.at(i)->direction.y));
			light_dir_radians = glm::rotateZ(light_dir_radians, glm::radians(registry::objects::directional_lights.at(i)->direction.z));
			std::string int_str = std::to_string(i);
			std::string ptr_prefix = int_str + std::string("]");
			std::string dir_loc = "d_light_direction[" + ptr_prefix;
			std::string col_loc = "d_light_color[" + ptr_prefix;
			std::string pow_loc = "d_light_power[" + ptr_prefix;
			opengl::uniform_load_vec3(dir_loc.c_str(), light_dir_radians, model.shader);
			opengl::uniform_load_vec3(col_loc.c_str(), registry::objects::directional_lights.at(i)->color, model.shader);
			opengl::uniform_load_float(pow_loc.c_str(), registry::objects::directional_lights.at(i)->power, model.shader);
		}
	}
	for (int i = 0; i < registry::objects::point_lights.size(); i++) {
		if (i < RENDER_SETTINGS.max_point_lights) {
			std::string int_str = std::to_string(i);
			std::string ptr_prefix = int_str + std::string("]");
			std::string pos_loc = "p_light_position[" + ptr_prefix;
			std::string col_loc = "p_light_color[" + ptr_prefix;
			std::string pow_loc = "p_light_power[" + ptr_prefix;
			std::string ran_loc = "p_light_range[" + ptr_prefix;
			opengl::uniform_load_vec3(pos_loc.c_str(), registry::objects::point_lights.at(i)->position, model.shader);
			opengl::uniform_load_vec3(col_loc.c_str(), registry::objects::point_lights.at(i)->color, model.shader);
			opengl::uniform_load_float(pow_loc.c_str(), registry::objects::point_lights.at(i)->power, model.shader);
			opengl::uniform_load_float(ran_loc.c_str(), registry::objects::point_lights.at(i)->range, model.shader);
		}
	}
	opengl::ex::vao_activate(model.vao);
	glDrawElements(GL_TRIANGLES, sizeof(model.mesh.indices) * model.mesh.indices.size(), GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	opengl::ex::vao_terminate();
	opengl::shader_terminate();
}
void opengl::draw_model(eng::model* model, eng::camera_locked* camera) {
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	opengl::shader_activate(model->shader);
	opengl::uniform_load_mat4("model_matrix", gen_transform_matrix(model->transform), model->shader);
	opengl::uniform_load_mat4("view_matrix", gen_view_matrix(camera), model->shader);
	if (camera->isometric) opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix_ortho(camera), model->shader);
	else opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix(camera), model->shader);
	opengl::uniform_load_float("tex0_scale", model->texture_scale, model->shader);
	opengl::uniform_load_texture_2D("tex0", model->texture, model->shader, GL_TEXTURE0);
	opengl::uniform_load_float("fog_amount", opengl::RENDER_SETTINGS.fog_amount, model->shader);
	opengl::uniform_load_vec3("fog_color", opengl::RENDER_SETTINGS.fog_color, model->shader);
	opengl::uniform_load_float("ambient_light_power", opengl::RENDER_SETTINGS.ambient_light_power, model->shader);
	for (int i = 0; i < registry::objects::directional_lights.size(); i++) {
		if (i < RENDER_SETTINGS.max_directional_lights) {
			glm::vec3 light_dir_radians = glm::vec3(0.0f, 1.0f, 0.0f);
			light_dir_radians = glm::rotateX(light_dir_radians, glm::radians(registry::objects::directional_lights.at(i)->direction.x));
			light_dir_radians = glm::rotateY(light_dir_radians, glm::radians(registry::objects::directional_lights.at(i)->direction.y));
			light_dir_radians = glm::rotateZ(light_dir_radians, glm::radians(registry::objects::directional_lights.at(i)->direction.z));
			std::string int_str = std::to_string(i);
			std::string ptr_prefix = int_str + std::string("]");
			std::string dir_loc = "d_light_direction[" + ptr_prefix;
			std::string col_loc = "d_light_color[" + ptr_prefix;
			std::string pow_loc = "d_light_power[" + ptr_prefix;
			opengl::uniform_load_vec3(dir_loc.c_str(), light_dir_radians, model->shader);
			opengl::uniform_load_vec3(col_loc.c_str(), registry::objects::directional_lights.at(i)->color, model->shader);
			opengl::uniform_load_float(pow_loc.c_str(), registry::objects::directional_lights.at(i)->power, model->shader);
		}
	}
	for (int i = 0; i < registry::objects::point_lights.size(); i++) {
		if (i < RENDER_SETTINGS.max_point_lights) {
			std::string int_str = std::to_string(i);
			std::string ptr_prefix = int_str + std::string("]");
			std::string pos_loc = "p_light_position[" + ptr_prefix;
			std::string col_loc = "p_light_color[" + ptr_prefix;
			std::string pow_loc = "p_light_power[" + ptr_prefix;
			std::string ran_loc = "p_light_range[" + ptr_prefix;
			opengl::uniform_load_vec3(pos_loc.c_str(), registry::objects::point_lights.at(i)->position, model->shader);
			opengl::uniform_load_vec3(col_loc.c_str(), registry::objects::point_lights.at(i)->color, model->shader);
			opengl::uniform_load_float(pow_loc.c_str(), registry::objects::point_lights.at(i)->power, model->shader);
			opengl::uniform_load_float(ran_loc.c_str(), registry::objects::point_lights.at(i)->range, model->shader);
		}
	}
	opengl::ex::vao_activate(model->vao);
	glDrawElements(GL_TRIANGLES, sizeof(model->mesh.indices) * model->mesh.indices.size(), GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	opengl::ex::vao_terminate();
	opengl::shader_terminate();
}

eng::debug_element opengl::create_debug_element(const char* mesh_fpath, bool wireframe, glm::vec3 draw_color, eng::transform transform) {
	eng::mesh mesh = load_mesh_from_fpath_ptr(mesh_fpath);
		std::vector<GLfloat>* positions = &mesh.positions;
		std::vector<GLuint>* indices = &mesh.indices;
		std::vector<GLfloat>* uvs = &mesh.uvs;
		std::vector<GLfloat>* normals = &mesh.normals;
	eng::debug_element result = eng::debug_element();
		result.mesh = mesh;
		result.transform = transform;
		result.config.draw_color = draw_color;
		result.config.wireframe = wireframe;
		result.vao = load_vao_id(positions, indices, uvs, normals);
		result.shader = res::debug_shader;
	return result;
}
void opengl::create_debug_element(const char* mesh_fpath, bool wireframe, glm::vec3 draw_color, eng::transform transform, eng::debug_element* _address) {
	eng::mesh mesh = load_mesh_from_fpath_ptr(mesh_fpath);
		std::vector<GLfloat>* positions = &mesh.positions;
		std::vector<GLuint>* indices = &mesh.indices;
		std::vector<GLfloat>* uvs = &mesh.uvs;
		std::vector<GLfloat>* normals = &mesh.normals;
	eng::debug_element result = eng::debug_element();
		result.mesh = mesh;
		result.transform = transform;
		result.config.draw_color = draw_color;
		result.config.wireframe = wireframe;
		result.vao = load_vao_id(positions, indices, uvs, normals);
		result.shader = res::debug_shader;
	*_address = result;
}

void opengl::draw_debug_element(eng::debug_element debug_element, eng::camera_radians camera) {
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	if (debug_element.config.wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		opengl::shader_activate(debug_element.shader);
		opengl::uniform_load_mat4("model_matrix", gen_transform_matrix(debug_element.transform), debug_element.shader);
		opengl::uniform_load_mat4("view_matrix", gen_view_matrix(camera), debug_element.shader);
		if (camera.isometric) opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix_ortho(camera), debug_element.shader);
		else opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix(camera), debug_element.shader);
			opengl::uniform_load_vec3("draw_color", debug_element.config.draw_color, debug_element.shader);
			opengl::ex::vao_activate(debug_element.vao);
				glDrawElements(GL_TRIANGLES, sizeof(debug_element.mesh.indices) * debug_element.mesh.indices.size(), GL_UNSIGNED_INT, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		opengl::ex::vao_terminate();
	opengl::shader_terminate();
}
void opengl::draw_debug_element(eng::debug_element* debug_element, eng::camera_radians* camera) {
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	if (debug_element->config.wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		opengl::shader_activate(debug_element->shader);
		opengl::uniform_load_mat4("model_matrix", gen_transform_matrix(debug_element->transform), debug_element->shader);
		opengl::uniform_load_mat4("view_matrix", gen_view_matrix(camera), debug_element->shader);
		if (camera->isometric) opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix_ortho(camera), debug_element->shader);
		else opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix(camera), debug_element->shader);
			opengl::uniform_load_vec3("draw_color", debug_element->config.draw_color, debug_element->shader);
			opengl::ex::vao_activate(debug_element->vao);
				glDrawElements(GL_TRIANGLES, sizeof(debug_element->mesh.indices) * debug_element->mesh.indices.size(), GL_UNSIGNED_INT, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		opengl::ex::vao_terminate();
	opengl::shader_terminate();
}
void opengl::draw_debug_element(eng::debug_element debug_element, eng::camera_locked camera) {
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	if (debug_element.config.wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	opengl::shader_activate(debug_element.shader);
		opengl::uniform_load_mat4("model_matrix", gen_transform_matrix(debug_element.transform), debug_element.shader);
		opengl::uniform_load_mat4("view_matrix", gen_view_matrix(camera), debug_element.shader);
		if (camera.isometric) opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix_ortho(camera), debug_element.shader);
		else opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix(camera), debug_element.shader);
			opengl::uniform_load_vec3("draw_color", debug_element.config.draw_color, debug_element.shader);
			opengl::ex::vao_activate(debug_element.vao);
				glDrawElements(GL_TRIANGLES, sizeof(debug_element.mesh.indices) * debug_element.mesh.indices.size(), GL_UNSIGNED_INT, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		opengl::ex::vao_terminate();
	opengl::shader_terminate();
}
void opengl::draw_debug_element(eng::debug_element* debug_element, eng::camera_locked* camera) {
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	if (debug_element->config.wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	opengl::shader_activate(debug_element->shader);
		opengl::uniform_load_mat4("model_matrix", gen_transform_matrix(debug_element->transform), debug_element->shader);
		opengl::uniform_load_mat4("view_matrix", gen_view_matrix(camera), debug_element->shader);
		if (camera->isometric) opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix_ortho(camera), debug_element->shader);
		else opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix(camera), debug_element->shader);
			opengl::uniform_load_vec3("draw_color", debug_element->config.draw_color, debug_element->shader);
			opengl::ex::vao_activate(debug_element->vao);
				glDrawElements(GL_TRIANGLES, sizeof(debug_element->mesh.indices) * debug_element->mesh.indices.size(), GL_UNSIGNED_INT, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		opengl::ex::vao_terminate();
	opengl::shader_terminate();
}

void opengl::create_directional_light(glm::vec3 direction, glm::vec3 color, float power, eng::directional_light* _address) {
		_address->direction = direction;
		_address->color = color;
		_address->power = power;
	_address->registry._register = 1; // Tell the engine if this light data is being sent to the GPU...
	registry::objects::directional_lights.push_back(_address);
}
void opengl::remove_directional_light(eng::directional_light* _address) {
	if (_address->registry._register == 1) {
		for (int i = 0; i < registry::objects::directional_lights.size(); i++) {
			if (registry::objects::directional_lights.at(i) == _address) {
				registry::objects::directional_lights.erase(registry::objects::directional_lights.begin() + (i - 1));
			}
		}
	}
	_address->registry._register = -1; // Tell the engine if this light data is being sent to the GPU...
}
void opengl::create_point_light(glm::vec3 position, glm::vec3 color, float power, float range, eng::point_light* _address) {
		_address->position = position;
		_address->color = color;
		_address->power = power;
		_address->range = range;
	_address->registry._register = 1; // Tell the engine if this light data is being sent to the GPU...
	registry::objects::point_lights.push_back(_address);
}
void opengl::remove_point_light(eng::point_light* _address) {
	if (_address->registry._register == 1) {
		for (int i = 0; i < registry::objects::point_lights.size(); i++) {
			if (registry::objects::point_lights.at(i) == _address) {
				registry::objects::point_lights.erase(registry::objects::point_lights.begin() + (i - 1));
			}
		}
	}
	_address->registry._register = -1; // Tell the engine if this light data is being sent to the GPU...
}

void opengl::window::start_window(uint16_t width, uint16_t height, const char* title, bool fullscreen) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_MAXIMIZED, fullscreen);
		data.width = width; data.height = height;
		data.main_window = glfwCreateWindow(width, height, title, NULL, NULL);
		data.title = title;
	glfwMakeContextCurrent(data.main_window);
	gladLoadGL();
	glViewport(0, 0, data.width, data.height);
	glEnable(GL_DEPTH_TEST);
	GLuint post_shader = load_shader_id("post.vert", "post.frag");
		res::debug_shader = load_shader_id("debug.vert", "debug.frag");
		res::default_shader = load_shader_id("default.vert", "default.frag");
		framebuffer::load_framebuffer(data.width, data.height, &post_shader, &data.main_framebuffer);
}
void opengl::window::start_window(uint16_t width, uint16_t height, const char* title, bool fullscreen, glm::vec3 clear_color) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_MAXIMIZED, fullscreen);
		data.width = width; data.height = height;
		data.main_window = glfwCreateWindow(width, height, title, NULL, NULL);
		data.title = title; data.clear_color = clear_color;
	glfwMakeContextCurrent(data.main_window);
	gladLoadGL();
	glViewport(0, 0, data.width, data.height);
	glEnable(GL_DEPTH_TEST);
	GLuint post_shader = load_shader_id("post.vert", "post.frag");
		res::debug_shader = load_shader_id("debug.vert", "debug.frag");
		res::default_shader = load_shader_id("default.vert", "default.frag");
		framebuffer::load_framebuffer(data.width, data.height, &post_shader, &data.main_framebuffer);
}
void opengl::window::start_window(uint16_t width, uint16_t height, const char* title, bool fullscreen, const char* shader_v, const char* shader_f) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_MAXIMIZED, fullscreen);
		data.width = width; data.height = height;
		data.main_window = glfwCreateWindow(width, height, title, NULL, NULL);
		data.title = title;
	glfwMakeContextCurrent(data.main_window);
	gladLoadGL();
	glViewport(0, 0, data.width, data.height);
	glEnable(GL_DEPTH_TEST);
	GLuint post_shader = load_shader_id(shader_v, shader_f);
		res::debug_shader = load_shader_id("debug.vert", "debug.frag");
		res::default_shader = load_shader_id("default.vert", "default.frag");
		framebuffer::load_framebuffer(data.width, data.height, &post_shader, &data.main_framebuffer);
}
void opengl::window::start_window(uint16_t width, uint16_t height, const char* title, bool fullscreen, glm::vec3 clear_color, const char* shader_v, const char* shader_f) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_MAXIMIZED, fullscreen);
		data.width = width; data.height = height;
		data.main_window = glfwCreateWindow(width, height, title, NULL, NULL);
		data.title = title; data.clear_color = clear_color;
	glfwMakeContextCurrent(data.main_window);
	gladLoadGL();
	glViewport(0, 0, data.width, data.height);
	glEnable(GL_DEPTH_TEST);
	GLuint post_shader = load_shader_id(shader_v, shader_f);
		res::debug_shader = load_shader_id("debug.vert", "debug.frag");
		res::default_shader = load_shader_id("default.vert", "default.frag");
		framebuffer::load_framebuffer(data.width, data.height, &post_shader, &data.main_framebuffer);
}

void opengl::window::end_window() {
	if (
		registry::framebuffers.size() != 0 ||
		registry::vaos.size() != 0 ||
		registry::textures.size() != 0 ||
		registry::shaders.size() != 0
	) opengl::free();
	glfwTerminate();
}
bool opengl::window::window_closed() {
	if (data.main_window != NULL) return glfwWindowShouldClose(data.main_window);
	else return true;
}

GLFWwindow* opengl::window::get_window() {
	if (data.main_window != NULL) return data.main_window;
}

void opengl::window::bind_window() {
	framebuffer::bind_framebuffer_rgb(&data.main_framebuffer);
	glClearColor(
			data.clear_color.x,
			data.clear_color.y,
			data.clear_color.z,
		1.0f
	);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void opengl::window::unbind_window() {
	framebuffer::unbind_framebuffer_rgb();

	GLuint bloom_tex[1] = { framebuffer::load_tex_from_framebuffer_rgb(&data.main_framebuffer, GL_COLOR_ATTACHMENT1) };
	framebuffer::draw_framebuffer(&data.main_framebuffer, bloom_tex);
	glDeleteTextures(1, &bloom_tex[0]);

	glfwSwapBuffers(data.main_window);
	glfwPollEvents();
}

void opengl::free() {
	if (window::data.main_window != NULL) glfwDestroyWindow(window::data.main_window);
	for (GLuint shader : registry::shaders) glDeleteProgram(shader);
	for (GLuint vao : registry::vaos) glDeleteVertexArrays(1, &vao);
	for (GLuint texture : registry::textures) glDeleteTextures(1, &texture);
	for (GLuint framebuffer : registry::framebuffers) glDeleteFramebuffers(1, &framebuffer);
}

eng::skybox opengl::create_skybox(const char* cubemap_fpath) {
	std::string faces_cubemap[6] = {
		std::string(cubemap_fpath) + "/right.png",
		std::string(cubemap_fpath) + "/left.png",
		std::string(cubemap_fpath) + "/top.png",
		std::string(cubemap_fpath) + "/bottom.png",
		std::string(cubemap_fpath) + "/front.png",
		std::string(cubemap_fpath) + "/back.png"
	};

	GLuint cubemap;
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	for (unsigned int i = 0; i < 6; i++) {
		int width, height, nrChannels;
		unsigned char* data = stbi_load(faces_cubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGBA,
				width,
				height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else {
			std::cout << "Failed to load texture: " << faces_cubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	GLfloat _positions[24] = {
		-1.0f, -1.0f,  1.0f,	//        7--------6
		 1.0f, -1.0f,  1.0f,	//       /|       /|
		 1.0f, -1.0f, -1.0f,	//      4--------5 |
		-1.0f, -1.0f, -1.0f,	//      | |      | |
		-1.0f,  1.0f,  1.0f,	//      | 3------|-2
		 1.0f,  1.0f,  1.0f,	//      |/       |/
		 1.0f,  1.0f, -1.0f,	//      0--------1
		-1.0f,  1.0f, -1.0f
	};
	GLuint _indices[36] = {
		1, 2, 6,
		6, 5, 1,
		0, 4, 7,
		7, 3, 0,
		4, 5, 6,
		6, 7, 4,
		0, 3, 2,
		2, 1, 0,
		0, 1, 5,
		5, 4, 0,
		3, 7, 6,
		6, 2, 3
	};

	eng::skybox result = eng::skybox();
	std::vector<GLfloat> positions(_positions, _positions + sizeof(_positions) / sizeof(_positions[0]));
	std::vector<GLuint> indices(_indices, _indices + sizeof(_indices) / sizeof(_indices[0]));
	std::vector<GLfloat> uvs;
	std::vector<GLfloat> normals;
	eng::mesh mesh = eng::mesh{ positions, indices, uvs, normals };
	result.shader = opengl::load_shader_id("skybox.vert", "skybox.frag");
	result.vao = opengl::load_vao_id(&mesh.positions, &mesh.indices, &mesh.uvs, &mesh.normals);
	result.cubemap = cubemap;
	result.mesh = mesh;
	return result;
}
void opengl::create_skybox(const char* cubemap_fpath, eng::skybox* _address) {
	std::string faces_cubemap[6] = {
		std::string(cubemap_fpath) + "/right.png",
		std::string(cubemap_fpath) + "/left.png",
		std::string(cubemap_fpath) + "/top.png",
		std::string(cubemap_fpath) + "/bottom.png",
		std::string(cubemap_fpath) + "/front.png",
		std::string(cubemap_fpath) + "/back.png"
	};

	GLuint cubemap;
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	for (unsigned int i = 0; i < 6; i++) {
		int width, height, nrChannels;
		unsigned char* data = stbi_load(faces_cubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGBA,
				width,
				height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else {
			std::cout << "Failed to load texture: " << faces_cubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	GLfloat _positions[24] = {
		-1.0f, -1.0f,  1.0f,	//        7--------6
		 1.0f, -1.0f,  1.0f,	//       /|       /|
		 1.0f, -1.0f, -1.0f,	//      4--------5 |
		-1.0f, -1.0f, -1.0f,	//      | |      | |
		-1.0f,  1.0f,  1.0f,	//      | 3------|-2
		 1.0f,  1.0f,  1.0f,	//      |/       |/
		 1.0f,  1.0f, -1.0f,	//      0--------1
		-1.0f,  1.0f, -1.0f
	};
	GLuint _indices[36] = {
		1, 2, 6,
		6, 5, 1,
		0, 4, 7,
		7, 3, 0,
		4, 5, 6,
		6, 7, 4,
		0, 3, 2,
		2, 1, 0,
		0, 1, 5,
		5, 4, 0,
		3, 7, 6,
		6, 2, 3
	};

	eng::skybox result = eng::skybox();
		std::vector<GLfloat> positions(_positions, _positions + sizeof(_positions) / sizeof(_positions[0]));
		std::vector<GLuint> indices(_indices, _indices + sizeof(_indices) / sizeof(_indices[0]));
		std::vector<GLfloat> uvs;
		std::vector<GLfloat> normals;
	eng::mesh mesh = eng::mesh{ positions, indices, uvs, normals };
		result.shader = opengl::load_shader_id("skybox.vert", "skybox.frag");
		result.vao = opengl::load_vao_id(&mesh.positions, &mesh.indices, &mesh.uvs, &mesh.normals);
		result.cubemap = cubemap;
		result.mesh = mesh;
	*_address = result;
}
void opengl::draw_skybox(eng::skybox skybox, eng::camera_radians camera) {
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	opengl::shader_activate(skybox.shader);
		opengl::uniform_load_mat4("model_matrix", gen_transform_matrix(eng::transform{ NULL, -camera.position, glm::vec3(0.0f), glm::vec3(45.0f) }), skybox.shader);
		opengl::uniform_load_mat4("view_matrix", gen_view_matrix(camera), skybox.shader);
		opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix(camera), skybox.shader);
			opengl::uniform_load_vec3("fog_color", opengl::RENDER_SETTINGS.fog_color, skybox.shader);
			opengl::uniform_load_float("fog_height", opengl::skybox::RENDER_SETTINGS.fog_height, skybox.shader);
			opengl::uniform_load_float("fog_depth", opengl::skybox::RENDER_SETTINGS.fog_depth, skybox.shader);
			opengl::uniform_load_texture_cubemap("tex0", skybox.cubemap, skybox.shader);
				opengl::ex::vao_activate(skybox.vao);
					glDrawElements(GL_TRIANGLES, sizeof(skybox.mesh.indices) * skybox.mesh.indices.size(), GL_UNSIGNED_INT, 0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			opengl::ex::vao_terminate();
	opengl::shader_terminate();
}
void opengl::draw_skybox(eng::skybox* skybox, eng::camera_radians* camera) {
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	opengl::shader_activate(skybox->shader);
		opengl::uniform_load_mat4("model_matrix", gen_transform_matrix(eng::transform{ NULL, -camera->position, glm::vec3(0.0f), glm::vec3(45.0f) }), skybox->shader);
		opengl::uniform_load_mat4("view_matrix", gen_view_matrix(camera), skybox->shader);
		opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix(camera), skybox->shader);
			opengl::uniform_load_vec3("fog_color", opengl::RENDER_SETTINGS.fog_color, skybox->shader);
			opengl::uniform_load_float("fog_height", opengl::skybox::RENDER_SETTINGS.fog_height, skybox->shader);
			opengl::uniform_load_float("fog_depth", opengl::skybox::RENDER_SETTINGS.fog_depth, skybox->shader);
			opengl::uniform_load_texture_cubemap("tex0", skybox->cubemap, skybox->shader);
				opengl::ex::vao_activate(skybox->vao);
					glDrawElements(GL_TRIANGLES, sizeof(skybox->mesh.indices) * skybox->mesh.indices.size(), GL_UNSIGNED_INT, 0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			opengl::ex::vao_terminate();
	opengl::shader_terminate();
}
void opengl::draw_skybox(eng::skybox skybox, eng::camera_locked camera) {
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	opengl::shader_activate(skybox.shader);
		opengl::uniform_load_mat4("model_matrix", gen_transform_matrix(eng::transform{ NULL, -camera.position, glm::vec3(0.0f), glm::vec3(45.0f) }), skybox.shader);
		opengl::uniform_load_mat4("view_matrix", gen_view_matrix(camera), skybox.shader);
		opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix(camera), skybox.shader);
			opengl::uniform_load_vec3("fog_color", opengl::RENDER_SETTINGS.fog_color, skybox.shader);
			opengl::uniform_load_float("fog_height", opengl::skybox::RENDER_SETTINGS.fog_height, skybox.shader);
			opengl::uniform_load_float("fog_depth", opengl::skybox::RENDER_SETTINGS.fog_depth, skybox.shader);
			opengl::uniform_load_texture_cubemap("tex0", skybox.cubemap, skybox.shader);
				opengl::ex::vao_activate(skybox.vao);
					glDrawElements(GL_TRIANGLES, sizeof(skybox.mesh.indices) * skybox.mesh.indices.size(), GL_UNSIGNED_INT, 0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			opengl::ex::vao_terminate();
	opengl::shader_terminate();
}
void opengl::draw_skybox(eng::skybox* skybox, eng::camera_locked* camera) {
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	opengl::shader_activate(skybox->shader);
		opengl::uniform_load_mat4("model_matrix", gen_transform_matrix(eng::transform{ NULL, -camera->position, glm::vec3(0.0f), glm::vec3(45.0f) }), skybox->shader);
		opengl::uniform_load_mat4("view_matrix", gen_view_matrix(camera), skybox->shader);
		opengl::uniform_load_mat4("projection_matrix", gen_projection_matrix(camera), skybox->shader);
			opengl::uniform_load_vec3("fog_color", opengl::RENDER_SETTINGS.fog_color, skybox->shader);
			opengl::uniform_load_float("fog_height", opengl::skybox::RENDER_SETTINGS.fog_height, skybox->shader);
			opengl::uniform_load_float("fog_depth", opengl::skybox::RENDER_SETTINGS.fog_depth, skybox->shader);
			opengl::uniform_load_texture_cubemap("tex0", skybox->cubemap, skybox->shader);
				opengl::ex::vao_activate(skybox->vao);
					glDrawElements(GL_TRIANGLES, sizeof(skybox->mesh.indices) * skybox->mesh.indices.size(), GL_UNSIGNED_INT, 0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			opengl::ex::vao_terminate();
	opengl::shader_terminate();
}

eng::framebuffer_rgb opengl::framebuffer::load_framebuffer(uint16_t width, uint16_t height, GLuint* shader) {
	eng::framebuffer_rgb result = eng::framebuffer_rgb();
	GLfloat _positions[18] = {
		1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
	};
	GLfloat _uvs[12] = {
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
	};
	std::vector<GLfloat> positions(_positions, _positions + sizeof _positions / sizeof _positions[0]);
	std::vector<GLuint> indices;
	std::vector<GLfloat> uvs(_uvs, _uvs + sizeof _uvs / sizeof _uvs[0]);
	std::vector<GLfloat> normals;
	result.shader = *shader;
	result.vao = opengl::load_vao_id(&positions, &indices, &uvs, &normals);
	result.width = width; result.height = height;

	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, result.width, result.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, result.width, result.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;

	result.id = fbo;
	result.output = texture;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	opengl::registry::framebuffers.push_back(fbo);
	return result;
}
void opengl::framebuffer::load_framebuffer(uint16_t width, uint16_t height, GLuint* shader, eng::framebuffer_rgb* _address) {
	eng::framebuffer_rgb result = eng::framebuffer_rgb();
	GLfloat _positions[18] = {
		1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
	};
	GLfloat _uvs[12] = {
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
	};
	std::vector<GLfloat> positions(_positions, _positions + sizeof _positions / sizeof _positions[0]);
	std::vector<GLuint> indices;
	std::vector<GLfloat> uvs(_uvs, _uvs + sizeof _uvs / sizeof _uvs[0]);
	std::vector<GLfloat> normals;
	result.shader = *shader;
	result.vao = opengl::load_vao_id(&positions, &indices, &uvs, &normals);
	result.width = width; result.height = height;

	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, result.width, result.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, result.width, result.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;

	result.id = fbo;
	result.output = texture;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	opengl::registry::framebuffers.push_back(fbo);
	*_address = result;
}
GLuint opengl::framebuffer::load_tex_from_framebuffer_rgb(const eng::framebuffer_rgb* t_framebuffer, const GLuint& color_attachment) {
	glBindFramebuffer(GL_FRAMEBUFFER, t_framebuffer->id);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_framebuffer->width, t_framebuffer->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, color_attachment, GL_TEXTURE_2D, texture, 0);
	GLuint attachments[5] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4
	};
	glDrawBuffers(5, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}
void opengl::framebuffer::bind_framebuffer_rgb(const eng::framebuffer_rgb* t_framebuffer) {
	glBindFramebuffer(GL_FRAMEBUFFER, t_framebuffer->id);
}
void opengl::framebuffer::unbind_framebuffer_rgb() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void opengl::framebuffer::draw_framebuffer(const eng::framebuffer_rgb* t_framebuffer, GLuint textures[]) {
	opengl::shader_activate(t_framebuffer->shader);
		opengl::uniform_load_texture_2D("tex0", t_framebuffer->output, t_framebuffer->shader, GL_TEXTURE0);
		opengl::uniform_load_texture_2D("tex1", textures[0], t_framebuffer->shader, GL_TEXTURE0);
		opengl::uniform_load_float("render_scale", opengl::RENDER_SETTINGS.render_scale, t_framebuffer->shader);
		opengl::uniform_load_int("width", t_framebuffer->width, t_framebuffer->shader);
		opengl::uniform_load_int("height", t_framebuffer->width, t_framebuffer->shader);
			opengl::uniform_load_float("bloom_amount", opengl::RENDER_SETTINGS.bloom, t_framebuffer->shader);
			opengl::uniform_load_float("brightness", opengl::RENDER_SETTINGS.brightness, t_framebuffer->shader);
			glBindVertexArray(t_framebuffer->vao);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindTexture(GL_TEXTURE_2D, t_framebuffer->output);
					glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	opengl::shader_terminate();
}

glm::mat4 gen_view_matrix(eng::camera_radians camera) {
	glm::mat4 result = glm::mat4(1.0);
	result = glm::rotate(result, glm::radians(camera.rotation.y), glm::vec3(0, 1, 0));
	result = glm::rotate(result, glm::radians(camera.rotation.x), glm::vec3(1, 0, 0));
	result = glm::rotate(result, glm::radians(camera.rotation.z), glm::vec3(0, 0, 1));
	result = glm::translate(result, camera.position);
	return result;
}
glm::mat4 gen_view_matrix(eng::camera_radians* camera) {
	glm::mat4 result = glm::mat4(1.0);
	result = glm::rotate(result, glm::radians(camera->rotation.y), glm::vec3(0, 1, 0));
	result = glm::rotate(result, glm::radians(camera->rotation.x), glm::vec3(1, 0, 0));
	result = glm::rotate(result, glm::radians(camera->rotation.z), glm::vec3(0, 0, 1));
	result = glm::translate(result, camera->position);
	return result;
}
glm::mat4 gen_projection_matrix(eng::camera_radians camera) {
	glm::mat4 result = glm::mat4(1.0);
	result = glm::perspective(
		glm::radians(camera.fov),
		(float)opengl::window::data.width / (opengl::window::data.height),
		camera.near_plane,
		camera.far_plane
	); return result;
}
glm::mat4 gen_projection_matrix(eng::camera_radians* camera) {
	glm::mat4 result = glm::mat4(1.0);
	result = glm::perspective(
		glm::radians(camera->fov),
		(float)opengl::window::data.width / (opengl::window::data.height),
		camera->near_plane,
		camera->far_plane
	); return result;
}
glm::mat4 gen_view_matrix(eng::camera_locked camera) {
	glm::mat4 result = glm::mat4(1.0);
	result = glm::lookAt(-camera.position, camera.look_target, glm::vec3(0.0f, 1.0f, 0.0f));
	return result;
}
glm::mat4 gen_view_matrix(eng::camera_locked* camera) {
	glm::mat4 result = glm::mat4(1.0);
	result = glm::lookAt(-camera->position, camera->look_target, glm::vec3(0.0f, 1.0f, 0.0f));
	return result;
}
glm::mat4 gen_projection_matrix(eng::camera_locked camera) {
	glm::mat4 result = glm::mat4(1.0);
	result = glm::perspective(
		glm::radians(camera.fov),
		(float)opengl::window::data.width / (opengl::window::data.height),
		camera.near_plane,
		camera.far_plane
	); return result;
}
glm::mat4 gen_projection_matrix(eng::camera_locked* camera) {
	glm::mat4 result = glm::mat4(1.0);
	result = glm::perspective(
		glm::radians(camera->fov),
		(float)opengl::window::data.width / (opengl::window::data.height),
		camera->near_plane,
		camera->far_plane
	); return result;
}
glm::mat4 gen_transform_matrix(eng::transform transform) {
	glm::mat4 result = glm::mat4(1.0);
	result = glm::translate(result, transform.position);
	result = glm::rotate(result, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
	result = glm::rotate(result, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
	result = glm::rotate(result, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
	result = glm::scale(result, transform.scale);
		if (transform.parent != NULL) {
			result = glm::translate(result, transform.parent->position);
			result = glm::rotate(result, glm::radians(transform.parent->rotation.y), glm::vec3(0, 1, 0));
			result = glm::rotate(result, glm::radians(transform.parent->rotation.x), glm::vec3(1, 0, 0));
			result = glm::rotate(result, glm::radians(transform.parent->rotation.z), glm::vec3(0, 0, 1));
			result = glm::scale(result, transform.parent->scale);
		}
	return result;
}
glm::mat4 gen_transform_matrix(eng::transform* transform) {
	glm::mat4 result = glm::mat4(1.0);
		result = glm::translate(result, transform->position);
		result = glm::rotate(result, glm::radians(transform->rotation.y), glm::vec3(0, 1, 0));
		result = glm::rotate(result, glm::radians(transform->rotation.x), glm::vec3(1, 0, 0));
		result = glm::rotate(result, glm::radians(transform->rotation.z), glm::vec3(0, 0, 1));
		result = glm::scale(result, transform->scale);
			if (transform->parent != NULL) {
				result = glm::translate(result, transform->parent->position);
				result = glm::rotate(result, glm::radians(transform->parent->rotation.y), glm::vec3(0, 1, 0));
				result = glm::rotate(result, glm::radians(transform->parent->rotation.x), glm::vec3(1, 0, 0));
				result = glm::rotate(result, glm::radians(transform->parent->rotation.z), glm::vec3(0, 0, 1));
				result = glm::scale(result, transform->parent->scale);
			}
	return result;
}

glm::mat4 gen_projection_matrix_ortho(eng::camera_radians camera) {
	glm::mat4 result = glm::mat4(1.0);
	result = glm::ortho(
		-(opengl::window::data.width / 2.0f) * (camera.fov / 500),
		opengl::window::data.width / 2.0f * (camera.fov / 500),
		-(opengl::window::data.height / 2.0f) * (camera.fov / 500),
		opengl::window::data.height / 2.0f * (camera.fov / 500),
		camera.near_plane,
		camera.far_plane
	);
	return result;
}

glm::mat4 gen_projection_matrix_ortho(eng::camera_radians* camera) {
	glm::mat4 result = glm::mat4(1.0);
	result = glm::ortho(
		-(opengl::window::data.width / 2.0f) * (camera->fov / 500),
		opengl::window::data.width / 2.0f * (camera->fov / 500),
		-(opengl::window::data.height / 2.0f) * (camera->fov / 500),
		opengl::window::data.height / 2.0f * (camera->fov / 500),
		camera->near_plane,
		camera->far_plane
	);
	return result;
}

glm::mat4 gen_projection_matrix_ortho(eng::camera_locked camera) {
	glm::mat4 result = glm::mat4(1.0);
	result = glm::ortho(
		-(opengl::window::data.width / 2.0f) * (camera.fov / 500),
		opengl::window::data.width / 2.0f * (camera.fov / 500),
		-(opengl::window::data.height / 2.0f) * (camera.fov / 500),
		opengl::window::data.height / 2.0f * (camera.fov / 500),
		camera.near_plane,
		camera.far_plane
	);
	return result;
}

glm::mat4 gen_projection_matrix_ortho(eng::camera_locked* camera) {
	glm::mat4 result = glm::mat4(1.0);
	result = glm::ortho(
		-(opengl::window::data.width / 2.0f) * (camera->fov / 500),
		opengl::window::data.width / 2.0f * (camera->fov / 500),
		-(opengl::window::data.height / 2.0f) * (camera->fov / 500),
		opengl::window::data.height / 2.0f * (camera->fov / 500),
		camera->near_plane,
		camera->far_plane
	);
	return result;
}

void opengl::camera::update_camera_free(eng::camera_radians camera, float delta_time) {
	camera.position = eng::vec3_lerp(camera.position, camera.data.target_position, 14.0f * delta_time);
}
void opengl::camera::update_camera_free(eng::camera_radians* camera, float delta_time) {
	camera->position = eng::vec3_lerp(camera->position, camera->data.target_position, 14.0f * delta_time);
}
void opengl::camera::update_camera_free(eng::camera_locked camera, float delta_time) {
	camera.position = eng::vec3_lerp(camera.position, camera.data.target_position, 14.0f * delta_time);
}

glm::vec2 mouse_delta = glm::vec2(0.0f);
glm::vec3 orientation_radians = glm::vec3(0.0f);
void opengl::camera::update_camera_free(eng::camera_locked* camera, float delta_time) {
	glm::vec3 input_vector = glm::vec3(0.0f);
	{
		if (input::on_key(opengl::window::data.main_window, GLFW_KEY_W) && !input::on_key(opengl::window::data.main_window, GLFW_KEY_S))
			input_vector.z = 1;
		else if (!input::on_key(opengl::window::data.main_window, GLFW_KEY_W) && !input::on_key(opengl::window::data.main_window, GLFW_KEY_S))
			input_vector.z = 0;
		else if (!input::on_key(opengl::window::data.main_window, GLFW_KEY_W) && input::on_key(opengl::window::data.main_window, GLFW_KEY_S))
			input_vector.z = -1;

		if (input::on_key(opengl::window::data.main_window, GLFW_KEY_A) && !input::on_key(opengl::window::data.main_window, GLFW_KEY_D))
			input_vector.x = 1;
		else if (!input::on_key(opengl::window::data.main_window, GLFW_KEY_A) && !input::on_key(opengl::window::data.main_window, GLFW_KEY_D))
			input_vector.x = 0;
		else if (!input::on_key(opengl::window::data.main_window, GLFW_KEY_A) && input::on_key(opengl::window::data.main_window, GLFW_KEY_D))
			input_vector.x = -1;

		if (input::on_key(opengl::window::data.main_window, GLFW_KEY_E) && !input::on_key(opengl::window::data.main_window, GLFW_KEY_Q))
			input_vector.y = 1;
		else if (!input::on_key(opengl::window::data.main_window, GLFW_KEY_E) && !input::on_key(opengl::window::data.main_window, GLFW_KEY_Q))
			input_vector.y = 0;
		else if (!input::on_key(opengl::window::data.main_window, GLFW_KEY_E) && input::on_key(opengl::window::data.main_window, GLFW_KEY_Q))
			input_vector.y = -1;
	}

	camera->data.target_position -= glm::rotateY(glm::vec3((input_vector.x * 15.0f) * delta_time, (input_vector.y * 15.0f) * delta_time, (input_vector.z * 15.0f) * delta_time), glm::radians(mouse_delta.x));
	camera->position = eng::vec3_lerp(camera->position, camera->data.target_position, 14.0f * delta_time);
	
	glfwSetInputMode(opengl::window::data.main_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, 1.0f);
	if (!input::on_key(opengl::window::data.main_window, GLFW_KEY_TAB)) {
		glfwSetInputMode(opengl::window::data.main_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		mouse_delta -= input::mouse_delta(opengl::window::data.main_window) * delta_time * 85.0f;
		
		orientation = glm::rotateX(orientation, glm::radians(-mouse_delta.y));
		orientation_radians = glm::rotateY(orientation, glm::radians(mouse_delta.x));

		mouse_delta.y = glm::clamp(mouse_delta.y, -85.0f, 85.0f);
	}
	camera->look_target = orientation_radians - glm::vec3(camera->position.x, camera->position.y, camera->position.z);
}