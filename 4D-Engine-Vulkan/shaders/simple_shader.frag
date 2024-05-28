#version 450

precision highp float; 
/*
uniform vec2 u_resolution;  // Width and height of the shader
uniform float u_time;  // Time elapsed*/

layout(push_constant) uniform u_values
{
	vec2 u_resolution;
    float u_time;
	float u_rot;
	vec4 u_cam;
} push;

 
// Constants
#define PI 3.1415925359
#define TWO_PI 6.2831852
#define MAX_STEPS 1000
#define MAX_DIST 200.
#define SURFACE_DIST .001

layout(location = 0) in vec3 fragColor;
//layout(location = 1) in vec2 fragTexCoord;

//layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

/*
void main() {
    outColor = vec4(fragColor, 1.0); //* texture(texSampler, fragTexCoord);
}*/

struct shape {
	float dis;
	vec4 color;
};

shape hyperSphere (vec4 p, float r, vec4 color) {
	return shape(length(p) - r, color);
}

shape tesseract(vec4 p, vec4 b, vec4 color )
{
  //p -= b1;
  //vec4 b = b2 - b1;
  vec4 q = abs(p) - b;
  float result = min(max(q.x, max(q.y, max(q.z, q.w))), 0.) + length(max(q, 0.));
  return shape(result, color);
  //return shape(length(max(q,0.0)) + min(max(q.x,max(q.y,max(q.z, q.w))),0.0), color);
}

shape hyperCylinder (vec4 p, float r, float depth, vec4 color) {
	float d = length(p.xyz) - r;
	float dW = 0;
	if (p.w < 0 || p.w > depth) {
		d = max(d, 0);
		dW = min(abs(p.w), abs(p.w - depth));
	}
	return shape(sqrt(d * d + dW * dW), color);
}

float cylinder (vec3 p, float r, float depth) {
	float d = length(p.xy) - r;
	float dZ = 0;
	if (p.z < 0 || p.z > depth) {
		d = max(d, 0);
		dZ = min(abs(p.z), abs(p.z - depth));
	}
	return sqrt(d * d + dZ * dZ);
}

shape cylindricalPrism (vec4 p, vec2 c, float depth, vec4 color) {
	float d = cylinder(p.xyz, c.x, c.y);
	float dW = 0;
	if (p.w < 0 || p.w > depth) {
		d = max(d, 0);
		dW = min(abs(p.w), abs(p.w - depth));
	}
	return shape(sqrt(d * d + dW * dW), color);
}

shape tetrahedronalPrism(vec4 p, float depth, vec4 color)
{
	float d = (max(abs(p.x + p.y) - p.z, abs(p.x - p.y) + p.z) - 1.) / sqrt(3.);
	float dW = 0;
	if (p.w < 0 || p.w > depth) {
		d = max(d, 0);
		dW = min(abs(p.w), abs(p.w - depth));
	}
	return shape(sqrt(d * d + dW * dW), color);
}

shape cubePrism(vec4 p, vec3 b, float depth, vec4 color) {
	vec3 q = abs(p.xyz) - b;
	float d = length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
	float dW = 0;
	if (p.w < 0 || p.w > depth) {
		d = max(d, 0);
		dW = min(abs(p.w), abs(p.w - depth));
	}
	return shape(sqrt(d * d + dW * dW), color);
}

shape torusPrism(vec4 p, float smallRadius, float largeRadius, float depth, vec4 color) {
	float d = length(vec2(length(p.xz) - largeRadius, p.y)) - smallRadius;
	float dW = 0;
	if (p.w < 0 || p.w > depth) {
		d = max(d, 0);
		dW = min(abs(p.w), abs(p.w - depth));
	}
	return shape(sqrt(d * d + dW * dW), color);
}

