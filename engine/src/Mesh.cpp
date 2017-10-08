#define TINYOBJLOADER_IMPLEMENTATION
#include <unordered_map>
#include <glm/gtx/hash.hpp>
#include "Logger.hpp"
#include "Mesh.hpp"
#include "RenderDescription.hpp"
#include "RenderPipelineBuilder.hpp"


using glm::vec2;
using glm::vec3;
using std::get;
using std::make_shared;
using std::unordered_map;


struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 textureCoordinates;

	bool operator==(const Vertex& other) const {
		return position == other.position && textureCoordinates == other.textureCoordinates;
	}
};


namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(const Vertex& vertex) const {
			return (hash<vec3>()(vertex.position)
				 ^ (hash<vec3>()(vertex.normal) << 1) >> 1)
				 ^ (hash<vec2>()(vertex.textureCoordinates) << 1);
		}
	};
}


Mesh::Mesh(shared_ptr<const GraphicsContext> context, const string& filename) {
	auto mesh = loadMesh(filename);
	createBuffers(context, get<0>(mesh), get<1>(mesh));
	INFO("Loaded mesh ", filename);
}


void Mesh::createAttributeBindings(RenderPipelineBuilder& pipelineBuilder) const {
	pipelineBuilder.createAttributeBinding(sizeof(Vertex),
										   { VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32_SFLOAT },
										   { offsetof(Vertex, position), offsetof(Vertex, normal), offsetof(Vertex, textureCoordinates) });
}


void Mesh::describe(RenderDescription& renderDescription) const {
	renderDescription.addVertices(vertexBuffer);
	renderDescription.setIndices(indexBuffer);
}


tuple<tinyobj::attrib_t, vector<tinyobj::shape_t>> Mesh::loadMesh(const string& filename) {
	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	string err;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str())) {
		ERROR(err);
	}
	return { attrib, shapes };
}


void Mesh::createBuffers(shared_ptr<const GraphicsContext> context, tinyobj::attrib_t attrib, vector<tinyobj::shape_t> shapes) {
	vector<Vertex> vertices;
	vector<uint32_t> indices;
	unordered_map<Vertex, uint32_t> uniqueVertices;
	ASSERT(!attrib.normals.empty(), "Failed to load mesh - it does not define vertex normals");
	for (auto& shape : shapes) {
		for (auto& index : shape.mesh.indices) {
			Vertex vertex;
			vertex.position = { attrib.vertices[3 * index.vertex_index + 0],
								attrib.vertices[3 * index.vertex_index + 1],
								attrib.vertices[3 * index.vertex_index + 2] };
			vertex.normal = { attrib.normals[3 * index.normal_index + 0],
							  attrib.normals[3 * index.normal_index + 1],
							  attrib.normals[3 * index.normal_index + 2] };
			vertex.textureCoordinates = { attrib.texcoords[2 * index.texcoord_index + 0],
										  1 - attrib.texcoords[2 * index.texcoord_index + 1] };
			if (uniqueVertices.find(vertex) == uniqueVertices.end()) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
	vertexBuffer = make_shared<VertexBuffer>(context, vertices);
	indexBuffer = make_shared<IndexBuffer>(context, indices);
}
