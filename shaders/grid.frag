#version 430 core

in vec4 v_color;
in vec2 v_texCoord;
in vec3 v_fragPos;

uniform sampler2D u_sampler2d;

layout(location = 0) out vec4 fragColor;

void main() {
    vec2 uv = gl_FragCoord.xy / vec2(1280, 720);
    uv.y *= 1280 / 720;

    uv *= 15;
    uv = fract(uv);

    vec3 color = vec3(0);
    color = mix(color, vec3(1), step(.3, uv.x) * step(.1, uv.y) * step(uv.x, .7) * step(uv.y, .8));
    //color = vec3(uv, 1);
    fragColor = vec4(color, 1);
}