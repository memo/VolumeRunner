#version 120

//------------------------------------------------------------------------------------
// A lot of the functions adapted from iq.
// http://www.iquilezles.org/
// https://www.shadertoy.com/user/iq


#define kNumJoints   9
#define kNumMagma   5

uniform vec2 resolution; // screen resolution
uniform float time; // current time
uniform vec2 mouse; // mouse position (screen space)

// uniform vec3 box_pos, box_rot, box_scale;  // for testing individual transforms
// uniform mat4 box_mat;    // for testing whole transform
uniform mat4 box_mats[kNumJoints];

uniform vec3 centerPos;
uniform mat4 invViewMatrix;
uniform mat4 steerMatrix;

uniform float tanHalfFov; // tan(fov/2)

uniform float blend_k;

uniform sampler2D floor_image0;
uniform float floor_height0;
uniform float floor_scale0;
uniform float floor_offset0;
uniform float floor_center0;

uniform sampler2D floor_image1;
uniform float floor_height1;
uniform float floor_scale1;
uniform float floor_offset1;
uniform float floor_center1;

uniform sampler2D color_image;

uniform vec4 magma_pos[kNumMagma];  // xyz: position, w: active (!=0) or not (0)
uniform float magma_size;


const float EPSILON = 0.01;
const float PI = 3.1415926535;
const float PI2 = PI*2.0;

const vec3 light1  = normalize(vec3(0.7,1.0,0.9));

#define mtl_t float

// Modify these functions
float compute_scene( in vec3 p, out mtl_t mtl );
vec4 compute_color( in vec3 p, in float distance, in mtl_t mtl, in float normItCount );




//------------------------------------------------------------------------------------
#pragma mark NOISE
// SIMPLE NOISE
// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

// Simplex Noise (http://en.wikipedia.org/wiki/Simplex_noise), a type of gradient noise
// that uses N+1 vertices for random gradient interpolation instead of 2^N as in regular
// latice based Gradient Noise.

vec2 hash( vec2 p )
{
    p = vec2( dot(p,vec2(127.1,311.7)),
             dot(p,vec2(269.5,183.3)) );
    
    return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

float noise( in vec2 p )
{
    const float K1 = 0.366025404; // (sqrt(3)-1)/2;
    const float K2 = 0.211324865; // (3-sqrt(3))/6;
    
    vec2 i = floor( p + (p.x+p.y)*K1 );
    
    vec2 a = p - i + (i.x+i.y)*K2;
    vec2 o = (a.x>a.y) ? vec2(1.0,0.0) : vec2(0.0,1.0); //vec2 of = 0.5 + 0.5*vec2(sign(a.x-a.y), sign(a.y-a.x));
    vec2 b = a - o + K2;
    vec2 c = a - 1.0 + 2.0*K2;
    
    vec3 h = max( 0.5-vec3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );
    
    vec3 n = h*h*h*h*vec3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));
    
    return dot( n, vec3(70.0) );
}



//------------------------------------------------------------------------------------
#pragma mark UTILS
float saturate( in float v )
{
    return clamp(v,0.0,1.0);
}

float expose( in float l, in float e )
{
    return (1.5 - exp(-l*e));
}

const vec4 lumi = vec4(0.30, 0.59, 0.11, 0);

float luminosity( in vec4 clr )
{
    return dot(clr, lumi);
}

vec4  normal_color( in vec3 n )
{
    return vec4((n*vec3(0.5)+vec3(0.5)), 1);
}

float attenuation( in float distance, in float atten )
{
    return min( 1.0/(atten*distance*distance), 1.0 );
}

//// Smooth blend functions
////  http://www.iquilezles.org/www/articles/smin/smin.htm
float smin_exp( float a, float b, float k )
{
    float res = exp( -k*a ) + exp( -k*b );
    return -log( res )/k;
}

float smin_poly( float a, float b, float k )
{
    float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
    return mix( b, a, h ) - k*h*(1.0-h);
}


// power smooth min (k = 8);
float smin_power( float a, float b, float k )
{
    a = pow( a, k ); b = pow( b, k );
    return pow( (a*b)/(a+b), 1.0/k );
}


