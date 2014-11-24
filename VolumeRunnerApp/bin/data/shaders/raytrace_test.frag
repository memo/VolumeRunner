#version 120

#define kNumBones   6

uniform vec2 resolution; // screen resolution
uniform float time; // current time
uniform vec2 mouse; // mouse position (screen space)

uniform vec3 box_pos, box_rot, box_scale;  // for testing individual transforms
uniform mat4 box_mat;    // for testing whole transform
uniform mat4 box_mats[kNumBones];

uniform mat4 invViewMatrix;
uniform float tanHalfFov; // tan(fov/2)


const float EPSILON = 0.01;
const float PI = 3.1415926535;
const float PI2 = PI*2.0;

const vec3 light1  = vec3(0.5,1.0,0.3);

float saturate( in float v )
{
    return clamp(v,0.0,1.0);
}

float expose( in float l, in float e )
{
    return (1.5 - exp(-l*e));
}

const vec3 lumi = vec3(0.30, 0.59, 0.11);
float luminosity( in vec3 clr )
{
    return dot(clr,lumi);
}

vec3  normal_color( in vec3 n )
{
    return (n*vec3(0.5)+vec3(0.5));
}

float attenuation( in float distance, in float atten )
{
    return min( 1.0/(atten*distance*distance), 1.0 );
}


/*************************************/
/* Objects                           */
// p: sample position
// assumes object is at 0, 0, 0


float sdf_xz_plane(in vec3 p, float y)
{
    return p.y - y;
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



/*************************************/
/* Operators                         */

float sdf_union( in float d1, in float d2  )
{
    return min(d1,d2);
}

float sdf_blend(vec3 p, float a, float b)
{
    float s = smoothstep(length(p), 0.0, 1.0);
    float d = mix(a, b, s);
    return d;
}


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

/**************************************/
/* Scene description here..           */

float compute_scene( in vec3 p, out int mtl )
{
    mtl = 0;
    float d = 1e10;
    
    d = sdf_union(d, sdf_xz_plane(p,0.0) );

    vec3 samplepos;
    
    // repeated box
    samplepos = p;
    samplepos = sdf_repeat(p, vec3(5.0, 0.0, 5.0));
    samplepos = sdf_translate(samplepos, vec3(0.0, 1.0, 0.0));
    d = sdf_union(d, sdf_round_box(samplepos, vec3(3.0, 3.0, 3.0), 0.0) );
    
    // test box
    samplepos = p;
    //samplepos = sdf_translate(samplepos, box_pos);
    //samplepos = sdf_rotate_y(samplepos, box_rot.y);
    //samplepos = sdf_rotate_x(samplepos, box_rot.x);
    //samplepos = sdf_rotate_z(samplepos, box_rot.z);
    //samplepos = sdf_scale(samplepos, box_scale);
    samplepos = sdf_transform(samplepos, box_mat);
    //d = sdf_union(d, sdf_round_box(samplepos, vec3(10.0, 10.0, 10.0), 0.0) );
    
    
    for(int i=0; i<kNumBones; i++) {
       d = sdf_union(d, sdf_round_box(sdf_transform(p, box_mats[i]), vec3(1.0, 1.0, 1.0), 0.0) );
    }
    
    mtl = 1;
    return d;
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


/*************************************/
/* Lighting                          */

vec3 calc_normal ( in vec3 p )
{
    vec3 n = vec3(0.0);
    vec3 delta = vec3( 0.0001, 0.0, 0.0 );
    int mtl;
    n.x = compute_scene( p+delta.xyz, mtl ) - compute_scene( p-delta.xyz, mtl );
    n.y = compute_scene( p+delta.yxz, mtl ) - compute_scene( p-delta.yxz, mtl );
    n.z = compute_scene( p+delta.yzx, mtl ) - compute_scene( p-delta.yzx, mtl );
    return normalize( n );
}

float ambient_occlusion( in vec3 p, in vec3 n )
{
    float ao = 0.0;
    float weigth = 0.7;
    int mtl;
    
    for ( int i = 1; i < 6; ++i )
    {
        float delta = i*i*EPSILON *12.0;
        ao += weigth * (delta-compute_scene(p+n*delta, mtl));
        weigth *= 0.5;
    }
    
    return 1.0-saturate(ao);
}


vec3 rounded_squares_texture(in vec3 p)
{
    float div = 0.1;
    float v = 1.0;
    v = (fract(p.x*div)-0.5)*(fract(p.z*div)-0.5);
    v = saturate(pow(v*60,0.7));
    return vec3(max(0.3,v),v,0);//,v,0.0);//,v*0.5,0.0);
}

/*************************************/
/* Colors and materials              */

vec3 compute_color( in vec3 p, in float distance, in int mtl )
{
    vec3 n = calc_normal(p);
    //return normal_color(n); // use this to debug normals
    
    vec3 light = normalize(light1);
    float nl = max(0.3,dot(n,light));
    float fake = luminosity(normal_color(n))*1.4;
    vec3 clr = vec3(1.0);//,0.9,0.9);
    if(mtl==0)
    {
        clr = vec3(0.0,0.3,1.0);//rounded_squares_texture(p);
    }
    float l = nl*fake*ambient_occlusion(p,n);
    //l = expose(l,0.8);
    l *= attenuation(distance,0.0001);
    return clr*l;
}

/*************************************/
/* Ray marcher                       */

vec3 trace_ray(in vec3 p, in vec3 w, inout float distance)
{
    const float maxDistance = 1e10;
    const int maxIterations = 128;
    const float closeEnough = EPSILON; //1e-2;
    vec3 rp;
    int mtl;
    float t = 0;
    for (int i = 0; i < maxIterations; ++i)
    {
        rp = p+w*t;
        float d = compute_scene(rp,mtl);
        t += d;
        if (d < closeEnough)
        {
            distance = t;
            return compute_color(rp,t,mtl);
        }
        else if(t > distance)
        {
            return vec3(0.0);
        }
        
        
    }
    
    return vec3(0.0); // return skybox here
}

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
    gl_FragColor = vec4(trace_ray(p, w, distance),1.0);
}



