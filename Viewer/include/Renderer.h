#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Scene.h"
#include "ChessMatch.h"

// Lazy stuff
struct terrain_data {
	int data[8][8] = { 0 };
	int lastSquere[2] = { 0 };
	bool change = false;
};

extern terrain_data chessBoardColorData;

//

class Renderer
{
public:
	Renderer();
	virtual ~Renderer();
	void Render(const Scene& scene, const glm::vec3& clear_color, ChessMatch* chessMatch);
	void Renderer::InitOpenglRendering();

	void EnvironmentMapping(const Scene& scene);

	void setFillOrLine()
	{
		fillOrLine = !fillOrLine;
	}

	bool MouseOnSquere;
	int xSquere;
	int ySquere;

private:
	GLuint program;
	GLuint environmentProgram;
	bool fillOrLine;

};
