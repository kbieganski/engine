#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform Shading {
	uniform vec4 ambientLightColor;
	uniform vec4 lightColor;
	uniform vec4 lightDirection;
	uniform mat4 lightView;
	uniform vec4 eyePosition;
	uniform float lightAttenuation;
} shading;

layout(binding = 1) uniform sampler2D positionMap;
layout(binding = 2) uniform sampler2D normalMap;
layout(binding = 3) uniform sampler2D diffuseMap;
layout(binding = 4) uniform sampler2D specularColorMap;
layout(binding = 5) uniform sampler2D specularHardnessMap;
layout(binding = 6) uniform sampler2DShadow shadowMap;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

float multisampleShadowMap(vec3 coordinates) {
	vec2 poissonDisk[4] = vec2[] (
		vec2(-0.94201624, -0.39906216),
		vec2(0.94558609, -0.76890725),
		vec2(-0.094184101, -0.92938870),
		vec2(0.34495938, 0.29387760)
	);
	float sum = 0;
	for (int i = 0; i < 4; i++) {
		vec3 bias = vec3(poissonDisk[i] / 800, -0.01);
		sum += texture(shadowMap, coordinates + bias).r;
	}
	return sum / 4;
}

float calculateIlluminationFactor(vec3 fragPosition) {
	vec3 shadowMapCoords = (shading.lightView * vec4(fragPosition, 1.0)).xyz;
	shadowMapCoords.xy = (shadowMapCoords.xy + vec2(1)) / 2;
	return multisampleShadowMap(shadowMapCoords);
}

vec3 calculateAmbientComponent(vec3 diffuseColor) {
	return diffuseColor * shading.ambientLightColor.rgb;
}

vec3 calculateDiffuseComponent(vec3 fragNormal, vec3 diffuseColor) {
	float diffuseFactor = dot(fragNormal, -shading.lightDirection.xyz);
	return diffuseColor * shading.lightColor.xyz * diffuseFactor;
}

vec3 calculateSpecularComponent(vec3 fragPosition, vec3 fragNormal, vec3 specularColor, float specularHardness) {
	vec3 vertexToEye = normalize(shading.eyePosition.xyz - fragPosition);
	vec3 lightReflect = normalize(reflect(shading.lightDirection.xyz, fragNormal));
	float specularFactor = dot(vertexToEye, lightReflect);
	specularFactor = clamp(specularFactor, 0, 1);
	specularFactor = pow(specularFactor, specularHardness);
	return specularColor * shading.lightColor.rgb * specularFactor;
}

void main() {
	vec3 fragPosition = texture(positionMap, fragTexCoord).xyz;
	vec3 fragNormal = texture(normalMap, fragTexCoord).xyz;
	vec3 diffuseColor = texture(diffuseMap, fragTexCoord).rgb;
	vec3 specularColor = texture(specularColorMap, fragTexCoord).rgb;
	float specularHardness = texture(specularHardnessMap, fragTexCoord).r;
	float illuminationFactor = calculateIlluminationFactor(fragPosition);
	vec3 ambientComponent = calculateAmbientComponent(diffuseColor);
	vec3 diffuseComponent = calculateDiffuseComponent(fragNormal, diffuseColor);
	vec3 specularComponent = calculateSpecularComponent(fragPosition, fragNormal, specularColor, specularHardness);
	outColor = vec4(illuminationFactor * (diffuseComponent + specularComponent) + ambientComponent, 1);
}