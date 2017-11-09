#pragma once
#include "AssetCache.hpp"
#include "Renderer.hpp"


using glm::vec3;
using glm::vec4;


class RenderPipelineBuilder;


class ShadingRenderer : public Renderer {
public:
	struct AmbientShadingUniform {
		vec4 ambientLightColor;
		vec4 skyColor;
	};

	ShadingRenderer(shared_ptr<const GraphicsContext> context, shared_ptr<const RenderTarget> geometryBuffer, AssetCache<Shader> &shaderAssets);
	ShadingRenderer(ShadingRenderer&& moved);
	virtual ~ShadingRenderer();

	ShadingRenderer& operator=(ShadingRenderer&& moved);

	RenderDescription& addSunRender(shared_ptr<const UniformBuffer> uniformBuffer, shared_ptr<const Texture> shadowMap);
	RenderDescription& addSpotlightRender(shared_ptr<const UniformBuffer> uniformBuffer, shared_ptr<const Texture> shadowMap);

	void setAmbientLightColor(vec3 ambientLightColor);
	void setSkyColor(vec3 skyColor);

	vec3 getAmbientLightColor() const;
	vec3 getSkyColor() const;


private:
	void createScreenSurface(uvec2);
	void createAmbientLightRenderPipeline(AssetCache<Shader> &shaderAssets);
	void createSunRenderPipeline(AssetCache<Shader> &shaderAssets);
	void createSpotlightRenderPipeline(AssetCache<Shader> &shaderAssets);
	void configureLightRenderPipelineBuilder(RenderPipelineBuilder& pipelineBuilder) const;
	void createFramebuffer();

	shared_ptr<const RenderTarget> geometryBuffer;
	shared_ptr<RenderPipeline> ambientPipeline;
	shared_ptr<RenderPipeline> sunPipeline;
	shared_ptr<RenderPipeline> spotlightPipeline;
	shared_ptr<UniformBuffer> ambientShadingUniform;
	shared_ptr<VertexBuffer> screenRectangleVertices;
	shared_ptr<IndexBuffer> screenRectangleIndices;
	AmbientShadingUniform ambientShadingUniformData;
};
