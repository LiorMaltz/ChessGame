#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

Camera::Camera(const float& width, const float& height)
{
	centerOfProjection = { 0,0,0};
	_right = width / 1000.0f;;
	_left = -width / 1000.0f;;
	_up = height / 1000.0f;;
	_down = -height / 1000.0f;;
	_near = 1.5f;
	_far = 20.0f;


	viewVolumeScale.x = _right-_left;
	viewVolumeScale.y = _up-_down;
	viewVolumeScale.z = _far-_near;

	othoElsePerspective = true;

	SetCameraLookAt({ 0,0,-3 }, { 0,0,-4 }, { 0,1,0 });



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

	glm::vec3 zaxis = glm::normalize(at - eye);
	glm::vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
	glm::vec3 yaxis = glm::normalize(glm::cross(zaxis, xaxis));
	vec4 t = vec4(eye.x, eye.y, eye.z, 1.0);
	view_transformation =glm::transpose( mat4({ xaxis,0 }, { yaxis,0 }, { zaxis,0 }, t));
	cameraRotation = glm::transpose(mat4({ xaxis,0 }, { yaxis,0 }, { zaxis,0 }, { 0,0,0,1 }));

	//view_transformation=MultiplyMatrix(TranslatingTransformation(eye), view_transformation);
	view_Inverse = glm::inverse(view_transformation);

	cameraPosition = eye;
	centerOfProjection = at;
	SetProjection();

	//PrintMatrix(view_transformation);

}

void Camera::SetProjection()
{
	//glm::mat4 viewVolume;


	if (othoElsePerspective)
	{
		//viewVolume = { 1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1 };
		projection_transformation = glm::ortho(_left, _right , _down , _up , _near, _far);
		//projection_transformation = glm::ortho(_left, _right, _down, _up, _near, _far);

		


	}
	else
	{
		//viewVolume = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,1.0f / _near,1 };
		//projection_transformation = glm::transpose(glm::frustum(_left , _right , _down , _up, -_near, _far));
		projection_transformation = glm::frustum(_left, _right, _down, _up, _near, _far);
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
	/*glm::vec4 vecW = glm::vec4(view_transformation[3][0], view_transformation[3][1], view_transformation[3][2], 1);
	vecW = glm::normalize(vecW);
	glm::mat4 rotationTemp = view_transformation; rotationTemp[3][0] = 0; rotationTemp[3][1] = 0; rotationTemp[3][2] = 0; rotationTemp[3][3] = 1;
	rotationTemp = MultiplyMatrix(rotationTemp, MultiplyMatrix(RotationTransformation(degree, 'x'), glm::inverse(rotationTemp)));
	view_transformation = MultiplyMatrix(glm::inverse(rotationTemp), view_transformation);
	view_Inverse = glm::inverse(view_transformation);*/

	glm::mat4 rotationMatrix = RotationTransformation(degree, 'x');
	view_transformation = MultiplyMatrix(view_transformation, rotationMatrix);
	cameraRotation = MultiplyMatrix(cameraRotation, rotationMatrix);
	view_Inverse = MultiplyMatrix(glm::transpose(rotationMatrix), view_Inverse);
	SetProjection();
}
void Camera::RotateCameraAxisY(const int& degree)
{
	//glm::vec4 vecW = glm::vec4(view_transformation[3][0] , view_transformation[3][1], view_transformation[3][2] ,1 );
	//vecW = glm::normalize(vecW);
	////glm::mat4 rotationTemp = view_transformation; rotationTemp[3][0] = vecW.x; rotationTemp[3][1] = vecW.y; rotationTemp[3][2] = vecW.z; rotationTemp[3][3] = vecW.w;
	//glm::mat4 rotationTemp = view_transformation; rotationTemp[3][0] = 0; rotationTemp[3][1] = 0; rotationTemp[3][2] = 0; rotationTemp[3][3] = 1;
	//rotationTemp = MultiplyMatrix(rotationTemp, MultiplyMatrix(RotationTransformation(degree, 'y'), glm::inverse(rotationTemp)));
	////view_transformation = MultiplyMatrix(view_transformation, rotationTemp);
	//view_transformation = MultiplyMatrix(glm::inverse(rotationTemp), view_transformation);
	//view_Inverse = glm::inverse(view_transformation);

	glm::mat4 rotationMatrix = RotationTransformation(degree, 'y');
	view_transformation = MultiplyMatrix(view_transformation, rotationMatrix);
	cameraRotation = MultiplyMatrix(cameraRotation, rotationMatrix);
	view_Inverse = MultiplyMatrix(glm::transpose(rotationMatrix), view_Inverse);
	SetProjection();
}
void Camera::RotateCameraAxisZ(const int& degree)
{
	/*glm::vec4 vecW = glm::vec4(view_transformation[3][0], view_transformation[3][1], view_transformation[3][2], 1);
	vecW = glm::normalize(vecW);
	glm::mat4 rotationTemp = view_transformation; rotationTemp[3][0] = 0; rotationTemp[3][1] = 0; rotationTemp[3][2] = 0; rotationTemp[3][3] = 1;
	rotationTemp = MultiplyMatrix(rotationTemp, MultiplyMatrix(RotationTransformation(degree, 'z'), glm::inverse(rotationTemp)));
	view_transformation = MultiplyMatrix(glm::inverse(rotationTemp), view_transformation);
	view_Inverse = glm::inverse(view_transformation);*/

	glm::mat4 rotationMatrix = RotationTransformation(degree, 'z');
	view_transformation = MultiplyMatrix(view_transformation, rotationMatrix);
	cameraRotation = MultiplyMatrix(cameraRotation, rotationMatrix);
	view_Inverse = MultiplyMatrix(glm::transpose(rotationMatrix), view_Inverse);
	SetProjection();
}


void Camera::TranslateCamera(glm::vec3& moveVec)
{
	view_transformation = MultiplyMatrix(view_transformation, TranslatingTransformation(moveVec));
	view_Inverse = MultiplyMatrix(view_Inverse, TranslatingTransformation(-moveVec));

	cameraPosition += moveVec;
}

// not in use
void Camera::ScaleCamera(glm::vec3& scaleVec)
{
	/*view_transformation = MultiplyMatrix(view_transformation, ScalingTransformation(scaleVec));
	view_Inverse = glm::inverse(view_transformation);*/
	cameraScale = MultiplyMatrix(cameraScale, ScalingTransformation(scaleVec));
	UpdateView();
}


// not in use
void Camera::UpdateView()
{
	view_transformation = MultiplyMatrix(cameraTranslation, MultiplyMatrix(cameraRotation, cameraScale));
	view_Inverse = glm::inverse(view_transformation);
}



