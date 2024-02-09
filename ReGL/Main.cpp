#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


void void_framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// Vertex shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) " // The location of the vertex attribute is 0.
"in vec3 aPos;\n" // The position is 0, and attribute is a vec3.
"out vec4 vertexColor;\n" // Output a color to the fragment shader.
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // gl_Position is a special variable that holds the output position of the vertex shader.
"   vertexColor = vec4(0.5, 0.5, 0.0, 1.0);\n" // Set the output variable to a vec4 color. (RGBA)
"}\0";

// Fragment shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"in vec4 vertexColor;\n" // The input variable from the vertex shader (same name and same type).
"out vec4 FragColor;\n" // The output variable from this shader is a vec4 that is used as the fragment's color.
"void main()\n"
"{\n"
"   FragColor = vertexColor;\n" // Set the output variable to a vec4 color. (RGBA)
"}\0";

//while writing glsl, (for example fragment shader) should i write in first or out first? 
//in the fragment shader, you should write out first. WHY, explain
//The out keyword is used to declare an output variable in a shader. The output variable from the vertex shader is the input variable of the fragment shader.
//The in keyword is used to declare an input variable in a shader. The input variable of the fragment shader is the output variable of the vertex shader.
//Should i always write out first in any glsl shader?
//No, you should write in first in the vertex shader. The input variable of the vertex shader is the output variable of the vertex shader.
//



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
	// ------------------------VERTEX SHADER------------------------

	// Create a vertex shader object
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create a shader object

	// Attach the shader source code to the shader object and compile the shader.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Attach the shader source code to the shader object.
	glCompileShader(vertexShader); // Compile the shader.

	// Check if the shader compilation was successful
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // Get the compilation status of the shader.
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // Get the error message.
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl; // Print the error message.
	}


	// ------------------------FRAGMENT SHADER------------------------
	// Create a fragment shader object
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Create a shader object

	// Attach the shader source code to the shader object and compile the shader.
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // Attach the shader source code to the shader object.
	glCompileShader(fragmentShader); // Compile the shader.

	// Check if the shader compilation was successful
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // Get the compilation status of the shader.
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); // Get the error message.
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl; // Print the error message.
	}


	// Link shaders
	// Create a shader program object. A shader program object is the final linked version of multiple shaders combined.
	unsigned int shaderProgram = glCreateProgram(); // Create a shader program object.

	// Attach the vertex and fragment shaders to the shader program object.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); // Link the shaders.

	// Check if the shader linking was successful
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); // Get the linking status of the shader program.
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog); // Get the error message.
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl; // Print the error message.
	}


	//int nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	// It is 16 on my computer.


	// Use the shader program object
	glUseProgram(shaderProgram);

	// Delete the shader objects once we've linked them into a shader program; we no longer need them.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// -------------------RECTANGLE-------------------
	// vertices
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // Top left
		0.5f, 0.5f, 0.0f, // Top right
		0.5f, -0.5f, 0.0f, // Bottom right
		-0.5f, 0.5f, 0.0f // Bottom left
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
	// The first parameter specifies which vertex attribute we want to configure. Remember, we specified the location of the position vertex attribute in the vertex shader with layout (location = 0).
	// The second parameter specifies the size of the vertex attribute. The vertex attribute is a vec3 so it is composed of 3 values. (x, y, z)
	// The third parameter specifies the type of the data which is GL_FLOAT (a vec* in GLSL consists of floating point values).
	// The fourth parameter specifies if we want the data to be normalized. If we're inputting integer data types (unsigned byte, short, etc.), we need to normalize the data to 0 (or -1 for signed data) and 1. We're using GL_FLOAT so we don't want to normalize the data.
	// The fifth parameter specifies the stride between consecutive vertex attributes. Since the next set of position data is located exactly 3 times the size of a float away, we specify 3 * sizeof(float) as the stride.
	// The last parameter specifies the offset of where the position data begins in the buffer. Since the position data is at the start of the data array, we specify 0.
	glEnableVertexAttribArray(0); // Enable the vertex attribute at location 0.


	// Unbind the VBO and VAO. This is good practice so we don't accidentally modify the VBO and VAO while we're not using them.
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO.
	glBindVertexArray(0); // Unbind the VAO.


	// Wireframe & Fill modes
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment to draw in wireframe mode.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Draw in fill mode.



	// RENDER LOOP
	while (!glfwWindowShouldClose(window)) { // Check if the window should close, if not, render the next frame.

		processInput(window); // Check if the user has pressed the escape key, if so, close the window.

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the color to clear the screen with.
		glClear(GL_COLOR_BUFFER_BIT); // Clear the screen with the set color.
	

		// Draw the rectangle
		glUseProgram(shaderProgram); // Use the shader program object.
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
	glDeleteProgram(shaderProgram);

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