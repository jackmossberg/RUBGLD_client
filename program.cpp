#include"program.h"

eng::camera_locked _camera;
eng::skybox _skybox;

eng::model _model;
eng::model _terrain;

eng::directional_light _sun;

eng::debug_element point_light_debug;
eng::debug_element point_light_debug_2;
eng::debug_element point_light_debug_3;

eng::point_light point_light;
eng::point_light point_light_2;
eng::point_light point_light_3;

void start(app* sys) {
	_camera = opengl::create_camera_locked(glm::vec3(15.0f, -15.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	_camera.fov = 95.0f;
	_camera.far_plane = 1500.0f;
	_camera.near_plane = 0.01f;
	//_camera.isometric = true;

	_skybox = opengl::create_skybox("res/eng/skybox_3");

	_model = opengl::create_model("res/eng/BOX.gltf", "res/eng/UV_TEST.png", 3.5f, &opengl::res::default_shader, eng::transform{ NULL, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.3f) });

	uint16_t terrain_size = 150;
	_terrain = opengl::generate_plane("res/eng/UV_TEST.png", 25.5f, &opengl::res::default_shader, eng::transform{ NULL, glm::vec3(-terrain_size / 2, -5.0f, -terrain_size / 2) }, 14.0f, 15.0f, terrain_size);

	_sun; opengl::create_directional_light(glm::vec3(45.0f, 45.0f, 45.0f), glm::vec3(1.0f, 1.0f, 1.05f), 1.05f, &_sun);

	point_light_debug = opengl::create_debug_element("res/eng/BOX.gltf", true, glm::vec3(1.0f, 0.0f, 0.0f), eng::transform{ NULL, glm::vec3(3.0f, -2.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.2f) });
	point_light_debug_2 = opengl::create_debug_element("res/eng/BOX.gltf", true, glm::vec3(0.0f, 1.0f, 0.0f), eng::transform{ NULL, glm::vec3(-3.2f, -3.0f, 2.0f), glm::vec3(0.0f), glm::vec3(0.2f) });
	point_light_debug_3 = opengl::create_debug_element("res/eng/BOX.gltf", true, glm::vec3(0.0f, 0.0f, 1.0f), eng::transform{ NULL, glm::vec3(-3.5f, 1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.2f) });

	point_light; opengl::create_point_light(point_light_debug.transform.position, glm::vec3(1.0f, 0.0f, 0.0f), 3.0f, 8.0f, &point_light);
	point_light_2; opengl::create_point_light(point_light_debug_2.transform.position, glm::vec3(0.0f, 1.0f, 0.0f), 3.0f, 8.0f, &point_light_2);
	point_light_3; opengl::create_point_light(point_light_debug_3.transform.position, glm::vec3(0.0f, 0.0f, 1.0f), 3.0f, 8.0f, &point_light_3);
}

void update(app* sys) {
	opengl::camera::update_camera_free(&_camera, sys->delta_time);

	_model.transform.rotation.y += 45 * sys->delta_time;

	point_light_debug.transform.rotation.y += 45 * sys->delta_time;
	point_light_debug_2.transform.rotation.x += 45 * sys->delta_time;
	point_light_debug_3.transform.rotation.z += 45 * sys->delta_time;
}

void render(app* sys) {
	opengl::draw_skybox(&_skybox, &_camera);
	opengl::draw_model(&_model, &_camera);
	opengl::draw_model(&_terrain, &_camera);
	opengl::draw_debug_element(&point_light_debug, &_camera);
	opengl::draw_debug_element(&point_light_debug_2, &_camera);
	opengl::draw_debug_element(&point_light_debug_3, &_camera);
}

void del(app* sys) {}
