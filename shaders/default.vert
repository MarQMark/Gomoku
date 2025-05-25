#version 430

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;

//uniform mat4 u_mvp;

out vec4 v_color;
out vec2 v_texCoord;
out vec3 v_fragPos;

void main() {
    v_color = color;
    v_texCoord = texCoord;
    v_fragPos = vec3(position);
    //gl_Position = u_mvp * position;
    vec4 pos = position;
    //pos.xy = pos.xy * 2;
    //pos.x-=1;
    //pos.y-=1;
    //pos.y *= -1;
    gl_Position = pos;
}