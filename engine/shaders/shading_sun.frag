#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : enable

layout(binding = 0) uniform Shading {
	uniform vec4 lightColor;
	uniform vec4 lightDirection;
	uniform mat4 lightView;
	uniform vec4 cameraPosition;
} shading;

layout(binding = 1) uniform sampler2D positionMap;
layout(binding = 2) uniform sampler2D normalMap;
layout(binding = 3) uniform sampler2D diffuseMap;
layout(binding = 4) uniform sampler2D specularColorMap;
layout(binding = 5) uniform sampler2D specularHardnessMap;
layout(binding = 6) uniform sampler2DShadow shadowMap;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

float sampleShadowMap(vec3 coordinates) {
	if (coordinates.z <= 1) {
		return texture(shadowMap, coordinates).r;
	} else {
		return 0;
	}
}

#include "shading_common.glsl"

void main() {
	vec3 fragPosition = texture(positionMap, fragTexCoord).xyz;
	vec3 fragNormal = texture(normalMap, fragTexCoord).xyz;
	vec3 diffuseColor = texture(diffuseMap, fragTexCoord).rgb;
	vec3 specularColor = texture(specularColorMap, fragTexCoord).rgb;
	float specularHardness = texture(specularHardnessMap, fragTexCoord).r;
	float illuminationFactor = calculateIlluminationFactor(fragPosition, 400);
	vec3 diffuseComponent = calculateDiffuseComponent(fragNormal, shading.lightDirection.xyz, diffuseColor, shading.lightColor.xyz);
	vec3 specularComponent = calculateSpecularComponent(fragPosition, fragNormal, shading.lightDirection.xyz, specularColor, specularHardness, shading.lightColor.xyz);
	//outColor = vec4(0,0,0,1);
	outColor = vec4(illuminationFactor * (diffuseComponent + specularComponent), 1);
}
