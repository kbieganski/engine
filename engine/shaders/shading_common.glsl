
float multisampleShadowMap(vec3 coordinates, float focus) {
	vec2 poissonDisk[16] = vec2[] (
		vec2(-0.613392, 0.617481),
		vec2(0.170019, -0.040254),
		vec2(-0.299417, 0.791925),
		vec2(0.645680, 0.493210),
		vec2(-0.651784, 0.717887),
		vec2(0.421003, 0.027070),
		vec2(-0.817194, -0.271096),
		vec2(-0.705374, -0.668203),
		vec2(0.977050, -0.108615),
		vec2(0.063326, 0.142369),
		vec2(0.203528, 0.214331),
		vec2(-0.667531, 0.326090),
		vec2(-0.098422, -0.295755),
		vec2(-0.885922, 0.215369),
		vec2(0.566637, 0.605213),
		vec2(0.039766, -0.396100)
	);
	//coordinates.z += zBias;
	//return sampleShadowMap(coordinates).r;
	float illuminationFactor = 0;
	for (int i = 0; i < 16; i++) {
		vec3 bias = vec3(poissonDisk[i] / focus, 0);
		vec3 biasedCoordinates = coordinates + bias;
		illuminationFactor += sampleShadowMap(biasedCoordinates).r / 16;
	}
	return illuminationFactor;
}

float calculateIlluminationFactor(vec3 fragPosition, float focus, float zBias) {
	vec4 shadowMapCoords = shading.lightView * vec4(fragPosition, 1.0);
	shadowMapCoords /= shadowMapCoords.w;
	shadowMapCoords.xy = (shadowMapCoords.xy + vec2(1)) / 2;
	shadowMapCoords.z += zBias;
	return multisampleShadowMap(shadowMapCoords.xyz, focus);
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
