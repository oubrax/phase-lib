#version 330 core
uniform mat4 u_projection;

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUv;

layout (location = 2) in vec4 iColor;
layout (location = 3) in vec2 iSize;
layout (location = 4) in vec2 iPos;

out vec2 vUv;
out vec4 vColor;
out vec2 vSize;

void main() {
    gl_Position = u_projection * vec4(iPos + aPos * iSize, 0.0, 1.0);
    vUv = aUv;
    vColor = iColor;
    vSize = iSize;
}