//------------------------------------------------------------------------------------
#pragma mark SDF PRIMITIVES
// SDF Objects
// p: sample position
// assumes object is at 0, 0, 0

//f(x,z) = sin(x)·sin(z)
//color = pow( color, vec3(1.0/2.2) );
float sdf_xz_plane(in vec3 p, float y)
{
    return p.y - y;//+ sin(p.x*1.0)*sin(p.z*1.0)*0.9 - y; // + sin(p.x*3.0)*sin(p.z*2.0)*0.3
}

float sdf_box(in vec3 p, in vec3 size)
{
    vec3 d = abs(p) - size;
    return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

float sdf_round_box(in vec3 p, in vec3 size, float smoothness )
{
    return length(max(abs(p)-size*0.5,0.0))-smoothness;
}

float sdf_sphere(in vec3 p, in float radius)
{
    return length(p)-radius;
}


float sdf_torus(in vec3 p, in float radius, in float thickness )
{
    vec2 q = vec2(length(p.xz)-radius,p.y);
    return length(q)-thickness;
}

float sdf_prism( in vec3 p, in vec2 h )
{
    vec3 q = abs(p);
    return max(q.z-h.y,max(q.x*0.866025+p.y*0.5,-p.y)-h.x*0.5);
}


float sdf_torus( in vec3 p, in vec2 t )
{
    return length( vec2(length(p.xz)-t.x,p.y) )-t.y;
}

float sdf_hex_prism( in vec3 p, in vec2 h )
{
    vec3 q = abs(p);
#if 1
    return max(q.z-h.y,max((q.x*0.866025+q.y*0.5),q.y)-h.x);
#else
    float d1 = q.z-h.y;
    float d2 = max((q.x*0.866025+q.y*0.5),q.y)-h.x;
    return length(max(vec2(d1,d2),0.0)) + min(max(d1,d2), 0.);
#endif
}

float sdf_capsule( in vec3 p, in vec3 a, in vec3 b, in float r )
{
    vec3 pa = p-a, ba = b-a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h ) - r;
}

