#include "MeshModel.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> _vertices, std::vector<glm::vec3> _normals, std::vector<glm::vec2> _UVCoordinates, const std::string& model_name) :
	faces(faces),
	//vertices(_vertices),
	//normals(_normals),
	//UVCoordinates(_UVCoordinates),
	localTransformation(mat4(1.0f)),
	worldTransformation(mat4(1.0f)),
	localScale(mat4(1.0f)),
	localTranslation(mat4(1.0f)),
	localRotation(mat4(1.0f)),
	worldScale(mat4(1.0f)),
	worldTranslation(mat4(1.0f)),
	worldRotation(mat4(1.0f)),
	textureMap(NULL),
	modelTexture(NULL)
{
	UpdateRotation();

	useTextureAsNormalMap = false;
	DrawBoundingBox = false;
	DrawFaceNormals = false;
	DrawVertexNormals = false;
	DrawModelAxis = false;
	DrawModelFrameAxis = false;
	DrawWorldFrameAxis = false;
	textureMapBool = false;
	normalFaceScale = 120.f;
	normalVertexScale = 120.f;
	ModelAxisScale = 0.5f;
	ModelFrameAxisScale = 0.5f;
	WorldFrameAxisScale = 0.5f;
	textureRadius = 1.5f;
	textureType = 1;



	//	model colors-set
	modelAmbientColor = glm::vec3(0, 0, 0);
	modelDiffuseColor = glm::vec3(0.7f, 0.3f, 0.4f);
	modelSpecularColor = glm::vec3(0.7f, 0.3f, 0.4f);
	SpecularExponent = 2;

	//vertices.clear();
	//normals.clear();
	//UVCoordinates.clear();

	// reserve continuous memory for the model buffer 
	vertices.reserve(faces.size() * 3);
	normals.reserve(faces.size() * 3);
	UVCoordinates.reserve(faces.size() * 3);

	// building up the vertices according to their order of appearance in the faces vector. 
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces[i];
		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;
			int normalIndex = currentFace.GetNormalIndex(j) - 1;

			vertices.push_back(_vertices[vertexIndex]);
			normals.push_back(_normals[normalIndex]);

			if (_UVCoordinates.size() > 0)
			{
				int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
				UVCoordinates.push_back(_UVCoordinates[textureCoordsIndex]);
			}
		}
	}
	// averting memory errors
	if (normals.empty())
	{
		normals.push_back(vec3(0));
	}
	if (UVCoordinates.empty())
	{
		UVCoordinates.push_back(vec2(0));
	}

	// building model buffer
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3) + normals.size() * sizeof(vec3) + UVCoordinates.size() * sizeof(vec2), NULL, GL_STATIC_DRAW);

	// inserting the subpart of the model buffers
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vec3), &vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), normals.size() * sizeof(vec3), &normals[0]);
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3) + normals.size() * sizeof(vec3), UVCoordinates.size() * sizeof(vec2), &UVCoordinates[0]);

	// binding attributes
	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid*)(vertices.size() * sizeof(vec3)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (GLvoid*)(vertices.size() * sizeof(vec3) + normals.size() * sizeof(vec3)));
	glEnableVertexAttribArray(2);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);


	// we used the reference code at the beginning 
	////	building vector of the model vertices. refernce-copy
	//modelVertices.reserve(3 * faces.size());
	//for (int i = 0; i < faces.size(); i++)
	//{
	//	Face currentFace = faces.at(i);
	//	for (int j = 0; j < 3; j++)
	//	{
	//		Vertex vertex;
	//		int vertexIndex = currentFace.GetVertexIndex(j) - 1;
	//		int normalIndex = currentFace.GetNormalIndex(j) - 1;

	//		vertex.position = vertices[vertexIndex];
	//		vertex.normal = normals[normalIndex];

	//		if (UVCoordinates.size() > 0)
	//		{
	//			int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
	//			vertex.textureCoords = UVCoordinates[textureCoordsIndex];
	//		}
	//		modelVertices.push_back(vertex);
	//	}
	//}

	//// binding model stracture to the VAO of the model in form of vertieces that each one have his position, normal and UV.
	//glGenVertexArrays(1, &vao);
	//glGenBuffers(1, &vbo);

	//glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

	//// Vertex Positions
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	//glEnableVertexAttribArray(0);

	//// Normals attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);

	//// Vertex Texture Coords
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(2);

	//// unbind to make sure other code does not change it somewhere else
	//glBindVertexArray(0);

}

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces[index];
}

