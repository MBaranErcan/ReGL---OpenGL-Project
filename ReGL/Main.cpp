#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
		// positions			// colors
		-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f, // Top left
		 0.5f,	0.5f, 0.0f,	0.0f, 1.0f, 0.0f, // Top right
		 0.5f, -0.5f, 0.0f,	1.0f, 1.0f, 0.0f, // Bottom right
		-0.5f,	0.5f, 0.0f,	0.0f, 0.0f, 1.0f, // Bottom left
	};
	unsigned int indices[] = {	// Instead of drawing the rectangle with 6 vertices, we can draw it with 2 triangles using 4 vertices.
			0, 1, 2, // First Triangle
			0, 1, 3 // Second Triangle
		};


	// -------------------VBO & VAO & EBO-------------------	
	// Vertex Buffer Object (VBO) can store a large number of vertices in the GPU's memory so we can render a large object quickly.
	// Vertex Array Object (VAO) can store the configuration of vertex attributes (like pointers to vertex attributes in the VBO) and which VBO to use.
	// Element Buffer Object (EBO) is a buffer, just like a vertex buffer object, that stores indices that OpenGL uses to decide what vertices to draw.
	unsigned int VAO, VBO, EBO; // Create a VBO, VAO, and EBO.
	glGenVertexArrays(1, &VAO); // Generate 1 vertex array object and store the resulting identifier in VAO.
	glGenBuffers(1, &VBO); // Generate 1 buffer and store the resulting identifier in VBO.
	glGenBuffers(1, &EBO); // Generate 1 buffer and store the resulting identifier in EBO.

	// We want to bind to the VAO first. This is because the subsequent VBO calls we make are then stored inside the VAO.
	glBindVertexArray(VAO); // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

	// Then we bind to the buffer using the GL_ARRAY_BUFFER target. Since we've generated a buffer, we want to bind to it so we can perform operations on it.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// We pass the data to the buffer using the glBufferData function. The GL_ARRAY_BUFFER target is used to specify the type of buffer we want to copy the data into.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Specify the vertex attributes using glVertexAttribPointer.
	// The first parameter specifies which vertex attribute we want to configure. Remember, we specified the location of the position vertex attribute in the vertex shader with layout (location = 0).
	// The second parameter specifies the size of the vertex attribute. The vertex attribute is a vec3 so it is composed of 3 values. (x, y, z)
	// The third parameter specifies the type of the data which is GL_FLOAT (a vec* in GLSL consists of floating point values).
	// The fourth parameter specifies if we want the data to be normalized. If we're inputting integer data types (unsigned byte, short, etc.), we need to normalize the data to 0 (or -1 for signed data) and 1. We're using GL_FLOAT so we don't want to normalize the data.
	// The fifth parameter specifies the stride between consecutive vertex attributes. Since the next set of position data is located exactly 3 times the size of a float away, we specify 3 * sizeof(float) as the stride.
	// The last parameter specifies the offset of where the position data begins in the buffer. Since the position data is at the start of the data array, we specify 0.
	glEnableVertexAttribArray(0); // Enable the vertex attribute at location 0.
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Specify the vertex color attributes using glVertexAttribPointer.
	glEnableVertexAttribArray(1); // Enable the vertex attribute at location 1.

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


	// RENDER LOOP
	while (!glfwWindowShouldClose(window)) { // Check if the window should close, if not, render the next frame.

		processInput(window); // Check if the user has pressed the escape key, if so, close the window.

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the color to clear the screen with.
		glClear(GL_COLOR_BUFFER_BIT); // Clear the screen with the set color.
	

		// Draw the rectangle
		myShader.use(); // Use the shader program.
		glBindVertexArray(VAO); // Bind the VAO.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw the rectangle using the VAO and EBO.


		glBindVertexArray(VAO); // Bind the VAO.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw the rectangle using the VAO and EBO.
		glBindVertexArray(0); // Unbind the VAO.


		glfwPollEvents(); // Check if any events are triggered (like keyboard input or mouse movement events).
		glfwSwapBuffers(window); // Swap the front and back buffers so the user can see the output.
	
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