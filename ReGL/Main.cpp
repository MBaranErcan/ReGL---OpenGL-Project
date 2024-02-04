#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


void void_framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // Top left
		0.5f, 0.5f, 0.0f, // Top right
		0.5f, -0.5f, 0.0f, // Bottom right
	};

	// Instantiate the GLFW window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We want to use the core-profile means we'll get access to a smaller subset of OpenGL features.


	// Create a Window object.
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello, Window!", NULL, NULL); // width, height, title, monitor, share
	if (window == NULL) {	
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Make the context of the specified window current on the calling thread.

	// Load GLAD so it configures OpenGL. GLAD manages function pointers for OpenGL so we want to initialize GLAD before we call any OpenGL function.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // gladLoadGLLoader returns 0 if it fails
		std::cout << "Failed to initialize GLAD" << std::endl; 
		return -1;
	}

	// Set the viewport size
	glViewport(0, 0, 800, 600);

	// Set the callback function for the window resize event
	glfwSetFramebufferSizeCallback(window, void_framebuffer_size_callback);

	// Render loop
	while (!glfwWindowShouldClose(window)) { // Check if the window should close, if not, render the next frame.

		processInput(window); // Check if the user has pressed the escape key, if so, close the window.

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the color to clear the screen with.
		glClear(GL_COLOR_BUFFER_BIT); // Clear the screen with the set color.
	
		glfwPollEvents(); // Check if any events are triggered (like keyboard input or mouse movement events).
		glfwSwapBuffers(window); // Swap the front and back buffers so the user can see the output.
	
	}



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
{
		glViewport(0, 0, width, height);
}