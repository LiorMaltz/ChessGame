#include "Light.h"



Light::Light(glm::vec4 lightdirectionset) : pointSourceElseParallel(true), ambientLightColor(glm::vec3(1,1,1)),
diffuseLightColor(glm::vec3(1, 1, 1)),
specularLightColor(glm::vec3(1, 1, 1)),
lightPosition(glm::vec4(0, 0, 0,1)),
lightdirection(lightdirectionset)
{}



// Get / Set light-colors

void Light::SetAmbientLightColor(const glm::vec3& newColor)
{
	ambientLightColor = newColor;
}
glm::vec3 Light::GetAmbientLightColor() const
{
	return ambientLightColor;
}

void Light::SetDiffuseLightColor(const glm::vec3& newColor)
{
	diffuseLightColor = newColor;
}
glm::vec3 Light::GetDiffuseLightColor() const
{
	return diffuseLightColor;
}

void Light::SetSpecularLightColor(const glm::vec3& newColor)
{
	specularLightColor = newColor;
}
glm::vec3 Light::GetSpecularLightColor() const
{
	return specularLightColor;
}

// Get / Set light type
void Light::SetLightType()
{
	pointSourceElseParallel = !pointSourceElseParallel;
}
bool Light::GetLightType() const
{
	return pointSourceElseParallel;
}

glm::vec4 Light::GetLightPosition() const
{
	return lightPosition;
}
glm::vec4 Light::GetLightDirection() const
{
	return lightdirection;
}
void Light::SetLightDirection(glm::vec3& rotateVec)
{
	lightdirection = glm::vec4(rotateVec, 1);
}

void Light::TranslateLight(glm::vec3& moveVec)
{
	lightPosition = MultiplyVectorMatrix(TranslatingTransformation(moveVec), lightPosition);
}

void Light::RotationLight(const int& degree, const char& axis)
{
	lightdirection = MultiplyVectorMatrix(RotationTransformation(degree, axis), lightdirection);
}
