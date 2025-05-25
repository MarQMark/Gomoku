#version 430 core

in vec4 v_color;
in vec2 v_texCoords;

layout(location = 0) out vec4 fragColor;

uniform sampler2D u_sampler2d;

void main() {
    vec4 color = v_color;

    vec4 fontColor = texture(u_sampler2d, v_texCoords);
    if(fontColor.w == 0)
       discard;

    fragColor = vec4(color.xyz, fontColor.w);
}