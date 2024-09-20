#include<iostream>

#include"opengl.h"

int main() {
	opengl::window::start_window(1200, 800, "test render v.1.0", false, glm::vec3(0.2f));
	GLuint default_shader = opengl::load_shader_id("default.vert", "default.frag");

	eng::camera_locked _camera = opengl::create_camera_locked(glm::vec3(0.0f, -2.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	_camera.fov = 110.0f;
	_camera.far_plane = 1500.0f;
	_camera.near_plane = 0.01f;

	eng::skybox _skybox = opengl::create_skybox("res/eng/skybox_3");

	eng::model _model = opengl::create_model("res/eng/BOX.gltf", "res/eng/UV_TEST.png", 3.5f, &default_shader, eng::transform { NULL, glm::vec3(0.0f, 0.0f, 0.0f)});

	uint16_t terrain_size = 150;
	eng::model _terrain = opengl::generate_plane("res/eng/UV_TEST.png", 15.5f, &default_shader, eng::transform{ NULL, glm::vec3(-terrain_size/2, -5.0f, -terrain_size /2) }, 0.0f, 15.0f, terrain_size);

	eng::directional_light _sun; opengl::create_directional_light(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.4f, &_sun);

	while (!opengl::window::window_closed()) {
		_model.transform.rotation.y += 0.185f;
		opengl::window::bind_window();
		opengl::draw_skybox(&_skybox, &_camera);
			opengl::draw_model(&_model, &_camera);
			opengl::draw_model(&_terrain, &_camera);
		opengl::window::unbind_window();
	}
	opengl::window::end_window();
}