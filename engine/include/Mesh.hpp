#pragma once
#include <tiny_obj_loader.h>
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"


using std::string;
using std::tuple;


class RenderDescription;
class RenderPipelineBuilder;


class Mesh {
public:
	Mesh(shared_ptr<const GraphicsContext> context, const string& filename);

	void describe(RenderDescription& renderDescription) const;

	static void createAttributeBindings(RenderPipelineBuilder& pipelineBuilder);


private:
	tuple<tinyobj::attrib_t, vector<tinyobj::shape_t>> loadMesh(const string& filename);
	void createBuffers(shared_ptr<const GraphicsContext> context, tinyobj::attrib_t attrib, vector<tinyobj::shape_t> shapes);

	shared_ptr<IndexBuffer> indexBuffer;
	shared_ptr<VertexBuffer> vertexBuffer;
};
