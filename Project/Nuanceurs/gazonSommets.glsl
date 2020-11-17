#version 430 core

layout(location = 0) in vec3 vp;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 N;
uniform mat4 M;

out vec4 color;
out vec3 vPosition;
out vec3 wordPos;

void main () {
    gl_Position = (vec4(vp,1.0));
    vPosition = vp;
}