float cone ( vec3 p, float base, float h )
{
  vec2 c = vec2(h / sqrt(base*base + h*h), base / sqrt(base*base + h*h));
  vec2 q = h*vec2(c.x/c.y,-1.0);    
  vec2 w = vec2( length(p.xz), p.y );
  vec2 a = w - q*clamp( dot(w,q)/dot(q,q), 0.0, 1.0 );
  vec2 b = w - q*vec2( clamp( w.x/q.x, 0.0, 1.0 ), 1.0 );
  float k = sign( q.y );
  float d = min(dot( a, a ),dot(b, b));
  float s = max( k*(w.x*q.y-w.y*q.x),k*(w.y-q.y)  );
  return sqrt(d)*sign(s);
}

shape conePrism(vec4 p, float base, float h, float depth, vec4 color)
{
	float d = cone(p.xyz, base, h);
	float dW = 0;
	if (p.w < 0 || p.w > depth) {
		d = max(d, 0);
		dW = min(abs(p.w), abs(p.w - depth));
	}
	return shape(sqrt(d * d + dW * dW), color);
}

shape hyperPlane(vec4 p, vec4 n, float h, vec4 color)
{
	return shape(dot(p,n) - h ,color);
}

shape unionSdf (shape shapeA, shape shapeB)
{
	if (shapeA.dis < shapeB.dis) return shape(shapeA.dis, shapeA.color);
	else return shape(shapeB.dis, shapeB.color);
}

shape intersectSdf (shape shapeA, shape shapeB)
{
	return shape(max(shapeA.dis, shapeB.dis), (shapeA.color + shapeB.color) / 2);
}

shape subtractSdf (shape shapeA, shape shapeB)
{
	if (shapeA.dis > -1 * shapeB.dis) return shape(shapeA.dis, shapeA.color);
	else return shape(-1 * shapeB.dis, shapeA.color);
}


shape smoothUnionSDF(shape shapeA, shape shapeB, float k ) {
  float h = clamp(0.5 + 0.5 * (shapeA.dis - shapeB.dis) / k, 0., 1.);
  float result = mix(shapeA.dis, shapeB.dis, h) - k * h * (1. - h);
  float r = mix(shapeA.color.x, shapeB.color.x, h);
  float g = mix(shapeA.color.y, shapeB.color.y, h);
  float b = mix(shapeA.color.z, shapeB.color.z, h);
  float a = mix(shapeA.color.w, shapeB.color.w, h);
  return shape(result, vec4(r,g,b,a));
}

shape smoothIntersectSDF(shape shapeA, shape shapeB, float k) {
	float h = clamp(0.5 - 0.5 * (shapeA.dis - shapeB.dis) / k, 0., 1.);
	float result = mix(shapeA.dis, shapeB.dis, h ) + k * h * (1. - h);
	float r = mix(shapeA.color.x, shapeB.color.x, h);
	float g = mix(shapeA.color.y, shapeB.color.y, h);
	float b = mix(shapeA.color.z, shapeB.color.z, h);
	float a = mix(shapeA.color.w, shapeB.color.w, h);
	return shape(result, vec4(r,g,b,a));
}

shape smoothSubtractSDF(shape shapeA, shape shapeB, float k) {
    float h = clamp(0.5 - 0.5 * (shapeA.dis + shapeB.dis) / k, 0., 1.);
	float result = mix(shapeA.dis, -shapeB.dis, h ) + k * h * (1. - h);
    return shape(result, shapeA.color);
}


//enum rotationPlane {
const int ZW = 1;
const int YW = 2;
const int YZ = 3;
const int XW = 4;
const int XZ = 5;
const int XY = 6;
//};

