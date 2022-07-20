#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include "Face.h"
#include <glm/gtc/matrix_transform.hpp>

//

#include "Matrix.h"
//

using namespace glm;

// at first we used the referance code
//struct Vertex
//{
//	glm::vec3 position;
//	glm::vec3 normal;
//	glm::vec2 textureCoords;
//};

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> UVCoordinatesTemp, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	Face& GetFaceToSet(int index);
	int GetFacesCount() const;
	const std::string& GetModelName() const;

	// - Get vertices, vertex count, vertex-normals
	const glm::vec3& GetVertex(int index) const;
	const glm::vec3 GetNormal(int index) const;
	int GetVerticesCount() const;


	//	Get/Set min and max coordinates of the model
	void GetMinMaxVertices(glm::vec3& minVec, glm::vec3& maxVec) const;
	void SetMinMaxVertices(const glm::vec3& minVec, const glm:: vec3& maxVec);

	//	Get/Set bounding box bool
	void SetBoundingBox();
	bool GetBoundingBox() const;

	//	Get/Set normal face bool
	void SetNormalFace();
	bool GetNormalFace() const;

	//	Get/Set normal face scale
	void SetNormalFaceScale(const float& scale);
	float GetNormalFaceScale() const;

	//	Get/Set normal face bool
	void SetNormalVertex();
	bool GetNormalVertex() const;

	//	Get/Set normal face scale
	void SetNormalVertexScale(const float& scale);
	float GetNormalVertexScale() const;

	//	Get/Set model bool
	void SetModelAxis();
	bool GetModelAxis() const;

	//	Get/Set model scale
	void SetModelAxisScale(const float& scale);
	float GetModelAxisScale() const;


	//	Get/Set model frame bool
	void SetModelFrameAxis();
	bool GetModelFrameAxis() const;

	//	Get/Set model frame scale
	void SetModelFrameAxisScale(const float& scale);
	float GetModelFrameAxisScale() const;


	//	Get/Set world frame bool
	void SetWorldFrameAxis();
	bool GetWorldFrameAxis() const;

	//	Get/Set world frame scale
	void SetWorldFrameAxisScale(const float& scale);
	float GetWorldFrameAxisScale() const;

	// Get/Set model colors
	void SetAmbientLightColor(const glm::vec3& newColor);
	glm::vec3 GetAmbientLightColor() const;

	void SetDiffuseLightColor(const glm::vec3& newColor);
	glm::vec3 GetDiffuseLightColor() const;

	void SetSpecularLightColor(const glm::vec3& newColor);
	glm::vec3 GetSpecularLightColor() const;

	void SetSpecularExponent(const float& newExp);
	float GetSpecularExponent() const;

	glm::mat4 GetRotation() const;

	// Assignment 1b.2 - Calculate min/max vertices, and set the localTransformation matrix accordingly
	void ScaleToWindow(const float& windowSize);
	void TranslateObject(glm::vec3& moveVec, const bool& localElseWorld);
	void ScaleObject(glm::vec3& scaleVec, const bool& localElseWorld);
	void RotateObjectAxisX(const int& degree, const bool& localElseWorld);
	void RotateObjectAxisY(const int& degree, const bool& localElseWorld);
	void RotateObjectAxisZ(const int& degree, const bool& localElseWorld);
	void UpdateLocal();
	void UpdateWorld();
	void UpdateRotation();

	glm::mat4 GetLocalTransformation() const;
	glm::mat4 GetWorldTransformation() const;
	glm::mat4 GetWorldRotation() const;

	// texture map functions
	void buildTextureMap(char* atlasPath);
	glm::vec3 GetColorFromTexture(const glm::vec2& UVCoordinates) const;
	glm::vec2 GetUVCoordinates(const int& index) const;
	void SetTextureMapBool();
	bool GetTextureMapBool() const;
	GLuint GetModelTexture();
	int GetTextureType() const;
	void SetTextureType(int setter);
	float GetTextureRadius() const;
	void SetTextureRadius(float setter);
	bool GetUseNormalMap() const;
	void SetUseNormalMap();

	// assignment 3 adding

	GLuint GetVAO() const;
	int GetModelSize() const;

	//

private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> TextureUV;

	//std::vector<Vertex> modelVertices;


	std::string model_name;

	glm::vec3 maxVerticesValues;
	glm::vec3 minVerticesValues;
	// Assignment 1b.2 - Translate and scale vertices to the MeshViewer window's size
	glm::mat4 localTransformation;
	glm::mat4 worldTransformation;

	glm::mat4 localScale;
	glm::mat4 localTranslation;
	glm::mat4 localRotation;

	glm::mat4 worldScale;
	glm::mat4 worldTranslation;
	glm::mat4 worldRotation;

	glm::mat4 worldAndLocalRotation;

	// model colors
	glm::vec3 modelAmbientColor;
	glm::vec3 modelDiffuseColor;
	glm::vec3 modelSpecularColor;
	float SpecularExponent;

	bool useTextureAsNormalMap;
	bool DrawBoundingBox;
	bool DrawFaceNormals;
	bool DrawVertexNormals;
	bool DrawModelAxis;
	bool DrawModelFrameAxis;
	bool DrawWorldFrameAxis;
	float normalFaceScale;
	float normalVertexScale;
	float ModelAxisScale;
	float ModelFrameAxisScale;
	float WorldFrameAxisScale;

	// texture map 
	GLuint modelTexture;
	std::vector<glm::vec2> UVCoordinates;
	unsigned char* textureMap;
	int textureMapWidth;
	int textureMapHeight;
	bool textureMapBool;
	int channels;
	int textureType;
	float textureRadius;
	//

	// assignment 3 adding
	// openGl
	GLuint vbo;
	GLuint vao;

	int modelSize;
	//
};
