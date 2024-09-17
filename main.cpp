#include<iostream>

#include"opengl.h"

int main() {
	opengl::window::start_window(800, 800, "test render v.1.0", false, glm::vec3(1.0f, 0.0f, 0.0f));
	while (!opengl::window::window_closed()) {
		// Update stuff...
		opengl::window::bind_window();
			// Render stuff...
		opengl::window::unbind_window();
	}
	opengl::window::end_window();
}