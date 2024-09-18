#version 330 core

out vec4 main_frag;

in vec2 _uv;
in vec3 _position;

uniform sampler2D tex0;
uniform sampler2D tex1;

uniform int width;
uniform int height;
uniform float render_scale;

uniform float bloom_amount;
uniform float brightness;

const int radius = 6;

float spreadBlur = 2.0f;
float weights[radius];

vec4 blur_texture(sampler2D input, vec2 _uv, int iterations) {
    bool horizontal = true;
    float x = 0.0f;

    for (int i = 0; i < radius; i++) {
        if (spreadBlur <= 2.0f)
            x += 3.0f / radius;
        else
            x += 6.0f / radius;
        weights[i] = exp(-0.5f * pow(x / spreadBlur, 2.0f)) / (spreadBlur * sqrt(2 * 3.14159265f));
    }

    vec2 tex_offset = 1.0f / textureSize(input, 0);
    vec3 result = texture(input, _uv).rgb * weights[0];
    
    for (int z = 0; z < iterations; z++) {
        if(horizontal) {
            for(int i = 1; i < radius; i++) {
                result += texture(input, _uv + vec2(tex_offset.x * (i * z), 0.0)).rgb * weights[i];
                result += texture(input, _uv - vec2(tex_offset.x * (i * z), 0.0)).rgb * weights[i];
            }
            horizontal = false;
        } else {
            for(int i = 1; i < radius; i++) {
                result += texture(input, _uv + vec2(0.0, tex_offset.y * (i * z))).rgb * weights[i];
                result += texture(input, _uv - vec2(0.0, tex_offset.y * (i * z))).rgb * weights[i];
            }
            horizontal = true;
        }
    }
  
    return vec4(result, 1.0f) / vec4(vec3(iterations), 1.0f);
}

vec4 get_highlights(vec4 input, float threshold) {
	float brightness = dot(input.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > threshold)
        return vec4(input.rgb, 1.0f);
    else
        return vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void main() {
        float dx = (width/100) * (1.0 / render_scale);
        float dy = (height/100) * (1.0 / render_scale);
        vec2 _uv_f = vec2(dx * floor(_uv.x / dx),
                          dy * floor(_uv.y / dy));

    vec4 blurred_color = blur_texture(tex1, _uv_f, 18) * bloom_amount;

    vec4 final_color = ((texture(tex0, _uv_f) + blurred_color) / 2) * brightness;

    main_frag = final_color;
    //main_frag = vec4(_position, 1.0f);
}