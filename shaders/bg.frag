#version 430

uniform vec2  u_resolution; // Screen Resolution (width, height)
uniform float u_time;       // Time in Milli Seconds
uniform float u_duration;

layout(location = 0) out vec4 fragColor;


float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898,78.233))) * 43758.5453123);
}

vec3 base_color = vec3(0.01, 0.01, 0.08);

vec3 sky(){
    vec2 uv = gl_FragCoord.xy / u_resolution;
    vec3 color = base_color;
    float t = u_time / 10;

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

void main() {
    vec3 col = sky();
    fragColor = vec4(col, 1.0);
    //fragColor = vec4(vec3(sin(u_time)), 1.0);
}