mat4 rotationMatrix4D(float angle, int plane)
{
	float s = sin(angle);
    float c = cos(angle);
    mat4 rot = mat4(0);

    switch(plane)
    {
		case ZW:
			rot[0][0] = c; rot[0][1] = -s;
			rot[1][0] = s; rot[1][1] = c;
			rot[2][2] = 1;
			rot[3][3] = 1;
			break;
		case YW:
			rot[0][0] = c; rot[0][2] = -s;
			rot[1][1] = 1;
			rot[2][0] = s; rot[2][2] = c;
			rot[3][3] = 1;
			break;
		case YZ:
			rot[0][0] = c; rot[0][3] = -s;
			rot[1][1] = 1;
			rot[2][2] = 1;
			rot[3][0] = s; rot[1][3] = c;
			break;
		case XW:
			rot[0][0] = 1;
			rot[1][1] = c; rot[1][2] = -s;
			rot[2][1] = s; rot[2][2] = c;
			rot[3][3] = 1;
			break;
		case XZ:
			rot[0][0] = 1;
			rot[1][1] = c; rot[1][3] = -s;
			rot[2][2] = 1;
			rot[3][1] = s; rot[3][3] = c;
			break;
		case XY:
			rot[0][0] = 1;
			rot[1][1] = 1;
			rot[2][2] = c; rot[2][3] = -s;
			rot[3][2] = s; rot[3][3] = c;
			break;
		default:
			break;
    }

	return rot;
}

vec4 rotate4D(vec4 p, vec4 about, float angle, int plane)
{
	p -= about;
	mat4 rot = rotationMatrix4D(angle, plane);
	vec4 result = rot * p;
	return result + about;
}

int lastTransparentShape = -1;

float _dep = 1;

shape GetDist(vec4 p)
{
    //vec4 s = vec4(0,1,6. + sin(push.u_time)*3.,1); //Sphere. xyz is position w is radius
    shape shapes[3];

	shapes[0] = hyperSphere(p, 50, vec4(-1.));
	shapes[0].dis *= -1.;

	shapes[1] = hyperPlane(p, vec4(0,1,0,0), -2, vec4(0.1f,0.1f,0.5f,1));

	//shapes[2] = hyperSphere(p - vec4(0, 1, 0, 0), 1, vec4(0,0,1,1));

	float alpha = 1.;

	vec4 sphS = vec4(1, 2, 1, 1);

	shape sph = hyperSphere((p / sphS) - vec4(0, 1, 0, 0), 1, vec4(0,0,1, alpha));
	sph.dis *= max(sphS.x, max(sphS.y, max(sphS.z, sphS.w)));

	vec4 _p = p;

	shape core2 = hyperSphere(p - vec4(0, 1, 0, 0), 0.65, vec4(0.0f,1.f,0.0f,1));

	vec4 torP = p - vec4(-3 * 0.66f, 1, 0, 0);

	//vec4 torP = p - vec4(-1, 1, 0, 0.);
	shape tor = torusPrism(torP, 0.3, 1, _dep, vec4(1.f, 1.f, 0.f, alpha));

	vec4 tetP = p - vec4(1, 3 * 0.25f, 0, 0);
	//vec4 tetP = p - vec4(1, 1, 0, 0.);
	shape tet = tetrahedronalPrism(tetP, _dep, vec4(1.f, 0.f, 1.f, alpha));

	vec4 cylP = p - vec4(1, 1, 0, 0);
	cylP = rotate4D(cylP, vec4(0), PI / 4, XW);
	cylP = rotate4D(cylP, vec4(0), PI / 5, XZ);
	shape cyl = tesseract(cylP, vec4(0.5, 0.5, 3, 5), vec4(0.4f, 0.9f, 0.7f, alpha));
	//shape cyl = cylindricalPrism(cylP, vec2(0.5, 3), 5, vec4(0.4f, 0.9f, 0.7f, alpha));

	//shape result = smoothUnionSDF(sph, tess, 0.5);

	//result = smoothUnionSDF(result, tor, 0.5f);

	//result = smoothUnionSDF(result, tet, 0.5f);

	//result = smoothUnionSDF(result, cyl, 0.5f);

	shape result = sph;
	//shape result = unionSdf(sph, tess);
	result = unionSdf(result, tor);
	result = unionSdf(result, tet);
	result = unionSdf(result, cyl);

	//result = subtractSdf(result, core2);

	//result = smoothUnionSDF(tor, tet, 0.5f);

	//result = cubePrism(p - vec4(0,1,1,0), vec3(1,1,1), _dep, vec4(1.f, 1.f, 0.f, alpha));

	//shape result = smoothUnionSDF(sph, core, 0.5);

	shapes[2] = result;

	//shapes[2] = sph;

	//shapes[3] = core;

    vec4 b = vec4(5, 0, 20, 0);

    int index = 0;

    for(int i = 1; i < shapes.length(); i++) {
		if (i != lastTransparentShape || shapes[i].color.w == 1.)
		index = (shapes[i].dis < shapes[index].dis) ? i : index;
	}

	if (shapes[index].color.w < 1. && shapes[index].color.w >= 0) lastTransparentShape = index;

    return shapes[index];
}

