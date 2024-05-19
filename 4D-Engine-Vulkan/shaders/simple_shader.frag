#version 450

precision highp float; 
/*
uniform vec2 u_resolution;  // Width and height of the shader
uniform float u_time;  // Time elapsed*/

layout(push_constant) uniform u_values
{
	vec2 u_resolution;
    float u_time;
} push;

 
// Constants
#define PI 3.1415925359
#define TWO_PI 6.2831852
#define MAX_STEPS 100
#define MAX_DIST 100.
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

shape hyperBox( vec4 p, vec4 b, vec4 color )
{
  //p -= b1;
  //vec4 b = b2 - b1;
  vec4 q = abs(p) - b;
  float result = min(max(q.x, max(q.y, max(q.z, q.w))), 0.) + length(max(q, 0.));
  return shape(result, color);
  //return shape(length(max(q,0.0)) + min(max(q.x,max(q.y,max(q.z, q.w))),0.0), color);
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

shape GetDist(vec4 p)
{
    //vec4 s = vec4(0,1,6. + sin(push.u_time)*3.,1); //Sphere. xyz is position w is radius
    shape shapes[3];

	shapes[0] = hyperSphere(p, 50, vec4(-1.));
	shapes[0].dis *= -1.;
	//shapes[0] = hyperSphere(p - vec4(0, sin(push.u_time * 3. + 1321), 0, 0), 1, vec4(0,0,1,1));

	//shapes[1] = hyperBox(p - vec4(0, sin(push.u_time * 3. + 1321), 0, 0), vec4(1), vec4(0,0,1,1));

    //shapes[1] = hyperSphere(p - vec4(0, sin(push.u_time * 3. + 1321), 0, 0), 1, vec4(0,0,1,1)); //vec4(0,1,6. + sin(push.u_time)*3.,1), 1);
	shapes[1] = hyperSphere(p - vec4(0, 1, 0, 0), 1, vec4(0,0,1,1));
	
	shapes[2] = shape(abs(p.y + 2), vec4(0.1f,0.1f,0.5f,1));

    vec4 b = vec4(5, 0, 20, 0);
    //vec4 b2 = vec4(6, 2, 21, 2);

	//vec4 b3 = (b1 + b2) / 2.;

	b = rotate4D(b, b, push.u_time, YZ);
	b = rotate4D(b, b, push.u_time, XW);

	//b1 = inverse(b1);
	//b2 = inverse(b2);

    //shapes[1] = hyperBox(p, b, vec4(0,1,0,1));
    //shapes[1] = hyperSphere(p, vec4(5,1,5,0.5f), 1, vec4(0,1,0,1));
    //shapes[2] = shape(50, vec4(0.1f,0.1f,0.5f,1));

    int index = 0;

    for(int i = 1; i < shapes.length(); i++) {
        index = (shapes[i].dis < shapes[index].dis) ? i : index;
	}

    return shapes[index];
}

shape RayMarch(vec4 ro, vec4 rd) 
{
    shape dO = shape(0, vec4(0.5f,0.5f,0.5f,1)); //Distance Origin
    for(int i=0;i<MAX_STEPS;i++)
    {
        vec4 p = ro + rd * dO.dis;
        shape ds = GetDist(p); // ds is Distance Scene
        dO.dis += ds.dis;
        if(dO.dis > MAX_DIST || ds.dis < SURFACE_DIST) { dO.color = ds.color; break; }
    }
	//dO.color = dO.color * vec4(mod(dO.dis / 1000, 10),mod(dO.dis / 100, 10),mod(dO.dis / 10, 10),1);
    return dO;
}

vec4 getNormal(vec4 p)
{
	float eps = 0.0001;
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
	light lights[2];

	lights[0] = light(vec4(5, 12, -7, 0), vec4(1, 1, 1, 1), 0);
	lights[1] = light(vec4(1,-1, 1, 0), vec4(1, 1, 1, 1), 1);

	//light.y *= sin(push.u_time / 2.);

	

	float dif = 0.0;

	for (int i = 0; i < lights.length(); i++)
	{
		if (lights[i].type == 0)
		{
			vec4 l = normalize(lights[0].value - p);
			vec4 normal = getNormal(p);

			dif = max(dot(normal, l), dif);
			//float spec = pow(max(dot(normal, h), 0.0), 32.0);
			//return diff * 0.5 + spec;
		}
		else
		{
			vec4 l = normalize(lights[1].value * -1.);
			vec4 normal = getNormal(p);

			dif = max(dot(normal, l), dif);
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

	//ro = rotate4D(ro, vec4(0, 0, 0, 0), PI * ( -0.5 + 0.25 * (sin(push.u_time / 4) + 1)), XY);
	ro = rotate4D(ro, vec4(0, 0, 0, 0), PI * ( -0.45 + 0.5 * 0.5 * (sin(push.u_time / 4.) + 2)), XY);
	//rd = rotate4D(rd, vec4(0), atan(ro.y/ro.z), XW);

	rd = rotate4D(rd, vec4(0), -1 * atan( ro.y / length( vec2(ro.x, ro.z) ) ), XW);
	rd = rotate4D(rd, vec4(0), -1 * atan(ro.x/ro.z), YW);
	
	rd = normalize(rd);


    shape d = RayMarch(ro,rd); // Distance
    
	float dif = getLight(ro + rd * d.dis);

    //vec4 color = vec4(d.dis, d.dis, d.dis,1);
    vec4 color = d.color * dif;
	if (d.color == vec4(-1.))
	{
		if (0.5 * 0.5 * (sin(push.u_time / 2.) + 2) > 0) color = vec4(0.9, 0.9, 0.9, 1);
		else color = vec4(0.1, 0.1, 0.1 ,1);
		//color = vec4(0.9, 0.9, 0.5 + (0.4 * 0.5 * (sin(push.u_time / 4) + 1)),1);
		//color = vec4(ro.y / baseRo.y / 4. + 0.75, 0., 0.,1);
	}

    outColor = color;
}