#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
// GLEW
#include "dependente\glew\glew.h"
// GLFW
#include "dependente\glfw\glfw3.h"
// GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"


#include "shader.hpp"



GLFWwindow* window;
const int width = 1024, height = 1024;


// ulterior o sa fie ceva mai generalizat, inclusiv sfere/cilindri, dar acuma...
// clasa "rectangle"
class Rectangle
{
	// nume ID unic
	std::string name;
	// pozitia coltului de stanga sus (?)
	glm::vec3 position;		// de forma glm::vec3(0.5f, 0.5f, 0);
	// marime width & height (so, X and Y)
	glm::vec3 size;			// de forma glm::vec3(0.5f, 0.5f, 0);
	// culoare RGB si vizibilitate alpha
	glm::vec4 color;		// de forma glm::vec4(0.5f, 0.5f, 0.5f, 1.0);

public:
	
	Rectangle(std::string auxname, glm::vec3 auxposition, glm::vec3 auxsize, glm::vec4 auxcolor)
	{
		name = auxname;
		position = auxposition;
		size = auxsize;
		color = auxcolor;
	}

	std::string getName()
	{
		return name;
	}

	glm::vec3 getPosition()
	{
		return position;
	}

	glm::vec3 getSize()
	{
		return size;
	}

	glm::vec4 getColor()
	{
		return color;
	}
};




/*
//Handling cursor position
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << "The mouse cursor is: " << xpos << " " << ypos << std::endl;
}

//Ex 3: Add callback for mouse button
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		std::cout << "MIDDLE" << std::endl;

		//pozitie = (pozitie + 1)%10;
	}
}

//Ex 4: Complete callback for adjusting the viewport when resizing the window
void window_callback(GLFWwindow* window, int new_width, int new_height)
{	
	//what should we do here?
	glViewport(0, 0, new_width, new_height);
}
*/
/*
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W)
	{
		scala = scala + glm::vec3(1.0f, 0.5f, 0);
	}
	if (key == GLFW_KEY_S)
	{
		scala = scala - glm::vec3(1.0f, 0.5f, 0);
	}
}
*/





int main(void)
{
// setup la obiectele care ne intereseaza
	// aici tinem toate obiectele (momentan numai Rectangle) pe care le randam
	std::vector<Rectangle> obiecte;

	obiecte.push_back(Rectangle("patrat1", glm::vec3(0.3f, 0.05f, 0), glm::vec3(4.0f, 2.0f, 0), glm::vec4(0.5f, 0.5f, 0.5f, 0.5)));
	obiecte.push_back(Rectangle("patrat2", glm::vec3(0.2f, 0.1f, 0), glm::vec3(2.0f, 3.0f, 0), glm::vec4(0.5f, 0.5f, 0.5f, 0.5)));

	
	
// setup la opengl	
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Lab 4", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	// specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");




// introdus obiectele noastre in opengl
	// for each rectangle we need 3 coords * 4 corners = 12 floats per rectangle
	// we have obiecte.size() rectangles.
	std::vector<GLfloat> vertices(12 * obiecte.size());

	// for indices, it's 6 * obiecte.size()
	std::vector<GLuint> indices(6 * obiecte.size());

/*
	vertices = {
		0.05f,  0.05f, 0.0f,  // top right
		0.05f, -0.05f, 0.0f,  // bottom right
		-0.05f, -0.05f, 0.0f,  // bottom left
		-0.05f,  0.05f, 0.0f   // top left 
	};

	indices = {  // note that we start from 0!
		0, 3, 1,  // first Triangle
		1, 3, 2,   // second Triangle
	};
*/

	// "compile" all vertices and indices of defined objects
	for (int i = 0; i < obiecte.size(); i++)
	{
		// retrieve the x and y coords of the top left corner
		float tl_x = obiecte.at(i).getPosition().x;
		float tl_y = obiecte.at(i).getPosition().y;

		// compute the other 3 corners using the size and
		// insert them into the vertices array in the following order: TR, BR, BL, TL
		float width = obiecte.at(i).getSize().x;
		float height = obiecte.at(i).getSize().y;

		// top right
		vertices.at(12 * i + 0) = tl_x + width;
		vertices.at(12 * i + 1) = tl_y;
		vertices.at(12 * i + 2) = 0;
		// bottom right
		vertices.at(12 * i + 3) = tl_x + width;
		vertices.at(12 * i + 4) = tl_y + height;
		vertices.at(12 * i + 5) = 0;
		// bottom left
		vertices.at(12 * i + 6) = tl_x;
		vertices.at(12 * i + 7) = tl_y + height;
		vertices.at(12 * i + 8) = 0;
		// top left
		vertices.at(12 * i + 9) = tl_x;
		vertices.at(12 * i + 10) = tl_y;
		vertices.at(12 * i + 11) = 0;

		// for the indices array, insert the triangles with an offset of 4 per rectangle:
		indices.at(6 * i + 0) = 4 * i;
		indices.at(6 * i + 1) = 4 * i + 3;
		indices.at(6 * i + 2) = 4 * i + 1;
		indices.at(6 * i + 3) = 4 * i + 1;
		indices.at(6 * i + 4) = 4 * i + 3;
		indices.at(6 * i + 5) = 4 * i + 2;
	}


// setup la obiectele noastre in opengl
	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information for a complete rendered object. 
	GLuint vbo, vao, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	// bind VAO
	glBindVertexArray(vao);

	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	// bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	// set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	// create matrices for transforms
	glm::mat4 trans(1.0f);

	/*
	//maybe we can play with different positions
	glm::vec3 positions[) = {
		glm::vec3(0.0f,  0.0f,  0),
		glm::vec3(0.2f,  0.5f, 0),
		glm::vec3(-0.15f, -0.22f, 0),
		glm::vec3(-0.38f, -0.2f, 0),
		glm::vec3(0.24f, -0.4f, 0),
		glm::vec3(-0.17f,  0.3f, 0),
		glm::vec3(0.23f, -0.2f, 0),
		glm::vec3(0.15f,  0.2f, 0),
		glm::vec3(0.15f,  0.7f, 0),
		glm::vec3(-0.13f,  0.1f, 0)
	};

	// Ex3 - Set a callback for handling mouse cursor position
	// Decomment for a callback example
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Ex4 - Set callback for window resizing
	glfwSetFramebufferSizeCallback(window, window_callback);
	//glfwSetKeyCallback(window, key_callback);
	*/


// rendering loop
	// Check if the window was closed
	while (!glfwWindowShouldClose(window))
	{
		// Swap buffers
		glfwSwapBuffers(window);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			// rotire counter-clockwise
			trans = glm::rotate(trans, 0.1f, glm::vec3(0.0, 0.0, 1.0));
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			// rotire clockwise
			trans = glm::rotate(trans, -0.1f, glm::vec3(0.0, 0.0, 1.0));
		}


		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);



		//bind VAO
		glBindVertexArray(vao);

		unsigned int transformLoc = glGetUniformLocation(programID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		unsigned int transformLoc2 = glGetUniformLocation(programID, "color");
		glm::vec4 color = glm::vec4(0.5f, 0, 0.5f, 1.0);
		glUniform4fv(transformLoc2, 1, glm::value_ptr(color));

		glDrawElements(GL_TRIANGLES, obiecte.size()*6, GL_UNSIGNED_INT, 0);
	
	}

	// Cleanup
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}


