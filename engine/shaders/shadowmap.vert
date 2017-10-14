#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform Light {
	mat4 wvpTransform;
} light;

layout(location = 0) in vec3 inPosition;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	gl_Position = light.wvpTransform * vec4(inPosition, 1.0);
}
