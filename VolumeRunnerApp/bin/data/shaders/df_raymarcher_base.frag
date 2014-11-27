#version 120


#pragma mark SCENE

//------------------------------------------------------------------------------------
vec4 compute_color( in vec3 p, in float distance, in int mtl )
{
}

//------------------------------------------------------------------------------------
float compute_scene(in vec3 p, out int mtl) {
}


//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
#pragma mark UTILS
float saturate(in float v) {
    return clamp(v,0.0,1.0);
}

float expose(in float l, in float e) {
    return (1.5 - exp(-l*e));
}

const vec4 lumi = vec4(0.30, 0.59, 0.11, 0);
float luminosity(in vec4 clr) {
    return dot(clr, lumi);
}

vec4 normal_color(in vec3 n) {
    return vec4((n*vec3(0.5)+vec3(0.5)), 1);
}

float attenuation(in float distance, in float atten) {
    return min(1.0/(atten*distance*distance), 1.0);
}

// Smooth blend functions
// http://www.iquilezles.org/www/articles/smin/smin.htm
// exponential, k=32
float smin_exp(float a, float b, float k) {
    float res = exp(-k*a) + exp(-k*b);
    return -log(res)/k;
}

// polynomial, k=0.1 (Fastest)
float smin_poly(float a, float b, float k) {
    float h = clamp(0.5+0.5*(b-a)/k, 0.0, 1.0);
    return mix(b, a, h) - k*h*(1.0-h);
}

// power, k=8
float smin_power(float a, float b, float k) {
    a = pow(a, k); b = pow(b, k);
    return pow((a*b)/(a+b), 1.0/k);
}


//------------------------------------------------------------------------------------
#pragma mark DF PRIMITIVES
// p: sample position
// assumes object is at 0, 0, 0

float dot2(in vec3 v) { return dot(v,v); }

float sd_plane(vec3 p, vec4 n) {
    // n must be normalized
    return dot(p,n.xyz) + n.w;
}

float sd_plane_xz(in vec3 p, float y) {
    return p.y - y;
}

float sd_plane_xy(in vec3 p, float z) {
    return p.z - z;
}

float sd_plane_yz(in vec3 p, float x) {
    return p.x - x;
}

float sd_sphere(vec3 p, float r) {
    return length(p)-r;
}

float ud_box(vec3 p, vec3 b) {
    return length(max(abs(p)-b, 0.0));
}

