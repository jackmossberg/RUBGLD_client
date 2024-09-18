#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec2 _uv;
out vec3 _position;

void main() {
    gl_Position = vec4(position, 1.0); 
    _position = position;
    _uv = uv;
}  