#include "ImGui.h"



// Assignment 1b.4 - Various additional variables
// Scene specific rotation vector
int screenWidth=0, screenHeight=0;
int oldScreenWidth = 0, oldScreenHeight = 0;


//	Model Window 
vector<ivec3> modelLocalRotation;
vector<bool> DrawBoundingBox = { false };
vector<bool> DrawNormalFace = { false };
vector<bool> DrawNormalVertex = { false };
vector<bool> DrawModelAxis = { false };
vector<bool> DrawModelFrameAxis = { false };
vector<bool> UseTexture = { };
vector<bool> UseTextureAsNormalMap;
vector<int> textureType;
vector<float> textureRadius;
int activeModelIndex = -1;
float objectTranslation = 1.0f;


// model colors components
glm::vec3 currentModelAmbient;
glm::vec3 currentModelDiffuse;
glm::vec3 currentModelspecular;
float specularExponent;

//	World Window bools
bool activeMode = false;
bool useEnvironmentTexture = false;
bool useToonShading = false;
vector<bool> DrawWorldFrameAxis = { false };
ivec3 sceneWorldRotation = { 0,0,0 };
vector<ivec3> modelWorldRotation;
bool DrawBoundingRactangle = false ;
float worldTranslation = 1.0f;
int numberOfLevelsToon = 3;
//affects
bool fogEffect = false;
int fogType = 0.02;
float fogDensity;
glm::vec3 fogColor;
int MSAA = 4;
bool MSAABool = false;

//	camera window
vector<vector<float>> viewVolume;
vector<ivec3> CameraRotation = { { 0,0,0 } };
int activeCameraIndex = -1;
bool scaleWindowSize = false;
bool stretchWindowSize = false;
bool rightLeftSymmetric = false;
bool upDownSymmetric = false;
bool colorElseGreyMode = false;
bool fillFacesBool = false;
bool drawOutlines = false;
bool useLight = false;
int shadingType = 1;
vector<bool> cameraMode = { true };
// lookAt
glm::vec3 eye{ 0,0,-1 };
glm::vec3 at{ 0,0,0 };
glm::vec3 up{ 0,1,0 };
float cameraTranslation = 1.0f;
float scaleViewVolume = 1.05f;

//	light window
vector<ivec3> LightRotation = { { 0,0,0 } };
vector<bool> lightType = { true };
float lightTranslation = 1.0f;
// light colors components
glm::vec3 currentAmbient;
glm::vec3 currentDiffuse;
glm::vec3 currentspecular;
glm::vec3 lightDirection;
bool specularVectorBool = false;


int activeLightIndex = -1;
//	
bool initialization = true;
//	imgui windows bools
bool show_worldTransform_window = false;
bool show_localTransform_window = false;
bool show_cameraTransform_window = false;
bool show_light_window = false;
bool show_data_window = false;
bool InitializeWindows[5] = { true };

// debug

int highFace = -1;
int lowFace = -1;

// Add model not through imgui
void AddModel(nfdchar_t* outPath, Scene& scene, nfdchar_t* texturePath)
{
	// Add model to scene
	scene.AddModel(Utils::LoadMeshModel(outPath));
	//	Initialize the GUI's rotation vectors for each loaded model
	modelLocalRotation.push_back({ 0,0,0 });
	modelWorldRotation.push_back({ 0,0,0 });
	DrawWorldFrameAxis.push_back(false);
	DrawModelAxis.push_back(false);
	DrawBoundingBox.push_back(false);
	DrawNormalFace.push_back(false);
	DrawNormalVertex.push_back(false);
	DrawModelFrameAxis.push_back(false);
	
	textureType.push_back(1);
	textureRadius.push_back(1);
	UseTextureAsNormalMap.push_back(false);

	if (texturePath != NULL)
	{
		scene.GetModel(scene.GetModelCount() - 1).buildTextureMap(texturePath);
		UseTexture.push_back(true);
	}
	else
	{
		UseTexture.push_back(false);
	}

}


