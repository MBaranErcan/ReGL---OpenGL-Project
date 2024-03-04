#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
	// The program ID
	unsigned int ID;

	// The constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath) // Shader constructor that reads and builds the shader.
	{
		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		// Ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// First, open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream; // Stream objects to read the file
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// Close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// 2. Compile shaders
		unsigned int vertex, fragment;

		// Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL); // Attach the vertex shader source code to the vertex shader object
		glCompileShader(vertex); // Compile the shader
		checkCompileErrors(vertex, "VERTEX");

		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL); // Attach the fragment shader source code to the fragment shader object
		glCompileShader(fragment); // Compile the shader
		checkCompileErrors(fragment, "FRAGMENT");

		// Shader Program
		ID = glCreateProgram(); // Create a shader program
		glAttachShader(ID, vertex); // Attach the vertex shader to the shader program
		glAttachShader(ID, fragment); // Attach the fragment shader to the shader program
		glLinkProgram(ID); // Link the shader program
		checkCompileErrors(ID, "PROGRAM");

		// Delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	// /activate the shader
	void use() // This function is used to set the current shader program to be the one that is used.
	{
		glUseProgram(ID);
	}


	// Utility uniform functions{
	void setBool(const std::string& name, bool value) const // This function is used to set a boolean uniform in the shader.
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setInt(const std::string &name, int value) const // This function is used to set an integer uniform in the shader.
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const // This function is used to set a float uniform in the shader.
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	// This function is used to set a 4x4 matrix uniform in the shader.
	void setMat4(const std::string& name, const glm::mat4& mat) const 
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}


// Explanations:
// 1. The constructor reads and builds the shader with this line of code: Shader(const char* vertexPath, const char* fragmentPath);
// 2. The use() function is used to set the current shader program to be the one that is used.
// 3. The setBool(), setInt(), and setFloat() functions are used to set a boolean, integer, and float uniform in the shader, respectively. 
//	  We do this because we cannot directly modify the uniforms in the shader. We have to use these functions to do so./

// we need set functions to set the values of the uniforms in the shader.
// These uniforms are used to pass data from the CPU to the GPU.


private: 
	// Utility function for checking shader compilation/linking errors.
	void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // Check if the shader was compiled successfully
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog); // If the shader was not compiled successfully, we retrieve the error message and print it to the console.
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success); // Check if the shader program was linked successfully
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog); // If the shader program was not linked successfully, we retrieve the error message and print it to the console.
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}

};