#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform Material {
	vec4 specularColor;
	float specularHardness;
} material;

layout(binding = 2) uniform sampler2D modelTexture;

layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoords;

layout(location = 0) out vec3 outPosition;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec4 outDiffuse;
layout(location = 3) out vec4 outSpecularColor;
layout(location = 4) out float outSpecularHardness;

void main() {
	outPosition = fragPosition;
	outNormal = fragNormal;
	outDiffuse = texture(modelTexture, fragTexCoords);
	outSpecularColor = vec4(material.specularColor.rgb, 1);
	outSpecularHardness = material.specularHardness;
}
