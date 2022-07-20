
#include "Matrix.h"



class Light
{
public:
	Light(glm::vec4 lightdirectionset);
	


	// Get / Set light-colors
	void SetAmbientLightColor(const glm::vec3& newColor);
	glm::vec3 GetAmbientLightColor() const;

	void SetDiffuseLightColor(const glm::vec3& newColor);
	glm::vec3 GetDiffuseLightColor() const;

	void SetSpecularLightColor(const glm::vec3& newColor);
	glm::vec3 GetSpecularLightColor() const;

	// Get / Set light type
	void SetLightType();
	bool GetLightType() const;

	void TranslateLight(glm::vec3& moveVec);
	void RotationLight(const int& degree, const char& axis);

	glm::vec4 GetLightPosition() const;
	glm::vec4 GetLightDirection() const;
	void SetLightDirection(glm::vec3& rotateVec);

private:
	
	bool pointSourceElseParallel;

	glm::vec3 ambientLightColor;
	glm::vec3 diffuseLightColor;
	glm::vec3 specularLightColor;

	glm::vec4 lightPosition;
	glm::vec4 lightdirection;

};
