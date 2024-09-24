#include"std_input.h"

glm::vec2 input::get_joy_stick_left(GLFWwindow* window) {
	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	if (1 == present) {
		int axes_count;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);

		glm::vec2 result;
		result.x = axes[0];
		result.y = axes[1];

		return result;
	}
}

glm::vec2 input::get_joy_stick_right(GLFWwindow* window) {
	int present = glfwJoystickPresent(GLFW_JOYSTICK_2);
	if (1 == present) {
		int axes_count;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_2, &axes_count);

		glm::vec2 result;
		result.x = axes[4];
		result.y = axes[5];

		return result;
	}
}

glm::vec2 input::get_trigger_left(GLFWwindow* window) {

}

glm::vec2 input::get_trigger_right(GLFWwindow* window) {

}

glm::vec2 input::get_bump_left(GLFWwindow* window) {

}

glm::vec2 input::get_bump_right(GLFWwindow* window) {

}

bool input::on_key(GLFWwindow* window, int key_code) {
	return glfwGetKey(window, key_code);
}

bool input::on_key_pressed(GLFWwindow* window, int key_code) {
	int state = glfwGetKey(window, key_code);
	if (state == GLFW_PRESS)
		return true;
	else
		return false;
}

bool input::on_key_released(GLFWwindow* window, int key_code) {
	int state = glfwGetKey(window, key_code);
	if (state == GLFW_RELEASE)
		return true;
	else
		return false;
}

double
mouse_pos_x = 0,
mouse_pos_y = 0,
prev_mouse_pos_x = 0,
prev_mouse_pos_y = 0;
glm::vec2 input::mouse_delta(GLFWwindow* window) {
	glm::vec2 result;
		glfwGetCursorPos(window, &mouse_pos_x, &mouse_pos_y);
		result = glm::vec2((float)mouse_pos_x - (float)prev_mouse_pos_x, (float)mouse_pos_y - (float)prev_mouse_pos_y);
		glfwGetCursorPos(window, &prev_mouse_pos_x, &prev_mouse_pos_y);
	return result;
}

glm::vec2 input::mouse_position(GLFWwindow* window) {
	return glm::vec2((float)mouse_pos_x, (float)mouse_pos_y);
}