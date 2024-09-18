#include<iostream>

#include"opengl.h"

int main() {
	opengl::window::start_window(1200, 800, "test render v.1.0", false, glm::vec3(0.2f));
	GLuint default_shader = opengl::load_shader_id("default.vert", "default.frag");

	eng::camera _camera = opengl::create_camera(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	_camera.fov = 110.0f;
	_camera.far_plane = 1500.0f;
	_camera.near_plane = 0.01f;

	eng::model _model = opengl::create_model("res/eng/BOX.gltf", "res/eng/UV_TEST.png", 3.5f, &default_shader, eng::transform { NULL, glm::vec3(0.0f, 0.0f, 0.0f)});
	
	while (!opengl::window::window_closed()) {
		_model.transform.rotation.y += 0.035f;
		opengl::window::bind_window();
			opengl::draw_model(&_model, &_camera);
		opengl::window::unbind_window();
	}
	opengl::window::end_window();
}