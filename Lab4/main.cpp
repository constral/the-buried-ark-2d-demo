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
const int width = 1600, height = 900;

<<<<<<< HEAD
// plm github
class Rectangle
{
=======

class Rectangle{
>>>>>>> 3213aba15e841dc447746a24aa9c5162ea9c8e6f
	// nume ID unic
	std::string name;
	// pozitia coltului de stanga jos
	glm::vec2 position;		// de forma glm::vec2(0.5f, 0.5f);
	// marime width & height (so, X and Y)
	glm::vec2 size;			// de forma glm::vec2(0.5f, 0.5f);
	// culoare RGB si vizibilitate alpha
	glm::vec4 color;		// de forma glm::vec4(0.5f, 0.5f, 0.5f, 1.0);

	GLfloat vertices[12];
	GLuint indices[6];

	GLuint vbo, vao, ibo;


//constructorul
public:
	
	Rectangle(std::string auxname, glm::vec2 auxposition, glm::vec2 auxsize, glm::vec4 auxcolor)
	{

	// arguments:
		name = auxname;
		position = auxposition;
		size = auxsize;
		color = auxcolor;



	// "compile" all vertices and indices of defined objects:
		// retrieve the x and y coords of the top left corner
		float tl_x = position.x;
		float tl_y = position.y;

		// compute the other 3 corners using the size and
		// insert them into the vertices array in the following order: TR, BR, BL, TL
		float width = size.x;
		float height = size.y;



		// BR
		vertices[0] = tl_x + width;  // x
		vertices[1] = tl_y;          // y
		vertices[2] = 0.0f;          // z

		// TR
		vertices[3] = tl_x + width;  // x
		vertices[4] = tl_y + height; // y
		vertices[5] = 0.0f;          // z

		// TL
		vertices[6] = tl_x;         // x
		vertices[7] = tl_y + height;// y
		vertices[8] = 0.0f;         // z

		// BL
		vertices[9] = tl_x;         // x
		vertices[10] = tl_y;         // y
		vertices[11] = 0.0f;         // z

		// Define indices for two triangles that form the rectangle
		indices[0] = 0; // BR // 0
		indices[1] = 3; // BL // 3
		indices[2] = 2; // TL // 2

		indices[3] = 1; // TR
		indices[4] = 0; // BR
		indices[5] = 2; // TL

	}

//getters
	std::string getName()
	{
		return name;
	}

	glm::vec2 getPosition()
	{
		return position;
	}

	glm::vec2 getSize()
	{
		return size;
	}

	glm::vec4 getColor()
	{
		return color;
	}

	GLfloat* getVertices()
	{
		return vertices;
	}

	GLuint* getIndices()
	{
		return indices;
	}

	GLuint* getVBO()
	{
		return &vbo;
	}
	GLuint* getVAO()
	{
		return &vao;
	}
	GLuint* getIBO()
	{
		return &ibo;
	}

};



// aici tinem toate obiectele (momentan numai Rectangle) pe care le randam
std::vector<Rectangle> obiecte;

// ca sa poti sa cauti in lista de obiecte dupa numele-id
Rectangle* getRectangleByName(std::string name)
{
	for (int i = 0; i < obiecte.size(); i++)
	{
		if (obiecte.at(i).getName() == name)
		{
			return &(obiecte.at(i));
		}
	}
}

float camera_pos_x = 0;
float camera_pos_y = 0;
bool jumping = 0;

//jump stuff
float deltaJumpTime = 0.0f;
float firstJumpFrame = 0.0f;
float jumpDuration = 1.0f; // seconds
float jumpHeight = 0.2f;





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

	// position - coltul stanga jos, deci: size - width creste catre dreapta, height creste in sus
	obiecte.push_back(Rectangle("patrat1", glm::vec2(0.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0)));
	obiecte.push_back(Rectangle("patrat2", glm::vec2(0.7f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5)));
	obiecte.push_back(Rectangle("patrat3", glm::vec2(2.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5)));
	obiecte.push_back(Rectangle("indianajones", glm::vec2(0.0f, -0.5f), glm::vec2(0.1f, 0.5f), glm::vec4(0.7f, 0.5f, 0.5f, 0.5)));



	
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



