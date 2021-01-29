#version 460 core

precision highp float;

layout (location = 0) in vec3 aPos;

out vec3 pos;

void main() {
    pos = aPos;
    //vec2 relative = normalize(aPos.xy - botleft.xy);

    gl_Position = vec4(aPos, 1);// vec4((relative * 2 * sqrt(2)) - vec2(1, 1), 0, 1);
}
