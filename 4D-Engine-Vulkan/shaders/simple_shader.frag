#version 450

precision highp float; 

const int MAX_INSTRUCTIONS = 200;
const int MAX_MATERIALS = 128;
const int MAX_INSTRUCTION_DATA = 8;

/*
uniform vec2 u_resolution;  // Width and height of the shader
uniform float u_time;  // Time elapsed*/

layout(push_constant) uniform u_values
{
	vec2 u_resolution;
    float u_time;
	float u_rot;
	vec4 u_cam;
	int u_numInstructions;
	int u_numMaterials;
} push;

//See 
struct InstructionData
{
	mat4 valueA;
	float floatB;
	float floatC;
	float floatD;
	int valueE;
};

struct Instruction
{
	int instructionType;
	mat4 valueA;
	vec4 valueB;
};

struct Material {
	vec4 diffuse;
	float ambient;
};

layout(set = 0, binding = 0) buffer InstructionsBuffer
{
	InstructionData instructions_data[MAX_INSTRUCTIONS];
};

layout(set = 1, binding = 0) uniform MaterialsBuffer
{
	Material materials_data[MAX_MATERIALS];
};
 
// Constants
#define PI 3.1415925359
#define TWO_PI 6.2831852
#define MAX_STEPS 1000
#define MAX_DIST 200.
#define SURFACE_DIST .001

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

float hyperSphere (vec4 p) {
	float r = 1;
	//return shape(length(p) - r, color);
	return length(p) - r;
}

float tesseract(vec4 p)
{
	vec4 b = vec4(1);
	//p -= b1;
	//vec4 b = b2 - b1;
	vec4 q = abs(p) - b;
	float result = min(max(q.x, max(q.y, max(q.z, q.w))), 0.) + length(max(q, 0.));
	return result;
}

float hyperCylinder (vec4 p) {
	float r = 1;
	float depth = 1;
	float d = length(p.xyz) - r;
	float dW = 0;
	if (p.w < 0 || p.w > depth) {
		d = max(d, 0);
		dW = min(abs(p.w), abs(p.w - depth));
	}
	return sqrt(d * d + dW * dW);
}

float cylinder (vec3 p) {
	float r = 1;
	float depth = 1;
	float d = length(p.xy) - r;
	float dZ = 0;
	if (p.z < 0 || p.z > depth) {
		d = max(d, 0);
		dZ = min(abs(p.z), abs(p.z - depth));
	}
	return sqrt(d * d + dZ * dZ);
}

float cylindricalPrism (vec4 p) {
	float depth = 1;
	float d = cylinder(p.xyz);
	float dW = 0;
	if (p.w < 0 || p.w > depth) {
		d = max(d, 0);
		dW = min(abs(p.w), abs(p.w - depth));
	}
	return sqrt(d * d + dW * dW);
}

float tetrahedronalPrism(vec4 p)
{
	float depth = 1;
	float d = (max(abs(p.x + p.y) - p.z, abs(p.x - p.y) + p.z) - 1.) / sqrt(3.);
	float dW = 0;
	if (p.w < 0 || p.w > depth) {
		d = max(d, 0);
		dW = min(abs(p.w), abs(p.w - depth));
	}
	return sqrt(d * d + dW * dW);
}

float cubePrism(vec4 p) {
	vec3 b = vec3(1);
	float depth = 1;
	vec3 q = abs(p.xyz) - b;
	float d = length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
	float dW = 0;
	if (p.w < 0 || p.w > depth) {
		d = max(d, 0);
		dW = min(abs(p.w), abs(p.w - depth));
	}
	return sqrt(d * d + dW * dW);
}

float torusPrism(vec4 p) {
	float smallRadius = 0.5;
	float largeRadius = 1;
	float depth = 1;
	float d = length(vec2(length(p.xz) - largeRadius, p.y)) - smallRadius;
	float dW = 0;
	if (p.w < 0 || p.w > depth) {
		d = max(d, 0);
		dW = min(abs(p.w), abs(p.w - depth));
	}
	return sqrt(d * d + dW * dW);
}

