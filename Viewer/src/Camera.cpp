#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

Camera::Camera(const float& width, const float& height)
{
	centerOfProjection = { 0,0,0};
	_right = width / 1000.0f;
	_left = -width / 1000.0f;
	_up = height / 1000.0f;
	_down = -height / 1000.0f;
	_near = 0.5f;
	_far = 20.0f;


	viewVolumeScale.x = _right - _left;
	viewVolumeScale.y = _up - _down;
	viewVolumeScale.z = _far - _near;

	othoElsePerspective = true;

	SetCameraLookAt({ 0,0, -10 }, { 0,0,0.01 }, { 0,1,0 });



}

Camera::~Camera()
{
	
}

const glm::mat4& Camera::GetProjectionTransformation() const
{
	return projection_transformation;
}

const glm::mat4& Camera::GetViewTransformation() const
{
	return view_transformation;
}
glm::vec3 Camera::getCameraPosition() const
{
	return cameraPosition;
}


	// calculate the transformation of the camera in the world and it's invers
void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	// builds the Coordinate system of the camera

	//



	vec3 zaxis = normalize(at - eye);
	vec3 xaxis = -normalize(cross(zaxis, up));
	vec3 yaxis = -normalize(cross(xaxis, zaxis));
	//
	//vec4 t = vec4(eye.x, eye.y, eye.z, 1.0);
	
	view_transformation = mat4(vec4(xaxis, 0), vec4(yaxis, 0), -vec4(zaxis, 0), vec4(eye, 1));
	//PrintMatrix(view_transformation);
	//view_transformation = glm::lookAt(eye, at, up);
	
	//cameraRotation = glm::rotate(view_transformation, glm::radians(180.0f), vec3(1, 0, 0));
	//cameraRotation = mat4(1);
	//PrintMatrix(cameraRotation);
	cameraRotation = mat4(view_transformation);
	cameraRotation[3][0] = cameraRotation[3][1] = cameraRotation[3][2] = 0; cameraRotation[2][2] = -cameraRotation[2][2];
	
	view_Inverse = glm::inverse(view_transformation);
	
	cameraPosition = eye;
	centerOfProjection = at;
	
	SetProjection();
}

void Camera::SetProjection()
{
	//glm::mat4 viewVolume;


	if (othoElsePerspective)
	{
		projection_transformation = glm::ortho(_left, _right , _down , _up , _near, _far);
		//projection_transformation = glm::ortho(_left, _right, _down, _up, _near, _far);


	}
	else
	{
		//projection_transformation = transpose(frustum(_left, _right, _down, _up, -_far, -_near));
		projection_transformation = frustum(_left, _right, _down, _up, _near, _far);
		//projection_transformation = MultiplyMatrix(projection_transformation, viewVolume);
	}

	

	//PrintMatrix(projection_transformation);
	
}

void Camera::SetViewVolumeSize(const float& left, const float& right, const float& down, const float& up, const float& near, const float& far)
{
	_right = right;
	_left = left;
	_up = up;
	_down = down;
	_near = near;
	_far = far;

	viewVolumeScale.x = _right - _left;
	viewVolumeScale.y = _up - _down;
	viewVolumeScale.z = _far - _near;

	//viewVolumeScale = newScale;
	SetProjection();

}

// Get/Set camera mode
void Camera::SetCameraMode()
{
	othoElsePerspective = !othoElsePerspective;
	SetProjection();
}

bool Camera::GetCameraMode() const
{
	return othoElsePerspective;
}


//
glm::mat4 Camera::GetRotationMatrix() const
{
	return cameraRotation;
}
glm::mat4 Camera::GetInverse() const
{
	return view_Inverse;
}

glm::mat4 Camera::GetVeiw() const
{
	return view_transformation;
}