float sdf_cylinder( in vec3 p, in vec2 h )
{
    vec2 d = abs(vec2(length(p.xz),p.y)) - h;
    return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float sdf_cone( in vec3 p, in vec3 c )
{
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
#pragma mark SDF OPERATORS

float sdf_union(in float d1, in float d2)
{
    return min(d1, d2);
}

float sdf_subtract(in float d1, in float d2)
{
    return max(-d2, d1);
}

float sdf_intersect(in float d1, in float d2)
{
    return max(d1, d2);
}

float sdf_blend_exp( in float d1, in float d2, in float k )
{
    return smin_exp(d1, d2, k);
}

float sdf_blend_poly( in float d1, in float d2, in float k )
{
    return smin_poly(d1, d2, k);
}

float sdf_blend_power( in float d1, in float d2, in float k )
{
    return smin_power(d1, d2, k);
}

/*
 float sdf_blend(vec3 p, float a, float b)
 {
 float s = smoothstep(length(p), 0.0, 1.0);
 float d = mix(a, b, s);
 return d;
 }
 */

vec3 sdf_repeat( in vec3 p, in vec3 rep)
{
    vec3 d = mod(p, rep) - 0.5*rep;
    return d;
}

vec3 sdf_translate( in vec3 p, in vec3 offset )
{
    return p-offset;
}


vec3 sdf_rotate_y(in vec3 p, float theta)
{
    float c = cos(theta);
    float s = sin(theta);
    vec3 res;
    res.x = p.x * c - p.z * s;
    res.y = p.y;
    res.z = p.x * s + p.z * c;
    return res;
}

vec3 sdf_rotate_x(in vec3 p, float theta)
{
    float c = cos(theta);
    float s = sin(theta);
    vec3 res;
    res.x = p.x;
    res.y = p.y * c - p.z * s;
    res.z = p.y * s + p.z * c;
    return res;
}

vec3 sdf_rotate_z(in vec3 p, float theta)
{
    float c = cos(theta);
    float s = sin(theta);
    vec3 res;
    res.x = p.x * c - p.y * s;
    res.y = p.x * s + p.y * c;
    res.z = p.z;
    return res;
}

/// We actually pass in the inverse transformation here because it would be slow to do it in the shader.
vec3 sdf_transform(in vec3 p, in mat4 inv_mat)
{
    return (inv_mat*vec4(p,1.0)).xyz;
}

vec3 sdf_scale(in vec3 p, in vec3 scale) {
    return p / scale;
}



//------------------------------------------------------------------------------------
#pragma mark LIGHTING

//---------------------------------------------------
// from iq. https://www.shadertoy.com/view/Xds3zN
vec3 calc_normal ( in vec3 p )
{
    vec3 delta = vec3( 0.004, 0.0, 0.0 );
    mtl_t mtl;
    vec3 n;
    n.x = compute_scene( p+delta.xyz, mtl ) - compute_scene( p-delta.xyz, mtl );
    n.y = compute_scene( p+delta.yxz, mtl ) - compute_scene( p-delta.yxz, mtl );
    n.z = compute_scene( p+delta.yzx, mtl ) - compute_scene( p-delta.yzx, mtl );
    return normalize( n );
}

//---------------------------------------------------
#define ambient_occlusion ambient_occlusion1

// from iq. https://www.shadertoy.com/view/Xds3zN
float ambient_occlusion3( in vec3 pos, in vec3 nor )
{
    float occ = 0.0;
    float sca = 1.0;
    mtl_t mtl;
    for( int i=0; i<5; i++ )
    {
        float hr = 0.01 + 0.12*float(i)/4.0;
        vec3 aopos =  nor * hr + pos;
        float dd = compute_scene( aopos, mtl );
        occ += -(dd-hr)*sca;
        sca *= 0.95;
    }
    return clamp( 1.0 - 3.0*occ, 0.0, 1.0 );
}


//---------------------------------------------------
float ambient_occlusion2( in vec3 p, vec3 n ) //, float stepDistance, float samples)
{
    const float stepDistance = 0.25;//EPSILON;
    float samples = 5.0;
    float occlusion = 1.0;
    mtl_t mtl;
    for (occlusion = 1.0 ; samples > 0.0 ; samples-=1.0) {
        occlusion -= (samples * stepDistance - (compute_scene( p + n * samples * stepDistance, mtl))) / pow(2.0, samples);
    }
    return occlusion;
}

//---------------------------------------------------
float ambient_occlusion1( in vec3 p, in vec3 n, float startweight, float diminishweight )
{
    //n = vec3(0.0,1.0,1.0);
    float ao = 0.0;
    float weight = startweight;
    mtl_t mtl;
    
    for ( int i = 1; i < 6; ++i )
    {
        float delta = i*i*EPSILON *12.0;
        ao += weight * (delta-compute_scene(p+n*(0.0+delta), mtl));
        weight *= diminishweight;
    }
    
    return 1.0-saturate(ao);
}


//---------------------------------------------------
#define soft_shadow     soft_shadow1

// from iq. https://www.shadertoy.com/view/Xds3zN
float soft_shadow2( in vec3 ro, in vec3 rd, in float mint, in float tmax, float k )
{
    float res = 1.0;
    float t = mint;
    mtl_t mtl;
    for( int i=0; i<76; i++ )
    {
        float h = compute_scene( ro + rd*t, mtl );
        res = min( res, k*h/t );
        t += h;//clamp( h, 0.02, 0.10 );
        if( h<0.001 || t>tmax ) break;
    }
    return clamp( res, 0.0, 1.0 );
}


float soft_shadow1( in vec3 p, in vec3 w, float mint, float maxt, float k )
{
    float res = 1.0;
    mtl_t mtl;
    for( float t=mint; t < maxt; )
    {
        float h = compute_scene(p + w*t,mtl);
        if( h<0.001 )
            return 0.0;
        res = min( res, k*h/t );
        t += h * 1.0;
    }
    return res;
}

float hard_shadow(in vec3 ro, in vec3 rd, float mint, float maxt) {
    mtl_t mtl;
    for(float t=mint; t < maxt;) {
        float h = compute_scene(ro + rd*t, mtl);
        if(h<0.001) return 0.0;
        t += h;
    }
    return 1.0;
}



//------------------------------------------------------------------------------------
#pragma mark RAY MARCHER


// Ray marcher
vec4 trace_ray_relaxed(in vec3 p, in vec3 w, in vec4 bg_clr, inout float distance)
{
    const int maxIterations = 256;
    const float t_min = 3.3;
    const float t_max = 10000.0;
    const float pixelRadius = 0.00001;
    const bool forceHit = true;
    
    mtl_t mtl;
    
    float omega = 1.2;
    
    float t = t_min;
    float candidate_error = 10e8;//INFINITY;
    float candidate_t = t_min;
    float previousRadius = 0;
    float stepLength = 0;
    float functionSign = 1.0;///compute_scene(p,mtl) < 0 ? -1 : +1;
    
    vec3 rp;
    int i;
    for (i = 0; i < maxIterations; ++i)
    {
        rp = p+w*t;
        float signedRadius = functionSign * compute_scene(rp,mtl);
        float radius = abs(signedRadius);
        bool sorFail = omega > 1 && (radius + previousRadius) < stepLength;
        
        //since an excessively small value might not only cause numerical problems,
        // but also diminish theperformance of tracing a new refraction ray starting at the offset position as sphere tracing has to accelerate away from the surface initially.
        // However, manually choosing εmin proved to be much eas- ier and more robust than choosing a global ε. The cost for
        if (sorFail)
        {
            stepLength -= omega*stepLength;
            omega = 1;
        }
        else
        {
            stepLength = signedRadius * omega;
        }
        
        previousRadius = radius;
        float error = radius / t;
        
        if (!sorFail && error < candidate_error)
        {
            candidate_t = t;
            candidate_error = error;
        }
        
        if (!sorFail && error < pixelRadius || t > t_max)
            break;
        t += stepLength;
    }
    
    if ((t > t_max || candidate_error > pixelRadius) && !forceHit)
        return bg_clr;
    
    return compute_color(rp, candidate_t, mtl, float(i) * 1.0/float(maxIterations));//+vec3(float(i)/128.0);;
}


// Ray marcher
vec4 trace_ray(in vec3 p, in vec3 w, in vec4 bg_clr, inout float distance)
{
    //    const float maxDistance = 50;//1e10;
    const int maxIterations =256;
    const float closeEnough = 1e-5;
    vec3 rp;
    mtl_t mtl;
    float t = 0;
    for (int i = 0; i < maxIterations; ++i)
    {
        rp = p+w*t;
        float d = compute_scene(rp,mtl);
        t += d;
        if (d < closeEnough)
        {
            distance = t;
            // use this to debug number of ray casts
//            return vec4(vec3(float(i)/128.0), 1.0);
//            return mtl == 0 ? vec4(vec3(float(i)/128.0), 1.0) : compute_color(rp,t,mtl);
            return compute_color(rp, t, mtl, float(i) * 1.0/float(maxIterations));//+vec3(float(i)/128.0);
        }
        else if(t > distance)
        {
            
            return bg_clr;//vec3(0.0);
        }
        
        
    }
    
    return bg_clr;//vec3(0.0); // return skybox here
}




//------------------------------------------------------------------------------------
#pragma mark MATERIALS

float rounded_squares_texture(in vec3 p)
{
    float div = 0.1;
    float v = 1.0;
    v = (fract(p.x*div)-0.5)*(fract(p.z*div)-0.5);
    v = saturate(pow(v*220,1.9));
    return max(0.9,v);
}

float texture_test(in vec3 p)
{
    vec2 uv = mod(p.xz*0.01,vec2(1.0));
    return texture2D(floor_image0,uv).x;
}


const vec4 fog_clr = vec4(0.8, 0.9, 1.0, 1.0);//0.5,0.9,1.0, 1.0);
const vec4 floor_color = vec4(1.0, 1.0, 0.9, 1.0); //vec3(0.8,0.9,1.0);
const vec4 man_color = vec4(0.8, 0.95, 1.2, 1.0);


vec4 compute_color( in vec3 p, in float distance, in mtl_t mtl, in float normItCount )
{
    //return vec4(vec3(mtl),1.0);
    vec4 clr = vec4(vec3(normItCount), 1.0) * 2.0;
    clr.xyz = pow( clr.xyz, vec3(1.0/2.2)); // gamma correction.
    
    float v = mtl;
    float lum = luminosity(clr);
    float y = -p.y;
    const float limit = 4.0;
    if(y>limit)
    {
        y = clamp((y-limit)/30.0,0.0,1.0);
        v = mix(0.5,1.0,y);
//        lum = y;
    }
    
    clr.xyz = texture2D(color_image, vec2(lum, v)).xyz*vec3(1.0,0.97,0.82);
    float fog = attenuation(max(0.0,distance-100.0),0.000021);
    clr.xyz = mix(clr.xyz,fog_clr.xyz,(1.0-fog));
    
    return clr;
}



//------------------------------------------------------------------------------------
#pragma mark SCENE

#define blending sdf_blend_poly


vec3 guy_transform_inner( in vec3 p )
{
    return sdf_rotate_y(p,p.y*-0.0);//+p.x*0.2);// sdf_repeat(p,vec3(0.0,0.0,0.0));
}

vec3 guy_transform_outer( in vec3 p )
{
//    float o = texture2D(floor_image, p.xz * floor_scale).r * floor_height - floor_offset;//sdf_repeat(p,vec3(23.0,0.0,53.0));
    return p;//+vec3(0,o,0);
}

float guy_primitive( in vec3 p )
{
    //return sdf_sphere(p,2.0);
    //return sdf_hex_prism(p,1.0);
    //return sdf_round_box(p, vec3(1.0, 2.0, 1.0), 0.1);
    //return sdf_box(p,vec3(0.5));//sdf_torus(p,1.0,0.1);
    return sdf_prism(p,vec2(2.0,0.5));//sdf_round_box(p, vec3(4.0, 3.0, 1.0), 0.1);
    float d = sdf_round_box(p, vec3(0.3, 1.0, 1.0), 0.1);
    d = sdf_union(d,sdf_round_box(sdf_translate(p,vec3(1.3,1.5,0.1)), vec3(2.0, 3.0, 1.0), 0.1));
    return d;
}

float sdf_guy( in vec3 p )
{
    float d = 1000.0;
    vec3 pt;
    
    for(int i=0; i<kNumJoints; i++) {
        pt = p;
        
//        pt = guy_transform_outer(pt);
        pt = guy_transform_inner(pt);
        pt = sdf_transform(pt,box_mats[i]);
        pt = sdf_translate(pt,vec3(0.0,0.0,0.5));
        
        
        d = blending(d, guy_primitive(pt), blend_k);
    }
    
    return d;
}


//------------------------------------------------------------------------------------

float sdf_box_texture( in vec3 p, in vec3 size, in sampler2D tex )
{
    vec4 clr = texture2D(tex,(p.xz*0.1)+vec2(0.5));
    return sdf_box((p-vec3(0.0,-(clr.r)*333,0.0)),size);
}

vec4 texture2DGood( sampler2D sam, vec2 uv, float bias )
{
    uv = uv*1024.0 - 0.5;
    vec2 iuv = floor(uv);
    vec2 f = fract(uv);
    vec4 rg1 = texture2D( sam, (iuv+ vec2(0.5,0.5))/1024.0, bias );
    vec4 rg2 = texture2D( sam, (iuv+ vec2(1.5,0.5))/1024.0, bias );
    vec4 rg3 = texture2D( sam, (iuv+ vec2(0.5,1.5))/1024.0, bias );
    vec4 rg4 = texture2D( sam, (iuv+ vec2(1.5,1.5))/1024.0, bias );
    return mix( mix(rg1,rg2,f.x), mix(rg3,rg4,f.x), f.y );
}

vec4 tex2d( sampler2D sam, vec2 uv )
{
    return texture2D(sam,uv);
//    return smoothstep(0.01,0.99,texture2D(sam,uv));
    //return texture2DGood(sam,uv,-100);
}

float blend_mtl( in float a, in float b, in float mtl1, in float mtl2, float k )
{
//    float h = smoothstep(0,1,clamp((b-a)/k,0.0,1.0));
//    float h = smoothstep(0,1,clamp((b-a)/k,0.0,1.0));
    float h = clamp((b-a)/4.0,0.0,1.0);
 //   h = smoothstep(0.0,0.9,h);
//    h = smoothstep(0.0,0.9,h);
    return mix(mtl2,mtl1,h);//log(h*30.0));
//    float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
//    return mix( mtl2, mtl1, h ) - (k*h*(1.0-h))*(mtl1-mtl2);
}

//float blend_mtl( float d1, float d2, )
float compute_scene( in vec3 p, out mtl_t mtl )
{
    mtl = 0.0;
    float d = 10000.0;
    float dome =  -sdf_sphere(sdf_translate(p,centerPos),300.0);
    
    //d = sdf_union(d, sdf_xz_plane(p, sin(p.x*0.3)*sin(p.z*0.1)));//noise(p.xz) * 5.0) );
//    d = sdf_union(d, sdf_xz_plane(p,  0));
    
    
    float floor_y = 0.0;
    floor_y += (tex2d(floor_image0, p.xz * floor_scale0).r - floor_center0) * floor_height0 - floor_offset0;
    floor_y += (tex2d(floor_image1, p.xz * floor_scale1).r - floor_center1) * floor_height1 - floor_offset1;
    float terr = sdf_xz_plane(p, floor_y);
    d = sdf_union(terr, dome);
    
    if(terr<dome)
    {
        mtl = 1.0;
    }
    
//    float dguy = 100000.0;
//    for(int i=0; i<kNumJoints; i++)
//    {
//        dguy = sdf_union(dguy, sdf_guy(sdf_transform(p,steerMatrix)));
//    }
//    
    float dguy = sdf_guy(sdf_transform(p, steerMatrix));
    
//    mtl = clamp(blend_mtl(dome,terr,1.0,0.5,4.0),0.0,0.5);
    mtl = clamp(blend_mtl(d,dguy,0.5,0.0,18.0),0.0,1.0);

    
//    mtl = blend_mtl_poly(dguy,terr,0.5,0.0,blend_k*2.0);
//    mtl =
    
    d = blending(d, dguy, blend_k);
    
    /*
    float floorlow = sdf_xz_plane(p,-10.0);
    
    if(floorlow<d)
    {
        //d = floorlow;
        mtl = 2.0;
    }*/
    
    
    
    return d;// + texture2D(floor_image, p.xz * floor_scale).r * floor_height - floor_offset;
    /*
     mtl = 0;
     float d1,d2;
     d1 = sdf_xz_plane(p,3.0);
     vec3 repp = rotate_x(repeat(p,vec3(10.0,0.0,10.0)),rotamt*p.x);
     d2 = sdf_round_box(repp,vec3(2.0,3.3,0.9),0.8); //sdf_sphere(rotate_x(repeat(p,vec3(10.0,1.0,10.0)),rotamt),2.0); //
     //d2 = op_blend(p,d2,sdf_torus(p+vec3(0.0,-2.0,0.0),26.0,5.3));
     if(d2<d1)
     {
     d1 = d2;
     mtl = 1;
     }
     
     d2 = sdf_torus(rotate_x(p,rotamt*p.z)+vec3(0.0,-7.0,0.0),26.0,2.3);//vec2(16.0,5.3));
     if(d2<d1)
     {
     d1 = d2;
     mtl = 2;
     }
     
     return d1;//sdf_union(d1,d2);*/
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
    
    float distance = 1e3;
    
    vec4 clr = trace_ray(p, w, fog_clr, distance);
    
    
    
    clr.w  = 1.0;
    gl_FragColor = clr;
}



