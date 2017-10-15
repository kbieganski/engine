#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform Transform {
	mat4 world;
	mat4 viewProjection;
} transform;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;

layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoords;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	vec4 position = transform.world * vec4(inPosition, 1.0);
	gl_Position = transform.viewProjection * position;
	fragPosition = position.xyz;
	fragNormal = inNormal;
	fragTexCoords = inTexCoords;
}
