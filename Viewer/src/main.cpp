#define _USE_MATH_DEFINES

//#include <chrono>
#include <cmath>
#include <stdio.h>
#include <glad/glad.h>
#include <Renderer.h>

#include "ImGui.h"

//#include "ChessMatch.h"


/**
 * Fields
 */

int CurrentModelsIndes = NULL;
glm::vec4 mesh_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

// Another fields
ImVec2 MousePosVec = { 0,0 };


bool show_demo_window = false;
bool show_another_window = false;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io, ChessMatch* chessMatch);
void Cleanup(GLFWwindow* window);
// This default Imgui draw function has been superseded
//void DrawImguiMenus(ImGuiIO& io, Scene& scene);

// Another Function declarations
void KeyBoardMouseEvents(Scene& scene, GLFWwindow* window, ImGuiIO& io, Renderer& renderer, ChessMatch* chessMatch);
ChessMatch* StartChessGame(const int& time, Scene& scene);
void EndChessGame(ChessMatch* chessMatch);
void InitializeChessModels(Scene& scene, ChessMatch* chessMatch);
void MoveTransperantWindow(Scene& scene, float xSquere, float ySquere);



/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char **argv)
{
	// memory leak check
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//

	int windowWidth = 1920, windowHeight = 1080;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);

	// Enable depth considaration
	glEnable(GL_DEPTH_TEST);

	// Enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer();
	Scene scene = Scene(windowWidth, windowHeight);



	
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);

	ChessMatch* chessMatch = StartChessGame(10, scene);

	//scene.AddModel(Utils::LoadMeshModel("../Data/ChessData/ChessBoard.obj"));
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
		StartFrame();		


		MyImGui(io, scene, window);
		KeyBoardMouseEvents(scene, window, io, renderer, chessMatch);
		RenderFrame(window, scene, renderer, io, chessMatch);
    }

	EndChessGame(chessMatch);

	Cleanup(window);
    return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	
	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io, ChessMatch* chessMatch)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	

	

	// if fog affect in the scene is on the background will change to the color of the fog.
	/*if (scene.GetFogEffect())
	{
		renderer.SetBackground(glm::vec4((scene.GetFogColor()), 1));
		renderer.ClearColorBuffer(glm::vec4((scene.GetFogColor()), 1));
	}
		
	else
	{
		renderer.SetBackground(clear_color);
		renderer.ClearColorBuffer(clear_color);
	}*/
		
	// Clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	renderer.Render(scene, clear_color, chessMatch);


	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

//chrono::milliseconds myTimer;