float Camera::GetViewVolumeWidth() const
{
	return viewVolumeScale.x;
}
float Camera::GetViewVolumeHeight() const
{
	return viewVolumeScale.y;
}
float Camera::GetViewVolumeFar() const
{
	return _far;
}
float Camera::GetViewVolumeNear() const
{
	return _near;
}



// camera rotations functions. making the camera to rotate on it's place. or to look at other point without tanslating.
void Camera::RotateCameraAxisX(const int& degree)
{
	float deg = degree;

	glm::mat4 rotationMatrix = glm::rotate(mat4(1), glm::radians(deg), glm::vec3(1, 0, 0));
	cameraRotation = glm::rotate(cameraRotation, glm::radians(deg), glm::vec3(1, 0, 0));
	view_Inverse = rotationMatrix * view_Inverse;
	//glm::mat4 rotationMatrix = RotationTransformation(degree, 'x');
	//cameraRotation = MultiplyMatrix(cameraRotation, rotationMatrix);
	//view_Inverse = MultiplyMatrix(glm::transpose(rotationMatrix), view_Inverse);
	SetProjection();
}
void Camera::RotateCameraAxisY(const int& degree)
{
	float deg = degree;

	glm::mat4 rotationMatrix = glm::rotate(mat4(1), glm::radians(deg), glm::vec3(0, 1, 0));
	cameraRotation = glm::rotate(cameraRotation, glm::radians(deg), glm::vec3(0, 1, 0));
	view_Inverse = rotationMatrix * view_Inverse;

	//glm::mat4 rotationMatrix = RotationTransformation(degree, 'y');
	//view_transformation = MultiplyMatrix(view_transformation, rotationMatrix);
	//cameraRotation = MultiplyMatrix(cameraRotation, rotationMatrix);
	//view_Inverse = MultiplyMatrix(glm::transpose(rotationMatrix), view_Inverse);
	SetProjection();
}
void Camera::RotateCameraAxisZ(const int& degree)
{
	float deg = degree;

	glm::mat4 rotationMatrix = glm::rotate(mat4(1), glm::radians(deg), glm::vec3(0, 0, 1));
	cameraRotation = glm::rotate(cameraRotation, glm::radians(deg), glm::vec3(0, 0, 1));
	view_Inverse = rotationMatrix * view_Inverse;

	//glm::mat4 rotationMatrix = RotationTransformation(degree, 'z');
	//view_transformation = MultiplyMatrix(view_transformation, rotationMatrix);
	//cameraRotation = MultiplyMatrix(cameraRotation, rotationMatrix);
	//view_Inverse = MultiplyMatrix(glm::transpose(rotationMatrix), view_Inverse);
	SetProjection();
}


void Camera::TranslateCamera(glm::vec3& moveVec)
{
	//moveVec.y = -moveVec.y;
	//glm::mat4 temp = translate(mat4(1), moveVec);
	//view_transformation = view_transformation * temp;
	//view_Inverse = view_Inverse * -temp;

	view_transformation = translate(view_transformation, moveVec);
	view_Inverse = translate(view_Inverse, -moveVec);
	//PrintMatrix(view_Inverse);
	//view_transformation = MultiplyMatrix(view_transformation, TranslatingTransformation(moveVec));
	//view_Inverse = MultiplyMatrix(view_Inverse, TranslatingTransformation(-moveVec));

	cameraPosition += moveVec;
}

// not in use
void Camera::ScaleCamera(glm::vec3& scaleVec)
{
	/*view_transformation = MultiplyMatrix(view_transformation, ScalingTransformation(scaleVec));
	view_Inverse = glm::inverse(view_transformation);*/
	cameraScale = MultiplyMatrix(cameraScale, ScalingTransformation(scaleVec));
	//UpdateView();
}


// not in use
void Camera::UpdateView()
{
	view_transformation = cameraRotation * cameraTranslation * cameraScale;
	//view_transformation = MultiplyMatrix(cameraTranslation, MultiplyMatrix(cameraRotation, cameraScale));
	view_Inverse = glm::inverse(view_transformation);
}



