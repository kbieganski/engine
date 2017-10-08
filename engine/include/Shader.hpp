#pragma once
#include "GraphicsContext.hpp"


using std::string;


class Shader {
public:
	Shader(shared_ptr<const GraphicsContext> context, const string& filename);
	Shader(const Shader& copied) = delete;
	Shader(Shader&& moved);
	~Shader();

	Shader& operator=(const Shader& copied) = delete;
	Shader& operator=(Shader&& moved);

	VkShaderModule getHandle() const;


private:
	VkShaderModule handle;
	shared_ptr<const GraphicsContext> context;
};
