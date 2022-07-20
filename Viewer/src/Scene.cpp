#include "Scene.h"
#include <string>
#include <iostream>


#include "stb_image.h"

//
#define M_PI 3.14159265358979323846
//

Scene::Scene(const float& windowwidth, const float& windowheight) :
	active_camera_index(0),
	active_model_index(0),
	worldTransformation(glm::mat4(1)),
	active_mode(false),
	useEnvironmentTexture(false)
{
	windowHeight = windowheight;
	windowWidth = windowwidth;
	//
	std::shared_ptr<Camera> firstCamera=std::make_shared<Camera>(windowWidth,windowHeight);
	AddCamera(firstCamera);
	active_light_index = 0;
	//

	//scene affects
	fogEffect = false;
	fogType = 1;
	fogColor = glm::vec3(0.5f, 0.5f, 0.5f);
	fogDensity = 0.02;

	useToonShading = false;
	levelsForToon = 3;
}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model, int minWindowSize)
{
	mesh_models.push_back(mesh_model);
	// Assignment 1b.1
	//printModelVerticesAndFaces(*mesh_models.back());
	findMaxMinVerticesOfModel(*mesh_models.back());
	// Assignment 1b.2 - Pre-transform model and prepare the current transform state matrix for the model
	//mesh_models.back()->ScaleToWindow(float(minWindowSize));

	// set average vertex and face normal for each vertex
	for (int i = 0; i < mesh_models.back()->GetFacesCount(); i++)
	{
		vector<glm::vec3> originalVertices;
		Face currentFace = mesh_models.back()->GetFace(i);
		for (int j = 0; j < 3; j++)
		{
			originalVertices.push_back(mesh_models.back()->GetVertex(currentFace.GetVertexIndex(j)-1));
		}
		glm::vec3 averageVertex = { 0,0,0 };
		glm::vec3 normal = glm::vec3(glm::normalize(glm::cross(originalVertices[0] - originalVertices[1], originalVertices[0] - originalVertices[2])));
		for (int j = 0; j < 3; j++)
		{
			averageVertex += glm::vec3(originalVertices[j]);
		}
		averageVertex = { averageVertex.x / 3.0,averageVertex.y / 3.0 ,averageVertex.z / 3.0};
		//normal += averageVertex;
		
		mesh_models.back()->GetFaceToSet(i).SetAverageVertex(averageVertex);
		mesh_models.back()->GetFaceToSet(i).SetFaceNormal(normal);
	}

}

int Scene::GetModelCount() const
{
	return mesh_models.size();
}

MeshModel& Scene::GetModel(int index) const
{
	return *mesh_models[index];
}

MeshModel& Scene::GetActiveModel() const
{
	return *mesh_models[active_model_index];
}


void Scene::SetActiveMode(const bool& mode)
{
	active_mode = mode;
}
bool Scene::GetActiveMode() const
{
	return active_mode;
}

//scene affects
void Scene::SetFogEffect()
{
	fogEffect = !fogEffect;
}
bool Scene::GetFogEffect() const
{
	return fogEffect;
}

void Scene::SetFogType(const int& type)
{
	fogType = type;
}
int Scene::GetFogType() const
{
	return fogType;
}

void Scene::SetFogDensity(const float& setter)
{
	fogDensity = setter;
}
float Scene::GetFogDensity() const
{
	return fogDensity;
}

void Scene::SetFogColor(const glm::vec3& fogColorSetter)
{
	fogColor = fogColorSetter;
}
glm::vec3 Scene::GetFogColor() const
{
	return fogColor;
}





// set the min and max coordinates of a model to print it's bounding box
void Scene::findMaxMinVerticesOfModel(MeshModel& model)
{
	glm::vec4 minCoordinates = { model.GetVertex(0),1 };
	glm::vec4 maxCoordinates = minCoordinates;
	for (int j = 0; j < model.GetVerticesCount(); j++)
	{
		glm::vec3 candidate = model.GetVertex(j);
		minCoordinates.x = std::min(minCoordinates.x, candidate.x);
		maxCoordinates.x = std::max(maxCoordinates.x, candidate.x);
		minCoordinates.y = std::min(minCoordinates.y, candidate.y);
		maxCoordinates.y = std::max(maxCoordinates.y, candidate.y);
		minCoordinates.z = std::min(minCoordinates.z, candidate.z);
		maxCoordinates.z = std::max(maxCoordinates.z, candidate.z);

	}
	model.SetMinMaxVertices(minCoordinates, maxCoordinates);
}
// Assignment 1b.1 - Print model's vertices and faces to the console
void Scene::printModelVerticesAndFaces(const MeshModel& model) const
{
	for (int i = 0; i < model.GetVerticesCount(); i++)
	{
		std::cout << Utils::StringFromVec3f(model.GetVertex(i)) <<  endl;
	}

	for (int i = 0; i < model.GetFacesCount(); i++)
	{

		std::cout << Utils::StringFromFace(model.GetFace(i)) << endl;
	}
}