Face& MeshModel::GetFaceToSet(int index)
{
	return faces[index];
}

int MeshModel::GetFacesCount() const
{
	return faces.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}

const glm::vec3 MeshModel::GetNormal(int index) const
{
	return normals[index];
}
// Assignment 1b.1
const glm::vec3& MeshModel::GetVertex(int index) const
{
	return vertices[index];
}

int MeshModel::GetVerticesCount() const
{
	return vertices.size();
}

void MeshModel::GetMinMaxVertices(glm::vec3& minVec, glm::vec3& maxVec) const
{
	minVec = minVerticesValues;
	maxVec = maxVerticesValues;
}

void MeshModel::SetMinMaxVertices(const glm::vec3& minVec, const glm::vec3& maxVec) 
{
	minVerticesValues = minVec;
	maxVerticesValues = maxVec;
}

//	Get/Set bounding box bool
void MeshModel::SetBoundingBox()
{
	DrawBoundingBox = !DrawBoundingBox;
}

bool MeshModel::GetBoundingBox() const
{
	return DrawBoundingBox;
}

//	Get/Set normal face bool
void MeshModel::SetNormalFace()
{
	DrawFaceNormals = !DrawFaceNormals;
}

bool MeshModel::GetNormalFace() const
{
	return DrawFaceNormals;
}
//	Get/Set normal face scale
void MeshModel::SetNormalFaceScale(const float& scale)
{
	normalFaceScale *= scale;
}

float MeshModel::GetNormalFaceScale() const
{
	return normalFaceScale;
}
//	Get/Set normal Vertex bool
void MeshModel::SetNormalVertex()
{
	DrawVertexNormals = !DrawVertexNormals;
}

bool MeshModel::GetNormalVertex() const
{
	return DrawVertexNormals;
}

//	Get/Set normal Vertex scale
void MeshModel::SetNormalVertexScale(const float& scale)
{
	normalVertexScale *= scale;
}

float MeshModel::GetNormalVertexScale() const
{
	return normalVertexScale;
}

//	Get/Set Model axis bool
void MeshModel::SetModelAxis()
{
	DrawModelAxis = !DrawModelAxis;
}

bool MeshModel::GetModelAxis() const
{
	return DrawModelAxis;
}
//	Get/Set Model axis scale
void MeshModel::SetModelAxisScale(const float& scale)
{
	ModelAxisScale *= scale;
}

float MeshModel::GetModelAxisScale() const
{
	return ModelAxisScale;
}

//	Get/Set Model-Frame axis bool
void MeshModel::SetModelFrameAxis()
{
	DrawModelFrameAxis = !DrawModelFrameAxis;
}

bool MeshModel::GetModelFrameAxis() const
{
	return DrawModelFrameAxis;
}

//	Get/Set Model-Frame axis scale
void MeshModel::SetModelFrameAxisScale(const float& scale)
{
	ModelFrameAxisScale *= scale;
}

float MeshModel::GetModelFrameAxisScale() const
{
	return ModelFrameAxisScale;
}

//	Get/Set World-Frame axis bool
void MeshModel::SetWorldFrameAxis()
{
	DrawWorldFrameAxis = !DrawWorldFrameAxis;
}

bool MeshModel::GetWorldFrameAxis() const
{
	return DrawWorldFrameAxis;
}

//	Get/Set World-Frame axis scale
void MeshModel::SetWorldFrameAxisScale(const float& scale)
{
	WorldFrameAxisScale *= scale;
}

float MeshModel::GetWorldFrameAxisScale() const
{
	return WorldFrameAxisScale;
}


// Get / Set light-colors

void MeshModel::SetAmbientLightColor(const glm::vec3& newColor)
{
	modelAmbientColor = newColor;
}
glm::vec3 MeshModel::GetAmbientLightColor() const
{
	return modelAmbientColor;
}

void MeshModel::SetDiffuseLightColor(const glm::vec3& newColor)
{
	modelDiffuseColor = newColor;
}
glm::vec3 MeshModel::GetDiffuseLightColor() const
{
	return modelDiffuseColor;
}

void MeshModel::SetSpecularLightColor(const glm::vec3& newColor)
{
	modelSpecularColor = newColor;
}
glm::vec3 MeshModel::GetSpecularLightColor() const
{
	return modelSpecularColor;
}

