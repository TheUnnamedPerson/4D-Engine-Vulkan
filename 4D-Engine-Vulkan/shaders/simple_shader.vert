#version 450
#extension GL_KHR_vulkan_glsl : enable

vec2 positions[3] = vec2[3](
	vec2(-0.50, -0.50),
	vec2(0.50, -0.50),
	vec2(-0.50, 0.50));

vec3 colors[3] = vec3[](
    vec3(0.1, 0.1, 0.5),
    vec3(0.2, 0.5, 0.7),
    vec3(0.3, 0.7, 1.0)
);

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragColor = colors[gl_VertexIndex];
}