void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
	cameras.push_back(camera);
}

int Scene::GetCameraCount() const
{
	return cameras.size();
}

Camera& Scene::GetCamera(int index) const //made it const for renderer
{
	return *cameras[index];
}

Camera& Scene::GetActiveCamera() const	//made it const for renderer
{
	return *cameras[active_camera_index];
}

// Light segment
void Scene::AddLight(const shared_ptr<Light>& newLight)
{
	lights.push_back(newLight);
}
int Scene::GetLightCount() const
{
	return lights.size();
}
Light& Scene::GetActiveLight() const
{
	return *lights[active_light_index];
}



// Get/Sets
void Scene::SetActiveCameraIndex(int index)
{
	active_camera_index = index;
}

int Scene::GetActiveCameraIndex() const
{
	return active_camera_index;
}

void Scene::SetActiveModelIndex(int index)
{
	active_model_index = index;
}

int Scene::GetActiveModelIndex() const
{
	return active_model_index;
}

void Scene::SetActiveLightIndex(int index)
{
	active_light_index = index;
}

int Scene::GetActiveLightIndex() const
{
	return active_light_index;
}


// Assignment 1b.4  World rotation methods
void Scene::RotateWorldAxisX(const int& degree)
{
	worldTransformation = MultiplyMatrix(worldTransformation, RotationTransformation(degree, 'x'));
	//PrintMatrix(worldTransformation);
}

void Scene::RotateWorldAxisY(const int& degree)
{
	worldTransformation = MultiplyMatrix(worldTransformation, RotationTransformation(degree, 'y'));
	//PrintMatrix(worldTransformation);
}

void Scene::RotateWorldAxisZ(const int& degree)
{
	worldTransformation = MultiplyMatrix(worldTransformation,  RotationTransformation(degree,'z'));
	//PrintMatrix(worldTransformation);
}

// Assignment 1b.4  World translation method
void Scene::TranslateWorld(glm::vec3& moveVec)
{
	worldTransformation = MultiplyMatrix(worldTransformation, TranslatingTransformation(moveVec));
	//PrintMatrix(worldTransformation);
}

glm::mat4 Scene::GetWorldTransformation() const
{
	return worldTransformation;
}

// Assignment 1b.4  World scale methods
void Scene::ScaleWorld(glm::vec3& moveVec)
{
	worldTransformation = MultiplyMatrix(worldTransformation, ScalingTransformation(moveVec));
}

// loading cube map texture

void Scene::LoadCubeMapTexture()
{
	// the skeletion of the cube that we'll print on it the texture. conssist of 6 faces that each is assembled from 2 triangles
	float skyboxVertices[] = 
	{
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};


	// Create VAO and VBO for the skybox
	glGenVertexArrays(1, &sceneVAO);
	glGenBuffers(1, &sceneVBO);
	glBindVertexArray(sceneVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sceneVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);




	vector<std::string> faces;
	faces.push_back("../Data/Textures/CubeTextures/skybox/righter.png");
	faces.push_back("../Data/Textures/CubeTextures/skybox/left.png");
	faces.push_back("../Data/Textures/CubeTextures/skybox/top.png");
	faces.push_back("../Data/Textures/CubeTextures/skybox/bottom.png");
	faces.push_back("../Data/Textures/CubeTextures/skybox/front.png");
	faces.push_back("../Data/Textures/CubeTextures/skybox/back.png");


	int width, height, Channels;

	glGenTextures(1, &sceneTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sceneTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &Channels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Error loading texture in location: '" << faces[i] << "'" << std::endl;
		}
		stbi_image_free(data);
	}
}

GLuint Scene::GetSceneTexture() const
{
	return sceneTexture;
}

GLuint Scene::GetSceneVAO() const
{
	return sceneVAO;
}

void Scene::SetUseEnvironmentTexture()
{
	useEnvironmentTexture = !useEnvironmentTexture;
}
bool Scene::GetUseEnvironmentTexture() const
{
	return useEnvironmentTexture;
}

void Scene::SetuseToonShading()
{
	useToonShading = !useToonShading;
}
bool Scene::GetuseToonShading() const
{
	return useToonShading;
}

void Scene::SetLevelsForToon(float setter)
{
	levelsForToon = setter;
}
float Scene::GetLevelsForToon() const
{
	return levelsForToon;
}