	// set up vbo, vao, ibo and such for every object
	for (int g = 0; g < obiecte.size(); g++)
	{
		// setup vbo, vao, ibo
		glGenVertexArrays(1, obiecte.at(g).getVAO());
		glGenBuffers(1, obiecte.at(g).getVBO());
		glGenBuffers(1, obiecte.at(g).getIBO());

		// bind VAO
		glBindVertexArray(*(obiecte.at(g).getVAO()));

		// bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, *(obiecte.at(g).getVBO()));
		glBufferData(GL_ARRAY_BUFFER, sizeof(obiecte.at(g).getVertices()[0]) * 12, obiecte.at(g).getVertices(), GL_STATIC_DRAW);

		// bind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(obiecte.at(g).getIBO()));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(obiecte.at(g).getIndices()[0]) * 6, obiecte.at(g).getIndices(), GL_STATIC_DRAW);


		// Position attribute
		glVertexAttribPointer(
			0,                  // attribute 0, must match the layout in the shader.
			3,                  // size of each attribute
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			3 * sizeof(float),  // stride - size 3 for position + 2 coords for the texture
			(void*)0            // has an offset of 0 in the vertex memory (this is the first thing in there)
		);
		glEnableVertexAttribArray(0);

	}




	/*
	// Ex3 - Set a callback for handling mouse cursor position
	// Decomment for a callback example
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Ex4 - Set callback for window resizing
	glfwSetFramebufferSizeCallback(window, window_callback);
	//glfwSetKeyCallback(window, key_callback);
	*/



	// transformarea terrainului
	glm::mat4 trans(1.0f);
	// playerul care nu isi ia translatie
	glm::mat4 locplayer(1.0f);

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

// controale

		// WASD, de fapt A-D si SPACE
		//// ne-ar trebui ceva fizica la treaba asta, si sa se miste actual pozitia caracterului
		//// (de care tii cont ca sa determini unde e si, spre ex, sa il randezi cazand)
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera_pos_x += 0.001f;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera_pos_x += -0.001f;
		}
		


		// space to jump
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && jumping == 0)
		{
			jumping = 1;
			firstJumpFrame = glfwGetTime();
		}

		// if jumping => animation
		if (jumping == 1) {

			float currentJumpFrame = glfwGetTime();
			deltaJumpTime = currentJumpFrame - firstJumpFrame;
			float normalizedTime = (deltaJumpTime / jumpDuration*2) * 3.1456f;

			const float radius = jumpHeight;
			float cosinus = cos(normalizedTime);		// values (-1, 1)
			camera_pos_y = (-1.0f + cosinus) * radius;	// values (0, jumpHeight)

			// stop jumping animation when duration expires
			if (deltaJumpTime > jumpDuration)
				jumping = 0;

		}

		// tine cont si de translatii laterale si verticale
		trans = glm::translate(glm::mat4(1.0f), glm::vec3(camera_pos_x, camera_pos_y, 0.0));



		// la treaba cu dashul peste prapastie -- acelasi lucru ca la jump, dar se misca pe orizontala + ceva trigonometrie pt. inaltime?



		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);



		// randat obiectele
		for (int j = 0; j < obiecte.size(); j++)
		{
			// bind VAO
			glBindVertexArray(*(obiecte.at(j).getVAO()));


			// shaderu de pozitie
			unsigned int transformLoc = glGetUniformLocation(programID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

			if (obiecte.at(j).getName() == "indianajones")
			{
				// aplicam peste transformarea terenului o transformare care nu se misca
				unsigned int transformLoc2 = glGetUniformLocation(programID, "transform");
				glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(locplayer));
			}
				

			// shaderu de culoare
			unsigned int transformColor = glGetUniformLocation(programID, "color");
			glUniform4fv(transformColor, 1, glm::value_ptr(obiecte.at(j).getColor()));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	
	}

	// Cleanup
	// // asta treb si el sa fie un loop
	//glDeleteBuffers(1, &vbo);
	//glDeleteBuffers(1, &ibo);
	//glDeleteVertexArrays(1, &vao);
	//glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}


