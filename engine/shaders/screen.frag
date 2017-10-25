#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoords;

layout(location = 0) out vec4 outColor;

void main() {
	vec2 texCoords = vec2(fragTexCoords.x, 1 - fragTexCoords.y);
    outColor = texture(texSampler, texCoords);
}