// Custom function to handle keyboard presses and Mouse dragging
void KeyBoardMouseEvents(Scene& scene, GLFWwindow* window, ImGuiIO& io, Renderer& renderer, ChessMatch* chessMatch)
{
	// for camera
	
	// Get current window's screenWidth and screenHeight for scaling purposes
	int screenWidth, screenHeight;
	glfwGetWindowSize(window, &screenWidth, &screenHeight);
	float keySpeed = 10.0f;
	glm::mat4 cameraRotation = scene.GetCamera(scene.GetActiveCameraIndex()).GetRotationMatrix();
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		float scale = scene.GetActiveCamera().GetViewVolumeHeight()/(float)screenHeight;
		if (!scene.GetActiveCamera().GetCameraMode())
		{
			scale *= 20;
		}
		glm::vec3 keyboardMoveVec = { 0,keySpeed * scale,0 };
		//keyboardMoveVec = MultiplyVectorMatrix(scene.GetCamera(scene.GetActiveCameraIndex()).GetRotationMatrix(), { keyboardMoveVec,1 });
		keyboardMoveVec = cameraRotation * vec4(keyboardMoveVec, 1);
		scene.GetCamera(scene.GetActiveCameraIndex()).TranslateCamera(keyboardMoveVec);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		float scale = scene.GetActiveCamera().GetViewVolumeHeight()/(float)screenHeight;
		if (!scene.GetActiveCamera().GetCameraMode())
		{
			scale *= 20;
		}
		glm::vec3 keyboardMoveVec = { 0,-keySpeed * scale,0 };
		//keyboardMoveVec = MultiplyVectorMatrix(scene.GetCamera(scene.GetActiveCameraIndex()).GetRotationMatrix(), { keyboardMoveVec,1 });
		keyboardMoveVec = cameraRotation * vec4(keyboardMoveVec, 1);
		scene.GetCamera(scene.GetActiveCameraIndex()).TranslateCamera(keyboardMoveVec);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		float scale = scene.GetActiveCamera().GetViewVolumeWidth() / (float)screenHeight;
		if (!scene.GetActiveCamera().GetCameraMode())
		{
			scale *= 20;
		}
		glm::vec3 keyboardMoveVec = { -keySpeed * scale, 0, 0 };
		//keyboardMoveVec = MultiplyVectorMatrix(scene.GetCamera(scene.GetActiveCameraIndex()).GetRotationMatrix(), { keyboardMoveVec,1 });
		keyboardMoveVec = cameraRotation * vec4(keyboardMoveVec, 1);
		scene.GetCamera(scene.GetActiveCameraIndex()).TranslateCamera(keyboardMoveVec);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		float scale = scene.GetActiveCamera().GetViewVolumeWidth() / (float)screenHeight;
		if (!scene.GetActiveCamera().GetCameraMode())
		{
			scale *= 20;
		}
		glm::vec3 keyboardMoveVec = { keySpeed * scale, 0, 0 };
		//keyboardMoveVec = MultiplyVectorMatrix(scene.GetCamera(scene.GetActiveCameraIndex()).GetRotationMatrix(), { keyboardMoveVec,1 });
		keyboardMoveVec = cameraRotation * vec4(keyboardMoveVec, 1);
		scene.GetCamera(scene.GetActiveCameraIndex()).TranslateCamera(keyboardMoveVec);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		float scale = scene.GetActiveCamera().GetViewVolumeWidth() / (float)screenHeight;
		if (!scene.GetActiveCamera().GetCameraMode())
		{
			scale *= 20;
		}
		glm::vec3 keyboardMoveVec = { 0, 0, keySpeed * scale };
		//keyboardMoveVec = MultiplyVectorMatrix(scene.GetCamera(scene.GetActiveCameraIndex()).GetRotationMatrix(), { keyboardMoveVec,1 });
		keyboardMoveVec = cameraRotation * vec4(keyboardMoveVec, 1);
		scene.GetCamera(scene.GetActiveCameraIndex()).TranslateCamera(keyboardMoveVec);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		float scale = scene.GetActiveCamera().GetViewVolumeWidth() / (float)screenHeight;
		if (!scene.GetActiveCamera().GetCameraMode())
		{
			scale *= 20;
		}
		glm::vec3 keyboardMoveVec = { 0, 0, -keySpeed * scale };
		//keyboardMoveVec = MultiplyVectorMatrix(scene.GetCamera(scene.GetActiveCameraIndex()).GetRotationMatrix(), { keyboardMoveVec,1 });
		keyboardMoveVec = cameraRotation * vec4(keyboardMoveVec, 1);
		scene.GetCamera(scene.GetActiveCameraIndex()).TranslateCamera(keyboardMoveVec);
	}


	else if (io.MouseDown[2])
	{
		if (MousePosVec.x != io.MousePos.x || MousePosVec.y != io.MousePos.y)
		{
			float scale = 1;
			// if camera is perspective
			if (!scene.GetActiveCamera().GetCameraMode())
			{
				scale = 20;
			}
			float widthScale = (scene.GetActiveCamera().GetViewVolumeWidth()* scale) / (float)screenHeight;
			float heightScale = (scene.GetActiveCamera().GetViewVolumeHeight()* scale) / (float)screenHeight;
			vec3 mouseVec = { (io.MousePos.x - MousePosVec.x)* widthScale,(MousePosVec.y - io.MousePos.y)* heightScale, 0 };
			mouseVec = MultiplyVectorMatrix(scene.GetCamera(scene.GetActiveCameraIndex()).GetRotationMatrix(), { mouseVec,1 });
			scene.GetCamera(scene.GetActiveCameraIndex()).TranslateCamera(mouseVec);
			MousePosVec = io.MousePos;
		}
	}
	else if (io.MouseDown[1])
	{
		if (MousePosVec.x != io.MousePos.x)
		{
			scene.GetCamera(scene.GetActiveCameraIndex()).RotateCameraAxisY(io.MousePos.x - MousePosVec.x);
		}
		if (MousePosVec.y != io.MousePos.y)
		{
			scene.GetCamera(scene.GetActiveCameraIndex()).RotateCameraAxisX(io.MousePos.y - MousePosVec.y);

		}
		MousePosVec = io.MousePos;
	}
	//else if (io.MouseClicked[0] && renderer.MouseOnSquere)
	//{
	//	c
	//	MousePosVec = io.MousePos;
	//}

	else
		MousePosVec = io.MousePos;


	// Chess Related
	ImVec2 MouseC = io.MousePos;
	if (MouseC[0] >= 710.0f && MouseC[0] <= 1210.0f && MouseC[1] >= 275.0f && MouseC[1] <= 775.0f)
	{
		bool validation = false;
		renderer.MouseOnSquere = true;
		TableColorType& chessBoardColorRef = *(chessMatch->GetTableColorType());

		// If the crusor on a table squere we will assign them
		ImVec2 squeres = { (MouseC[0] - 710.0f) / (1210.0f - 710.0f) , (MouseC[1] - 275.0f) / (775.0f - 275.0f) };
		if (squeres[0] * 8.0f >= 0 && squeres[0] * 8.0f < 8)
		{
			renderer.xSquere = squeres[0] * 8.0f;
			validation = true;
		}
		if (squeres[1] * 8.0f >= 0 && squeres[1] * 8.0f < 8 && validation)
		{
			renderer.ySquere = squeres[1] * 8.0f;
		}
		else
		{
			validation = false;
		}
		renderer.ySquere = abs(renderer.ySquere - 7);

		// if we moved to other squere we will replace the pointed squere
		if ((renderer.xSquere != chessBoardColorRef.currentSquere[0] || renderer.ySquere != chessBoardColorRef.currentSquere[1]) && validation)
		{
			// update current the cursor pointed squere
			// zero the later squere only if the player can't move to there
			int clickedSquere[2] = {chessBoardColorRef.ClickedSquere[0], chessBoardColorRef.ClickedSquere[1]};
			if (!(chessBoardColorRef.clicked && chessMatch->TestMove(clickedSquere , &chessBoardColorRef.currentSquere[0])))
			{
				chessBoardColorRef.data[chessBoardColorRef.currentSquere[0]][chessBoardColorRef.currentSquere[1]] = 0;
			}			
			chessBoardColorRef.currentSquere[0] = renderer.xSquere;
			chessBoardColorRef.currentSquere[1] = renderer.ySquere;
			chessBoardColorRef.data[chessBoardColorRef.currentSquere[0]][chessBoardColorRef.currentSquere[1]] = 1;
			chessBoardColorRef.change = true;
		}

		// If Clicked squere was changed, reset the colored movable squeres
		if ((chessBoardColorRef.ClickedSquere[0] != renderer.xSquere || chessBoardColorRef.ClickedSquere[1] != renderer.ySquere ) && io.MouseClicked[0] == 1)
		{
			bool moveWasMade = false;
			// zeroing outgoing squeres
			while (!chessBoardColorRef.MoveToSqueresFromClicked.empty())
			{
				int outgoingSquere[2] = { chessBoardColorRef.MoveToSqueresFromClicked.top()[0], chessBoardColorRef.MoveToSqueresFromClicked.top()[1] };
				chessBoardColorRef.MoveToSqueresFromClicked.pop();
				// zero the squere only if the cursor is not pointing on him
				if (outgoingSquere[0] != renderer.xSquere || outgoingSquere[1] != renderer.ySquere)
				{
					chessBoardColorRef.data[outgoingSquere[0]][outgoingSquere[1]] = 0;
				}
			}

			// check if a clicked squere was saved and if so check to make a move from there the the new clicked sqeure
			if (chessBoardColorRef.ClickedSquere[0] != -1 && chessBoardColorRef.ClickedSquere[1] != -1 && chessBoardColorRef.clicked)
			{
				int from[2] = { chessBoardColorRef.ClickedSquere[0], chessBoardColorRef.ClickedSquere[1] };
				int toSquere[2] = { renderer.xSquere, renderer.ySquere };
				if (chessMatch->TestMove(from, toSquere))
				{
					chessMatch->MakeMove(from, toSquere);

					// reset tabel color buffer
					
					// reset clicked color
					chessBoardColorRef.data[chessBoardColorRef.ClickedSquere[0]][chessBoardColorRef.ClickedSquere[1]] = 0;
					chessBoardColorRef.ClickedSquere[0] = -1;
					chessBoardColorRef.ClickedSquere[1] = -1;
					moveWasMade = true;
				}
				chessBoardColorRef.clicked = false;
			}
			if(!moveWasMade)
			{

				chessBoardColorRef.ClickedSquere[0] = renderer.xSquere;
				chessBoardColorRef.ClickedSquere[1] = renderer.ySquere;

				// Inserting every possible squere the player can move to
				int currentSquere[2] = { chessBoardColorRef.ClickedSquere[0], chessBoardColorRef.ClickedSquere[1] };
				for (int i = 0; i < BOARD_ROWS; i++)
				{
					for (int j = 0; j < BOARD_COLS; j++)
					{
						int toSquere[2] = { i, j };
						if (chessMatch->TestMove(currentSquere, toSquere))
						{
							std::array<int, 2> toSquere = { i,j };
							chessBoardColorRef.MoveToSqueresFromClicked.push(toSquere);
							chessBoardColorRef.data[toSquere[0]][toSquere[1]] = 1;

						}
					}
				}
				chessBoardColorRef.clicked = true;
			}

			chessBoardColorRef.change = true;
		}

	}
	else
	{
		renderer.MouseOnSquere = false;
	}

}



