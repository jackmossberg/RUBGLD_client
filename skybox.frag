#version 330 core

out vec4 main_frag;

in vec3 _position;
in vec2 _uv;
in vec3 _normal;

uniform samplerCube tex0;

uniform vec3 fog_color;

uniform float fog_height;
uniform float fog_depth;

void main() {
    vec3 _uv_cb = vec3(_position.x, _position.y, -_position.z);
	float fog_factor = (_uv_cb.y - fog_depth) / (fog_height - fog_depth);
	fog_factor = clamp(fog_factor, 0.0f, 1.0f);
	main_frag = mix(vec4(fog_color, 1.0f), texture(tex0, _uv_cb), fog_factor);
}