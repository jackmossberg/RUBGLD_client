#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec2 uv;
layout (location=2) in vec3 normal;

out vec3 _position;
out vec2 _uv;
out vec3 _normal;

out vec3 global_normal;
out float distance_to_camera;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 model_matrix;

void main() {
	_position = position;
	_uv = uv;
	_normal = normal;

	mat4 normal_matrix = inverse(transpose(model_matrix));
	global_normal = vec3(normal_matrix * vec4(normal, 0.0f));

	vec4 pos_relative_to_camera = view_matrix * (model_matrix * vec4(position, 1.0f));
	distance_to_camera = length(vec3(pos_relative_to_camera.xyz));

	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
}