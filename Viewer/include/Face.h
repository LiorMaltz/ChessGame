#pragma once
#include "vector"
#include <string>
#include <glm/glm.hpp>

class Face
{
public:
	Face(std::istream& issLine);
	int GetVertexIndex(int index) const;
	int GetNormalIndex(int index) const;
	int GetTextureIndex(int index) const;

	void SetFaceColor();
	glm::vec3 GetFaceColor() const;

	void SetAverageVertex(const glm::vec3& setter);
	glm::vec3 GetAverageVertex() const;
	void SetFaceNormal(const glm::vec3& setter);
	glm::vec3 GetFaceNormal() const;

private:
	std::vector<int> vertex_indices;
	std::vector<int> normal_indices;
	std::vector<int> texture_indices;

	glm::vec3 faceColor;

	glm::vec3 averageVertex;
	glm::vec3 faceNormal;
};