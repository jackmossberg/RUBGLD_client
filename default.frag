#version 330 core

out vec4 main_frag;

in vec3 _position_global;
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

uniform vec3 p_light_position[POINT_LIGHT_COUNT];
uniform vec3 p_light_color[POINT_LIGHT_COUNT];
uniform float p_light_power[POINT_LIGHT_COUNT];
uniform float p_light_range[POINT_LIGHT_COUNT];

uniform float ambient_light_power;

vec4 white_world_rgba()	{
	float visiblity = exp(-pow(distance_to_camera * fog_amount, fog_gradient));
	visiblity = clamp(visiblity, 0.0f, 1.0f);

	float shading_factor;

	vec4 shaded_rgba;
	vec4 final_rgba;

	shaded_rgba += vec4(1.0f) * ambient_light_power;

	for (int i = 0; i < DIR_LIGHT_COUNT; i++) {
			shading_factor = dot(global_normal, d_light_direction[i]);
			shading_factor = clamp(shading_factor, 0.0, 1.0f);
		shaded_rgba += mix(vec4(vec3(0.0f), 1.0f), vec4(1.0f) * (vec4(d_light_color[i], 1.0f) * vec4(vec3(d_light_power[i]), 1.0f)), shading_factor);
	}

	float light_dist;
	vec4 diffuse_rgba;

	for (int i = 0; i < POINT_LIGHT_COUNT; i++) {
		light_dist = (abs(length(_position_global - p_light_position[i])) / p_light_range[i]);
		diffuse_rgba = vec4(p_light_color[i], 1.0f) / vec4(vec3(light_dist * light_dist), 1.0f);
		shaded_rgba += (vec4(1.0f)) * (diffuse_rgba * vec4(vec3(p_light_power[i] * 0.05f), 1.0f));
	}

	final_rgba = mix(vec4(fog_color, 1.0f), shaded_rgba, visiblity);
	return final_rgba;
}

void main() {
	float visiblity = exp(-pow(distance_to_camera * fog_amount, fog_gradient));
	visiblity = clamp(visiblity, 0.0f, 1.0f);

	float shading_factor;

	vec4 shaded_rgba;
	vec4 final_rgba;

	shaded_rgba += texture(tex0, _uv * tex0_scale) * ambient_light_power;

	for (int i = 0; i < DIR_LIGHT_COUNT; i++) {
			shading_factor = dot(global_normal, d_light_direction[i]);
			shading_factor = clamp(shading_factor, 0.0, 1.0f);
		shaded_rgba += mix(vec4(vec3(0.0f), 1.0f), (texture(tex0, _uv * tex0_scale)) * (vec4(d_light_color[i], 1.0f) * vec4(vec3(d_light_power[i]), 1.0f)), shading_factor);
	}

	float diffuse_factor;

	float light_dist;
	vec4 diffuse_rgba;

	for (int i = 0; i < POINT_LIGHT_COUNT; i++) {
			diffuse_factor = dot(global_normal, p_light_position[i] - _position_global);
			diffuse_factor = clamp(diffuse_factor, 0.0, 1.0f);
		light_dist = (abs(length(_position_global - p_light_position[i])) / p_light_range[i]);
		diffuse_rgba = vec4(p_light_color[i], 1.0f) / vec4(vec3(light_dist * light_dist), 1.0f);
		shaded_rgba += mix(vec4(vec3(0.0f), 1.0f), texture(tex0, _uv * tex0_scale) * (diffuse_rgba * vec4(vec3(p_light_power[i] * 0.05f), 1.0f)), diffuse_factor);
	}

	final_rgba = mix(vec4(fog_color, 1.0f), shaded_rgba, visiblity);

	main_frag = final_rgba;
	//main_frag = white_world_rgba();
}