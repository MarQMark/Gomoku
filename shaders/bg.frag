#version 430

uniform float u_time;
uniform vec2 u_resolution;

layout(location = 0) out vec4 fragColor;

// Pixelation function
vec2 pixelate(vec2 uv, float pixelSize) {
    return floor(uv * pixelSize) / pixelSize;
}

// Random function for stars
float rand(vec2 co) {
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

// Stars layer
float starLayer(vec2 uv, float t, float speed, float density) {
    uv.x += t * speed;
    uv = pixelate(uv, 120.0);  // pixelated star grid
    float r = rand(uv * density);
    return step(0.995, r);
}

// Wavy cloud layer
// Generate a cloud layer with different properties
float cloudLayer(vec2 uv, float t, float speed, float freq, float amp, float pixelSize) {
    uv.x += t * speed;
    uv.y += sin(uv.x * freq + t) * amp;
    uv = pixelate(uv, pixelSize);
    return smoothstep(0.4, 0.5, sin(uv.y * 10.0 + t));
}

void main() {
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    uv.y = 1.0 - uv.y;

    float t = u_time / 2;
    //t /= 1000.;

    // Background color
    vec3 col = vec3(0.1, 0.1, 0.2);

    // Stars
    float stars1 = starLayer(uv, t, 0.02, 20.0);
    float stars2 = starLayer(uv, t, 0.04, 40.0);
    col += vec3(1.0) * stars1 * 0.5;
    col += vec3(1.0) * stars2 * 0.5;

    // Cloud layers with varying speeds/frequencies/pixelation
    float c1 = cloudLayer(uv, t, 0.01, 4.0, 0.03, 90.0);
    float c2 = cloudLayer(uv + vec2(0.2, 0.1), t, 0.02, 6.0, 0.04, 70.0);
    float c3 = cloudLayer(uv + vec2(-0.1, 0.15), t, 0.035, 8.0, 0.05, 50.0);

    // Blend clouds with depth feeling
    col = mix(col, vec3(0.8, 0.8, 0.9), c1 * 0.2); // back layer (light)
    col = mix(col, vec3(0.7, 0.7, 0.85), c2 * 0.3); // mid layer
    col = mix(col, vec3(0.6, 0.6, 0.8), c3 * 0.4); // front layer (dark, faster)

    fragColor = vec4(col, 1.0);
}
