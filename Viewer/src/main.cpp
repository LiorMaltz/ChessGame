#define _USE_MATH_DEFINES


#include <cmath>
#include <stdio.h>
#include <glad/glad.h>


#include "Scene.h"
#include "ImGui.h"

//#include "Matrix.h"



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
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
// This default Imgui draw function has been superseded
//void DrawImguiMenus(ImGuiIO& io, Scene& scene);

// Another Function declarations
void KeyBoardMouseEvents(Scene& scene, GLFWwindow* window, ImGuiIO& io, Renderer& renderer);



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
	glEnable(GL_DEPTH_TEST);

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer();
	Scene scene = Scene(windowWidth, windowHeight);
	
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
		StartFrame();
		// Replaced with MyImGui
		//DrawImguiMenus(io, scene);

		
		/*const float radius = 2.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		float camY = cos(glfwGetTime()) * radius;
		scene.GetActiveCamera().SetCameraLookAt(glm::vec3(camX, camY, camZ), glm::vec3(0, 0, 0), glm::vec3(0.0, 1.0, 0.0));*/
		


		MyImGui(io, scene, window, renderer);
		KeyBoardMouseEvents(scene, window, io,renderer);
		RenderFrame(window, scene, renderer, io);
    }
	//	Clear z-Depth array
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

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	

	if (!io.WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here
		if (io.KeysDown[65])
		{
			// A key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)
		}
	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
			// Left mouse button is down
		}
	}

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
	
	renderer.Render(scene,clear_color);


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


// Custom function to handle keyboard presses and Mouse dragging
void KeyBoardMouseEvents(Scene& scene, GLFWwindow* window, ImGuiIO& io, Renderer& renderer)
{
	// for camera
	
	// Get current window's screenWidth and screenHeight for scaling purposes
	int screenWidth, screenHeight;
	glfwGetWindowSize(window, &screenWidth, &screenHeight);

	float keySpeed = 10.0f;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		float scale = scene.GetActiveCamera().GetViewVolumeHeight()/(float)screenHeight;
		if (!scene.GetActiveCamera().GetCameraMode())
		{
			scale *= 20;
		}
		glm::vec3 keyboardMoveVec = { 0,keySpeed* scale,0 };
		keyboardMoveVec = MultiplyVectorMatrix(scene.GetCamera(scene.GetActiveCameraIndex()).GetRotationMatrix(), { keyboardMoveVec,1 });
		scene.GetCamera(scene.GetActiveCameraIndex()).TranslateCamera(keyboardMoveVec);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		float scale = scene.GetActiveCamera().GetViewVolumeHeight()/(float)screenHeight;
		if (!scene.GetActiveCamera().GetCameraMode())
		{
			scale *= 20;
		}
		glm::vec3 keyboardMoveVec = { 0,-keySpeed* scale,0 };
		keyboardMoveVec = MultiplyVectorMatrix(scene.GetCamera(scene.GetActiveCameraIndex()).GetRotationMatrix(), { keyboardMoveVec,1 });
		scene.GetCamera(scene.GetActiveCameraIndex()).TranslateCamera(keyboardMoveVec);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		float scale = scene.GetActiveCamera().GetViewVolumeWidth() / (float)screenHeight;
		if (!scene.GetActiveCamera().GetCameraMode())
		{
			scale *= 20;
		}
		glm::vec3 keyboardMoveVec = { -keySpeed* scale,0,0 };
		keyboardMoveVec = MultiplyVectorMatrix(scene.GetCamera(scene.GetActiveCameraIndex()).GetRotationMatrix(), { keyboardMoveVec,1 });
		scene.GetCamera(scene.GetActiveCameraIndex()).TranslateCamera(keyboardMoveVec);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		float scale = scene.GetActiveCamera().GetViewVolumeWidth() / (float)screenHeight;
		if (!scene.GetActiveCamera().GetCameraMode())
		{
			scale *= 20;
		}
		glm::vec3 keyboardMoveVec = { keySpeed* scale,0,0 };
		keyboardMoveVec = MultiplyVectorMatrix(scene.GetCamera(scene.GetActiveCameraIndex()).GetRotationMatrix(), { keyboardMoveVec,1 });
		scene.GetCamera(scene.GetActiveCameraIndex()).TranslateCamera(keyboardMoveVec);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		float scale = scene.GetActiveCamera().GetViewVolumeWidth() / (float)screenHeight;
		if (!scene.GetActiveCamera().GetCameraMode())
		{
			scale *= 20;
		}
		glm::vec3 keyboardMoveVec = { 0,0,-keySpeed * scale };
		keyboardMoveVec = MultiplyVectorMatrix(scene.GetCamera(scene.GetActiveCameraIndex()).GetRotationMatrix(), { keyboardMoveVec,1 });
		scene.GetCamera(scene.GetActiveCameraIndex()).TranslateCamera(keyboardMoveVec);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		float scale = scene.GetActiveCamera().GetViewVolumeWidth() / (float)screenHeight;
		if (!scene.GetActiveCamera().GetCameraMode())
		{
			scale *= 20;
		}
		glm::vec3 keyboardMoveVec = { 0,0,keySpeed * scale };
		keyboardMoveVec = MultiplyVectorMatrix(scene.GetCamera(scene.GetActiveCameraIndex()).GetRotationMatrix(), { keyboardMoveVec,1 });
		scene.GetCamera(scene.GetActiveCameraIndex()).TranslateCamera(keyboardMoveVec);
	}




	if (io.MouseDown[2])
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
	else
		MousePosVec = io.MousePos;

}