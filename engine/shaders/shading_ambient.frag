#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform Shading {
	uniform vec4 ambientLightColor;
	uniform vec4 skyColor;
} shading;

layout(binding = 1) uniform sampler2D normalMap;
layout(binding = 2) uniform sampler2D diffuseMap;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

vec3 calculateAmbientComponent(vec3 fragNormal, vec3 diffuseColor) {
	float ambientFactor = (dot(fragNormal, vec3(0, 1, 0)) + 1) / 2;
	return ambientFactor * diffuseColor * shading.ambientLightColor.rgb;
}

void main() {
	vec3 fragNormal = texture(normalMap, fragTexCoord).xyz;
	if (fragNormal == vec3(0)) {
		outColor = shading.skyColor;
	} else {
		vec3 diffuseColor = texture(diffuseMap, fragTexCoord).rgb;
		vec3 ambientComponent = calculateAmbientComponent(fragNormal, diffuseColor);
		outColor = vec4(ambientComponent, 1);
	}
}
