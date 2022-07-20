#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <Utils.h>
#include <iostream>
#include "Matrix.h"



Renderer::Renderer()
{
	InitOpenglRendering();
	fillOrLine = true;
}

Renderer::~Renderer()
{

}


// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenglRendering()
{

	// Loads and compiles a shader.
	program = InitShader( "vshader.glsl", "fshader.glsl" );
	environmentProgram = InitShader("skyBoxVshader.glsl", "skyBoxFshader.glsl");

	// Make this program the current one.
	glUseProgram(program);

}

void Renderer::EnvironmentMapping(const Scene& scene)
{
	glUseProgram(environmentProgram);
	glUniform1i(glGetUniformLocation(environmentProgram, "skybox"), 0);

	glDepthFunc(GL_LEQUAL);
	glm::mat4 view = glm::mat4(glm::mat3(scene.GetActiveCamera().GetInverse()));
	glm::mat4 projection = scene.GetActiveCamera().GetProjectionTransformation();
	glUniformMatrix4fv(glGetUniformLocation(environmentProgram, "view"), 1, GL_FALSE, &(view[0].x));
	glUniformMatrix4fv(glGetUniformLocation(environmentProgram, "projection"), 1, GL_FALSE, &(projection[0].x));
	glUniform1f(glGetUniformLocation(environmentProgram, "orthoElsePerspective"), scene.GetActiveCamera().GetCameraMode());
	
	glBindVertexArray(scene.GetSceneVAO());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, scene.GetSceneTexture());
	// the cube structure consist of 36 vertices
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vec3) * 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);

	glUseProgram(program);
}


void Renderer::Render(const Scene& scene, const glm::vec3& clear_color)
{

	int cameraCount = scene.GetCameraCount();
	if (cameraCount > 0)
	{
		int modelCount = scene.GetModelCount();
		const Camera& camera = scene.GetActiveCamera();

		// get camera's matrix
		//glm::mat4 cameraInverse = MultiplyMatrix(camera.GetInverse(), RotationTransformation(180.0f,'y'));
		glm::mat4 cameraInverse = camera.GetInverse() ;
		glm::mat4 projection = camera.GetProjectionTransformation();

		for (int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		{
			MeshModel currentModel = scene.GetModel(currentModelIndex);
			
			// get model's matrix
			glm::mat4 modelTransformation = MultiplyMatrix(currentModel.GetWorldTransformation(), currentModel.GetLocalTransformation());
			glm::mat4 modelRotation = currentModel.GetRotation();
			
			// Set the uniform variables
			//GLint ptr = glGetUniformLocation(program, "model");
			glUniformMatrix4fv(glGetUniformLocation(program, "model"),1, GL_FALSE, &(modelTransformation[0].x));
			glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, &(cameraInverse[0].x));
			glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &(projection[0].x));

			
			// useTexture use as a bool in fshader to tell if to use texture or shading
			glUniform1i(glGetUniformLocation(program, "useTexture"), currentModel.GetTextureMapBool());
			if (currentModel.GetTextureMapBool())
			{
				// Bools that execute differant kinds of mapping and shading
				glUniform1i(glGetUniformLocation(program, "useTextureAsNormalMap"), currentModel.GetUseNormalMap());
				glUniform1i(glGetUniformLocation(program, "useEnvironmentMap"), scene.GetUseEnvironmentTexture());
				

				// tell the type of texture mapping method to use
				glUniform1i(glGetUniformLocation(program, "textureType"), currentModel.GetTextureType());
				glUniform1f(glGetUniformLocation(program, "textureRadius"), currentModel.GetTextureRadius());

				// sent max and min y,x coords to help calculating canonical mapping
				vec3 maxCoords, minCoords;
				currentModel.GetMinMaxVertices(minCoords, maxCoords);
				glUniform1f(glGetUniformLocation(program, "MaxXCoord"), maxCoords.x);
				glUniform1f(glGetUniformLocation(program, "MinXCoord"), minCoords.x);
				glUniform1f(glGetUniformLocation(program, "MaxYCoord"), maxCoords.y);
				glUniform1f(glGetUniformLocation(program, "MinYCoord"), minCoords.y);

				// binding model//environment texture
				glUniform1i(glGetUniformLocation(program, "textureMap"), 0);
				glUniform1i(glGetUniformLocation(program, "skybox"), 0);
			}
			if(scene.GetLightCount())
			{
				// passing model components
				glm::vec3 modelAmbient = currentModel.GetAmbientLightColor();
				glm::vec3 modelDiffuse = currentModel.GetDiffuseLightColor();
				glm::vec3 modelSpecular = currentModel.GetSpecularLightColor();
				GLfloat specularExp = currentModel.GetSpecularExponent();

				glUniform3fv(glGetUniformLocation(program, "modelAmbient"), 1, &(modelAmbient[0]));
				glUniform3fv(glGetUniformLocation(program, "modelDiffuse"), 1, &(modelDiffuse[0]));
				glUniform3fv(glGetUniformLocation(program, "modelSpecular"), 1, &(modelSpecular[0]));

				// passing light components
				glm::vec3 lightAmbient = scene.GetActiveLight().GetAmbientLightColor();
				glm::vec3 lightDiffuse = scene.GetActiveLight().GetDiffuseLightColor();
				glm::vec3 lightSpecular = scene.GetActiveLight().GetSpecularLightColor();
				glm::vec3 lightPos = scene.GetActiveLight().GetLightPosition();

				glUniform3fv(glGetUniformLocation(program, "lightAmbient"), 1, &(lightAmbient[0]));
				glUniform3fv(glGetUniformLocation(program, "lightDiffuse"), 1, &(lightDiffuse[0]));
				glUniform3fv(glGetUniformLocation(program, "lightSpecular"), 1, &(lightSpecular[0]));
				glUniform3fv(glGetUniformLocation(program, "lightPos"), 1, &(lightPos[0]));
				glUniform1f(glGetUniformLocation(program, "specularExp"), specularExp);
			}

			// passing toon shading flag and light level count
			glUniform1i(glGetUniformLocation(program, "useToonShading"), scene.GetuseToonShading());
			glUniform1f(glGetUniformLocation(program, "numberOfLevels"), scene.GetLevelsForToon());

			//	passing camera position
			glm::vec3 cameraPos = camera.getCameraPosition();
			glUniform3fv(glGetUniformLocation(program, "cameraPos"), 1, &(cameraPos[0]));

			// model rotation for normals rotations
			glUniformMatrix4fv(glGetUniformLocation(program, "modelRotation"), 1, GL_FALSE, &(modelRotation[0].x));


			

			// Drag our model's faces (triangles) in fill mode
			glPolygonMode(GL_FRONT_AND_BACK, fillOrLine ? GL_FILL : GL_LINE);

			
			if (scene.GetUseEnvironmentTexture())
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, scene.GetSceneTexture());
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, currentModel.GetModelTexture());
			}

			

			glBindVertexArray(currentModel.GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel.GetModelSize());
			glBindVertexArray(0);

			// unbind texture
			glBindTexture(GL_TEXTURE_2D, 0);


		}

		if (scene.GetUseEnvironmentTexture())
		{
			EnvironmentMapping(scene);
		}
	}
}
