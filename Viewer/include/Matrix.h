#pragma once
#include<iostream>
#include <glm/glm.hpp>

using namespace glm;
#define M_PI 3.14159265358979323846

// Due to unforseen complications, certain matrix operations and transformations have been manually implemented in custom methods.
template<typename T>
T MultiplyMatrix(const T& mat1, const T& mat2)
{
	T result(0);
	for (int i = 0; i < mat1.length(); i++)
	{
		for (int j = 0; j < mat1.length(); j++)
		{
			float sumtemp = 0;
			for (int k = 0; k < mat1.length(); k++)
			{
				sumtemp += mat1[i][k] * mat2[k][j];
			}
			result[i][j] = sumtemp;
		}
	}
	return result;
}

vec4 MultiplyVectorMatrix(const mat4& mat,const vec4& vec);
mat4 ScalingTransformation(const glm::vec3& temp);
mat4 TranslatingTransformation(const glm::vec3& temp);
mat4 RotationTransformation(const float& angle, const char& axis);

template<typename T>
void PrintMatrix(T mat)
{
	for (int i = 0; i < mat.length(); i++)
	{
		for (int j = 0; j < mat.length(); j++)
		{
			std::cout << mat[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

template<typename T>
void PrintVector(T vec)
{
	for (int i = 0; i < vec.length(); i++)
	{
		std::cout << vec[i] << " " << std::endl;
	}
	
}