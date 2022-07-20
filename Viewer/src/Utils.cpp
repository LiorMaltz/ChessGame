#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Utils.h"

glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y ;
	return glm::vec2(x, y);
}
// Assignment 1b.1
std::string Utils::StringFromVec3f(glm::vec3 vertex)
{
	return std::to_string(vertex.x) + " " + std::to_string(vertex.y) + " " + std::to_string(vertex.z);
}
// Assignment 1b.1
std::string Utils::StringFromFace(Face face)
{
	std::string output = "";
	int TextureIndex, NormalIndex;
	//std::vector<int> faceVertex = face.GetVertexIndex();
	for (int i = 0; i < 3; i++)
	{
		TextureIndex = face.GetTextureIndex(i);
		NormalIndex = face.GetNormalIndex(i);
		if (NormalIndex == NULL && TextureIndex == NULL)
		{
			output += std::to_string(face.GetVertexIndex(i));
		}
		else if (NormalIndex == NULL)
		{
			output += std::to_string(face.GetVertexIndex(i)) + "/" + std::to_string(face.GetTextureIndex(i));
		}
		else
		{
			output += std::to_string(face.GetVertexIndex(i)) + "//" + std::to_string(face.GetNormalIndex(i));
		}

		if (i < 2)
		{
			output += " ";
		}
	}

	return output;
}


std::shared_ptr<MeshModel> Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureUV;
	std::ifstream ifile(filePath.c_str());

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			vertices.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vn")
		{
			normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			textureUV.push_back(Utils::Vec2fFromStream(issLine));
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}

	return std::make_shared<MeshModel>(faces, vertices, normals, textureUV, Utils::GetFileName(filePath));
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}

glm::vec3 Utils::Vec3fFromVec4(glm::vec4 homogenousVec4)
{
	glm::vec3 carthesianVec3 = glm::vec3(1.0f);
	carthesianVec3.x = homogenousVec4.x / homogenousVec4.w;
	carthesianVec3.y = homogenousVec4.y / homogenousVec4.w;
	carthesianVec3.z = homogenousVec4.z / homogenousVec4.w;
	return carthesianVec3;
}

// useless for now
glm::ivec2 Utils::Ivec2FromVec3WithAxisIgnoreSelect(glm::vec3 threeDimensionalVec, char axis)
{
	glm::ivec2 point = glm::ivec2(0);
	switch (axis)
	{
		case 'x':
			point.x = int(threeDimensionalVec.y);
			point.y = int(threeDimensionalVec.z);
			break;
		case 'y':
			point.x = int(threeDimensionalVec.x);
			point.y = int(threeDimensionalVec.z);
			break;
		case 'z':
			point.x = int(threeDimensionalVec.x);
			point.y = int(threeDimensionalVec.y);
			break;
	}
	return point;
}

// Creates a pixel-ready ivec2 from an ivec4 point in the canonical view volume
glm::ivec2 Utils::Ivec2FromVec4AfterProjection(glm::vec4& Vec,const float& width, const float& height)
{
	Vec.x = Vec.x / Vec.w;
	Vec.y = Vec.y / Vec.w;

	Vec = (Vec + glm::vec4(1, 1, 0,0));
	Vec.x *= (width / 2);
	Vec.y *= (height / 2);
	glm::ivec2 result = { int(Vec.x),int(Vec.y) };
	return result;
}
glm::ivec2 Utils::Ivec2FromVec3AfterProjection(glm::vec3& Vec, const float& width, const float& height)
{
	Vec = (Vec + glm::vec3(1, 1, 0));
	Vec.x *= (width / 2);
	Vec.y *= (height / 2);
	glm::ivec2 result = { int(Vec.x),int(Vec.y) };
	return result;
}