const int MAX_TRANSPARENT_SHAPES = 16;

shape RayMarch(vec4 ro, vec4 rd) 
{
    shape dO = shape(0, vec4(0.5f,0.5f,0.5f,1)); //Distance Origin
	shape[MAX_TRANSPARENT_SHAPES] transparentShapes;
	transparentShapes[0] = shape(0, vec4(-1.));
	int nT = 0;
	lastTransparentShape = -1;
    for(int i=0;i<MAX_STEPS;i++)
    {
		if (nT >= MAX_TRANSPARENT_SHAPES) nT = MAX_TRANSPARENT_SHAPES - 1;
        vec4 p = ro + rd * dO.dis;
        shape ds = GetDist(p); // ds is Distance Scene
        dO.dis += ds.dis;
        if(dO.dis > MAX_DIST || ds.dis < SURFACE_DIST) {
			if (ds.color.w < 1. && ds.color.w >= 0) {
				transparentShapes[nT] = ds;
				nT = nT + 1;
			}
			else {
				dO.color = ds.color;
				if (ds.color != vec4(-1.))
				{
					for (int j = 0; j < nT; j++)
					{
						if (transparentShapes[j].dis < dO.dis)
						{
							dO.color.x = mix(dO.color.x, transparentShapes[j].color.x, transparentShapes[j].color.w);
							dO.color.y = mix(dO.color.y, transparentShapes[j].color.y, transparentShapes[j].color.w);
							dO.color.z = mix(dO.color.z, transparentShapes[j].color.z, transparentShapes[j].color.w);
						}
					}
				}
				lastTransparentShape = -1;
				break;
			}
		}
		else
		{
			lastTransparentShape = -1; 
		}
    }
	//dO.color = dO.color * vec4(mod(dO.dis / 1000, 10),mod(dO.dis / 100, 10),mod(dO.dis / 10, 10),1);
    return dO;
}

vec4 getNormal(vec4 p)
{
	float eps = SURFACE_DIST;
	float d = GetDist(p).dis;

	vec4 n = vec4(d - GetDist(p - vec4(eps, 0, 0, 0)).dis, d - GetDist(p - vec4(0, eps, 0, 0)).dis, d - GetDist(p - vec4(0, 0, eps, 0)).dis, d - GetDist(p - vec4(0, 0, 0, eps)).dis);

	n = normalize(n);

	return n;
}

struct light {
	//vec4 pos;
	vec4 value;
	vec4 color;
	int type; // 0 = point, 1 = directional
};

float getLight (vec4 p)
{
	//vec4 light = vec4(-5, 12, -7, 0);
	light lights[1];

	lights[0] = light(vec4(5, 12, -7, 0), vec4(1, 1, 1, 1), 0);
	//lights[1] = light(vec4(1,-1, 1, 0), vec4(1, 1, 1, 1), 1);

	//light.y *= sin(push.u_time / 2.);

	

	float dif = 0.0;
	int index = 0;

	for (int i = 0; i < lights.length(); i++)
	{
		if (lights[i].type == 0)
		{
			vec4 l = normalize(lights[i].value - p);
			vec4 normal = getNormal(p);

			float dT = clamp(dot(normal, l), 0, 1);
			float r = RayMarch(p + normal + SURFACE_DIST * 2, l).dis;

			if (r < length(lights[i].value.xyz - p.xyz)) dT *= 0.1;

			if (dT > dif)
			{
				dif = dT;
				index = i;
			}

			//float spec = pow(max(dot(normal, h), 0.0), 32.0);
			//return diff * 0.5 + spec;
		}
		else
		{
			vec4 l = normalize(lights[i].value * -1.);
			vec4 normal = getNormal(p);

			float dT = clamp(dot(normal, l), 0, 1);

			if (dT > dif)
			{
				dif = dT;
				index = i;
			}
		}
	}

	return dif;
}

