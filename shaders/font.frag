#version 420 core

in vec4 v_color;
in vec2 v_texCoord;
in vec3 v_fragPos;

uniform sampler2D u_sampler2d;

layout(location = 0) out vec4 fragColor;

void main() {
    vec4 color = v_color;

    vec4 fontColor = texture(u_sampler2d, v_texCoord);
    if(fontColor.w == 0)
       discard;

    fragColor = vec4(color.xyz, fontColor.w);
}