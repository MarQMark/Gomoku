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

// sky() is from: https://www.shadertoy.com/view/fsjXDh
float hash21(vec2 p)
{
    p=fract(p*vec2(123.456,789.01));
    p+=dot(p,p+45.67);
    return fract(p.x*p.y);
}
float star(vec2 uv,float brightness)
{
    float color=0.0;
    float star=length(uv);
    float diffraction=abs(uv.x*uv.y);
    //diffraction *= abs((uv.x + 0.001953125) * (uv.y + 0.001953125));
    star=brightness/star;
    diffraction=pow(brightness,2.0)/diffraction;
    diffraction=min(star,diffraction);
    diffraction*=sqrt(star);
    color+=star*sqrt(brightness)*8.0;
    color+=diffraction*8.0;
    return color;
}
vec3 sky(){
    vec2 UV=gl_FragCoord.xy/u_resolution.yy;
    vec3 color=vec3(0.0);
    float dist=1.0;
    float brightness=.02;
    vec2 uv=(floor(UV*256.)/256.)-.51019;
    uv*=128.;
    uv+=floor((u_total_time / 1000)*64.)/3072.0;
    vec2 gv=fract(uv)-.5;
    vec2 id;
    float displacement;
    for(float y=-dist;y<=dist;y++)
    {
        for(float x=-dist;x<=dist;x++)
        {
            id=floor(uv);
            displacement=hash21(id+vec2(x,y));
            //color+=vec3(star(gv-vec2(x,y)-vec2(displacement,fract(displacement*16.))+.5,(hash21(id+vec2(x,y))/128.)));
            //color=min(color,.4);
        }
    }
    uv/=2.;
    gv=fract(uv)-.5;
    for(float y=-dist;y<=dist;y++)
    {
        for(float x=-dist;x<=dist;x++)
        {
            id=floor(uv);
            displacement=hash21(id+vec2(x,y));
            color+=vec3(star(gv-vec2(x,y)-vec2(displacement,fract(displacement*16.))+.5,(hash21(id+vec2(x,y))/128.)));
        }
    }
    uv/=8.;
    gv=fract(uv)-.5;
    for(float y=-dist;y<=dist;y++)
    {
        for(float x=-dist;x<=dist;x++)
        {
            id=floor(uv);
            displacement=hash21(id+vec2(x,y));
            color+=vec3(star(gv-vec2(x,y)-vec2(displacement,fract(displacement*16.))+.5,(hash21(id+vec2(x,y))/256.)));
        }
    }
    uv/=6.;
    gv=fract(uv)-.5;
    for(float y=-dist;y<=dist;y++)
    {
        for(float x=-dist;x<=dist;x++)
        {
            id=floor(uv);
            displacement=hash21(id+vec2(x,y));
            color+=vec3(star(gv-vec2(x,y)-vec2(displacement,fract(displacement*16.))+.5,(hash21(id+vec2(x,y))/256.)));
        }
    }
    color*=vec3(.5,.7,1.);
    return color;
}

vec3 stone(vec2 uv, float t, float speed, float tile_count, float threshold, float scale) {
    uv.x += t * speed;

    vec2 tileID = floor(uv * tile_count);
    vec2 local_uv = fract(uv * tile_count);

    // randomly skip most tiles
    float r = rand(tileID);
    if (r > threshold) return vec3(0.0);

    vec2 centered_uv = (local_uv - 0.5) / scale + 0.5;
    centered_uv.y = 1 - centered_uv.y;

    // discard out of bounds samples
    if (any(lessThan(centered_uv, vec2(0.0))) || any(greaterThan(centered_uv, vec2(1.0)))) {
        return vec3(0.0);
    }

    return texture2D(u_sampler2d, centered_uv).rgb;
}

vec3 base_color = vec3(0.01, 0.01, 0.08);

vec3 sky2(){
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

    const int numBalls = 100;
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
            vec4 tex = mix(texture2D(u_sampler2d2, texUV), texture2D(u_sampler2d, texUV), min(u_time / .3, 1));
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

    vec3 stone_a = stone(uv, t, 0.01, 20.0, 0.02, 1.5);
    vec3 stone_b = stone(uv + vec2(0.2), t, 0.02, 25.0, 0.04, 1.2);
    vec3 stone_c = stone(uv + vec2(-0.1, 0.1), t, 0.04, 30.0, 0.03, 1.0);

    //col = mix(col, stone_c, stone_c.x != 0);
    //col = mix(col, stone_b, stone_b.x != 0);
    //col = mix(col, stone_a, stone_a.x != 0);
    col = ball();

    col = mix(col, sky2(), col == base_color);

    fragColor = vec4(col, 1.0);
}