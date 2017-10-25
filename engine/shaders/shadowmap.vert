#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform Object {
	mat4 orientation;
	mat4 transform;
	mat4 worldViewProjection;
} object;

layout(binding = 1) uniform Light {
	mat4 viewProjection;
} light;

layout(location = 0) in vec3 inPosition;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	gl_Position = light.viewProjection * object.transform * vec4(inPosition, 1.0);
}
