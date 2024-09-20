#version 330 core

out vec4 main_frag;

in vec3 _position_global;
in vec3 _position;
in vec2 _uv;
in vec3 _normal;

in vec3 global_normal;
in float distance_to_camera;

uniform vec3 draw_color;

void main() {
	main_frag = vec4(draw_color, 1.0f)*2.0f;
}