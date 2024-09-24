#ifndef STD_INPUT_H
#define STD_INPUT_H

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/vec3.hpp>
#include<glm/glm.hpp>

#include<chrono>
#include<thread>

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

namespace input {
	glm::vec2 get_joy_stick_left(GLFWwindow* window);
	glm::vec2 get_joy_stick_right(GLFWwindow* window);
	glm::vec2 get_trigger_left(GLFWwindow* window);
	glm::vec2 get_trigger_right(GLFWwindow* window);
	glm::vec2 get_bump_left(GLFWwindow* window);
	glm::vec2 get_bump_right(GLFWwindow* window);

	bool on_key(GLFWwindow* window, int key_code);
	bool on_key_pressed(GLFWwindow* window, int key_code);
	bool on_key_released(GLFWwindow* window, int key_code);

	glm::vec2 mouse_delta(GLFWwindow* window);
	glm::vec2 mouse_position(GLFWwindow* window);
}

#endif // !STD_INPUT_H
