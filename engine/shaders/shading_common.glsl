
float multisampleShadowMap(vec3 coordinates, float focus, float zBias) {
	vec2 poissonDisk[4] = vec2[] (
		vec2(-0.94201624, -0.39906216),
		vec2(0.94558609, -0.76890725),
		vec2(-0.094184101, -0.92938870),
		vec2(0.34495938, 0.29387760)
	);
	float illuminationFactor = 0;
	for (int i = 0; i < 4; i++) {
		vec3 bias = vec3(poissonDisk[i] / focus, zBias);
		vec3 biasedCoordinates = coordinates + bias;
		illuminationFactor += sampleShadowMap(biasedCoordinates).r / 4;
	}
	return illuminationFactor;
}

float calculateIlluminationFactor(vec3 fragPosition, float focus, float zBias) {
	vec4 shadowMapCoords = shading.lightView * vec4(fragPosition, 1.0);
	shadowMapCoords /= shadowMapCoords.w;
	shadowMapCoords.xy = (shadowMapCoords.xy + vec2(1)) / 2;
	return multisampleShadowMap(shadowMapCoords.xyz, focus, zBias);
}

vec3 calculateDiffuseComponent(vec3 fragNormal, vec3 lightDirection, vec3 diffuseColor, vec3 lightColor) {
	float diffuseFactor = dot(fragNormal, -lightDirection);
	diffuseFactor = clamp(diffuseFactor, 0, 1);
	return diffuseColor * lightColor * diffuseFactor;
}

vec3 calculateSpecularComponent(vec3 fragPosition, vec3 fragNormal, vec3 lightDirection, vec3 specularColor, float specularHardness, vec3 lightColor) {
	vec3 vertexToCamera = normalize(shading.cameraPosition.xyz - fragPosition);
	vec3 lightReflect = normalize(reflect(lightDirection, fragNormal));
	float specularFactor = dot(vertexToCamera, lightReflect);
	specularFactor = clamp(specularFactor, 0, 1);
	specularFactor = pow(specularFactor, specularHardness);
	return specularColor * specularFactor * lightColor;
}