ChessMatch* StartChessGame(const int& time, Scene& scene)
{
	ChessMatch* chessMatch = new ChessMatch(time);
	AddModel("../Data/ChessData/ChessBoard.obj", scene, chessMatch->chessBoardImage);
	InitializeChessModels(scene, chessMatch);
	//AddModel("../Data/ChessData/ChessBoard.obj", scene, chessMatch->chessBoardImage);

	return chessMatch;
}

void InitializeChessModels(Scene& scene , ChessMatch* chessMatch)
{
	// Initialize white

	for (int i = 0; i < NUM_OF_PIECES; i++)
	{
		AddModel("../Data/ChessData/ChessBoard.obj", scene, chessMatch->GetPlayer(WHITE)->pieces[i]->pieceImage);
		chessMatch->GetPlayer(WHITE)->pieces[i]->SetModel(&scene.GetModel(scene.GetModelCount() - 1));
		chessMatch->GetPlayer(WHITE)->pieces[i]->InitializeModelPlace();
	}

	// Initialize black

	for (int i = 0; i < NUM_OF_PIECES; i++)
	{
		AddModel("../Data/ChessData/ChessBoard.obj", scene, chessMatch->GetPlayer(BLACK)->pieces[i]->pieceImage);
		chessMatch->GetPlayer(BLACK)->pieces[i]->SetModel(&scene.GetModel(scene.GetModelCount() - 1));
		chessMatch->GetPlayer(BLACK)->pieces[i]->InitializeModelPlace();
	}
}

void EndChessGame(ChessMatch* chessMatch)
{
	delete chessMatch;
}

void MoveTransperantWindow(Scene& scene, float xSquere, float ySquere)
{
	scene.GetModel(33).ResetTransformation();
	scene.GetModel(33).ScaleObject(vec3(0.125f), true);
	xSquere -= 3.5; ySquere -= 3.5;
	scene.GetModel(33).TranslateObject(vec3(0, 0, -1.05), true);
	if (xSquere < 0)
	{
		xSquere = floor(xSquere);
		scene.GetModel(33).TranslateObject(vec3(0.125028f * -xSquere - (0.125028f / 2), 0 , 0), true);
	}
	else
	{
		xSquere = ceil(xSquere);
		scene.GetModel(33).TranslateObject(vec3(0.125028f * -xSquere + (0.125028f / 2), 0, 0), true);
	}

	if (ySquere < 0)
	{
		ySquere = floor(ySquere);
		scene.GetModel(33).TranslateObject(vec3(0, 0.125028f * -ySquere - (0.125028f / 2), 0), true);
	}
	else
	{
		ySquere = ceil(ySquere);
		scene.GetModel(33).TranslateObject(vec3(0, 0.125028f * -ySquere + (0.125028f / 2), 0), true);
	}

}