#include <fstream>
#include "Logger.hpp"
#include "Shader.hpp"


using std::ifstream;
using std::ios;
using std::move;


Shader::Shader(shared_ptr<const GraphicsContext> context, const string& filename) {
	this->context = context;
	ifstream file(filename, ios::ate | ios::binary);
	if (!file.is_open()) {
		ERROR("Failed to open file ", filename);
	}
	auto shaderFileSize = file.tellg();
	vector<char> code(shaderFileSize);
	file.seekg(0);
	file.read(code.data(), code.size());
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<uint32_t*>(code.data());
	if (vkCreateShaderModule(context->getDevice(), &createInfo, nullptr, &handle) != VK_SUCCESS) {
		ERROR("Failed to create shader " + filename);
	}
	INFO("Created shader ", handle);
}


Shader::Shader(Shader&& moved) {
	*this = move(moved);
}


Shader::~Shader() {
	if (handle != VK_NULL_HANDLE) {
		vkDestroyShaderModule(context->getDevice(), handle, nullptr);
	}
}


Shader& Shader::operator=(Shader&& moved) {
	handle = moved.handle;
	context = move(moved.context);
	moved.handle = VK_NULL_HANDLE;
	return *this;
}


VkShaderModule Shader::getHandle() const {
	return handle;
}