void MeshModel::SetSpecularExponent(const float& newExp)
{
	SpecularExponent = newExp;
}
float MeshModel::GetSpecularExponent() const
{
	return SpecularExponent;
}





// Assignment 1b.2
void MeshModel::ScaleToWindow(const float& windowSize)
{
	///
	float scale = 7000;
	glm::vec3 vec = { scale, scale, scale };
	ScaleObject(vec, true);
	///


	//localTransformation = glm::mat4(1.0f);

	//float minCoordinateVal = vertices[0][0];
	//float maxCoordinateVal = vertices[0][0];
	//for (int i = 0; i < vertices.size(); i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		minCoordinateVal = std::min(minCoordinateVal, vertices[i][j]);
	//		maxCoordinateVal = std::max(maxCoordinateVal, vertices[i][j]);
	//	}
	//}

	//float scale = 0.75 * (windowSize / (maxCoordinateVal - minCoordinateVal));
	//glm::vec3 vec = { scale, scale, scale };
	//ScaleObject(vec, true);
	//vec = { -minCoordinateVal, -minCoordinateVal, -minCoordinateVal };
	//TranslateObject(vec, true);
	//
	////PrintMatrix(localTransformation);
}

// Assignment 1b.3
glm::mat4 MeshModel::GetLocalTransformation() const
{
	return this->localTransformation;
}

glm::mat4 MeshModel::GetWorldTransformation() const
{
	return this->worldTransformation;
}

glm::mat4 MeshModel::GetWorldRotation() const
{
	return this->worldRotation;
}

glm::mat4 MeshModel::GetRotation() const
{
	return worldAndLocalRotation;
}


// Assignment 1b.4 Local Rotation transformations
void MeshModel::RotateObjectAxisX(const int& degree, const bool& localElseWorld)
{
	if (localElseWorld)
	{
		//localTransformation = MultiplyMatrix(localTransformation, RotationTransformation(degree, 'x'));
		localRotation = MultiplyMatrix(localRotation, RotationTransformation(degree, 'x'));
		UpdateLocal();
	}
		
	else
	{
		//worldRotation = MultiplyMatrix(RotationTransformation(degree, 'x'), worldRotation);
		worldRotation = MultiplyMatrix(worldRotation, RotationTransformation(degree, 'x'));
		UpdateWorld();
	}
		
	//PrintMatrix(worldTransformation);
}

void MeshModel::RotateObjectAxisY(const int& degree, const bool& localElseWorld)
{
	if (localElseWorld)
	{
		//localTransformation = MultiplyMatrix(localTransformation, RotationTransformation(degree, 'y'));
		localRotation = MultiplyMatrix(localRotation, RotationTransformation(degree, 'y'));
		UpdateLocal();
	}
		
	else
	{
		//worldRotation = MultiplyMatrix(RotationTransformation(degree, 'y'), worldRotation);
		worldRotation = MultiplyMatrix(worldRotation, RotationTransformation(degree, 'y'));
		UpdateWorld();
	}
		
	//PrintMatrix(worldTransformation);
}

void MeshModel::RotateObjectAxisZ(const int& degree, const bool& localElseWorld)
{
	if (localElseWorld)
	{
		//localTransformation = MultiplyMatrix(localTransformation, RotationTransformation(degree, 'z'));
		localRotation = MultiplyMatrix(localRotation, RotationTransformation(degree, 'z'));
		UpdateLocal();
	}
		
	else
	{
		//worldRotation = MultiplyMatrix(RotationTransformation(degree, 'z'), worldRotation);
		worldRotation = MultiplyMatrix(worldRotation, RotationTransformation(degree, 'z'));
		UpdateWorld();
	}
	//PrintMatrix(worldTransformation);
}

// Assignment 1b.4 Local Translation transformation, and of note: The multiplication is supposedly in the wrong order, but what it gives is uniform translation regardless of model size. Some wicked math stuff going on in here.
void MeshModel::TranslateObject(glm::vec3& moveVec, const bool& localElseWorld)
{
	if (localElseWorld)
	{
		//localTransformation = MultiplyMatrix(localTransformation, TranslatingTransformation(moveVec));
		localTranslation = MultiplyMatrix(TranslatingTransformation(moveVec), localTranslation);
		UpdateLocal();
	}
		
	else
	{
		worldTranslation = MultiplyMatrix(TranslatingTransformation(moveVec), worldTranslation);
		//worldTransformation = MultiplyMatrix(worldTransformation, TranslatingTransformation(moveVec));
		UpdateWorld();
	}
		
	//PrintMatrix(worldTransformation);
}

