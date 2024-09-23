#include<iostream>

#include"program.h"

app _app = app();

double delta_time = 0;
uint16_t width = 1200, height = 800;

const float frame_delay = 1000.0 / 246;
uint32_t frame_start;
uint32_t frame_end;
float frame_time_ms;

float m_second_counter;
float m_temp_fps;
float fps;

void start_delta_calc() {
	auto prev_now = std::chrono::system_clock::now();
	auto prev_duration = prev_now.time_since_epoch();
	auto prev_milliseconds
		= std::chrono::duration_cast<std::chrono::milliseconds>(
			prev_duration)
		.count();
	frame_start = prev_milliseconds;
}

void end_delta_calc(bool print_debug) {
	auto crnt_now = std::chrono::system_clock::now();
	auto crnt_duration = crnt_now.time_since_epoch();
	auto crnt_milliseconds
		= std::chrono::duration_cast<std::chrono::milliseconds>(
			crnt_duration)
		.count();
	frame_end = crnt_milliseconds;
	frame_time_ms = frame_end - frame_start;
	delta_time = frame_time_ms / 1000.0f;
	if (print_debug) std::cout << "DELTA::" << delta_time << "\n";
}

void start_fps_calc(bool print_debug, float delta) {
	if (m_second_counter <= 1) {
		m_second_counter += delta;
		m_temp_fps++;
	}
	else {
		fps = m_temp_fps;
		m_second_counter = 0;
		m_temp_fps = 0;
	}
	if (print_debug) std::cout << "FPS::" << fps << "\n";
}

int main() {
		_app.delta_time = delta_time;
		_app.window_width = width;
		_app.window_height = height;
	opengl::window::start_window(width, height, "WINDOW", false, glm::vec3(0.0f));
	start(&_app);
	while (!opengl::window::window_closed()) {
			_app.delta_time = delta_time;
			_app.window_width = width;
			_app.window_height = height;
		update(&_app);
		start_delta_calc();
		opengl::window::bind_window();
			render(&_app);
		opengl::window::unbind_window();
		end_delta_calc(false);
		start_fps_calc(false, (float)delta_time);
	}
		_app.delta_time = delta_time;
		_app.window_width = width;
		_app.window_height = height;
	del(&_app);
	opengl::window::end_window();
}