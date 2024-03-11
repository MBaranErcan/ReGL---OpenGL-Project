#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// GLM Mathematics Library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Dependencies/stb_image.h"
#include "Shaders/Shader.h"
#include "Source/Camera.h"


void void_framebuffer_size_callback(GLFWwindow* window, int width, int height);	// Whenever the window is resized, this callback function executes. It adjusts the viewport so that the OpenGL renders to the new window size.
void mouse_callback(GLFWwindow* window, double xpos, double ypos);				// Whenever the mouse moves, this callback function executes.
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);											// Check if the user has pressed the escape key, if so, close the window.

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); // Create a camera object (starting position is at (0.0f, 0.0f, 3.0f)).
float lastX = SCR_WIDTH / 2.0f; // Set the last x position of the mouse to the middle of the screen.
float lastY = SCR_HEIGHT / 2.0f; // Set the last y position of the mouse to the middle of the screen.
bool firstMouse = true; // Set the first mouse movement to true.


// Timing
float deltaTime = 0.0f; // Time between current frame and last frame.
float lastFrame = 0.0f; // Time of last frame.


int main() {

	// Initialize GLFW
	glfwInit(); // Initialize the GLFW library.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We want to use the core-profile means we'll get access to a smaller subset of OpenGL features.

	// Create a Window object.
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello, Window!", NULL, NULL); // width, height, title, monitor, share

	if (window == NULL) {	
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Make the context of the specified window current on the calling thread.
	glfwSetFramebufferSizeCallback(window, void_framebuffer_size_callback);// Set the callback function for the window resize event
	glfwSetCursorPosCallback(window, mouse_callback); // Set the callback function for the mouse movement event.
	glfwSetScrollCallback(window, scroll_callback); // Set the callback function for the mouse scroll event.

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide the cursor and capture it.

	// Load GLAD so it configures OpenGL. GLAD manages function pointers for OpenGL so we want to initialize GLAD before we call any OpenGL function.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // gladLoadGLLoader returns 0 if it fails
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// -----------------------------
	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// ------------------------SHADERS------------------------
	Shader myShader("Shaders/Texture.vert", "Shaders/Texture.frag"); // Create a shader object and read the vertex and fragment shader files.


	// ------------------------VERTICES------------------------
	// --------------------------------------
	// Cube vertices
	float vertices[] = {
		// positions			// colors			// texture coords
		// 4 vertices (front) for the cube
		 0.5f,  0.5f, 0.5f,		1.0f, 0.0f, 0.0f,   1.0f, 1.0f,	// front-top right
		 0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,   1.0f, 0.0f,	// front-bottom right
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,   0.0f, 0.0f,	// front-bottom left
		-0.5f,  0.5f, 0.5f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f,	// front-top left 
		// 4 vertices (right) for the cube
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,   1.0f, 1.0f,	// front-top right
		 0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 0.0f,   1.0f, 0.0f,	// front-bottom right
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,   0.0f, 0.0f,	// back-bottom right
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,	// back-top right
		// 4 vertices (left) for the cube
		-0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,   1.0f, 1.0f,	// back-top left
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,   1.0f, 0.0f,	// back-bottom left
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,   0.0f, 0.0f,	// front-bottom left
		-0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,	// front-top left		
		// 4 vertices (back) for the cube		 
		 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,   1.0f, 1.0f,	// back-top right
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,   1.0f, 0.0f,	// back-bottom right
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,   0.0f, 0.0f,	// back-bottom left
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,	// back-top left
		// 4 vertices (top) for the cube
		 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,   1.0f, 1.0f,	// back-top right
		 0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,   1.0f, 0.0f,	// front-top right
		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,   0.0f, 0.0f,	// front-top left
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,	// back-top left
		 // 4 vertices (bottom) for the cube
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,   1.0f, 1.0f,	// front-bottom right
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,   1.0f, 0.0f,	// back-bottom right
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,   0.0f, 0.0f,	// back-bottom left
		-0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,	// front-bottom left
	};
	

	unsigned int indices[] = {	// Instead of drawing the rectangle with 6 vertices, we can draw it with 2 triangles using 4 vertices.
		// Front
		0, 3, 1, // First tri
		1, 3, 2, // Second tri
		// Right
		4, 7, 5, // First tri
		5, 7, 6, // Second tri
		// Left
		8, 11, 9, // First tri
		9, 11, 10, // Second tri
		// Back
		12, 15, 13, // First tri
		13, 15, 14, // Second tri
		// Top
		16, 19, 17, // First tri
		17, 19, 18, // Second tri
		// Bottom
		20, 23, 21, // First tri
		21, 23, 22, // Second tri
		};


	// -------------------VBO & VAO & EBO-------------------	
	// Vertex Buffer Object (VBO) can store a large number of vertices in the GPU's memory so we can render a large object quickly.
	// Vertex Array Object (VAO) can store the configuration of vertex attributes (like pointers to vertex attributes in the VBO) and which VBO to use.
	// Element Buffer Object (EBO) is a buffer, just like a vertex buffer object, that stores indices that OpenGL uses to decide what vertices to draw.
	unsigned int VAO, VBO, EBO; // Create a VBO, VAO, and EBO.
	glGenVertexArrays(1, &VAO); // Generate 1 vertex array object and store the resulting identifier in VAO.
	glGenBuffers(1, &VBO); // Generate 1 buffer and store the resulting identifier in VBO.
	glGenBuffers(1, &EBO); // Generate 1 buffer and store the resulting identifier in EBO.

	glBindVertexArray(VAO); // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Send the data to the buffer.
	/*The fourth parameter specifies how we want the graphics card to manage the given data. This can take 3 forms:
	GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
	GL_STATIC_DRAW : the data is set only once and used many times.
	GL_DYNAMIC_DRAW : the data is changed a lot and used many times.
	*/

	// Bind the EBO and send the data to the buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind the EBO.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Send the data to the buffer.

	// Specify how OpenGL should interpret the vertex data before rendering.
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Specify the vertex attributes using glVertexAttribPointer.
	glEnableVertexAttribArray(0); // Enable the vertex attribute at location 0.
	
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Specify the vertex color attributes using glVertexAttribPointer.
	glEnableVertexAttribArray(1); // Enable the vertex attribute at location 1.

	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // Specify the vertex texture attributes using glVertexAttribPointer.
	glEnableVertexAttribArray(2); // Enable the vertex attribute at location 2.

	// Unbind the VBO and VAO. This is good practice so we don't accidentally modify the VBO and VAO while we're not using them.
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO.
	glBindVertexArray(0); // Unbind the VAO.


	// Wireframe & Fill modes
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment to draw in wireframe mode.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Draw in fill mode.


	//int nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;


	// -------------------TEXTURE-------------------
	// Texture 1
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1); // Generate 1 texture and store the resulting identifier in texture.
	glBindTexture(GL_TEXTURE_2D, texture1); // Bind the texture so that all subsequent texture commands will configure the currently bound texture.

	// Texture Wrapping S,T,R (x, y, z)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT - S
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // GL_REPEAT - T
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT); // GL_REPEAT - R
	

	// Texture Filtering
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Use nearest neighbor filtering for minifying the texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Use mipmaps for minifying the texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Use linear filtering for magnifying the texture. Mipmaps are not useful here.


	// Load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // Tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("Textures/wall.jpg", &width, &height, &nrChannels, 0);
	if (data) { // If the data is not null
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // Generate a 2D texture image.
		glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmaps for the currently bound texture.
	}
	else {
		std::cout << "Failed to load texture1" << std::endl;
	}
	stbi_image_free(data); // Free the image memory.

	//------------
	// Texture 2
	glGenTextures(1, &texture2); // Generate the second texture.
	glBindTexture(GL_TEXTURE_2D, texture2); // Bind the second texture so subsequent texture commands will configure only the second texture.

	// Texture Wrapping S,T,R (x, y, z)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT - S
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // GL_REPEAT - T
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT); // GL_REPEAT - R
	
	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_LINEAR is better for downscaling.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_LINEAR is better for upscaling.

	// Load and generate the texture2
	data = stbi_load("Textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); // Generate a 2D texture image.
			glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmaps for the currently bound texture.
		}
	else {
		std::cout << "Failed to load texture2" << std::endl;
	}
	stbi_image_free(data); // Free the image memory.

	// Tell OpenGL for each sampler to which texture unit it belongs to (only has to be done once)
	myShader.use(); // Use the shader program.
	myShader.setInt("texture1", 0); // Set the texture1 sampler to the texture unit 0.
	myShader.setInt("texture2", 1); // Set the texture2 sampler to the texture unit 1.


	// Transformations (Translate, Rotate, Scale)
	glm::mat4 transform = glm::mat4(1.0f); // Initialize the transformation matrix as the identity matrix.
	transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -3.0f)); // Translate the transformation matrix.
	transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate the transformation matrix by 45 degrees on the z-axis.
	transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f)); // Scale the transformation matrix.


	//----------------------------------------------------
	// RENDER LOOP
	while (!glfwWindowShouldClose(window)) { // Check if the window should close, if not, render the next frame.

		// Per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime()); // Get the current time as seconds.
		deltaTime = currentFrame - lastFrame; // Calculate the time difference between the current frame and the last frame.
		lastFrame = currentFrame; // Set the lastFrame to the currentFrame.

		// Input
		processInput(window); // Check if the user has pressed the escape key, if so, close the window.

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the color to clear the screen with.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen's color and depth buffer.

		// Bind the textures
		glActiveTexture(GL_TEXTURE0); // Activate the texture unit first before binding the texture.
		glBindTexture(GL_TEXTURE_2D, texture1); // Bind the texture so that all subsequent texture commands will configure the currently bound texture.
		glActiveTexture(GL_TEXTURE1); // Activate the texture unit first before binding the texture.
		glBindTexture(GL_TEXTURE_2D, texture2); // Bind the texture so that all subsequent texture commands will configure the currently bound texture.


		// Draw the rectangle
		myShader.use(); // Use the shader program.


		// Projection matrix
		glm::mat4 projectiýn = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); // Create a projection matrix.
		myShader.setMat4("projection", projectiýn); // Set the projection matrix in the shader.
		// Camera/view	formation
		glm::mat4 view = camera.GetViewMatrix(); // Create a view matrix.
		myShader.setMat4("view", view); // Set the view matrix in the shader.

		// Model matrix
		glm::mat4 model = glm::mat4(1.0f); // Initialize the model matrix as the identity matrix.
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f)); // Rotate the model matrix.
		myShader.setMat4("model", model); // Set the model matrix in the shader.


		// Render
		glBindVertexArray(VAO); // Bind the VAO.
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0); // Draw the rectangle using the VAO and EBO.
		glBindVertexArray(0); // Unbind the VAO.


		glfwSwapBuffers(window); // Swap the front and back buffers so the user can see the output.
		glfwPollEvents(); // Check if any events are triggered (like keyboard input or mouse movement events).
	
	}


	// De-allocate all resources once they've outlived their purpose.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);


	// Clean up
	glfwTerminate(); // Clean up the resources we've allocated.
	return 0;

}

//-----------------------------------------------------------
// USER INPUT
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

//-----------------------------------------------------------
// CALLBACK FUNCTIONS
// glfw: Framebuffer size callback
void void_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{		// Whenever the window is resized, this callback function executes. It adjusts the viewport so that the OpenGL renders to the new window size.
		glViewport(0, 0, width, height);
}

// glfw: Mouse callback
void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{		// Whenever the mouse moves, this callback function executes.
	float xpos = static_cast<float>(xPos);
	float ypos = static_cast<float>(yPos);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: Scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{		// Whenever the mouse scroll wheel scrolls, this callback function executes.
	camera.ProcessMouseScroll(yoffset);
}
