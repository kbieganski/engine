#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform Transform {
	mat4 orientation;
	mat4 world;
	mat4 worldViewProjection;
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
	gl_Position = transform.worldViewProjection * vec4(inPosition, 1);
	fragPosition = (transform.world * vec4(inPosition, 1)).xyz;
	fragNormal = (transform.orientation * vec4(inNormal, 1)).xyz;
	fragTexCoords = inTexCoords;
}
