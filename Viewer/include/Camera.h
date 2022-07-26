#pragma once
#include <glm/glm.hpp>
#include "Matrix.h"

//
#include "Matrix.h"
//

class Camera
{
public:
	Camera(const float& width, const float& height);
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	const glm::mat4& GetProjectionTransformation() const;
	const glm::mat4& GetViewTransformation() const;

	//
	glm::mat4 GetRotationMatrix() const;
	glm::mat4 GetInverse() const;
	glm::mat4 Camera::GetVeiw() const;
	// Transformation functions
	void RotateCameraAxisX(const int& degree);
	void RotateCameraAxisY(const int& degree);
	void RotateCameraAxisZ(const int& degree);
	void TranslateCamera(glm::vec3& moveVec);
	void ScaleCamera(glm::vec3& scaleVec);
	void UpdateView();

	//void OrthogonalProjections(const int& degree);
	void SetProjection();
	void SetViewVolumeSize(const float& left, const float& right, const float& down, const float& up, const float& near, const float& far);
	
	// Get/Set camera mode
	void SetCameraMode();
	bool GetCameraMode() const;

	float GetViewVolumeWidth() const;
	float GetViewVolumeHeight() const;
	float GetViewVolumeFar() const;
	float GetViewVolumeNear() const;
	glm::vec3 getCameraPosition() const;


private:
	// view volume parameters.
	bool othoElsePerspective;
	float _right, _left, _down, _up, _near, _far;
	glm::vec3 viewVolumeScale;
	glm::vec3 centerOfProjection;
	glm::vec3 cameraPosition;

	glm::mat4 view_transformation;

	//


	glm::mat4 cameraScale;
	glm::mat4 cameraTranslation;
	glm::mat4 cameraRotation;
	//
	glm::mat4 view_Inverse;
	glm::mat4 projection_transformation;
};
