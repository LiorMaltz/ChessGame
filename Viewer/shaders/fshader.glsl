#version 330 core

#define PI 3.141592654

// model shading components
uniform vec3 modelAmbient;
uniform vec3 modelDiffuse;
uniform vec3 modelSpecular;

// light shading components

uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

// color aid tools
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform mat4 modelRotation;
uniform float specularExp;

// textures
uniform samplerCube skybox;
uniform	sampler2D textureMap;

// texture bool
uniform bool useTexture;
uniform bool useTextureAsNormalMap;
uniform bool useEnvironmentMap;

// parameters sent by the vertexShader
in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexCoords;
in vec3 localVetexPos;

// values that help of mapping canonical textures
flat in int textureTypeFrag;
uniform int textureType;
uniform float textureRadius;
uniform float MinYCoord;
uniform float MaxYCoord;
uniform float MinXCoord;
uniform float MaxXCoord;

// toon shading
uniform bool useToonShading;
uniform float numberOfLevels;

// final color to print
out vec4 frag_color;


// phong shading function
void PhongShading(in vec3 normal, inout vec4 FinalColor)
{
	// calculate ambient
	vec3 ambientResult = modelAmbient * lightAmbient;

	// calculate diffuse
	vec3 diffuseResult = vec3(0);

	//	update normal direction
	vec3 updatedNormal = vec3(vec4(normal,1.0f) * modelRotation);

	vec3 centeredLightPos = normalize(lightPos - fragPos);
	float normalLightAngleCos = dot(centeredLightPos,updatedNormal);
	if(normalLightAngleCos>=0)
	{
		// floor coefficient for toon shading
		if(useToonShading)
		{
			normalLightAngleCos = floor(normalLightAngleCos * numberOfLevels) / numberOfLevels;
		}
		diffuseResult = modelDiffuse * normalLightAngleCos * lightDiffuse;
	}

	// calculate specular
	vec3 specularResult = vec3(0);
	if (normalLightAngleCos >= 0)
	{
		// reflection calculation
		centeredLightPos = -centeredLightPos;
		vec3 lightReflection = (centeredLightPos - 2.0f * updatedNormal * (dot(updatedNormal, centeredLightPos)));

		// calculate angles between the camera and the reflection and also the angle of normal-camera. and check if they are no more than 90 degrees
		vec3 centeredCameraPosition = cameraPos - fragPos;
		float specularAngleCos = dot(lightReflection, centeredCameraPosition) / (length(lightReflection) * length(centeredCameraPosition));
		float faceNormalToCameraCos = dot(centeredCameraPosition, updatedNormal) / (length(centeredCameraPosition) * length(updatedNormal));

		if (specularAngleCos >= 0 && faceNormalToCameraCos>=0)
		{
			// floor coefficient for toon shading
			if(useToonShading)
			{
				specularAngleCos = floor(specularAngleCos * numberOfLevels) / numberOfLevels;
			}
			specularResult = modelSpecular * pow(specularAngleCos, specularExp) * lightSpecular;
		}
	}

	// phong reflection model
	vec3 finalColor = ambientResult + diffuseResult + specularResult;
	FinalColor = vec4(finalColor ,1.0f);
}

void main() 
{
	vec4 FinalColor;
	// use phong shading
	if(!useTexture) 
	{
		PhongShading(fragNormal, FinalColor);
	}

	// use normal mapping
	else if(useTextureAsNormalMap)
	{
		//vec3 normal = normalize(vec3(texture(textureMap, fragTexCoords)) * 2.0f - 1.0f);
		vec3 normal = normalize(vec3(texture(textureMap, fragTexCoords)));
		PhongShading(normal, FinalColor);
	}

	// use environment mapping
	else if(useEnvironmentMap)
	{
		//vec3 Normal = fragNormal * mat3(inverse(modelRotation));
		vec3 Normal = mat3(inverse(modelRotation)) * fragNormal;
		vec3 CenteredPostion = normalize(fragPos - cameraPos);
		// build in reflect function
		vec3 Reflection = reflect(CenteredPostion, normalize(Normal));
		FinalColor = vec4(texture(skybox, Reflection).rgb, 1.0);
	}

	// use texture
	else
	{
		vec2 finalUV = fragTexCoords;

		// plane projection case
		if (textureType == 2)
		{
			finalUV.x = (MaxXCoord - localVetexPos.x)/ (MaxXCoord - MinXCoord);
			finalUV.y = (MaxYCoord - localVetexPos.y)/ (MaxYCoord - MinYCoord);
		}

		// cylinder projection case
		//if (textureType == 3 && localVetexPos.y != MaxYCoord && localVetexPos.y != MinYCoord)
		if (textureType == 3)
		{
			// need to designate an interpolated v coordinate from the range of the vertices-model Y's coordinates
			finalUV.y = 1 - ((MaxYCoord - localVetexPos.y) / (MaxYCoord - MinYCoord) / 2) ;

			// similar to x,y coordinates of sphere mapping
			finalUV.x = 1 - atan(localVetexPos.z, localVetexPos.x) / (2 * PI);
		}

		// case of sphere mapping (doing reflection on the y axis) and handle 359-0 interpolation case
		if(textureType == 4)
		{
			// z = r*cos(phi)
			finalUV.y = acos(localVetexPos.z/textureRadius) / PI;

			// x = r*cos(theta)*sin(phi) and y = r*sin(theta)*sin(phi) so in the end we'll get tan(theta) = y/x
			finalUV.x = 1 - atan(localVetexPos.y, localVetexPos.x) / (2 * PI);
		}

		// textureColor get the color from the texture map that loaded to the app in 'finalUV' coordinate
		vec3 textureColor = vec3(texture(textureMap, finalUV));
		FinalColor = vec4(textureColor ,1.0f);
	}

	//FinalColor = vec4(0,0,0,1);
	frag_color = FinalColor;
	

} 

