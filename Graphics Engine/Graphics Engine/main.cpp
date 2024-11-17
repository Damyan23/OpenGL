#include <glad/glad.h> // Helps load the correct OpenGL function pointers
#include <GLFW/glfw3.h> // Helps set up an environment for OpenGl
#include <iostream> // Include the iostream header
#include <stb-master/stb_image.h>
#include "imgui-master/imgui.h"
#include "imgui-master/backends/imgui_impl_glfw.h"
#include "imgui-master/backends/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <C:\Users\dpeyc\OneDrive\Desktop\Graphics Engine\Graphics Engine\Header Files\Shader.h>
#include <C:\Users\dpeyc\OneDrive\Desktop\Graphics Engine\Graphics Engine\Header Files\TextureGenerator.h>
#include <C:\Users\dpeyc\OneDrive\Desktop\Graphics Engine\Graphics Engine\Header Files\Camera.h>

#include <string>

void framebuffer_size_callback(GLFWwindow* window, int aWidth, int aHeigt);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const* path);
void renderCube(glm::vec3 position, glm::mat4 model, Shader shader);
void renderPyramid(glm::vec3 position, glm::mat4 model, Shader shader);
void renderSphere(glm::vec3 position, glm::mat4 model, Shader shader, float radius = 1.0f, unsigned int sectorCount = 36, unsigned int stackCount = 18);
unsigned int width = 800, height = 600;

glm::vec3 cameraPos, cameraFront, cameraUp;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

bool firstMouse;

double xpos, ypos;
float pitch, yaw;

float lastX, lastY;
float fov = 45;

GLuint VAO, VBO;

// Define a variable to store the rotation angle
float rotationAngle = 0.0f;

int main()
{
	// Initialize GLFW
	glfwInit();

	// Setup the version of OpenGl I am going to use (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//// Set the profile to the core one
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Graphics Engine", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize Glad" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);

	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	Camera camera(window, cameraPos, cameraUp);
	glfwSetWindowUserPointer(window, &camera);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Shader lightingCubeShader("Shaders/lighting_cube_vertex.glsl", "Shaders/lighting_cube_fragment.glsl");
	Shader lightingShader("Shaders/basic_lighting_vertex.glsl", "Shaders/basic_lighting_fragment.glsl");
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// only use the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	lightingShader.setVec3("lightPos", lightPos);

	glm::mat4 view, proj;


	// creating defuse and specular
	unsigned int diffuseMap = loadTexture(std::string("C:/Users/dpeyc/OneDrive/Desktop/Graphics Engine/Graphics Engine/container2.png").c_str());
	unsigned int specularMap = loadTexture(std::string("C:/Users/dpeyc/OneDrive/Desktop/Graphics Engine/Graphics Engine/container2_specular.png").c_str());

	// shader configuration
	lightingShader.use();
	// Activate texture unit 0 and bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	lightingShader.setInt("material.diffuse", 0);  // Set the uniform to match texture unit 0

	// Activate texture unit 1 and bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	lightingShader.setInt("material.specular", 1);  // Set the uniform to match texture unit 1



	//// Render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwGetCursorPos(window, &xpos, &ypos);

		camera.ProcessKeyboardInput(deltaTime);
		camera.ProcessMouseMovement(xpos, ypos);
		glfwSetScrollCallback(window, scroll_callback);

		lightingShader.use();
		// Set object and light color
		lightingShader.setVec3("viewPos", camera.position);

		lightingShader.setFloat("material.shininess", 32.0f);

		//// point light 1
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09f);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032f);

		//// Directional Light
		//lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		//lightingShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
		//lightingShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
		//lightingShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

		// Spotlight
		//lightingShader.setVec3("spotLight.position", camera.position);
		//lightingShader.setVec3("spotLight.direction", camera.front);
		//lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		//lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		//lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		//lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		//lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		//lightingShader.setFloat("spotLight.constant", 1.0f);
		//lightingShader.setFloat("spotLight.linear", 0.09f);
		//lightingShader.setFloat("spotLight.quadratic", 0.032f);

		// view/projection transformations
		view = camera.GetViewMatrix();
		proj = camera.GetProjectionMatrix(width / height, 0.1f, 100.0f);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", proj);

		// Change thecolor of the background
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Make opengl preform depth testing
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);

		// Bind textures to appropriate texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		//render the cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		renderCube(glm::vec3 (0, 0, 0), model, lightingShader);
		renderSphere(glm::vec3(2, 0, 0), model, lightingShader);
		renderPyramid(glm::vec3(-2, 0, 0), model, lightingShader);

		// also draw the lamp object
		lightingCubeShader.use();
		lightingCubeShader.setMat4("projection", proj);
		lightingCubeShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lightingCubeShader.setMat4("model", model);

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}

