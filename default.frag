#version 330 core

out vec4 main_frag;

in vec3 _position;
in vec2 _uv;
in vec3 _normal;

in vec3 global_normal;
in float distance_to_camera;

uniform float tex0_scale;
uniform sampler2D tex0;

uniform float fog_amount;
uniform vec3 fog_color;

float fog_gradient = 1.5f;

#define DIR_LIGHT_COUNT 64
#define POINT_LIGHT_COUNT 64

uniform vec3 d_light_direction[DIR_LIGHT_COUNT];
uniform vec3 d_light_color[DIR_LIGHT_COUNT];
uniform float d_light_power[DIR_LIGHT_COUNT];

uniform float ambient_light_power;

void main() {
	float visiblity = exp(-pow(distance_to_camera * fog_amount, fog_gradient));
	visiblity = clamp(visiblity, 0.0f, 1.0f);

	float shading_factor;

	vec4 shaded_rgba;
	vec4 final_rgba;

	for (int i = 0; i < DIR_LIGHT_COUNT; i++) {
			shading_factor = dot(global_normal, d_light_direction[i]);
			shading_factor = clamp(shading_factor, ambient_light_power, 1.0f);
		shaded_rgba += mix(vec4(vec3(0.0f), 1.0f), (texture(tex0, _uv * tex0_scale)) * (vec4(d_light_color[i], 1.0f) * vec4(vec3(d_light_power[i]), 1.0f)), shading_factor);
	}

	final_rgba = mix(vec4(fog_color, 1.0f), shaded_rgba, visiblity);

	main_frag = final_rgba;
}