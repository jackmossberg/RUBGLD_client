#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec2 uv;
layout (location=2) in vec3 normal;

out vec3 _position;
out vec2 _uv;
out vec3 _normal;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 model_matrix;

void main() {
	_position = position;
	_uv = uv;
	_normal = normal;

	gl_Position =  projection_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
}