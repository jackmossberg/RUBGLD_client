#include<iostream>

#include"opengl.h"

int main() {
	opengl::window::start_window(1200, 800, "test render v.1.0", false, glm::vec3(0.0f));

	eng::camera_locked _camera = opengl::create_camera_locked(glm::vec3(0.0f, -2.0f, -7.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	_camera.fov = 110.0f;
	_camera.far_plane = 1500.0f;
	_camera.near_plane = 0.01f;
	GLuint default_shader = opengl::load_shader_id("default.vert", "default.frag");


	eng::skybox _skybox = opengl::create_skybox("res/eng/skybox_2");

	eng::model _model = opengl::create_model("res/eng/MONKEY.gltf", "res/eng/UV_TEST.png", 3.5f, &default_shader, eng::transform { NULL, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(2.3f)});

	uint16_t terrain_size = 150;
	eng::model _terrain = opengl::generate_plane("res/eng/UV_TEST.png", 25.5f, &default_shader, eng::transform{ NULL, glm::vec3(-terrain_size/2, -5.0f, -terrain_size /2) }, 0.0f, 15.0f, terrain_size);

	opengl::RENDER_SETTINGS.ambient_light_power = 0.0f;
	opengl::RENDER_SETTINGS.fog_amount = 0.014f;
	opengl::RENDER_SETTINGS.fog_color = glm::vec3(0.0);
	opengl::RENDER_SETTINGS.bloom = 0.45f;
	opengl::RENDER_SETTINGS.render_scale = 5500;

	opengl::skybox::RENDER_SETTINGS.fog_height = 11.88f;

	eng::directional_light _sun; opengl::create_directional_light(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 0.85f, &_sun);

	eng::debug_element point_light_debug = opengl::create_debug_element("res/eng/BOX.gltf", false, glm::vec3(1.0f, 0.0f, 0.0f), eng::transform { NULL, glm::vec3(3.0f, -2.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.2f) });
	eng::debug_element point_light_debug_2 = opengl::create_debug_element("res/eng/BOX.gltf", false, glm::vec3(0.0f, 1.0f, 0.0f), eng::transform{ NULL, glm::vec3(-3.2f, -3.0f, 2.0f), glm::vec3(0.0f), glm::vec3(0.2f) });
	eng::debug_element point_light_debug_3 = opengl::create_debug_element("res/eng/BOX.gltf", false, glm::vec3(0.0f, 0.0f, 1.0f), eng::transform{ NULL, glm::vec3(-3.5f, 1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.2f) });

	eng::point_light point_light; opengl::create_point_light(point_light_debug.transform.position, glm::vec3(1.0f, 0.0f, 0.0f), 3.0f, 5.0f, &point_light);
	eng::point_light point_light_2; opengl::create_point_light(point_light_debug_2.transform.position, glm::vec3(0.0f, 1.0f, 0.0f), 3.0f, 5.0f, &point_light_2);
	eng::point_light point_light_3; opengl::create_point_light(point_light_debug_3.transform.position, glm::vec3(0.0f, 0.0f, 1.0f), 3.0f, 5.0f, &point_light_3);

	while (!opengl::window::window_closed()) {
		_model.transform.rotation.y += 0.185f;
		opengl::window::bind_window();
		opengl::draw_skybox(&_skybox, &_camera);
			opengl::draw_model(&_model, &_camera);
			opengl::draw_model(&_terrain, &_camera);
				opengl::draw_debug_element(&point_light_debug, &_camera);
				opengl::draw_debug_element(&point_light_debug_2, &_camera);
				opengl::draw_debug_element(&point_light_debug_3, &_camera);
		opengl::window::unbind_window();
	}
	opengl::window::end_window();
}