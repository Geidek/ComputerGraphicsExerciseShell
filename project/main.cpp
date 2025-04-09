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

std::string loadShaderSource(const char* filepath) {
	std::ifstream file;
	std::stringstream buffer;

	file.open(filepath);
	if (file.is_open()) {
		buffer << file.rdbuf();
		file.close();
	}
	else {
		std::cerr << "Konnte Datei nicht öffnen: " << filepath << std::endl;
	}
	return buffer.str();
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

	// Vertex-Shader Quellcode
	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aCol;\n"
		"out vec4 vertexColor;\n"
		"void main() {\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"   vertexColor = vec4(aCol, 1.0);\n"
		"}\0";

	// Fragment-Shader Quellcode
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec4 vertexColor;\n"
		"void main() {\n"
		"   FragColor = vertexColor;\n"
		"}\0";


	// Kompiliere den Vertex-Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);


	// Kompiliere den Fragment-Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	// Verlinke die Shader zu einem Shaderprogramm
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	// Lösche die Shader, da sie im Programm bereits verlinkt sind
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		// Position (x,y,z)   // Farbe (r,g,b)
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // oben rechts
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // unten rechts
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  // unten links
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f   // oben links
	};


	// Indizes, die die beiden Dreiecke definieren, welche das Rechteck bilden
	unsigned int indices[] = {
		0, 1, 3,  // erstes Dreieck
		1, 2, 3   // zweites Dreieck
	};

	unsigned int VBO, VAO, EBO;
	// Erzeuge ein Vertex Array Object, einen Vertex Buffer Object und ein Element Buffer Object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Binde das VAO, dann den VBO und kopiere die Vertex-Daten in den Puffer
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Definiere, wie die Vertex-Daten interpretiert werden sollen
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Enable Wireframe Mode = draw only the edges of the triangles
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Disable Wireframe Mode = fill the triangles
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


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

		// Verwende das Shaderprogramm und zeichne das Dreieck
		glUseProgram(shaderProgram);

		// update the uniform color
		float timeValue = glfwGetTime();
		float strength = cos(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "strength");
		glUniform1f(vertexColorLocation, strength);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// swap buffers to present the rendered image
		glfwSwapBuffers(window);

		// poll events to receive updates such as key down event or resizes
		glfwPollEvents();



	}
	// Ressourcen freigeben
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	// terminate glfw to enshure clean shutdown
	glfwTerminate();
	return 0;

}
