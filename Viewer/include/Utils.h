#pragma once
#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"

class Utils
{
public:
	static glm::vec3 Vec3fFromStream(std::istream& issLine);
	static glm::vec2 Vec2fFromStream(std::istream& issLine);
	// Assignment 1b.1
	// Create string stream from vertices
	static std::string StringFromVec3f(glm::vec3 vertex);
	// Create string stream from faces
	static std::string StringFromFace(Face face);

	// Assignment 1b.3
	static glm::vec3 Vec3fFromVec4(glm::vec4 homogenousVec4);
	static glm::ivec2 Ivec2FromVec3WithAxisIgnoreSelect(glm::vec3 threeDimensionalVec, char axis);
	static glm::ivec2 Ivec2FromVec4AfterProjection(glm::vec4& Vec, const float& width, const float& height);
	static glm::ivec2 Ivec2FromVec3AfterProjection(glm::vec3& Vec, const float& width, const float& height);



	static std::shared_ptr<MeshModel> LoadMeshModel(const std::string& filePath);
	static std::string GetFileName(const std::string& filePath);
};
