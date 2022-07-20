#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer();
	virtual ~Renderer();
	void Render(const Scene& scene, const glm::vec3& clear_color);
	void Renderer::InitOpenglRendering();

	void EnvironmentMapping(const Scene& scene);

	void setFillOrLine()
	{
		fillOrLine = !fillOrLine;
	}


private:
	GLuint program;
	GLuint environmentProgram;

	bool fillOrLine;
};