// Assignment 1b.4 Local Scale transformation
void MeshModel::ScaleObject(glm::vec3& moveVec, const bool& localElseWorld)
{
	if (localElseWorld)
	{
		//localTransformation = MultiplyMatrix(localTransformation, ScalingTransformation(moveVec));
		localScale = MultiplyMatrix(ScalingTransformation(moveVec), localScale);
		UpdateLocal();
	}
	else
	{
		worldScale = MultiplyMatrix(ScalingTransformation(moveVec), worldScale);
		//worldTransformation = MultiplyMatrix(worldTransformation, ScalingTransformation(moveVec));
		UpdateWorld();
	}
		
}


void MeshModel::UpdateLocal()
{
	localTransformation = MultiplyMatrix(MultiplyMatrix(localTranslation, localRotation), localScale);
	UpdateRotation();
}


void MeshModel::UpdateWorld()
{
	worldTransformation = MultiplyMatrix(MultiplyMatrix(worldTranslation, worldRotation), worldScale);
	UpdateRotation();
}


void MeshModel::UpdateRotation()
{
	worldAndLocalRotation = MultiplyMatrix(worldRotation, localRotation);
}

void MeshModel::buildTextureMap(char* atlasPath)
{
	textureMapHeight;
	textureMapWidth;
	channels = 3;

	textureMap = stbi_load(atlasPath, &textureMapWidth, &textureMapHeight, &channels, STBI_rgb_alpha);

	if (textureMap == NULL)
	{
		std::cerr << "Error loading texture in location: '" << atlasPath << "'" << std::endl;
	}
	else
	{

		// image 'rotation' in 180 degrees - reference inspiration
		int widthInBytes = textureMapWidth * 4;
		unsigned char temp;
		int halfHeight = textureMapHeight / 2;
		for (int row = 0; row < halfHeight; row++)
		{
			for (int col = 0; col < widthInBytes; col++)
			{
				temp = textureMap[row * widthInBytes + col];
				textureMap[row * widthInBytes + col] = textureMap[(textureMapHeight - row - 1) * widthInBytes + col];
				textureMap[(textureMapHeight - row - 1) * widthInBytes + col] = temp;
			}
		}

		// building texture to model
		glGenTextures(1, &modelTexture);
		glBindTexture(GL_TEXTURE_2D, modelTexture); 

		// As i understood S and T are the U and V coordinates Respectively of the texure. Telling to use for them repeating technique
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Injecting the image to the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureMapWidth, textureMapHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureMap);
		glGenerateMipmap(GL_TEXTURE_2D);

		// unbind texture when done so we don't accidentally mess up our modelTexture
		glBindTexture(GL_TEXTURE_2D, 0); 
		
	}

	free(textureMap);

}

glm::vec3 MeshModel::GetColorFromTexture(const glm::vec2& UVCoordinates) const
{
	int uPixel = UVCoordinates.x * textureMapWidth;
	int vPixel = textureMapHeight - UVCoordinates.y * textureMapHeight;

	glm::vec3 color;
	unsigned char* pixelSetOf = textureMap + (uPixel + textureMapWidth * vPixel) * channels;
	for (int k = 0; k < 3; k++)
	{
		color[k] = pixelSetOf[k];
	}


	return color/255.0f;
}

GLuint MeshModel::GetModelTexture()
{
	return modelTexture;
}

glm::vec2 MeshModel::GetUVCoordinates(const int& index) const
{
	return UVCoordinates[index];
}

void MeshModel::SetTextureMapBool()
{
	textureMapBool = !textureMapBool;
}
bool MeshModel::GetTextureMapBool() const
{
	return textureMapBool;
}

int MeshModel::GetTextureType() const
{
	return textureType;
}

void MeshModel::SetTextureType(int setter)
{
	textureType = setter;
}

float MeshModel::GetTextureRadius() const
{
	return textureRadius;
}

void MeshModel::SetTextureRadius(float setter)
{
	textureRadius = setter;
}

bool MeshModel::GetUseNormalMap() const
{
	return useTextureAsNormalMap;
}

void MeshModel::SetUseNormalMap()
{
	useTextureAsNormalMap = !useTextureAsNormalMap;
}



// assignment 3 adding

GLuint MeshModel::GetVAO() const
{
	return vao;
}

int MeshModel::GetModelSize() const
{
	return vertices.size();
}
//