float cone (vec3 p)
{
	float base = 1;
	float h = 1;
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

float conePrism(vec4 p)
{
	float depth = 1;
	float d = cone(p.xyz);
	float dW = 0;
	if (p.w < 0 || p.w > depth) {
		d = max(d, 0);
		dW = min(abs(p.w), abs(p.w - depth));
	}
	return sqrt(d * d + dW * dW);
}

float hyperPlane(vec4 p, vec4 n, float h)
{
	return dot(p,n) - h;
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

float unionSdf (float distanceA, float distanceB)
{
	if (distanceA < distanceB)
	{
		
		return distanceA;
	}
	else
	{
		
		return distanceB;
	}
}

float intersectSdf (float distanceA, float distanceB)
{
	
	return max(distanceA, distanceB);
}

float subtractSdf (float distanceA, float distanceB)
{
	if (distanceA > -1 * distanceB)
	{
		
		return distanceA;
	}
	else
	{
		
		return -1 * distanceB;
	}
}

float smoothUnionSDF(float distanceA, float distanceB, float k ) {
  float h = clamp(0.5 + 0.5 * (distanceA - distanceB) / k, 0., 1.);
  float result = mix(distanceA, distanceB, h) - k * h * (1. - h);
  /*float r = mix(distanceA.color.x, distanceB.color.x, h);
  float g = mix(distanceA.color.y, distanceB.color.y, h);
  float b = mix(distanceA.color.z, distanceB.color.z, h);
  float a = mix(distanceA.color.w, distanceB.color.w, h);*/
  return result;
}

float smoothIntersectSDF(float distanceA, float distanceB, float k) {
	float h = clamp(0.5 - 0.5 * (distanceA - distanceB) / k, 0., 1.);
	float result = mix(distanceA, distanceB, h ) + k * h * (1. - h);
	/*float r = mix(distanceA.color.x, distanceB.color.x, h);
  float g = mix(distanceA.color.y, distanceB.color.y, h);
  float b = mix(distanceA.color.z, distanceB.color.z, h);
  float a = mix(distanceA.color.w, distanceB.color.w, h);*/
	return result;
}

float smoothSubtractSDF(float distanceA, float distanceB, float k) {
    float h = clamp(0.5 - 0.5 * (distanceA + distanceB) / k, 0., 1.);
	float result = mix(distanceA, -distanceB, h ) + k * h * (1. - h);
	/*float r = mix(distanceA.color.x, distanceB.color.x, h);
  float g = mix(distanceA.color.y, distanceB.color.y, h);
  float b = mix(distanceA.color.z, distanceB.color.z, h);
  float a = mix(distanceA.color.w, distanceB.color.w, h);*/
    return result;
}

int lastTransparentShape = -1;

float _dep = 1;



float ParseInstruction (Instruction inst, vec4 p)
{
	vec4 _p = inst.valueA * (p - inst.valueB);
	switch (inst.instructionType)
	{
		case 1:
			return hyperSphere(_p);
		case 2:
			return tesseract(_p);
		case 3:
			vec4 n = normalize(inst.valueA * vec4(0,1,0,0));
			return hyperPlane(p, n, inst.valueB.x);
		case 4:
			return tetrahedronalPrism(_p);
		case 5:
			return hyperCylinder(_p);
		case 6:
			return conePrism(_p);
		case 7:
			return cylindricalPrism(_p);
		case 8:
			return torusPrism(_p);
	}
	return 0;
}

Material materials[9];

struct shapeData {
	float dis;
	int matIndex;
};


shapeData ParseInstructions (vec4 pp, int materialIndex, inout int index)
{
	vec4 p = pp;
	int n = push.u_numInstructions;

	int operatorTypes[MAX_INSTRUCTION_DATA];
	float ShapeAs[MAX_INSTRUCTION_DATA];
	float ShapeBs[MAX_INSTRUCTION_DATA];
	bool hasShapes[MAX_INSTRUCTION_DATA];

	int pseudoRecursionDataIndex = 0;

	operatorTypes[0] = 0;
	ShapeAs[0] = 0;
	ShapeBs[0] = 0;
	hasShapes[0] = false;


	float _shape = 0;

	int j = 0;
	int _j = 0;
	int _jj = 0;

	int sI = index;

	for (int i = sI; i < n; i++)
	{
		index++;
		j++;

		int _opType = instructions_data[i].valueE % 64;
		float _E_value = ((instructions_data[i].valueE - _opType) / 64) / 10000.0;

		Instruction inst = Instruction(_opType, instructions_data[i].valueA, vec4(instructions_data[i].floatB, instructions_data[i].floatC, instructions_data[i].floatD, _E_value));
		if (inst.instructionType == 21)
		{
			materialIndex = int(inst.valueB.x);
		}
		else if (inst.instructionType >= 13 && inst.instructionType <= 18)
		{
			operatorTypes[pseudoRecursionDataIndex] = inst.instructionType;
			ShapeAs[pseudoRecursionDataIndex] = 0;
			ShapeBs[pseudoRecursionDataIndex] = 0;
			hasShapes[pseudoRecursionDataIndex] = false;
			pseudoRecursionDataIndex++;
			if (pseudoRecursionDataIndex >= MAX_INSTRUCTION_DATA) pseudoRecursionDataIndex = MAX_INSTRUCTION_DATA - 1;
		}
		else if (inst.instructionType >= 1 && inst.instructionType <= 8)
		{
			vec4 _p = inst.valueA * (p);
			_shape = ParseInstruction(inst, p);

			if (pseudoRecursionDataIndex == 0)
			{
				_p = inst.valueA * p;
				float scalingFactor = length(_p);
				_p = normalize(p) * scalingFactor;
				_p = abs(_p / p);
				scalingFactor = max(_p.x, max(_p.y, max(_p.z, _p.w)));
				scalingFactor = max(scalingFactor, 1.0 / scalingFactor);
				_shape /= scalingFactor;
				break;
			}
			_j++;
			for (int ii = pseudoRecursionDataIndex; ii >= 0; ii--)
			{
				_jj++;

				_p = inst.valueA * p;
				float scalingFactor = length(_p);
				_p = normalize(p) * scalingFactor;
				_p = abs(_p / p);
				scalingFactor = max(_p.x, max(_p.y, max(_p.z, _p.w)));
				scalingFactor = max(scalingFactor, 1.0 / scalingFactor);
				_shape /= scalingFactor;

				if (!hasShapes[pseudoRecursionDataIndex])
				{
					ShapeAs[pseudoRecursionDataIndex] = _shape;
					break;
				}
				else
				{
					ShapeBs[pseudoRecursionDataIndex] = _shape;

					int op = operatorTypes[pseudoRecursionDataIndex] * 2 - 27;
					int op1 = op / abs(op);
					int op2 = op - 2;
					op2 = -1 * op2 / abs(op2);

					_shape = max(ShapeAs[pseudoRecursionDataIndex] * op1, ShapeBs[pseudoRecursionDataIndex] * op1 * op2) * op1;

					operatorTypes[pseudoRecursionDataIndex] = 0;
					hasShapes[pseudoRecursionDataIndex] = false;
					pseudoRecursionDataIndex--;
				}
			}


			if (pseudoRecursionDataIndex < 0) break;
		}
	}
	return shapeData(_shape, materialIndex);
}

shapeData GetDist(vec4 p)
{
    shapeData shapes[2048];
	//getDistOutput shapes[2];

	int totalShapesForInstructions = 1;

	shapes[0] = shapeData(hyperSphere(p / 100.0) * -100.0, 0);

	int instructionIndex = 0;
	int shapeIndex = 1;

	//getDistOutput curShape = getDistOutput(0, 0);
	//for (instructionIndex = 0; totalShapesForInstructions <= 2; instructionIndex++)
	for (instructionIndex = 0; instructionIndex < push.u_numInstructions; instructionIndex++)
	{
		totalShapesForInstructions++;
		shapes[shapeIndex] = ParseInstructions(p, 0, instructionIndex);
		instructionIndex--;
		shapeIndex++;
	}

	//instMat = 2;
	//shapesCount = 4;
	//totalShapesForInstructions = 3;
	/*int n = 0;
	if (totalShapesForInstructions > 1000) n = 3;
	else n = 4;

	float dac = hyperSphere((p / vec4(1.5)) - vec4(-2, 1, -2, 0)) * 1.5;
	//dac = hyperPlane(p, vec4(0, 1, 0, 0), 0);
	for (int i = 1; i < totalShapesForInstructions; i++)
	{
		float _nextShape = hyperSphere((p / vec4(1.5)) - vec4(-2 + 1.5 * i, 1 + 0.5 * (i % 10), -2, 0)) * 1.5;
		dac = unionSdf(dac, _nextShape);
	}

	shapes[1] = getDistOutput(dac, 1);*/

	//shapes[1] = curShape;

	//materials[0] = Material(vec4(vec3(shapesCount / 5.0, shapesCount / 10.0, shapesCount / 20.0), 1));



    int index = 0;

    for(int i = 1; i < totalShapesForInstructions; i++) {
		if (i != lastTransparentShape || materials_data[shapes[i].matIndex].diffuse.w == 1.)
		index = (shapes[i].dis < shapes[index].dis) ? i : index;
	}

	if (materials_data[shapes[index].matIndex].diffuse.w < 1. && materials_data[shapes[index].matIndex].diffuse.w >= 0) lastTransparentShape = index;

	//if (instructionIndex > 8) instructionIndex = 8;
	//if (index != 0) shapes[index].matIndex = instructionIndex;

    return shapes[index];
}

const int MAX_TRANSPARENT_SHAPES = 16;

float RayMarch(vec4 ro, vec4 rd, out Material _mat) 
{
    shapeData dO = shapeData(0, 0); //Distance Origin
	Material[MAX_TRANSPARENT_SHAPES] transparentMats;
	float[MAX_TRANSPARENT_SHAPES] transparentShapeDistances;
	int nT = 0;
	lastTransparentShape = -1;
    for(int i=0;i<MAX_STEPS;i++)
    {
		if (nT >= MAX_TRANSPARENT_SHAPES) nT = MAX_TRANSPARENT_SHAPES - 1;
        vec4 p = ro + rd * dO.dis;
        shapeData ds = GetDist(p); // ds is Distance Scene
        dO.dis += ds.dis;
        if(dO.dis > MAX_DIST || ds.dis < SURFACE_DIST) {
			if (materials_data[ds.matIndex].diffuse.w < 1. && materials_data[ds.matIndex].diffuse.w >= 0) {
				transparentMats[nT] = materials_data[ds.matIndex];
				transparentShapeDistances[nT] = ds.dis;
				nT = nT + 1;
			}
			else {
				if (ds.matIndex == -1)
				{
					_mat = Material(vec4(vec3(0.1), 1), 0);
				}
				else
				{
					_mat = materials_data[ds.matIndex];
					for (int j = 0; j < nT; j++)
					{
						if (transparentShapeDistances[j] < dO.dis)
						{
							_mat.diffuse.x = mix(_mat.diffuse.x, transparentMats[j].diffuse.x, transparentMats[j].diffuse.w);
							_mat.diffuse.y = mix(_mat.diffuse.y, transparentMats[j].diffuse.y, transparentMats[j].diffuse.w);
							_mat.diffuse.z = mix(_mat.diffuse.z, transparentMats[j].diffuse.z, transparentMats[j].diffuse.w);
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
    return dO.dis;
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
			Material mat;
			float r = RayMarch(p + normal + SURFACE_DIST * 2, l, mat);

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
			Material mat;
			float r = RayMarch(p + vec4(normal, 0) + SURFACE_DIST * 2, vec4(l, 0), mat);

			if (r < length(lights[i].value.xyz - p.xyz)) dT *= 0.5;

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

	materials[0] = Material(vec4(0.1, 0.1, 0.1, 1), 0);
	materials[1] = Material(vec4(0.0, 0.25, 0.25, 1), 0.9);
	materials[2] = Material(vec4(0, 0.0, 1, 1), 0.9);
	materials[3] = Material(vec4(1, 0, 0, 1), 0.9);
	materials[4] = Material(vec4(0, 1, 0, 1), 0.9);
	materials[5] = Material(vec4(0, 0, 1, 1), 0.9);
	materials[6] = Material(vec4(1, 1, 0, 1), 0.9);
	materials[7] = Material(vec4(0, 1, 1, 1), 0.9);
	materials[8] = Material(vec4(1, 0, 1, 1), 0.9);

	//ro = rotate4D(ro, vec4(0, 0, 0, 0), PI * ( -0.5 + 0.25 * (sin(push.u_time / 4) + 1)), XY);
	//ro = rotate4D(ro, vec4(0, 0, 0, 0), PI * ( -0.45 + 0.5 * 0.5 * (sin(push.u_time / 4.) + 2)), XY);
	//rd = rotate4D(rd, vec4(0), atan(ro.y/ro.z), XW);

	//float t = PI * ( -0.45 + 0.5 * 0.5 * (sin(push.u_time / 4.) + 2) );

	//float t = push.u_time / 0.33;
	float t = push.u_rot;

	rd = rotate4D(rd, vec4(0), -t, YW);
	//vec4 camPos = rotate4D(push.u_cam, vec4(0), t, YW);
	vec4 camPos = push.u_cam;
	ro += camPos;

	//ro = rotate4D(ro, vec4(0), t, YW);
	//rd = rotate4D(rd, vec4(0), t, YW);
	//rd = normalize(rd);
	Material outputMat;// = Material(vec4(0.1, 0.1, 0.1, 1));
    float d = RayMarch(ro,rd, outputMat); // Distance
    
	float dif = getLight(ro + rd * d);
	//float dif = getLight3D(ro + rd * d.dis);

    vec4 color = outputMat.diffuse;
	color.xyz = mix(color.xyz, color.xyz * dif, outputMat.ambient);

	/*if (color == vec4(-1.))
	{
		color = materials_data[0].diffuse;
		//color = vec4(0.1, 0.1, 0.1 ,1);
		//color = vec4(camdep, camdep, camdep, 1);
	}
	else
	{
		color.xyz *= dif;
	}*/
    outColor = color;
}