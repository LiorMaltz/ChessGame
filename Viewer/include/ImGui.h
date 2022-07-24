#pragma once

#include <nfd.h>
#include "Scene.h"
//#include "Renderer.h"
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Utils.h"

// Assignment 1b.4 Seperate and expanded ImGui class, used in main

void MyImGui(ImGuiIO& io, Scene& scene, GLFWwindow* window);
void ContructsNames(int index, char** names, const char& type);
void DestructsNames(int index, char** names);
void TranslateFunction(const bool& localElseWorld, const bool& plusMinus, const char& axis, Scene& scene, const int& index);
void ScaleFunction(const bool& localElseWorld, const bool& plusMinus, Scene& scene, const int& index, const float& scale);
void RotateFunction(const bool& localElseWorld, const char& axis, Scene& scene, const int& index, const int& angle);
void AddModel(nfdchar_t* outPath, Scene& scene, nfdchar_t* texturePath = NULL);


extern vector<bool> UseTexture;
extern vector<bool> UseTextureAsNormalMap;
extern vector<int> textureType;
extern vector<float> textureRadius;
extern bool useEnvironmentTexture;