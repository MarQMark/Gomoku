#version 430 core

in vec4 v_color;
in vec2 v_texCoord;
in vec3 v_fragPos;

uniform sampler2D u_sampler2d;

layout(location = 0) out vec4 fragColor;

void main() {
   if(v_color.a == 0)
       fragColor = texture(u_sampler2d, v_texCoord);
   else
       fragColor = v_color;
}