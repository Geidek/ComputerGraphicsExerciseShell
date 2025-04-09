#include <iostream>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Shader.h"
#include "Mesh.h"

// frame buffer size callback is needed to resize the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void InitGLFWSetOpenGLVersionAndExtensionHints()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Only for MacOS
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}


int main()
{

	InitGLFWSetOpenGLVersionAndExtensionHints();

	// Create a Window  
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	// Handle Window creation failure
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// set newly created window to current window in glfw context
	glfwMakeContextCurrent(window);

	// use glad to load opengl function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// set open gl view port 
	glViewport(0, 0, 800, 600);


	// Framebuffer size callback to resize the window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Einfaches Vertex- und Fragment-Shader-Source (als String)
	const std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        out vec3 ourColor;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            ourColor = aColor;
        }
    )";

	const std::string fragmentShaderSource = R"(
        #version 330 core
        in vec3 ourColor;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(ourColor, 1.0);
        }
    )";

	// Erstelle den Shader aus den Source-Strings
	Shader shader(vertexShaderSource, fragmentShaderSource);

	// Beispiel-Daten: Ein Quadrat bestehend aus 4 Punkten und 2 Dreiecken (Index-Puffer)
	std::vector<float> vertices = {
		// Position             // Farbe
		 0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,  // rechts oben
		 0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,  // rechts unten
		-0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,  // links unten
		-0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 1.0f   // links oben
	};

	std::vector<uint32_t> indices = {
		0, 1, 3,  // Erstes Dreieck
		1, 2, 3   // Zweites Dreieck
	};

	// Beschreibe das Layout: Zuerst 3 Float für Position, dann 3 Float für Farbe
	std::vector<Mesh::Attribute> vertexLayout = {
		{ GL_FLOAT, 3 },
		{ GL_FLOAT, 3 }
	};

	Mesh mesh(vertices, indices, vertexLayout);

	// Setze einen einfachen Model-, View- und Projection-Uniform
	// (Für einen echten Anwendungscode würden hier etwa Kameras und Transformationen eingesetzt)
	shader.use();
	// Hier setzen wir mal Identitätsmatrizen; in einem echten Szenengraphen wären hier die entsprechenden Matrizen
	float identity[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramId(), "model"), 1, GL_FALSE, identity);
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramId(), "view"), 1, GL_FALSE, identity);
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramId(), "projection"), 1, GL_FALSE, identity);



	// Setup Imgui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();



	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// if esc is pressed - exit application
		processInput(window);

		// Prepare ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Create UI Logic
		ImGui::Begin("Hello, world!");
		ImGui::SetWindowSize(ImVec2(200, 75), ImGuiCond_Once);
		ImGui::End();

		// Render ImGui
		ImGui::Render();

		// set clear framebuffer with abitrary color 
		// first set clear color and then clear color buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// use the shader program
		shader.use();
		mesh.Draw();


		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// swap buffers to present the rendered image
		glfwSwapBuffers(window);

		// poll events to receive updates such as key down event or resizes
		glfwPollEvents();



	}

	// terminate glfw to enshure clean shutdown
	glfwTerminate();
	return 0;

}