float getLight3D (vec4 p)
{
	light lights[1];

	lights[0] = light(vec4(5, 12, -7, 0), vec4(1, 1, 1, 1), 0);
	//lights[1] = light(vec4(1,-1, 1, 0), vec4(1, 1, 1, 1), 1);

	

	float dif = 0.0;
	int index = 0;

	for (int i = 0; i < lights.length(); i++)
	{
		if (lights[i].type == 0)
		{
			vec3 l = normalize(lights[i].value.xyz - p.xyz);
			vec3 normal = getNormal(p).xyz;
			normal = normalize(normal);

			float dT = clamp(dot(normal, l), 0, 1);
			float r = RayMarch(p + vec4(normal, 0) + SURFACE_DIST * 2, vec4(l, 0)).dis;

			if (r < length(lights[i].value.xyz - p.xyz)) dT *= 0.1;

			if (dT > dif)
			{
				dif = dT;
				index = i;
			}
		}
		else
		{
			vec3 l = normalize(lights[i].value.xyz * -1.);
			vec3 normal = getNormal(p).xyz;
			normal = normalize(normal);

			float dT = clamp(dot(normal, l), 0, 1);

			if (dT > dif)
			{
				dif = dT;
				index = i;
			}
		}
	}

	return dif;
}
 
void main()
{
    vec2 uv = (gl_FragCoord.xy-.5*push.u_resolution.xy)/push.u_resolution.y;
	uv.y *= -1.0;
	vec4 baseRo = vec4(0, 3, -7, 0);
    vec4 ro = baseRo; // Ray Origin/ Camera
    vec4 rd = normalize(vec4(uv.x,uv.y, 1, 0));

	rd = rotate4D(rd, vec4(0), -1 * atan( ro.y / length( vec2(ro.x, ro.z) ) ), XW);
	rd = rotate4D(rd, vec4(0), -1 * atan(ro.x/ro.z), YW);
	
	rd = normalize(rd);

	//ro = rotate4D(ro, vec4(0, 0, 0, 0), PI * ( -0.5 + 0.25 * (sin(push.u_time / 4) + 1)), XY);
	//ro = rotate4D(ro, vec4(0, 0, 0, 0), PI * ( -0.45 + 0.5 * 0.5 * (sin(push.u_time / 4.) + 2)), XY);
	//rd = rotate4D(rd, vec4(0), atan(ro.y/ro.z), XW);

	//float t = PI * ( -0.45 + 0.5 * 0.5 * (sin(push.u_time / 4.) + 2) );

	//float t = push.u_time / 0.33;
	float t = push.u_rot;

	//ro.w = 0.5;
	//ro.w = sin(t / 2);

	//ro = rotate4D(ro, ro, t, YW);
	rd = rotate4D(rd, vec4(0), -t, YW);
	//vec4 camPos = rotate4D(push.u_cam, vec4(0), t, YW);
	vec4 camPos = push.u_cam;
	ro += camPos;

	//ro = rotate4D(ro, vec4(0), t, YW);
	//rd = rotate4D(rd, vec4(0), t, YW);
	//rd = normalize(rd);

    shape d = RayMarch(ro,rd); // Distance
    
	float dif = getLight(ro + rd * d.dis);
	//float dif = getLight3D(ro + rd * d.dis);

    vec4 color = d.color;
	if (d.color == vec4(-1.))
	{
		color = vec4(0.1, 0.1, 0.1 ,1);
		//color = vec4(camdep, camdep, camdep, 1);
	}
	else color.xyz *= dif;

    outColor = color;
}