float sd_box(vec3 p, vec3 b) {
    vec3 d = abs(p) - b;
    return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

float ud_round_box(vec3 p, vec3 b, float r) {
    return length(max(abs(p)-b,0.0))-r;
}

float sd_torus(vec3 p, vec2 t) {
    return length(vec2(length(p.xz)-t.x,p.y))-t.y;
}

float sd_cylinder(vec3 p, vec3 c) {
    return length(p.xz-c.xy)-c.z;
}

float sd_cone(vec3 p, vec2 c) {
    // c must be normalized
    float q = length(p.xy);
    return dot(c,vec2(q,p.z));
}

float sd_hex_prism(vec3 p, vec2 h) {
    vec3 q = abs(p);
#if 0
    return max(q.z-h.y,max((q.x*0.866025+q.y*0.5),q.y)-h.x);
#else
    float d1 = q.z-h.y;
    float d2 = max((q.x*0.866025+q.y*0.5),q.y)-h.x;
    return length(max(vec2(d1,d2),0.0)) + min(max(d1,d2), 0.);
#endif
}

float sd_tri_prism(vec3 p, vec2 h) {
    vec3 q = abs(p);
#if 0
    return max(q.z-h.y,max(q.x*0.866025+p.y*0.5,-p.y)-h.x*0.5);
#else
    float d1 = q.z-h.y;
    float d2 = max(q.x*0.866025+p.y*0.5,-p.y)-h.x*0.5;
    return length(max(vec2(d1,d2),0.0)) + min(max(d1,d2), 0.);
#endif
}

float sd_capsule(vec3 p, vec3 a, vec3 b, float r) {
    vec3 pa = p - a, ba = b - a;
    float h = clamp(dot(pa,ba)/dot(ba,ba), 0.0, 1.0);
    return length(pa - ba*h) - r;
}

float sd_cylinder(vec3 p, vec2 h) {
    vec2 d = abs(vec2(length(p.xz),p.y)) - h;
    return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float ud_triangle(vec3 p, vec3 a, vec3 b, vec3 c)
{
    vec3 ba = b - a; vec3 pa = p - a;
    vec3 cb = c - b; vec3 pb = p - b;
    vec3 ac = a - c; vec3 pc = p - c;
    vec3 nor = cross(ba, ac);
    
    return sqrt(
                (sign(dot(cross(ba,nor),pa)) +
                 sign(dot(cross(cb,nor),pb)) +
                 sign(dot(cross(ac,nor),pc))<2.0)
                ?
                min(min(
                        dot2(ba*clamp(dot(ba,pa)/dot2(ba),0.0,1.0)-pa),
                        dot2(cb*clamp(dot(cb,pb)/dot2(cb),0.0,1.0)-pb)),
                    dot2(ac*clamp(dot(ac,pc)/dot2(ac),0.0,1.0)-pc))
                :
                dot(nor,pa)*dot(nor,pa)/dot2(nor));
}

float ud_quad(vec3 p, vec3 a, vec3 b, vec3 c, vec3 d)
{
    vec3 ba = b - a; vec3 pa = p - a;
    vec3 cb = c - b; vec3 pb = p - b;
    vec3 dc = d - c; vec3 pc = p - c;
    vec3 ad = a - d; vec3 pd = p - d;
    vec3 nor = cross(ba, ad);
    
    return sqrt(
                (sign(dot(cross(ba,nor),pa)) +
                 sign(dot(cross(cb,nor),pb)) +
                 sign(dot(cross(dc,nor),pc)) +
                 sign(dot(cross(ad,nor),pd))<3.0)
                ?
                min(min(min(
                            dot2(ba*clamp(dot(ba,pa)/dot2(ba),0.0,1.0)-pa),
                            dot2(cb*clamp(dot(cb,pb)/dot2(cb),0.0,1.0)-pb)),
                        dot2(dc*clamp(dot(dc,pc)/dot2(dc),0.0,1.0)-pc)),
                    dot2(ad*clamp(dot(ad,pd)/dot2(ad),0.0,1.0)-pd))
                :
                dot(nor,pa)*dot(nor,pa)/dot2(nor));
}

float sd_cone(in vec3 p, in vec3 c) {
    vec2 q = vec2( length(p.xz), p.y );
#if 0
    return max( max( dot(q,c.xy), p.y), -p.y-c.z );
#else
    float d1 = -p.y-c.z;
    float d2 = max( dot(q,c.xy), p.y);
    return length(max(vec2(d1,d2),0.0)) + min(max(d1,d2), 0.);
#endif
}



//------------------------------------------------------------------------------------
#pragma mark DF OPERATORS

float df_union(in float d1, in float d2) {
    return min(d1, d2);
}

float df_subtract(in float d1, in float d2) {
    return max(-d1, d2);
}

float df_intersect(in float d1, in float d2)
{
    return max(d1, d2);
}

vec3 df_repeat(in vec3 p, in vec3 rep) {
    return mod(p, rep) - 0.5*rep;
}

vec3 df_translate(in vec3 p, in vec3 offset) {
    return p-offset;
}

vec3 df_rotate_y(in vec3 p, float theta) {
    float c = cos(theta);
    float s = sin(theta);
    vec3 res;
    res.x = p.x * c - p.z * s;
    res.y = p.y;
    res.z = p.x * s + p.z * c;
    return res;
}

vec3 df_rotate_x(in vec3 p, float theta) {
    float c = cos(theta);
    float s = sin(theta);
    vec3 res;
    res.x = p.x;
    res.y = p.y * c - p.z * s;
    res.z = p.y * s + p.z * c;
    return res;
}

vec3 df_rotate_z(in vec3 p, float theta) {
    float c = cos(theta);
    float s = sin(theta);
    vec3 res;
    res.x = p.x * c - p.y * s;
    res.y = p.x * s + p.y * c;
    res.z = p.z;
    return res;
}

vec3 df_scale(in vec3 p, in vec3 scale) {
    return p / scale;
}

vec3 df_transform(in vec3 p, in mat4 mat) {
    return (invert(mat)*vec4(p,1.0)).xyz;
}

/// We pass in the inverse transformation here for speed
vec3 df_transform_inv(in vec3 p, in mat4 inv_mat) {
    return (inv_mat*vec4(p,1.0)).xyz;
}

float df_blend_exp(in float d1, in float d2, in float k) {
    return smin_exp(d1, d2, k);
}

float df_blend_poly(in float d1, in float d2, in float k) {
    return smin_poly(d1, d2, k);
}

float df_blend_power(in float d1, in float d2, in float k) {
    return smin_power(d1, d2, k);
}

/*
 float df_blend(vec3 p, float a, float b)
 {
 float s = smoothstep(length(p), 0.0, 1.0);
 float d = mix(a, b, s);
 return d;
 }
 */

//------------------------------------------------------------------------------------
#pragma mark LIGHTING

//---------------------------------------------------
// from iq. https://www.shadertoy.com/view/Xds3zN
vec3 calc_normal (in vec3 p) {
    vec3 delta = vec3( 0.004, 0.0, 0.0 );
    int mtl;
    vec3 n;
    n.x = compute_scene( p+delta.xyz, mtl ) - compute_scene( p-delta.xyz, mtl );
    n.y = compute_scene( p+delta.yxz, mtl ) - compute_scene( p-delta.yxz, mtl );
    n.z = compute_scene( p+delta.yzx, mtl ) - compute_scene( p-delta.yzx, mtl );
    return normalize( n );
}

float shadow_hard(in vec3 ro, in vec3 rd, float mint, float maxt) {
    for(float t=mint; t < maxt;) {
        float h = map(ro + rd*t);
        if(h<0.001) return 0.0;
        t += h;
    }
    return 1.0;
}

float shadow_soft(in vec3 ro, in vec3 rd, float mint, float maxt, float k) {
    float res = 1.0;
    for(float t=mint; t < maxt;) {
        float h = map(ro + rd*t);
        if(h<0.001) return 0.0;
        res = min(res, k*h/t);
        t += h;
    }
    return res;
}

// from iq. https://www.shadertoy.com/view/Xds3zN
float ambient_occlusion(in vec3 pos, in vec3 nor)
{
    float occ = 0.0;
    float sca = 1.0;
    int mtl;
    for( int i=0; i<5; i++) {
        float hr = 0.01 + 0.12*float(i)/4.0;
        vec3 aopos =  nor * hr + pos;
        float dd = compute_scene( aopos, mtl );
        occ += -(dd-hr)*sca;
        sca *= 0.95;
    }
    return clamp( 1.0 - 3.0*occ, 0.0, 1.0 );
}


//------------------------------------------------------------------------------------
#pragma mark RAY MARCHER

// Ray marcher
vec4 ray_march(in vec3 p, in vec3 w, in vec4 bg_clr, inout float distance)
{
    const int maxIterations = 128;
    const float closeEnough = EPSILON; //1e-2;
    vec3 rp;
    int mtl;
    float t = 0;
    for (int i = 0; i < maxIterations; ++i) {
        rp = p + w * t;
        float d = compute_scene(rp,mtl);
        t += d;
        if (d < closeEnough)
        {
            distance = t;
            // use this to debug number of ray casts
            //return vec3(float(i)/128.0);
            return compute_color(rp,t,mtl);//+vec3(float(i)/128.0);
        }
        else if(t > distance)
        {
            return bg_clr;//vec3(0.0);
        }
        
        
    }
    
    return bg_clr;//vec3(0.0); // return skybox here
}

//------------------------------------------------------------------------------------
#pragma mark MAIN
void main(void)
{
    vec2 xy = gl_FragCoord.xy;
    // Primary ray origin
    vec3 p = invViewMatrix[3].xyz;
    // Primary ray direction
    vec3 w = mat3(invViewMatrix) * normalize(
                                             vec3( (xy - resolution / 2.0)*vec2(1.0,1.0), resolution.y/(-2.0*tanHalfFov))
                                             );
    
    float distance = 1e10;
    
    vec4 clr = ray_march(p, w, fog_clr, distance);
    
    clr.xyz = pow( clr.xyz, vec3(1.0/2.2)); // gamma correction.
    //clr.w  = 1.0;
    gl_FragColor = clr;
}