// When the user stretches the window this function is called by GLFW and updates the window
void framebuffer_size_callback(GLFWwindow* window, int aWidth, int aHeight)
{
	glViewport(0, 0, aWidth, aHeight);
	width = aWidth;
	height = aHeight;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	if (camera) {
		camera->ProcessMouseScroll(yoffset);
	}
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}


unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube(glm::vec3 position, glm::mat4 model, Shader shader)
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
			float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	if (position != glm::vec3(0, 0, 0))
	{
		model = glm::translate(glm::mat4(1.0f), position);
		shader.setMat4("model", model);
	}

	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}



unsigned int pyramidVAO = 0;
unsigned int pyramidVBO = 0;
void renderPyramid(glm::vec3 position, glm::mat4 model, Shader shader)
{
	if (pyramidVAO == 0)
	{
		float vertices[] = {
			// Positions            // Normals           // Texture Coordinates
			// Front face (Triangle 1)
			0.0f, 1.0f, 0.0f,      0.0f, 1.0f, 0.0f,      0.5f, 1.0f,  // Apex
			-1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f,      0.0f, 0.0f, // Bottom-left
			1.0f, -1.0f, 1.0f,     0.0f, 1.0f, 0.0f,      1.0f, 0.0f, // Bottom-right

			// Right face (Triangle 2)
			0.0f, 1.0f, 0.0f,      1.0f, 0.0f, 0.0f,      0.5f, 1.0f,  // Apex
			1.0f, -1.0f, 1.0f,     1.0f, 0.0f, 0.0f,      0.0f, 0.0f, // Bottom-left
			1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 0.0f,      1.0f, 0.0f, // Bottom-right

			// Back face (Triangle 3)
			0.0f, 1.0f, 0.0f,      0.0f, -1.0f, 0.0f,     0.5f, 1.0f,  // Apex
			1.0f, -1.0f, -1.0f,    0.0f, -1.0f, 0.0f,     0.0f, 0.0f, // Bottom-left
			-1.0f, -1.0f, -1.0f,   0.0f, -1.0f, 0.0f,     1.0f, 0.0f, // Bottom-right

			// Left face (Triangle 4)
			0.0f, 1.0f, 0.0f,      -1.0f, 0.0f, 0.0f,     0.5f, 1.0f, // Apex
			-1.0f, -1.0f, -1.0f,   -1.0f, 0.0f, 0.0f,     0.0f, 0.0f, // Bottom-left
			-1.0f, -1.0f, 1.0f,    -1.0f, 0.0f, 0.0f,     1.0f, 0.0f, // Bottom-right

			// Base (Square)
			-1.0f, -1.0f, 1.0f,    0.0f, 0.0f, -1.0f,     0.0f, 0.0f, // Bottom-left
			1.0f, -1.0f, 1.0f,     0.0f, 0.0f, -1.0f,     1.0f, 0.0f, // Bottom-right
			1.0f, -1.0f, -1.0f,    0.0f, 0.0f, -1.0f,     1.0f, 1.0f, // Top-right

			-1.0f, -1.0f, 1.0f,    0.0f, 0.0f, -1.0f,     0.0f, 0.0f, // Bottom-left
			1.0f, -1.0f, -1.0f,    0.0f, 0.0f, -1.0f,     1.0f, 1.0f, // Top-right
			-1.0f, -1.0f, -1.0f,   0.0f, 0.0f, -1.0f,     0.0f, 1.0f  // Top-left
		};

		glGenVertexArrays(1, &pyramidVAO);
		glGenBuffers(1, &pyramidVBO);

		glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(pyramidVAO);

		// Position Attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Normal Attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Texture Coordinate Attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// If a position is provided, apply translation
	if (position != glm::vec3(0, 0, 0))
	{
		model = glm::translate(glm::mat4(1.0f), position);
		shader.setMat4("model", model);
	}

	// Render the pyramid
	shader.use();
	glBindVertexArray(pyramidVAO);
	glDrawArrays(GL_TRIANGLES, 0, 18); // 6 faces * 3 vertices per face
	glBindVertexArray(0);
}




unsigned int sphereVAO = 0;
unsigned int sphereVBO = 0;
unsigned int sphereEBO = 0;

void renderSphere(glm::vec3 position, glm::mat4 model, Shader shader, float radius, unsigned int sectorCount, unsigned int stackCount)
{
	// Initialize (if necessary)
	if (sphereVAO == 0)
	{
		// Calculate the number of vertices and indices
		unsigned int vertexCount = (stackCount + 1) * (sectorCount + 1);
		unsigned int indexCount = stackCount * sectorCount * 6;

		// Allocate memory for vertices and indices arrays
		float* vertices = new float[vertexCount * 8]; // 8 values per vertex: 3 position, 3 normal, 2 texture coords
		unsigned int* indices = new unsigned int[indexCount];

		// Generate vertices for the sphere
		unsigned int vertexIndex = 0;
		for (unsigned int i = 0; i <= stackCount; ++i)
		{
			float stackAngle = glm::pi<float>() * i / stackCount; // Stack angle from 0 to pi
			float xy = radius * sin(stackAngle); // Radius at current stack level
			float z = radius * cos(stackAngle); // Z position at current stack level

			// Vertices of the current stack
			for (unsigned int j = 0; j <= sectorCount; ++j)
			{
				float sectorAngle = 2 * glm::pi<float>() * j / sectorCount; // Sector angle from 0 to 2pi

				// Calculate vertex position
				float x = xy * cos(sectorAngle);
				float y = xy * sin(sectorAngle);

				// Normal vector for lighting calculations
				glm::vec3 normal(x, y, z);
				normal = glm::normalize(normal);

				// Store the position, normal, and texture coordinates
				vertices[vertexIndex++] = x;
				vertices[vertexIndex++] = y;
				vertices[vertexIndex++] = z;
				vertices[vertexIndex++] = normal.x;
				vertices[vertexIndex++] = normal.y;
				vertices[vertexIndex++] = normal.z;
				vertices[vertexIndex++] = (float)j / sectorCount; // Texture coordinates (0 to 1)
				vertices[vertexIndex++] = (float)i / stackCount;
			}
		}

		// Generate indices for the sphere (connecting vertices to form triangles)
		unsigned int indexIndex = 0;
		for (unsigned int i = 0; i < stackCount; ++i)
		{
			unsigned int k1 = i * (sectorCount + 1); // Current stack's starting vertex index
			unsigned int k2 = k1 + sectorCount + 1; // Next stack's starting vertex index

			for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				// Two triangles per sector
				if (i != 0)
				{
					indices[indexIndex++] = k1;
					indices[indexIndex++] = k2;
					indices[indexIndex++] = k1 + 1;
				}

				if (i != (stackCount - 1))
				{
					indices[indexIndex++] = k1 + 1;
					indices[indexIndex++] = k2;
					indices[indexIndex++] = k2 + 1;
				}
			}
		}

		// Create and bind the VAO, VBO, and EBO
		glGenVertexArrays(1, &sphereVAO);
		glGenBuffers(1, &sphereVBO);
		glGenBuffers(1, &sphereEBO);

		// Bind the VAO
		glBindVertexArray(sphereVAO);

		// Fill the VBO with vertex data
		glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * 8 * sizeof(float), vertices, GL_STATIC_DRAW);

		// Fill the EBO with index data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		// Set vertex attributes
		glEnableVertexAttribArray(0); // Vertex position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1); // Vertex normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

		glEnableVertexAttribArray(2); // Texture coordinates
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		// Unbind the buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Cleanup
		delete[] vertices;
		delete[] indices;
	}

	if (position != glm::vec3(0, 0, 0))
	{
		model = glm::translate(glm::mat4(1.0f), position);
		shader.setMat4("model", model);
	}

	// In your render loop or update function, add a small increment to the angle and update the model matrix
	rotationAngle += 0.01f; // Adjust 0.01f to control the speed of rotation

	model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);


	// Render Sphere
	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLES, stackCount * sectorCount * 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

