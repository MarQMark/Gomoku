#version 430

uniform vec2  u_resolution; // Screen Resolution (width, height)
uniform float u_time;       // Time in Milli Seconds
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

vec3 sky(){
    vec2 uv = gl_FragCoord.xy / u_resolution;
    vec3 color = base_color;
    float t = u_total_time / 10;

    float gridSize = 8.0;
    vec2 gridUV = floor(gl_FragCoord.xy / gridSize);
    float r = rand(gridUV);

    if (r > 0.995) {
        vec2 localUV = fract(gl_FragCoord.xy / gridSize) - 0.5;

        float dist = length(localUV);
        float radius = 0.3;
        float alpha = smoothstep(radius, radius - 0.15, dist);

        float flicker = 0.5 + 0.5 * sin(t * 5.0 + r * 100.0 * uv.x);
        vec3 dotColor = vec3(1.0) * flicker;

        color = mix(color, dotColor, alpha);
    }
    return color;
}

vec3 ball(){
    vec2 uv = gl_FragCoord.xy / u_resolution;
    uv = (uv - 0.5) * vec2(u_resolution.x / u_resolution.y, 1.0) + 0.5;
    vec2 center = vec2(0.5, 0.5);
    float time = u_total_time;

    vec3 color = base_color;

    const int numBalls = 250;
    float maxAge = 15.0;

    for (int i = 0; i < numBalls; i++) {
        float fi = float(i);
        vec2 seed = hash2(fi);

        float launchTime = seed.x * maxAge;
        float age = mod(time - launchTime, maxAge);

        float angle = seed.y * 6.2831;
        float speed = mix(0.6, 1.3, rand(seed));
        float dist = (age / maxAge) * speed;
        vec2 dir = vec2(cos(angle), sin(angle));
        vec2 pos = center + dir * dist * 1.5;

        float size = mix(0.001, 0.12, dist); // scale: min, max

        vec2 texUV = (uv - pos) / size + 0.5;
        texUV.y = 1 - texUV.y;

        if (all(greaterThanEqual(texUV, vec2(0.0))) && all(lessThanEqual(texUV, vec2(1.0)))) {
            vec4 tex = mix(texture2D(u_sampler2d2, texUV), texture2D(u_sampler2d, texUV), mod(i, 2));
            color = mix(color, tex.rgb, tex.a);
        }
    }
    return color;
}

void main() {
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    uv = (uv - 0.5) * vec2(u_resolution.x / u_resolution.y, 1.0) + 0.5;
    float t = u_total_time / 2;

    vec3 col = base_color;
    col = ball();

    col = mix(col, sky(), col == base_color);

    fragColor = vec4(col, 1.0);
}