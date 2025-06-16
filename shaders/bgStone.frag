#version 420

in vec4 v_color;
in vec2 v_texCoord;
in vec3 v_fragPos;

uniform vec2  u_resolution; // Screen Resolution (width, height)
uniform float u_time;       // Time in Milli Seconds
uniform float u_duration;
uniform float u_total_time;

uniform sampler2D u_sampler2d;
uniform sampler2D u_sampler2d2;

layout(location = 0) out vec4 fragColor;

void main() {
    fragColor = mix(texture(u_sampler2d2, v_texCoord), texture(u_sampler2d, v_texCoord), min(u_time / .3, 1));

    if(fragColor.a == 0)
        discard;
}