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

void void_framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


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

	// Set the callback function for the window resize event
	glfwSetFramebufferSizeCallback(window, void_framebuffer_size_callback);


	// Load GLAD so it configures OpenGL. GLAD manages function pointers for OpenGL so we want to initialize GLAD before we call any OpenGL function.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // gladLoadGLLoader returns 0 if it fails
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ------------------------SHADERS------------------------
	Shader myShader("Shaders/Texture.vert", "Shaders/Texture.frag"); // Create a shader object and read the vertex and fragment shader files.

	

	// -------------------RECTANGLE-------------------
	// vertices
	float vertices[] = {
		// positions			// colors			// texture coords
		 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,   1.0f, 1.0f,	// top right
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,   1.0f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,   0.0f, 0.0f,	// bottom left
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f	// top left 
	};
	unsigned int indices[] = {	// Instead of drawing the rectangle with 6 vertices, we can draw it with 2 triangles using 4 vertices.
			0, 1, 3, // First Triangle
			1, 2, 3 // Second Triangle
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
	// It is 16 on my computer.


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
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_LINEAR is better for downscaling.
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_LINEAR is better for upscaling.

	// Texture Filtering by using Mipmaps
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


	// Transformations (Trans
	glm::mat4 transform = glm::mat4(1.0f); // Initialize the transformation matrix as the identity matrix.
	transform = glm::translate(transform, glm::vec3(-0.25f, 0.0f, 0.0f)); // Translate the transformation matrix.
	transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate the transformation matrix by 45 degrees on the z-axis.
	transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f)); // Scale the transformation matrix.

	//----------------------------------------------------
	// RENDER LOOP
	while (!glfwWindowShouldClose(window)) { // Check if the window should close, if not, render the next frame.

		processInput(window); // Check if the user has pressed the escape key, if so, close the window.

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the color to clear the screen with.
		glClear(GL_COLOR_BUFFER_BIT); // Clear the screen with the set color.
	

		glActiveTexture(GL_TEXTURE0); // Activate the texture unit first before binding the texture.
		glBindTexture(GL_TEXTURE_2D, texture1); // Bind the texture so that all subsequent texture commands will configure the currently bound texture.
		
		glActiveTexture(GL_TEXTURE1); // Activate the texture unit first before binding the texture.
		glBindTexture(GL_TEXTURE_2D, texture2); // Bind the texture so that all subsequent texture commands will configure the currently bound texture.



		// Draw the rectangle
		myShader.use(); // Use the shader program.
		unsigned int transformLoc = glGetUniformLocation(myShader.ID, "transform"); // Get the location of the uniform variable "transform" in the shader program.
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform)); // Set the value of the uniform variable "transform" in the shader program.


		// Set the model, view, and projection matrices
		glm::mat4 model = glm::mat4(1.0f); // Initialize the model matrix as the identity matrix.
		glm::mat4 view = glm::mat4(1.0f); // Initialize the view matrix as the identity matrix.
		glm::mat4 projection = glm::mat4(1.0f); // Initialize the projection matrix as the identity matrix.
		
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f)); // Rotate the model matrix.
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // Translate the view matrix.
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); // Set the projection matrix.

		unsigned int modelLoc = glGetUniformLocation(myShader.ID, "model"); // Get the location of the uniform variable "model" in the shader program.
		unsigned int viewLoc = glGetUniformLocation(myShader.ID, "view"); // Get the location of the uniform variable "view" in the shader program.

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Set the value of the uniform variable "model" in the shader program.
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view)); // Set the value of the uniform variable "view" in the shader program.

		myShader.setMat4("projection", projection); // Set the value of the uniform variable "projection" in the shader program.

		glBindVertexArray(VAO); // Bind the VAO.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw the rectangle using the VAO and EBO.
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


// USER INPUT
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // If the escape key is pressed
		glfwSetWindowShouldClose(window, true); // Set the expression to true so the window will close.
	}
}

// CALLBACK FUNCTIONS
void void_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{		// Whenever the window is resized, this callback function executes. It adjusts the viewport so that the OpenGL renders to the new window size.
		glViewport(0, 0, width, height);
}