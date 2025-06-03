#version 430

uniform vec2  u_resolution;
uniform float u_time;
uniform float u_duration;
uniform float u_total_time;

uniform sampler2D u_sampler2d;
uniform sampler2D u_sampler2d2;

layout(location = 0) out vec4 fragColor;

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898,78.233))) * 43758.5453123);
}
vec2 hash2(float i) {
    return fract(sin(vec2(i, i + 1.0) * vec2(12.9898, 78.233)) * 43758.5453);
}

vec3 base_color = vec3(0.01, 0.01, 0.08);

vec3 sky(vec2 fragCoord){
    vec2 uv = fragCoord / u_resolution;
    vec3 color = base_color;
    float t = u_total_time / 10.0;

    float gridSize = 8.0;
    vec2 gridUV = floor(fragCoord / gridSize);
    float r = rand(gridUV);

    if (r > 0.995) {
        vec2 localUV = fract(fragCoord / gridSize) - 0.5;
        float dist = dot(localUV, localUV); // cheaper than length
        float radius = 0.3;
        float alpha = smoothstep(radius*radius, (radius - 0.15)*(radius - 0.15), dist);

        float flicker = 0.5 + 0.5 * sin(t * 5.0 + r * 100.0 * uv.x);
        vec3 dotColor = vec3(1.0) * flicker;

        color = mix(color, dotColor, alpha);
    }
    return color;
}

vec3 ball(vec2 fragCoord){
    vec2 uv = fragCoord / u_resolution;
    uv = (uv - 0.5) * vec2(u_resolution.x / u_resolution.y, 1.0) + 0.5;
    vec2 center = vec2(0.5, 0.5);
    float time = u_total_time;

    vec3 color = base_color;

    const int numBalls = 80;
    float maxAge = 15.0;

    for (int i = 0; i < numBalls; i++) {
        float fi = float(i);
        vec2 seed = hash2(fi);
        float launchTime = seed.x * maxAge;
        float age = mod(time - launchTime, maxAge);
        float angle = seed.y * 6.2831;
        float speed = mix(0.6, 1.3, fract(sin(fi * 91.123) * 43758.5453)); // cheaper rand

        float dist = (age / maxAge) * speed;
        vec2 dir = vec2(cos(angle), sin(angle));
        vec2 pos = center + dir * dist * 1.5;

        float size = mix(0.001, 0.12, dist);
        vec2 texUV = (uv - pos) / size + 0.5;

        // Skip if outside range
        if (any(lessThan(texUV, vec2(0.0))) || any(greaterThan(texUV, vec2(1.0)))) continue;

        texUV.y = 1.0 - texUV.y;

        // Sample once with precomputed blend
        float blend = clamp(u_time / 0.3, 0.0, 1.0);
        vec4 tex1 = texture(u_sampler2d, texUV);
        vec4 tex2 = texture(u_sampler2d2, texUV);
        vec4 tex = mix(tex2, tex1, blend);

        color = mix(color, tex.rgb, tex.a);
    }

    return color;
}

void main() {
    vec2 fragCoord = gl_FragCoord.xy;

    vec3 col = ball(fragCoord);
    if (col == base_color) {
        col = sky(fragCoord);
    }

    fragColor = vec4(col, 1.0);
}