void MyImGui(ImGuiIO& io, Scene& scene, GLFWwindow* window)
{
	// Get current window's screenWidth and screenHeight for scaling purposes
	glfwGetWindowSize(window, &screenWidth, &screenHeight);
	// only in the first calling of Imgui
	
	if (initialization)
	{
		oldScreenHeight = screenHeight;
		oldScreenWidth = screenWidth;
		initialization = false;
		vector<float> viewVolumeTemp = { -screenWidth / 1000.f ,screenWidth / 1000.f ,-screenHeight / 1000.f,screenHeight / 1000.f,1.5f,5.0f };
		viewVolume.push_back(viewVolumeTemp);
	}
	

	// ImGui begins, maybe we should make use of this mandatory window? That, or remove it.
	//ImGui::Begin("MeshViewer Menu");
	//ImGui::Begin("");

	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					// Add model to scene
					scene.AddModel(Utils::LoadMeshModel(outPath));
					//	Initialize the GUI's rotation vectors for each loaded model
					modelLocalRotation.push_back({ 0,0,0 });
					modelWorldRotation.push_back({ 0,0,0 });
					DrawWorldFrameAxis.push_back(false);
					DrawModelAxis.push_back(false);
					DrawBoundingBox.push_back(false);
					DrawNormalFace.push_back(false);
					DrawNormalVertex.push_back(false);
					DrawModelFrameAxis.push_back(false);
					UseTexture.push_back(false);
					textureType.push_back(1);
					textureRadius.push_back(1);
					UseTextureAsNormalMap.push_back(false);
					
					free(outPath);

					
					// Note: Opening and closing the 'open file' dialog box creates a memory leak
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}
			}
			ImGui::EndMenu();
		}

		// World/Object transformations
		if (ImGui::BeginMenu("Transformations"))
		{
			if (ImGui::MenuItem("World"))
			{
				show_worldTransform_window = true;
				InitializeWindows[0] = true;
			}
			if (ImGui::MenuItem("Local"))
			{
				show_localTransform_window = true;
				InitializeWindows[1] = true;
			}
			if (ImGui::MenuItem("Camera"))
			{
				show_cameraTransform_window = true;
				InitializeWindows[3] = true;
			}
			if (ImGui::MenuItem("Lights"))
			{
				show_light_window = true;
				InitializeWindows[4] = true;
			}
			// Transformation matrices display
			if (ImGui::MenuItem("Show data"))
			{
				InitializeWindows[2] = true;
				show_data_window = true;
			}
			
			ImGui::EndMenu();
		}		
		ImGui::EndMainMenuBar();
	}

	if (show_worldTransform_window)
	{
		// Variable that stores the last rotation value in degrees for each model, or for the entire scene, depending on active mode.
		int lastXRotation, lastYRotation, lastZRotation;
		// Current active model's index.
		static int item_current = -1;

		ImGui::Begin("World Transormation window", &show_worldTransform_window);
		if (InitializeWindows[0])
		{
			InitializeWindows[0] = false;
			ImGui::SetWindowSize(ImVec2((float)screenWidth/2, (float)screenHeight/2));

			// reset fog color 
			fogColor = scene.GetFogColor();
			fogDensity = scene.GetFogDensity();


		}
		// Active mode on/off
		{
			ImGui::Text("ActiveMode: "); ImGui::SameLine();
			ImGui::Text("Yes"); ImGui::SameLine();
			if (ImGui::RadioButton("##ActiveMode1", activeMode == true))
			{
				activeMode = true;
			}
			ImGui::SameLine(); ImGui::Text("No"); ImGui::SameLine();
			if (ImGui::RadioButton("##ActiveMode2", activeMode == false))
			{
				activeMode = false;
			}
		}
		if (activeMode)
		{
			// Setup for a list of loaded models (using ImGui::Combo)

			char** models = new char* [scene.GetModelCount()];
			ContructsNames(scene.GetModelCount(), models,'m');
			int modelsSize = scene.GetModelCount();
			ImGui::Combo("Choose model", &item_current, models, modelsSize);
			DestructsNames(scene.GetModelCount(), models);
		}
		// As DJ Khaled once said: "Another one!"
		if (!(activeMode && item_current < 0))
		{	
			// World rotations
			ImGui::Text("Rotation sliders");
			// This pointer will be used to determine which current mode's rotation value needs to be changed/stored/related to - current model, or entire scene's.
			int* currentModeAxisValue;

			currentModeAxisValue = &(activeMode ? modelWorldRotation[item_current].x : sceneWorldRotation.x);
			lastXRotation = *currentModeAxisValue;

			ImGui::SliderInt("X Rotation slider", currentModeAxisValue, -360, 360);
			if (lastXRotation != *currentModeAxisValue)
			{
				// If activeMode is false, then we're changing the scene's world matrix, which globally changes all of the scene's models' world matrices as well.
				if (!activeMode)
				{
					for (int i = 0; i < scene.GetModelCount(); i++)
					{
						modelWorldRotation[i].x += sceneWorldRotation.x - lastXRotation;
						if (modelWorldRotation[i].x > 360)
							modelWorldRotation[i].x -= 720;
						if (modelWorldRotation[i].x < -360)
							modelWorldRotation[i].x += 720;
					}
				}
				RotateFunction(false, 'x', scene, item_current, *currentModeAxisValue - lastXRotation);
			}

			currentModeAxisValue = &(activeMode ? modelWorldRotation[item_current].y : sceneWorldRotation.y);
			lastYRotation = *currentModeAxisValue;


			// Same logic applies for Y and Z axis, the pointer is repurposed for each Axis separately.
			ImGui::SliderInt("Y Rotation slider", currentModeAxisValue, -360, 360);
			if (lastYRotation != *currentModeAxisValue)
			{
				if (!activeMode)
				{
					for (int i = 0; i < scene.GetModelCount(); i++)
					{
						modelWorldRotation[i].y += sceneWorldRotation.y - lastYRotation;
						if (modelWorldRotation[i].y > 360)
							modelWorldRotation[i].y -= 720;
						if (modelWorldRotation[i].y < -360)
							modelWorldRotation[i].y += 720;
					}
				}
				RotateFunction(false, 'y', scene, item_current, *currentModeAxisValue - lastYRotation);
			}

			currentModeAxisValue = &(activeMode ? modelWorldRotation[item_current].z : sceneWorldRotation.z);
			lastZRotation = *currentModeAxisValue;

			ImGui::SliderInt("Z Rotation slider", currentModeAxisValue, -360, 360);
			if (lastZRotation != *currentModeAxisValue)
			{
				if (!activeMode)
				{
					for (int i = 0; i < scene.GetModelCount(); i++)
					{
						modelWorldRotation[i].z += sceneWorldRotation.z - lastZRotation;
						if (modelWorldRotation[i].z > 360)
							modelWorldRotation[i].z -= 720;
						if (modelWorldRotation[i].z < -360)
							modelWorldRotation[i].z += 720;
					}
				}
				RotateFunction(false, 'z', scene, item_current, *currentModeAxisValue - lastZRotation);
			}

			// World Translations

			ImGui::PushButtonRepeat(true);
			ImGui::Text("Translate x: ");
			ImGui::SameLine();
			if (ImGui::Button("+##WTX"))
			{
				TranslateFunction(false, true, 'x', scene, item_current);
			}
			ImGui::SameLine();
			if (ImGui::Button("-##WTX"))
			{
				TranslateFunction(false, false, 'x', scene, item_current);
			}

			ImGui::SameLine(); ImGui::Text("Translate y: "); ImGui::SameLine();
			if (ImGui::Button("+##WTY"))
			{
				TranslateFunction(false, true, 'y', scene, item_current);
			}
			ImGui::SameLine();
			if (ImGui::Button("-##WTY"))
			{
				TranslateFunction(false, false, 'y', scene, item_current);
			}

			ImGui::SameLine(); ImGui::Text("Translate z: "); ImGui::SameLine();
			if (ImGui::Button("+##WTZ"))
			{
				TranslateFunction(false, true, 'z', scene, item_current);
			}
			ImGui::SameLine();
			if (ImGui::Button("-##WTZ"))
			{
				TranslateFunction(false, false, 'z', scene, item_current);
			}

			ImGui::SameLine();
			ImGui::Text("Set jump");
			ImGui::SameLine();
			ImGui::PushItemWidth(60);
			ImGui::InputFloat("##InputWorldTranslate", &worldTranslation);


			// World Scaling
			ImGui::Text("Scale Object: ");
			ImGui::SameLine();
			float scale = 1.03f;
			if (ImGui::Button("+##WS"))
			{
				ScaleFunction(false, true, scene, item_current, scale);
			}
			ImGui::SameLine();
			if (ImGui::Button("-##WS"))
			{
				ScaleFunction(false, false, scene, item_current, scale);
			}

			if (activeMode)
			{
				// World axis
				bool DrawWorldFrameAxisTemp = DrawWorldFrameAxis[item_current];
				ImGui::Text("Show World-Frame Axis:"); ImGui::SameLine();
				ImGui::Checkbox("##WorldFrameAxis", &DrawWorldFrameAxisTemp);
				if (DrawWorldFrameAxisTemp != DrawWorldFrameAxis[item_current])
				{
					DrawWorldFrameAxis[item_current] = DrawWorldFrameAxisTemp;
					scene.GetModel(item_current).SetWorldFrameAxis();
				}
				ImGui::SameLine();
				ImGui::Text("Scale World-Frame Axis:"); ImGui::SameLine();
				float worldFrameAxisScale = 1.1f;
				if (ImGui::Button("+##WorldAxis"))
				{
					scene.GetModel(item_current).SetWorldFrameAxisScale(worldFrameAxisScale);
				}
				ImGui::SameLine();
				if (ImGui::Button("-##WorldAxis"))
				{
					scene.GetModel(item_current).SetWorldFrameAxisScale(1 / worldFrameAxisScale);
				}
			}


			ImGui::PopButtonRepeat();
		}		

		// assingment 3a
		bool DrawBoundingRactangleTemp = DrawBoundingRactangle;
		ImGui::Text("Show Bounding-Ractangle for faces:"); ImGui::SameLine();
		ImGui::Checkbox("##Bounding-Ractangle", &DrawBoundingRactangleTemp);
		if (DrawBoundingRactangleTemp != DrawBoundingRactangle)
		{
			DrawBoundingRactangle = DrawBoundingRactangleTemp;
			//renderer.SetBoundingRectangle();
		}

		// scene affects
		bool fogaffectTemp = fogEffect;
		ImGui::Text("Fog effect:"); ImGui::SameLine();
		ImGui::Checkbox("##Fog", &fogEffect);
		if (fogaffectTemp != fogEffect)
		{
			scene.SetFogEffect();
		}
		if (fogEffect)
		{
			// set fog type
			int fogTypeTemp = fogType;
			ImGui::Text("Choose fog Type: "); ImGui::SameLine();
			ImGui::Text("Linear"); ImGui::SameLine();
			if (ImGui::RadioButton("##FogType1", fogType == 1))
			{
				fogType = 1;
			}
			ImGui::SameLine(); ImGui::Text("Exponential"); ImGui::SameLine();
			if (ImGui::RadioButton("##FogType2", fogType == 2))
			{
				fogType = 2;
			}
			ImGui::SameLine(); ImGui::Text("Exponential squared"); ImGui::SameLine();
			if (ImGui::RadioButton("##FogType3", fogType == 3))
			{
				fogType = 3;
			}
			if (fogType != fogTypeTemp)
			{
				scene.SetFogType(fogType);
			}

			// set fog color
			ImGui::PushItemWidth(180);
			
			ImGui::Text("Choose fog color: "); ImGui::SameLine();
			float fogColorTemp[3] = { fogColor.x, fogColor.y,  fogColor.z };
			ImGui::ColorEdit3("##FogColor", fogColorTemp);
			if (fogColor.x != fogColorTemp[0] || fogColor.y != fogColorTemp[1] || fogColor.z != fogColorTemp[2])
			{
				fogColor = { fogColorTemp[0] , fogColorTemp[1] , fogColorTemp[2] };
				scene.SetFogColor(fogColor);

			}

			if (fogType==2|| fogType==3)
			{
				float fogDensityTemp = fogDensity;
				ImGui::Text("Set fog Density: "); ImGui::SameLine();
				ImGui::PushItemWidth(90);
				ImGui::InputFloat("##fogDensity", &fogDensity);
				if (fogDensityTemp!= fogDensity)
				{
					scene.SetFogDensity(fogDensity);
				}
			}

		}

		// MSAA effect
		bool MSAABoolTemp = MSAABool;
		ImGui::Text("MSAA effect:"); ImGui::SameLine();
		ImGui::Checkbox("##MSAABool", &MSAABool);
		if (MSAABool != MSAABoolTemp)
		{
			//renderer.SetMSAABool();
		}
		if (MSAABool)
		{
			int MSAATemp = MSAA;
			ImGui::Text("Choose MSAA type: "); ImGui::SameLine();
			ImGui::Text("MSAA 4X"); ImGui::SameLine();
			if (ImGui::RadioButton("##MSAAType1", MSAA == 4))
			{
				MSAA = 4;
			}
			ImGui::SameLine(); ImGui::Text("MSAA 16X"); ImGui::SameLine();
			if (ImGui::RadioButton("##MSAAType2", MSAA == 16))
			{
				MSAA = 16;
			}

			if (MSAATemp != MSAA)
			{
				scene.SetFogType(MSAA);
			}
		}

		bool useToonShadingTemp = useToonShading;
		ImGui::Text("Use Toon Shading:"); ImGui::SameLine();
		ImGui::Checkbox("##ToonShading", &useToonShading);
		if (useToonShading != useToonShadingTemp)
		{
			scene.SetuseToonShading();
		}

		int numberOfLevelsToonTemp = numberOfLevelsToon;
		ImGui::SameLine();
		ImGui::Text("Number of levels:"); ImGui::SameLine();
		ImGui::InputInt("##ToonShadingLevels", &numberOfLevelsToon);

		if (numberOfLevelsToon != numberOfLevelsToonTemp)
		{
			scene.SetLevelsForToon(numberOfLevelsToon);
		}

		if (ImGui::Button("Load Environment Texture"))
		{
			scene.LoadCubeMapTexture();
		}

		bool useEnvironmentTextureTemp = useEnvironmentTexture;
		ImGui::Text("Use Environment Texture:"); ImGui::SameLine();
		ImGui::Checkbox("##EnvironmentTexture", &useEnvironmentTexture);
		if (useEnvironmentTextureTemp != useEnvironmentTexture)
		{
			scene.SetUseEnvironmentTexture();
		}


		ImGui::End();
	}

	// Objects Transformations
	if (show_localTransform_window)
	{
		ImGui::Begin("Local Transormation window", &show_localTransform_window);
		if (InitializeWindows[1])
		{
			InitializeWindows[1] = false;
			ImGui::SetWindowSize(ImVec2((float)screenWidth/2, (float)screenHeight/2));
		}

		// Setup for a list of loaded models (using ImGui::Combo)
		static int item_current = -1;
		char** models = new char* [scene.GetModelCount()];
		ContructsNames(scene.GetModelCount(), models,'m');
		int modelsSize = scene.GetModelCount();
		ImGui::Combo("Choose model", &item_current, models, modelsSize);
		DestructsNames(scene.GetModelCount(), models);

		if (item_current >= 0)
		{
			if (item_current!= activeModelIndex)
			{
				activeModelIndex = item_current;
				scene.SetActiveModelIndex(item_current);
				currentModelAmbient = scene.GetActiveModel().GetAmbientLightColor();
				currentModelDiffuse = scene.GetActiveModel().GetDiffuseLightColor();
				currentModelspecular = scene.GetActiveModel().GetSpecularLightColor();
				specularExponent = scene.GetActiveModel().GetSpecularExponent();

			}
			scene.SetActiveModelIndex(item_current);

			if (ImGui::TreeNode("Incrementation Transformations"))
			{
				int lastXRotation = modelLocalRotation[item_current].x;
				int lastYRotation = modelLocalRotation[item_current].y;
				int lastZRotation = modelLocalRotation[item_current].z;

				ImGui::Text("Object rotations:");
				ImGui::SliderInt("RotationX", &modelLocalRotation[item_current].x, -360, 360);
				if (lastXRotation != modelLocalRotation[item_current].x)
				{
					RotateFunction(true, 'x', scene, item_current, modelLocalRotation[item_current].x - lastXRotation);
				}

				ImGui::SliderInt("RotationY", &modelLocalRotation[item_current].y, -360, 360);
				if (lastYRotation != modelLocalRotation[item_current].y)
				{
					RotateFunction(true, 'y', scene, item_current, modelLocalRotation[item_current].y - lastYRotation);
				}

				ImGui::SliderInt("RotationZ", &modelLocalRotation[item_current].z, -360, 360);
				if (lastZRotation != modelLocalRotation[item_current].z)
				{
					RotateFunction(true, 'z', scene, item_current, modelLocalRotation[item_current].z - lastZRotation);
				}

				// Translating objects
				ImGui::PushButtonRepeat(true);
				ImGui::Text("Translate x: ");
				ImGui::SameLine();

				// For translations, we arbitrarily chose 6, but then normalized it for the current model's scale
				if (ImGui::Button("+##1"))
				{
					TranslateFunction(true, true, 'x', scene, item_current);
				}
				ImGui::SameLine();
				if (ImGui::Button("-##1"))
				{
					TranslateFunction(true, false, 'x', scene, item_current);
				}

				ImGui::SameLine();
				ImGui::Text("Translate y: ");
				ImGui::SameLine();
				if (ImGui::Button("+##2"))
				{
					TranslateFunction(true, true, 'y', scene, item_current);
				}
				ImGui::SameLine();
				if (ImGui::Button("-##2"))
				{
					TranslateFunction(true, false, 'y', scene, item_current);
				}

				ImGui::SameLine();
				ImGui::Text("Translate z: ");
				ImGui::SameLine();
				if (ImGui::Button("+##3"))
				{
					TranslateFunction(true, true, 'z', scene, item_current);
				}
				ImGui::SameLine();
				if (ImGui::Button("-##3"))
				{
					TranslateFunction(true, false, 'z', scene, item_current);
				}

				ImGui::SameLine();
				ImGui::Text("Translation interval: ");
				ImGui::SameLine();
				ImGui::PushItemWidth(60);
				ImGui::InputFloat("##InputObjectTranslate", &objectTranslation);

				// Scaling objects

				ImGui::Text("Scale Object: ");

				ImGui::SameLine();
				float modelScale = 1.1f;
				if (ImGui::Button("+##4"))
				{
					ScaleFunction(true, true, scene, item_current, modelScale);
				}
				ImGui::SameLine();
				if (ImGui::Button("-##4"))
				{
					ScaleFunction(true, false, scene, item_current, modelScale);
				}

				ImGui::PopButtonRepeat();

				ImGui::TreePop();
			}
			
			if (ImGui::TreeNode("Vizualized Data"))
			{
				ImGui::PushButtonRepeat(true);
				// bounding box imgui
				ImGui::Text("Draw Bounding-Box:"); ImGui::SameLine();
				bool LastDrawBoundingBox = DrawBoundingBox[item_current];
				ImGui::Checkbox("##boundingBox", &LastDrawBoundingBox);
				if (DrawBoundingBox[item_current] != LastDrawBoundingBox)
				{
					DrawBoundingBox[item_current] = LastDrawBoundingBox;
					scene.GetModel(item_current).SetBoundingBox();
				}

				// Normal face imgui
				ImGui::Text("Draw Normal-Face:"); ImGui::SameLine();
				bool LastDrawNormalFace = DrawNormalFace[item_current];
				ImGui::Checkbox("##NormalFace", &LastDrawNormalFace);
				if (DrawNormalFace[item_current] != LastDrawNormalFace)
				{
					DrawNormalFace[item_current] = LastDrawNormalFace;
					scene.GetModel(item_current).SetNormalFace();
				}
				ImGui::SameLine();
				ImGui::Text("Scale Normal-Face:"); ImGui::SameLine();
				float normalFaceScale = 1.1f;
				if (ImGui::Button("+##5"))
				{
					scene.GetModel(item_current).SetNormalFaceScale(1 / normalFaceScale);
				}
				ImGui::SameLine();
				if (ImGui::Button("-##5"))
				{
					scene.GetModel(item_current).SetNormalFaceScale(normalFaceScale);
				}

				// Normal vertex imgui
				ImGui::Text("Draw Normal-Vertex:"); ImGui::SameLine();
				bool LastDrawNormalVertex = DrawNormalVertex[item_current];
				ImGui::Checkbox("##NormalVertex", &LastDrawNormalVertex);
				if (DrawNormalVertex[item_current] != LastDrawNormalVertex)
				{
					DrawNormalVertex[item_current] = LastDrawNormalVertex;
					scene.GetModel(item_current).SetNormalVertex();
				}
				ImGui::SameLine();
				ImGui::Text("Scale Normal-Vertex:"); ImGui::SameLine();
				float normalVertexScale = 1.1f;
				if (ImGui::Button("+##6"))
				{
					scene.GetModel(item_current).SetNormalVertexScale(1 / normalVertexScale);
				}
				ImGui::SameLine();
				if (ImGui::Button("-##6"))
				{
					scene.GetModel(item_current).SetNormalVertexScale(normalVertexScale);
				}

				bool DrawModelAxisTemp = DrawModelAxis[item_current];
				ImGui::Text("Show Model Axis:"); ImGui::SameLine();
				ImGui::Checkbox("##ModelAxis", &DrawModelAxisTemp);
				if (DrawModelAxisTemp != DrawModelAxis[item_current])
				{
					DrawModelAxis[item_current] = DrawModelAxisTemp;
					scene.GetModel(item_current).SetModelAxis();
				}
				ImGui::SameLine();
				ImGui::Text("Scale Model Axis:"); ImGui::SameLine();
				float modelAxisScale = 1.1f;
				if (ImGui::Button("+##7"))
				{
					scene.GetModel(item_current).SetModelAxisScale(modelAxisScale);
				}
				ImGui::SameLine();
				if (ImGui::Button("-##7"))
				{
					scene.GetModel(item_current).SetModelAxisScale(1 / modelAxisScale);
				}

				bool DrawModelFrameAxisTemp = DrawModelFrameAxis[item_current];
				ImGui::Text("Show Model-Frame Axis:"); ImGui::SameLine();
				ImGui::Checkbox("##ModelFrameAxis", &DrawModelFrameAxisTemp);
				if (DrawModelFrameAxisTemp != DrawModelFrameAxis[item_current])
				{
					DrawModelFrameAxis[item_current] = DrawModelFrameAxisTemp;
					scene.GetModel(item_current).SetModelFrameAxis();
				}
				ImGui::SameLine();
				ImGui::Text("Scale Model-Frame Axis:"); ImGui::SameLine();
				float modelFrameAxisScale = 1.1f;
				if (ImGui::Button("+##8"))
				{
					scene.GetModel(item_current).SetModelFrameAxisScale(modelFrameAxisScale);
				}
				ImGui::SameLine();
				if (ImGui::Button("-##8"))
				{
					scene.GetModel(item_current).SetModelFrameAxisScale(1 / modelFrameAxisScale);
				}



				ImGui::PopButtonRepeat();
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Model Coloring"))
			{
				// light components user interactions
				ImGui::PushItemWidth(180);
				// Ambient
				ImGui::Text("Ambient: "); ImGui::SameLine();
				float AmbientFloat[3] = { currentModelAmbient.x, currentModelAmbient.y,  currentModelAmbient.z };
				ImGui::ColorEdit3("##Ambient-color", AmbientFloat);
				if (currentModelAmbient.x != AmbientFloat[0] || currentModelAmbient.y != AmbientFloat[1] || currentModelAmbient.z != AmbientFloat[2])
				{
					currentModelAmbient = { AmbientFloat[0] , AmbientFloat[1] , AmbientFloat[2] };
					scene.GetActiveModel().SetAmbientLightColor(currentModelAmbient);

				}
				// diffuse
				ImGui::Text("diffuse: "); ImGui::SameLine();
				float DiffuseFloat[3] = { currentModelDiffuse.x, currentModelDiffuse.y,  currentModelDiffuse.z };
				ImGui::ColorEdit3("##Diffus-color", DiffuseFloat);
				if (currentModelDiffuse.x != DiffuseFloat[0] || currentModelDiffuse.y != DiffuseFloat[1] || currentModelDiffuse.z != DiffuseFloat[2])
				{
					currentModelDiffuse = { DiffuseFloat[0] , DiffuseFloat[1] , DiffuseFloat[2] };
					scene.GetActiveModel().SetDiffuseLightColor(currentModelDiffuse);

				}
				// specular
				ImGui::Text("specular: "); ImGui::SameLine();
				float SpecularFloat[3] = { currentModelspecular.x, currentModelspecular.y,  currentModelspecular.z };
				ImGui::ColorEdit3("##Specular-color", SpecularFloat);
				if (currentModelspecular.x != SpecularFloat[0] || currentModelspecular.y != SpecularFloat[1] || currentModelspecular.z != SpecularFloat[2])
				{
					currentModelspecular = { SpecularFloat[0] , SpecularFloat[1] , SpecularFloat[2] };
					scene.GetActiveModel().SetSpecularLightColor(currentModelspecular);

				}
				float setExpBoolTemp = specularExponent;
				ImGui::Text("Set specular exponent: "); ImGui::SameLine();
				ImGui::PushItemWidth(60);
				ImGui::InputFloat("##SpecularExpSet", &specularExponent);
				if (setExpBoolTemp!= specularExponent)
				{
					scene.GetActiveModel().SetSpecularExponent(specularExponent);
				}



				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Model Texture"))
			{
				// texture insertion to app from the computer's memory
				if (ImGui::Button("Insert Texture"))
				{
					nfdchar_t* outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("png;", NULL, &outPath);
					if (result == NFD_OKAY)
					{
						scene.GetActiveModel().buildTextureMap(outPath);
					}
					free(outPath);
				}

				// tell app if to use texture
				bool UseTextureTemp = UseTexture[item_current];
				ImGui::Text("Use Texture:"); ImGui::SameLine();
				ImGui::Checkbox("##UseTexture", &UseTextureTemp);
				if (UseTextureTemp != UseTexture[item_current])
				{
					UseTexture[item_current] = UseTextureTemp;
					scene.GetModel(item_current).SetTextureMapBool();
				}

				// if we use texture, so here we choosing the texture type
				if (UseTexture[item_current])
				{				
					ImGui::Text("Choose texture type:");
					ImGui::Columns(2, "Textures1", false);
					ImGui::SetColumnWidth(0, 70.0f);
					ImGui::Text("Regular");
					ImGui::NextColumn();
					if (ImGui::RadioButton("##Regular", textureType[item_current] == 1))
					{
						textureType[item_current] = 1;
						scene.GetModel(item_current).SetTextureType(1);
					}
					ImGui::Columns();

					ImGui::Columns(2, "Textures2", false);
					ImGui::SetColumnWidth(0, 70.0f);
					ImGui::Text("Plane");
					ImGui::NextColumn();
					if (ImGui::RadioButton("##Plane", textureType[item_current] == 2))
					{
						textureType[item_current] = 2;
						scene.GetModel(item_current).SetTextureType(2);
					}
					ImGui::Columns();

					ImGui::Columns(2, "Textures3", false);
					ImGui::SetColumnWidth(0, 70.0f);
					ImGui::Text("Cylinder");
					ImGui::NextColumn();
					if (ImGui::RadioButton("##Cylinder", textureType[item_current] == 3))
					{
						textureType[item_current] = 3;
						scene.GetModel(item_current).SetTextureType(3);
					}
					ImGui::Columns();

					ImGui::Columns(2, "Textures4", false);
					ImGui::SetColumnWidth(0, 70.0f);
					ImGui::Text("Sphere");
					ImGui::NextColumn();
					if (ImGui::RadioButton("##Sphere", textureType[item_current] == 4))
					{
						textureType[item_current] = 4;
						scene.GetModel(item_current).SetTextureType(4);
					}
					ImGui::Columns();

					float tempTextureRadius = textureRadius[item_current];
					ImGui::Text("Set Texutre radius: "); ImGui::SameLine();
					ImGui::PushItemWidth(90);
					ImGui::InputFloat("##TextureRadius", &textureRadius[item_current],0.001f);
					if (tempTextureRadius != textureRadius[item_current])
					{
						scene.GetModel(item_current).SetTextureRadius(tempTextureRadius);
					}

					bool UseTextureAsNormalMapTemp = UseTextureAsNormalMap[item_current];
					ImGui::Text("Use Texture as normal map:"); ImGui::SameLine();
					ImGui::Checkbox("##UseNormalMap", &UseTextureAsNormalMapTemp);
					if (UseTextureAsNormalMapTemp != UseTextureAsNormalMap[item_current])
					{
						UseTextureAsNormalMap[item_current] = UseTextureAsNormalMapTemp;
						scene.GetModel(item_current).SetUseNormalMap();
					}

				}
				




				ImGui::TreePop();
			}


			int highFaceTemp = highFace;
			int lowFaceTemp = lowFace;
			ImGui::Text("Set high boundery: "); ImGui::SameLine();
			ImGui::PushItemWidth(90);
			ImGui::InputInt("##HighFace", &highFace); ImGui::SameLine();
			ImGui::Text("Set low boundery: "); ImGui::SameLine();
			ImGui::PushItemWidth(90);
			ImGui::InputInt("##LowFace", &lowFace);
			if (highFace != highFaceTemp)
			{
				//renderer.SetHighFace(highFace);
			}
			if (lowFaceTemp!= lowFace)
			{
				//renderer.SetLowFace(lowFace);
			}
			
			
		}
		ImGui::End();
	}

	// Displays the transformation matrices of world and selected model, useful for debugging and for guidance on proper implementations
	if (show_data_window)
	{
		ImGui::Begin("Data window", &show_data_window);
		if (InitializeWindows[2])
		{
			InitializeWindows[2] = false;
			ImGui::SetWindowSize(ImVec2((float)screenWidth/2, (float)screenHeight/2));
		}

		glm::mat4 mat;
		mat = scene.GetActiveCamera().GetViewTransformation();
		ImGui::Text("Camera transformation :\n%f		%f		%f		%f\n%f		%f		%f		%f\n%f		%f		%f		%f\n%f		%f		%f		%f", mat[0][0], mat[0][1], mat[0][2], mat[0][3],
			mat[1][0], mat[1][1], mat[1][2], mat[1][3], mat[2][0], mat[2][1], mat[2][2], mat[2][3], mat[3][0], mat[3][1], mat[3][2], mat[3][3]);

		mat = scene.GetActiveCamera().GetProjectionTransformation();
		ImGui::Text("Camera Projection :\n%f		%f		%f		%f\n%f		%f		%f		%f\n%f		%f		%f		%f\n%f		%f		%f		%f", mat[0][0], mat[0][1], mat[0][2], mat[0][3],
			mat[1][0], mat[1][1], mat[1][2], mat[1][3], mat[2][0], mat[2][1], mat[2][2], mat[2][3], mat[3][0], mat[3][1], mat[3][2], mat[3][3]);


		

		if (scene.GetModelCount())
		{
			static int item_current = -1;
			char** models = new char* [scene.GetModelCount()];
			ContructsNames(scene.GetModelCount(), models,'m');
			int modelsSize = scene.GetModelCount();
			ImGui::Combo("Choose model", &item_current, models, modelsSize);
			DestructsNames(scene.GetModelCount(), models);
			if (item_current >= 0)
			{
				mat = scene.GetModel(item_current).GetWorldTransformation();
				ImGui::Text("World transformation :\n%f		%f		%f		%f\n%f		%f		%f		%f\n%f		%f		%f		%f\n%f		%f		%f		%f", mat[0][0], mat[0][1], mat[0][2], mat[0][3],
					mat[1][0], mat[1][1], mat[1][2], mat[1][3], mat[2][0], mat[2][1], mat[2][2], mat[2][3], mat[3][0], mat[3][1], mat[3][2], mat[3][3]);

				// Overwrite mat with the local transform next.
				mat = scene.GetModel(item_current).GetLocalTransformation();
				ImGui::Text("Local transformation :\n%f		%f		%f		%f\n%f		%f		%f		%f\n%f		%f		%f		%f\n%f		%f		%f		%f", mat[0][0], mat[0][1], mat[0][2], mat[0][3],
					mat[1][0], mat[1][1], mat[1][2], mat[1][3], mat[2][0], mat[2][1], mat[2][2], mat[2][3], mat[3][0], mat[3][1], mat[3][2], mat[3][3]);

				glm::mat4 mat2 = scene.GetModel(item_current).GetWorldTransformation();
				mat2 = MultiplyMatrix(mat2, mat);
				ImGui::Text("World*Local transform :\n%f		%f		%f		%f\n%f		%f		%f		%f\n%f		%f		%f		%f\n%f		%f		%f		%f", mat2[0][0], mat2[0][1], mat2[0][2], mat2[0][3],
					mat2[1][0], mat2[1][1], mat2[1][2], mat2[1][3], mat2[2][0], mat2[2][1], mat2[2][2], mat2[2][3], mat2[3][0], mat2[3][1], mat2[3][2], mat2[3][3]);
				vec4 firstVec{ scene.GetModel(0).GetVertex(0) ,1 };
				
				//  world * local * vector 
				firstVec = MultiplyVectorMatrix(mat2, firstVec);
				ImGui::Text("FirstVetex in world :\n%f		%f		%f", firstVec.x, firstVec.y, firstVec.z);

				
				
				// inverse * world * local * vector 
				firstVec = MultiplyVectorMatrix(scene.GetActiveCamera().GetInverse(), firstVec);

				ImGui::Text("FirstVetex in camera view :\n%f		%f		%f", firstVec.x, firstVec.y, firstVec.z);
				
				// projection * inverse * world * local * vector 
				firstVec = MultiplyVectorMatrix(scene.GetActiveCamera().GetProjectionTransformation(), firstVec);
				ImGui::Text("FirstVetex with projection :\n%f		%f		%f", firstVec.x, firstVec.y, firstVec.z);
			}
			if (scene.GetLightCount())
			{
				glm::vec4 firstLightPos = scene.GetActiveLight().GetLightPosition();

				firstLightPos = MultiplyVectorMatrix(scene.GetActiveCamera().GetInverse(), firstLightPos);
				ImGui::Text("firstLightPos in camera view :\n%f		%f		%f", firstLightPos.x, firstLightPos.y, firstLightPos.z);
			}



		}
		ImGui::End();
	}

	// camera window
	if (show_cameraTransform_window)
	{
		ImGui::Begin("Camera Transormation window", &show_cameraTransform_window);
		if (InitializeWindows[3])
		{
			InitializeWindows[3] = false;
			ImGui::SetWindowSize(ImVec2((float)screenWidth / 2, (float)screenHeight / 2));
		}

		if (ImGui::Button("Add Camera"))
		{
			std::shared_ptr<Camera> firstCamera = std::make_shared<Camera>(screenWidth, screenHeight);
			scene.AddCamera(firstCamera);
			cameraMode.push_back(true);
			CameraRotation.push_back({ 0, 0, 0 });
			vector<float> viewVolumeTemp = { -screenWidth / 1000.f ,screenWidth / 1000.f ,-screenHeight / 1000.f,screenHeight / 1000.f,1.5f,5.0f };
			viewVolume.push_back(viewVolumeTemp);
		}

		// Setup for a list of loaded Cameras (using ImGui::Combo)
		static int item_current = -1;
		char** cameras = new char* [scene.GetCameraCount()];
		ContructsNames(scene.GetCameraCount(), cameras, 'c');
		int camerasSize = scene.GetCameraCount();
		ImGui::Combo("Choose model", &item_current, cameras, camerasSize);
		DestructsNames(scene.GetCameraCount(), cameras);

		if (item_current >= 0)
		{
			if (item_current!= activeCameraIndex)
			{
				activeCameraIndex = item_current;
				scene.SetActiveCameraIndex(item_current);
			}
			
			// choose between orthographic to perspective mode
			bool cameraModeTemp = cameraMode[item_current];
			ImGui::Text("Choose camera mode: "); ImGui::SameLine();
			ImGui::Text("Orthographic"); ImGui::SameLine();
			if (ImGui::RadioButton("##CameraMode1", cameraMode[item_current] == true))
			{
				cameraMode[item_current] = true;
			}
			ImGui::SameLine(); ImGui::Text("Perspective"); ImGui::SameLine();
			if (ImGui::RadioButton("##CameraMode2", cameraMode[item_current] == false))
			{
				cameraMode[item_current] = false;
			}
			if (cameraModeTemp != cameraMode[item_current])
			{
				scene.GetCamera(item_current).SetCameraMode();
			}


			if (ImGui::TreeNode("Incrementation Transformations"))
			{
				int lastXRotation = CameraRotation[item_current].x;
				int lastYRotation = CameraRotation[item_current].y;
				int lastZRotation = CameraRotation[item_current].z;

				ImGui::Text("Camera rotations:");
				ImGui::SliderInt("RotationX", &CameraRotation[item_current].x, -360, 360);
				if (lastXRotation != CameraRotation[item_current].x)
				{
					scene.GetCamera(item_current).RotateCameraAxisX(CameraRotation[item_current].x - lastXRotation);
				}

				ImGui::SliderInt("RotationY", &CameraRotation[item_current].y, -360, 360);
				if (lastYRotation != CameraRotation[item_current].y)
				{
					scene.GetCamera(item_current).RotateCameraAxisY(CameraRotation[item_current].y - lastYRotation);
				}

				ImGui::SliderInt("RotationZ", &CameraRotation[item_current].z, -360, 360);
				if (lastZRotation != CameraRotation[item_current].z)
				{
					scene.GetCamera(item_current).RotateCameraAxisZ(CameraRotation[item_current].z - lastZRotation);
				}

				// Translating the camera
				ImGui::PushButtonRepeat(true);
				ImGui::Text("Translate x: ");
				ImGui::SameLine();
				glm::vec3 MoveVec;
				if (ImGui::Button("+##1"))
				{
					MoveVec = { cameraTranslation ,0,0 };
					scene.GetCamera(item_current).TranslateCamera(MoveVec);
				}
				ImGui::SameLine();
				if (ImGui::Button("-##1"))
				{
					MoveVec = { -cameraTranslation ,0,0 };
					scene.GetCamera(item_current).TranslateCamera(MoveVec);
				}

				ImGui::SameLine();
				ImGui::Text("Translate y: ");
				ImGui::SameLine();
				if (ImGui::Button("+##2"))
				{
					MoveVec = { 0 ,cameraTranslation,0 };
					scene.GetCamera(item_current).TranslateCamera(MoveVec);
				}
				ImGui::SameLine();
				if (ImGui::Button("-##2"))
				{
					MoveVec = { 0 ,-cameraTranslation,0 };
					scene.GetCamera(item_current).TranslateCamera(MoveVec);
				}

				ImGui::SameLine();
				ImGui::Text("Translate z: ");
				ImGui::SameLine();
				if (ImGui::Button("+##3"))
				{
					MoveVec = { 0 ,0,cameraTranslation };
					scene.GetCamera(item_current).TranslateCamera(MoveVec);
				}
				ImGui::SameLine();
				if (ImGui::Button("-##3"))
				{
					MoveVec = { 0 ,0,-cameraTranslation };
					scene.GetCamera(item_current).TranslateCamera(MoveVec);
				}

				ImGui::SameLine();
				ImGui::Text("Translation interval: ");
				ImGui::SameLine();
				ImGui::PushItemWidth(60);
				ImGui::InputFloat("##InputObjectTranslate", &cameraTranslation);



				ImGui::PopButtonRepeat();


				ImGui::TreePop();
			}
			

			if (ImGui::TreeNode("LookAt function"))
			{

				//	LookAt function call
				ImGui::Text("set LookAt values");
				ImGui::Text("Eye:"); ImGui::SameLine();
				ImGui::PushItemWidth(50);
				ImGui::InputFloat("##EyeX", &eye.x); ImGui::SameLine(); ImGui::InputFloat("##EyeY", &eye.y);
				ImGui::SameLine(); ImGui::InputFloat("##EyeZ", &eye.z);
				ImGui::Text("At:"); ImGui::SameLine();
				ImGui::InputFloat("##AtX", &at.x); ImGui::SameLine(); ImGui::InputFloat("##AtY", &at.y);
				ImGui::SameLine(); ImGui::InputFloat("##AtZ", &at.z);
				ImGui::Text("Up:"); ImGui::SameLine();
				ImGui::InputFloat("##UPX", &up.x); ImGui::SameLine(); ImGui::InputFloat("##UPY", &up.y);
				ImGui::SameLine(); ImGui::InputFloat("##UPZ", &up.z);
				if (ImGui::Button("insert##LookAt"))
				{
					scene.GetActiveCamera().SetCameraLookAt(eye, at, up);
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("ViewVolume interactions"))
			{
				
				//	changing the view volume of the camera. we allow that it will be only symmetric to the camera 
				ImGui::PushButtonRepeat(true);
				vector<float> viewVolumeTemp = viewVolume[item_current];
				ImGui::Text("set view volume:"); ImGui::Text("Left:"); ImGui::SameLine();
				ImGui::PushItemWidth(350);
				ImGui::SliderFloat("##LeftViewVolume", &viewVolume[item_current][0], -10.0f, 10.0f);	ImGui::SameLine();	ImGui::Text("Right:"); ImGui::SameLine();
				ImGui::PushItemWidth(350);
				ImGui::SliderFloat("##RightViewVolume", &viewVolume[item_current][1], -10.0f, 10.0f);
				ImGui::SameLine(); ImGui::Text("Symmetric:"); ImGui::SameLine();
				ImGui::Checkbox("##RightLeftSymmetric", &rightLeftSymmetric);

				if (rightLeftSymmetric)
				{
					if (viewVolumeTemp[0] != viewVolume[item_current][0])
					{
						viewVolume[item_current][1] = -viewVolume[item_current][0];
					}
					if (viewVolumeTemp[1] != viewVolume[item_current][1])
					{
						viewVolume[item_current][0] = -viewVolume[item_current][1];
					}
				}

				ImGui::Text("Down:"); ImGui::SameLine();
				ImGui::PushItemWidth(350);
				ImGui::SliderFloat("##DownViewVolume", &viewVolume[item_current][2], -10.0f, 10.0f); ImGui::SameLine();	ImGui::Text("Up:"); ImGui::SameLine();
				ImGui::PushItemWidth(350);
				ImGui::SliderFloat("##UpViewVolume", &viewVolume[item_current][3], -10.0f, 10.0f);
				ImGui::SameLine(); ImGui::Text("Symmetric:"); ImGui::SameLine();
				ImGui::Checkbox("##UpDownSymmetric", &upDownSymmetric);
				if (upDownSymmetric)
				{
					if (viewVolumeTemp[2] != viewVolume[item_current][2])
					{
						viewVolume[item_current][3] = -viewVolume[item_current][2];
					}
					if (viewVolumeTemp[3] != viewVolume[item_current][3])
					{
						viewVolume[item_current][2] = -viewVolume[item_current][3];
					}
				}

				ImGui::Text("Near:"); ImGui::SameLine();
				ImGui::PushItemWidth(350);
				ImGui::SliderFloat("##NearViewVolume", &viewVolume[item_current][4], 0.1f, 50.0f); ImGui::SameLine();	ImGui::Text("Far:"); ImGui::SameLine();
				ImGui::PushItemWidth(350);
				ImGui::SliderFloat("##FarViewVolume", &viewVolume[item_current][5], 0.1, 500.0f);

				if (viewVolumeTemp != viewVolume[item_current])
				{
					scene.GetActiveCamera().SetViewVolumeSize(viewVolume[item_current][0], viewVolume[item_current][1], viewVolume[item_current][2], viewVolume[item_current][3], viewVolume[item_current][4], viewVolume[item_current][5]);
				}

				ImGui::Text("Scale view volume Size:"); ImGui::SameLine();

				if (ImGui::Button("+##ViewVolumeScale"))
				{
					viewVolume[item_current][0] *= scaleViewVolume;
					viewVolume[item_current][1] *= scaleViewVolume;
					viewVolume[item_current][2] *= scaleViewVolume;
					viewVolume[item_current][3] *= scaleViewVolume;
					scene.GetActiveCamera().SetViewVolumeSize(viewVolume[item_current][0], viewVolume[item_current][1], viewVolume[item_current][2], viewVolume[item_current][3], viewVolume[item_current][4], viewVolume[item_current][5]);
				}	ImGui::SameLine();
				if (ImGui::Button("-##ViewVolumeScale"))
				{
					viewVolume[item_current][0] /= scaleViewVolume;
					viewVolume[item_current][1] /= scaleViewVolume;
					viewVolume[item_current][2] /= scaleViewVolume;
					viewVolume[item_current][3] /= scaleViewVolume;
					scene.GetActiveCamera().SetViewVolumeSize(viewVolume[item_current][0], viewVolume[item_current][1], viewVolume[item_current][2], viewVolume[item_current][3], viewVolume[item_current][4], viewVolume[item_current][5]);
				} ImGui::SameLine();	ImGui::Text("Set scale:"); ImGui::SameLine();
				ImGui::InputFloat("##WidthViewScalar", &scaleViewVolume);


				ImGui::Text("Stretch to window size:");  ImGui::SameLine();
				ImGui::Checkbox("##ViewVolumeStretch", &stretchWindowSize);
				if (stretchWindowSize)
				{
					//renderer.SetWindowSize(screenWidth, screenHeight);
				}


				ImGui::Text("Scale to window size:");  ImGui::SameLine();
				ImGui::Checkbox("##ViewVolumeScale", &scaleWindowSize);
				if (scaleWindowSize)
				{
					if (screenWidth != oldScreenWidth || screenHeight != oldScreenHeight)
					{
						viewVolume[item_current][0] *= ((float)screenWidth / (float)oldScreenWidth);
						viewVolume[item_current][1] *= ((float)screenWidth / (float)oldScreenWidth);
						viewVolume[item_current][2] *= ((float)screenHeight / (float)oldScreenHeight);
						viewVolume[item_current][3] *= ((float)screenHeight / (float)oldScreenHeight);
						scene.GetActiveCamera().SetViewVolumeSize(viewVolume[item_current][0], viewVolume[item_current][1], viewVolume[item_current][2], viewVolume[item_current][3], viewVolume[item_current][4], viewVolume[item_current][5]);
					}
					//renderer.SetWindowSize(screenWidth, screenHeight);
					oldScreenWidth = screenWidth;
					oldScreenHeight = screenHeight;
				}





				ImGui::PopButtonRepeat();
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Renderer interactions"))
			{
				bool fillFacesBoolTemp = fillFacesBool;
				bool colorElseGreyModeTemp = colorElseGreyMode;
				bool drawOutlinesTemp = drawOutlines;


				ImGui::Text("Draw as wireframe: "); ImGui::SameLine();
				ImGui::Checkbox("##drawOutlines", &drawOutlines);
				//if (drawOutlinesTemp != drawOutlines)
				//{
				//	renderer.setFillOrLine();
				//}

				ImGui::Text("Fill Face mode: "); ImGui::SameLine();
				ImGui::Text("No fill"); ImGui::SameLine();
				if (ImGui::RadioButton("##FillFaceMode1", fillFacesBool == false))
				{
					fillFacesBool = false;
				}
				ImGui::SameLine(); ImGui::Text("Color Mode"); ImGui::SameLine();
				if (ImGui::RadioButton("##FillFaceMode2", ((fillFacesBool == true) && (colorElseGreyMode == false))))
				{
					fillFacesBool = true;
					colorElseGreyMode = false;
				}
				ImGui::SameLine(); ImGui::Text("Grey Mode"); ImGui::SameLine();
				if (ImGui::RadioButton("##FillFaceMode3", ((fillFacesBool == true) && (colorElseGreyMode == true))))
				{
					fillFacesBool = true;
					colorElseGreyMode = true;
				}

				if (fillFacesBoolTemp!= fillFacesBool)
				{
					//renderer.SetFillFacesBool();
				}

				if (colorElseGreyModeTemp != colorElseGreyMode)
				{
					//renderer.SetColorElseGrey();
				}


				// shading Section
				int useLightTemp = useLight;
				int shadingTypeTemp = shadingType;
				ImGui::Text("Use light: "); ImGui::SameLine();
				ImGui::Checkbox("##UseLight", &useLight);
				if (useLight)
				{
					ImGui::Text("Shading Type: "); ImGui::SameLine();
					ImGui::Text("Flat: "); ImGui::SameLine();
					if (ImGui::RadioButton("##ShadingType1", shadingType == 1))
					{
						shadingType=1;
					}
					ImGui::SameLine(); ImGui::Text("Gouraud: "); ImGui::SameLine();
					if (ImGui::RadioButton("##ShadingType2", shadingType==2))
					{
						shadingType = 2;
					}
					ImGui::SameLine(); ImGui::Text("Phong: "); ImGui::SameLine();
					if (ImGui::RadioButton("##ShadingType3", shadingType == 3))
					{
						shadingType = 3;
					}

					if (shadingTypeTemp != shadingType)
					{
						//renderer.SetShadingType(shadingType);
					}

				}
				if (useLightTemp != useLight)
				{
					//renderer.SetUseShadingBool();
				}

				ImGui::TreePop();
			}

			

			
		}
		ImGui::End();
	}

	if (show_light_window)
	{
		ImGui::Begin("Lights window", &show_light_window);
		if (InitializeWindows[4])
		{
			InitializeWindows[4] = false;
			ImGui::SetWindowSize(ImVec2((float)screenWidth / 2, (float)screenHeight / 2));
		}

		if (ImGui::Button("Add new light Source"))
		{
			std::shared_ptr<Light> newLight = std::make_shared<Light>(glm::vec4(0,0,1,1));
			scene.AddLight(newLight);
			lightType.push_back(true);
			LightRotation.push_back({ 0, 0, 0 });
		}
		
		// Setup for a list of loaded Lights (using ImGui::Combo)
		static int item_current = -1;
		int lightCount = scene.GetLightCount();
		char** lights = new char* [lightCount];
		ContructsNames(lightCount, lights, 'l');
		ImGui::Combo("Choose Light", &item_current, lights, lightCount);
		DestructsNames(lightCount, lights);

		if (item_current >= 0)
		{
			if (item_current != activeLightIndex)
			{
				activeLightIndex = item_current;
				scene.SetActiveLightIndex(item_current);

				currentAmbient = scene.GetActiveLight().GetAmbientLightColor();
				currentDiffuse = scene.GetActiveLight().GetDiffuseLightColor();
				currentspecular = scene.GetActiveLight().GetSpecularLightColor();
				lightDirection = scene.GetActiveLight().GetLightDirection();
			}

			// choose between pointSource to parallel light type
			bool lightTypeTemp = lightType[item_current];
			ImGui::Text("Choose Light Type: "); ImGui::SameLine();
			ImGui::Text("Point Source"); ImGui::SameLine();
			if (ImGui::RadioButton("##LightType1", lightType[item_current] == true))
			{
				lightType[item_current] = true;
			}
			ImGui::SameLine(); ImGui::Text("Directional"); ImGui::SameLine();
			if (ImGui::RadioButton("##LightType2", lightType[item_current] == false))
			{
				lightType[item_current] = false;
			}
			if (lightTypeTemp != lightType[item_current])
			{
				scene.GetActiveLight().SetLightType();
			}



			if (ImGui::TreeNode("Incrementation Transformations"))
			{
				if (lightType[item_current] == false)
				{
					int lastXRotation = LightRotation[item_current].x;
					int lastYRotation = LightRotation[item_current].y;
					int lastZRotation = LightRotation[item_current].z;

					ImGui::Text("Directional light rotations:");
					ImGui::SliderInt("RotationX", &LightRotation[item_current].x, -360, 360);
					if (lastXRotation != LightRotation[item_current].x)
					{
						scene.GetActiveLight().RotationLight(LightRotation[item_current].x - lastXRotation,'x');
						lightDirection = scene.GetActiveLight().GetLightDirection();
					}

					ImGui::SliderInt("RotationY", &LightRotation[item_current].y, -360, 360);
					if (lastYRotation != LightRotation[item_current].y)
					{
						scene.GetActiveLight().RotationLight(LightRotation[item_current].y - lastYRotation, 'y');
						lightDirection = scene.GetActiveLight().GetLightDirection();
					}

					ImGui::SliderInt("RotationZ", &LightRotation[item_current].z, -360, 360);
					if (lastZRotation != LightRotation[item_current].z)
					{
						scene.GetActiveLight().RotationLight(LightRotation[item_current].z - lastZRotation, 'z');
						lightDirection= scene.GetActiveLight().GetLightDirection();
					}
					glm::vec3 lightDirectionTemp = lightDirection;
					ImGui::Text("Choose direction: ");
					ImGui::SameLine();
					ImGui::PushItemWidth(90);
					ImGui::InputFloat("##LightDirX", &lightDirection.x);
					ImGui::SameLine(); ImGui::InputFloat("##LightDirY", &lightDirection.y); ImGui::SameLine();
					ImGui::InputFloat("##LightDirZ", &lightDirection.z);
					if (ImGui::Button("insert"))
					{
						lightDirection = glm::normalize(lightDirection);
						scene.GetActiveLight().SetLightDirection(lightDirection);
					}
				}
				
				else
				{
					// Translating light
					ImGui::PushButtonRepeat(true);
					ImGui::Text("Translate x: ");
					ImGui::SameLine();

					// For translations, we arbitrarily chose 6, but then normalized it for the current model's scale
					if (ImGui::Button("+##1"))
					{
						glm::vec3 moveVec = glm::vec3(lightTranslation, 0, 0);
						scene.GetActiveLight().TranslateLight(moveVec);
					}
					ImGui::SameLine();
					if (ImGui::Button("-##1"))
					{
						glm::vec3 moveVec = glm::vec3(-lightTranslation, 0, 0);
						scene.GetActiveLight().TranslateLight(moveVec);
					}

					ImGui::SameLine();
					ImGui::Text("Translate y: ");
					ImGui::SameLine();
					if (ImGui::Button("+##2"))
					{
						glm::vec3 moveVec = glm::vec3(0, lightTranslation, 0);
						scene.GetActiveLight().TranslateLight(moveVec);
					}
					ImGui::SameLine();
					if (ImGui::Button("-##2"))
					{
						glm::vec3 moveVec = glm::vec3(0, -lightTranslation, 0);
						scene.GetActiveLight().TranslateLight(moveVec);
					}

					ImGui::SameLine();
					ImGui::Text("Translate z: ");
					ImGui::SameLine();
					if (ImGui::Button("+##3"))
					{
						glm::vec3 moveVec = glm::vec3(0, 0, lightTranslation);
						scene.GetActiveLight().TranslateLight(moveVec);
					}
					ImGui::SameLine();
					if (ImGui::Button("-##3"))
					{
						glm::vec3 moveVec = glm::vec3(0, 0, -lightTranslation);
						scene.GetActiveLight().TranslateLight(moveVec);
					}

					ImGui::SameLine();
					ImGui::Text("Translation interval: ");
					ImGui::SameLine();
					ImGui::PushItemWidth(60);
					ImGui::InputFloat("##InputLightTranslate", &lightTranslation);

				}







				ImGui::TreePop();
			}

			
			if (ImGui::TreeNode("Light Colors"))
			{
				// light components user interactions
				ImGui::PushItemWidth(180);
				// Ambient
				ImGui::Text("Ambient: "); ImGui::SameLine();
				float AmbientFloat[3] = { currentAmbient.x, currentAmbient.y,  currentAmbient.z };
				ImGui::ColorEdit3("##Ambient-color", AmbientFloat);
				if (currentAmbient.x != AmbientFloat[0] || currentAmbient.y != AmbientFloat[1] || currentAmbient.z != AmbientFloat[2])
				{
					currentAmbient = { AmbientFloat[0] , AmbientFloat[1] , AmbientFloat[2] };
					scene.GetActiveLight().SetAmbientLightColor(currentAmbient);
				}
				// diffuse
				ImGui::Text("diffuse: "); ImGui::SameLine();
				float DiffuseFloat[3] = { currentDiffuse.x, currentDiffuse.y,  currentDiffuse.z };
				ImGui::ColorEdit3("##Diffus-color", DiffuseFloat);
				if (currentDiffuse.x != DiffuseFloat[0] || currentDiffuse.y != DiffuseFloat[1] || currentDiffuse.z != DiffuseFloat[2])
				{
					currentDiffuse = { DiffuseFloat[0] , DiffuseFloat[1] , DiffuseFloat[2] };
					scene.GetActiveLight().SetDiffuseLightColor(currentDiffuse);
				}
				// specular
				ImGui::Text("specular: "); ImGui::SameLine();
				float SpecularFloat[3] = { currentspecular.x, currentspecular.y,  currentspecular.z };
				ImGui::ColorEdit3("##Specular-color", SpecularFloat);
				if (currentspecular.x != SpecularFloat[0] || currentspecular.y != SpecularFloat[1] || currentspecular.z != SpecularFloat[2])
				{
					currentspecular = { SpecularFloat[0] , SpecularFloat[1] , SpecularFloat[2] };
					scene.GetActiveLight().SetSpecularLightColor(currentspecular);
				}

				bool specularVectorBoolTemp = specularVectorBool;
				ImGui::Text("Show specular reflection vectors: "); ImGui::SameLine();
				ImGui::Checkbox("##WorldFrameAxis", &specularVectorBool);
				if (specularVectorBoolTemp != specularVectorBool)
				{
					//renderer.SetSpecularVectorBool();
				}
				ImGui::TreePop();
			}
			
		}


		ImGui::End();
	}

	//ImGui::End();
	//addImGui::End();
}

// Setup function for model lists in the GUI
void ContructsNames(int index, char** names, const char& type)
{
	string temp;
	switch (type)
	{
	case 'm':
		temp = "Model ";
		break;
	case 'c':
		temp = "Camera ";
		break;
	case 'l':
		temp = "Light ";
	}
	for (int i = 0; i < index; i++)
	{
		// Maybe change "+ 2" later for supporting more than 9 models
		names[i] = new char[temp.length() + 2];
		for (int j = 0; j < temp.length(); j++)
		{
			names[i][j] = temp[j];
		}
		// Remember to add DEFINE for ASCII '1' later instead of 49
		names[i][temp.length()] = (i + 49);
		names[i][temp.length()+1] = '\0';
	}
}
// Deconstruction function for model lists in the GUI
void DestructsNames(int index, char** names)
{
	for (int i = 0; i < index; i++)
	{
		//printf("%s", names[i]);
		delete[] names[i];
	}
	delete[] names;
}
// General rotation function - applies changes from GUI to relevant rotation matrix
void RotateFunction(const bool& localElseWorld, const char& axis, Scene& scene, const int& index, const int& angle)
{
	if (!localElseWorld && !activeMode)
	{
		for (int i = 0; i < scene.GetModelCount(); i++)
		{
			switch (axis)
			{
			case 'x':
				scene.GetModel(i).RotateObjectAxisX(angle, localElseWorld);
				break;
			case 'y':
				scene.GetModel(i).RotateObjectAxisY(angle, localElseWorld);
				break;
			case 'z':
				scene.GetModel(i).RotateObjectAxisZ(angle, localElseWorld);

			}
		}
	}
	else
	{
		switch (axis)
		{
		case 'x':
			scene.GetModel(index).RotateObjectAxisX(angle, localElseWorld);
			break;
		case 'y':
			scene.GetModel(index).RotateObjectAxisY(angle, localElseWorld);
			break;
		case 'z':
			scene.GetModel(index).RotateObjectAxisZ(angle, localElseWorld);
		}
	}
}
// General scaling function - applies changes from GUI to relevant scaling matrix
void ScaleFunction(const bool& localElseWorld, const bool& plusMinus, Scene& scene, const int& index, const float& scale)
{
	glm::vec3 scaleVec;

	if (!localElseWorld && !activeMode)
	{
		for (int i = 0; i < scene.GetModelCount(); i++)
		{
			if (plusMinus)
				scaleVec = { scale,scale,scale };
			else
				scaleVec = { 1.0f / scale,1.0f / scale,1.0f / scale };
			scene.GetModel(i).ScaleObject(scaleVec, localElseWorld);
		}
	}
	else
	{
		if (plusMinus)
			scaleVec = { scale,scale,scale };
		else
			scaleVec = { 1.0f / scale,1.0f / scale,1.0f / scale };
		scene.GetModel(index).ScaleObject(scaleVec, localElseWorld);
	}
}
// General translation function - applies changes from GUI to relevant translation matrix
void TranslateFunction(const bool& localElseWorld, const bool& plusMinus, const char& axis, Scene& scene, const int& index)
{
	glm::vec3 MoveVec;
	if (localElseWorld)
	{
		switch (axis)
		{
		case 'x':
			if (plusMinus)
				MoveVec = { objectTranslation ,0,0 };
			else
				MoveVec = { -objectTranslation ,0,0 };
			scene.GetModel(index).TranslateObject(MoveVec, localElseWorld);
			break;
		case 'y':
			if (plusMinus)
				MoveVec = { 0, objectTranslation ,0 };
			else
				MoveVec = { 0, -objectTranslation,0 };
			scene.GetModel(index).TranslateObject(MoveVec, localElseWorld);
			break;
		case 'z':
			if (plusMinus)
				MoveVec = { 0,0, objectTranslation };
			else
				MoveVec = { 0,0,-objectTranslation };
			scene.GetModel(index).TranslateObject(MoveVec, localElseWorld);

		}
	}
	else
	{
		if (activeMode)
		{
			if (index >= 0)
			{
				switch (axis)
				{
				case 'x':
					if (plusMinus)
						MoveVec = { worldTranslation, 0 ,0 };
					else
						MoveVec = { -worldTranslation, 0 ,0 };
					scene.GetModel(index).TranslateObject(MoveVec, localElseWorld);
					break;
				case 'y':
					if (plusMinus)
						MoveVec = { 0, worldTranslation ,0 };
					else
						MoveVec = { 0, -worldTranslation ,0 };
					scene.GetModel(index).TranslateObject(MoveVec, localElseWorld);
					break;
				case 'z':
					if (plusMinus)
						MoveVec = { 0, 0 ,worldTranslation };
					else
						MoveVec = { 0, 0 ,-worldTranslation };
					scene.GetModel(index).TranslateObject(MoveVec, localElseWorld);

				}
			}
			
		}
		else
		{
			for (int i = 0; i < scene.GetModelCount(); i++)
			{
				switch (axis)
				{
				case 'x':
					if (plusMinus)
						MoveVec = { worldTranslation, 0 ,0 };
					else
						MoveVec = { -worldTranslation, 0 ,0 };
					scene.GetModel(i).TranslateObject(MoveVec, localElseWorld);
					break;
				case 'y':
					if (plusMinus)
						MoveVec = { 0, worldTranslation ,0 };
					else
						MoveVec = { 0, -worldTranslation ,0 };
					scene.GetModel(i).TranslateObject(MoveVec, localElseWorld);
					break;
				case 'z':
					if (plusMinus)
						MoveVec = { 0, 0 ,worldTranslation };
					else
						MoveVec = { 0, 0 ,-worldTranslation };
					scene.GetModel(i).TranslateObject(MoveVec, localElseWorld);

				}
			}
		}
	}
}