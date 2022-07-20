#include "Matrix.h"
#include <iostream>

// Matrix multiplication method


vec4 MultiplyVectorMatrix(const mat4& mat,const vec4& vec)
{

	vec4 result = { 0,0,0,1 };
	for (int i = 0; i < 4; i++)
	{
		float sumtemp = 0;
		for (int j = 0; j < 4; j++)
		{
			sumtemp += mat[i][j] * vec[j];
		}
		result[i] = sumtemp;
	}
	return result;
}


// Scale Transform mat4 using vec3 input
mat4 ScalingTransformation(const glm::vec3& temp)
{
	glm::mat4 scaleMat(1);
	for (int i = 0; i < scaleMat.length() - 1; i++)
	{
		scaleMat[i][i] = temp[i];
	}
	return scaleMat;
}

// Translate Transform mat4 using vec3 input
mat4 TranslatingTransformation(const glm::vec3& temp)
{
	glm::mat4 TranslateMat(1);
	for (int i = 0; i < TranslateMat.length() - 1; i++)
	{
		TranslateMat[i][3] = temp[i];
	}
	return TranslateMat;
}

// Rotate Transform mat4 using float angle and char axis input
mat4 RotationTransformation(const float& angle, const char& axis)
{
	float angleRadian = float(angle * (2 * M_PI) / 360);
	glm::mat4 RotateMat(1);
	int inc1 = 0, inc2 = 0;

	switch (axis)
	{
	case 'x':
		inc1 += 1;
		inc2 += 2;
		break;
	case 'y':
		inc2 += 2;
		break;
	case 'z':
		inc2 += 1;
		break;
	default:
		// worthy to put here an exception
		break;
	}
	RotateMat[inc1][inc1] = cos(angleRadian);
	RotateMat[inc1][inc2] = -sin(angleRadian);
	RotateMat[inc2][inc1] = sin(angleRadian);
	RotateMat[inc2][inc2] = cos(angleRadian);

	return RotateMat;
}




