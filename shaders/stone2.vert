#version 430

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec2 normal;

out vec4 v_color;
out vec2 v_texCoord;
out vec3 v_fragPos;

uniform float u_time;
uniform float u_duration;

float easeBackInOut(float t, float s) {
    float sMult = s * 1.525f;
    t *= 2.0f;
    if (t < 1.0f) {
        return 0.5f * (t * t * ((sMult + 1.0f) * t - sMult));
    } else {
        t -= 2.0f;
        return 0.5f * (t * t * ((sMult + 1.0f) * t + sMult) + 2.0f);
    }
}

void main() {
    v_color = color;
    v_texCoord = texCoord;
    v_fragPos = vec3(position);

    vec4 pos = position;
    pos.xy = pos.xy * 2;
    pos.x-=1;
    pos.y-=1;
    pos.y *= -1;

    float ratio = (2.f/3.f);
    float time = min(u_time / (u_duration * ratio), 1);
    pos.x = normal.x * .2 * (1 - time) + pos.x * time;
    pos.y = (1 - normal.y * .2) * (1 - time) + pos.y * time;

    time = step(ratio, u_time / (u_duration)) * (u_time - u_duration * ratio) / (u_duration * (1 - ratio));
    pos.x += normal.x * easeBackInOut(1 - time, 1.70158f) * 0.01;//* (9.f/16.f);
    pos.y -= normal.y * easeBackInOut(1 - time, 1.70158f) * 0.01;

    gl_Position = pos;
}