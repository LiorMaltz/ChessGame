#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "MeshModel.h"
#include "Utils.h"
#include "Light.h"

using namespace std;

class Scene {
public:
	Scene(const float& windowwidth, const float& windowheight);

	void AddModel(const shared_ptr<MeshModel>& mesh_model);
	int GetModelCount() const;
	MeshModel& GetModel(int index) const;
	MeshModel& GetActiveModel() const;

	// Assignment 1b.1 - Print model's vertices and faces to the console
	void printModelVerticesAndFaces(const MeshModel& model) const;
	void findMaxMinVerticesOfModel(MeshModel& model);
	
	void AddCamera(const shared_ptr<Camera>& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index) const;
	Camera& GetActiveCamera() const;

	// Light segemnt
	void AddLight(const shared_ptr<Light>& newLight);
	int GetLightCount() const;
	Light& GetActiveLight() const;

	// Get/Set
	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;

	// we will support only one active light for now
	void SetActiveLightIndex(int index);
	int GetActiveLightIndex() const;

	// Assignment 1b.6 Set and get the index of current active model in the scene
	void SetActiveMode(const bool& mode);
	bool GetActiveMode() const;

	//scene affects
	void SetFogEffect();
	bool GetFogEffect() const;

	void SetFogType(const int& type);
	int GetFogType() const;

	void SetFogDensity(const float& type);
	float GetFogDensity() const;

	void SetFogColor(const glm::vec3& fogColor);
	glm::vec3 GetFogColor() const;

	// Assignment 1b.4 World Transformation methods
	void RotateWorldAxisX(const int& degree);
	void RotateWorldAxisY(const int& degree);
	void RotateWorldAxisZ(const int& degree);
	void TranslateWorld(glm::vec3& moveVec);
	void ScaleWorld(glm::vec3& moveVec);
	glm::mat4 GetWorldTransformation() const;

	void LoadCubeMapTexture();
	GLuint GetSceneTexture() const;
	GLuint GetSceneVAO() const;
	void SetUseEnvironmentTexture(); 
	bool GetUseEnvironmentTexture() const;
	void SetuseToonShading();
	bool GetuseToonShading() const;
	void SetLevelsForToon(float setter);
	float GetLevelsForToon() const;
	
private:
	vector<shared_ptr<MeshModel>> mesh_models;
	vector<shared_ptr<Camera>> cameras;
	vector<shared_ptr<Light>> lights;

	int active_camera_index;
	int active_model_index;
	int active_light_index;
	bool active_mode;
	bool useEnvironmentTexture;
	bool useToonShading;
	float levelsForToon;

	//fog affect
	glm::vec3 fogColor;
	bool fogEffect;
	int fogType;
	float fogDensity;

	// Assignment 1b.4 Current World transform matrix
	glm::mat4 worldTransformation;

	// screen width, height for camera activation
	float windowWidth, windowHeight;

	// CubeMap texture
	GLuint sceneTexture;
	GLuint sceneVAO, sceneVBO;

};