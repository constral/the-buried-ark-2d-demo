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



struct Triangle {

	glm::vec2 v1;
	glm::vec2 v2;
	glm::vec2 v3;
	glm::vec4 color;

	GLfloat vertices[9];
	GLuint indices[3];

	GLuint vbo, vao, ibo;

	Triangle(const glm::vec2& vertex1, const glm::vec2& vertex2, const glm::vec2& vertex3, const glm::vec4& col)
		: v1(vertex1), v2(vertex2), v3(vertex3), color(col) {

		vertices[0] = v1.x;
		vertices[1] = v1.y;
		vertices[2] = 0.0f;

		vertices[3] = v2.x;
		vertices[4] = v2.y;
		vertices[5] = 0.0f;

		vertices[6] = v3.x;
		vertices[7] = v3.y;
		vertices[8] = 0.0f;

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
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

class Rectangle
{
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

class RectangleWithTriangles : public Rectangle {

	std::vector<Triangle> triunghiuri;

public:
	RectangleWithTriangles(
		std::string auxname,
		glm::vec2 auxposition,
		glm::vec2 auxsize,
		glm::vec4 auxcolor,
		const std::vector<Triangle>& externalTriangles
	) : Rectangle(auxname, auxposition, auxsize, auxcolor), triunghiuri(externalTriangles) {}



	std::vector<Triangle>& getTriunghiuri()
	{
		return triunghiuri;
	}



	// Display triangle details for debug
	void displayTriangles() const {
		for (size_t i = 0; i < triunghiuri.size(); ++i) {
			const auto& tri = triunghiuri[i];
			std::cout << "Triangle " << i + 1 << ":\n";
			std::cout << "  v1: (" << tri.v1.x << ", " << tri.v1.y << ")\n";
			std::cout << "  v2: (" << tri.v2.x << ", " << tri.v2.y << ")\n";
			std::cout << "  v3: (" << tri.v3.x << ", " << tri.v3.y << ")\n";
			std::cout << "  color: (" << tri.color.r << ", " << tri.color.g << ", " << tri.color.b << ", " << tri.color.a << ")\n";
		}
	}
};



// aici tinem toate obiectele (momentan numai Rectangle) pe care le randam
std::vector<RectangleWithTriangles> obiecte;

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
bool whipping = 0;
bool swinging = 0;	// cand se da la barna


//jump stuff
float deltaJumpTime = 0.0f;
float firstJumpFrame = 0.0f;
float jumpDuration = 1.0f; // seconds
float jumpHeight = 0.2f;

float razaBiciului = 0.5f;
float swingDuration = 1.0f; // seconds



const float gravity = 0.0005f;




//verifica daca biciul loveste bara
bool isWhipHittingBar()
{
	Rectangle* barna_ptr = getRectangleByName("barna");
	Rectangle* indiana_ptr = getRectangleByName("indianajones");
	

	// tinem cont numai de pozitia pe x, ca si asa a fost o mojicie aia cu sistemu de referinta la indiana jones
	float distance = (camera_pos_x + barna_ptr->getPosition().x);
	std::cout << distance << std::endl;
    return distance < razaBiciului; //distanta e mai mica decat raza de actiune a biciului
}


void updatePlayerToolState(bool whipActive, bool isHittingBar) {

    if (swinging == 0 && whipping == 0 && whipActive) {		//daca e in idle si porneste biciu trece in starea de bici
		whipping = 1;
    }
    if (whipping == 1 && isHittingBar) {
		swinging = 1;										//daca da cu biciul si loveste bara trece in starea de prins de bara
    }
    if (swinging == 1 && !whipActive) {
		whipping = 0;										//daca e prins de bara si nu mai da cu biciul trece in idle
    }
}


bool checkCollision() {

	glm::vec2 playerPos = glm::vec2(camera_pos_x, camera_pos_y);
	glm::vec2 playerSize = getRectangleByName("indianajones")->getSize();



	// Check if the player is touching any terrain
	if (
		-1.0f * (playerPos.x - playerSize.x) > getRectangleByName("patrat1")->getPosition().x &&
		-1.0f * (playerPos.x) < getRectangleByName("patrat1")->getPosition().x + getRectangleByName("patrat1")->getSize().x ||
		-1.0f * (playerPos.x - playerSize.x) > getRectangleByName("patrat2")->getPosition().x &&
		-1.0f * (playerPos.x) < getRectangleByName("patrat2")->getPosition().x + getRectangleByName("patrat2")->getSize().x ||
		-1.0f * (playerPos.x - playerSize.x) > getRectangleByName("patrat3")->getPosition().x &&
		-1.0f * (playerPos.x) < getRectangleByName("patrat3")->getPosition().x + getRectangleByName("patrat3")->getSize().x
	)
	{
		return true;
	}
	return false;
}





int main(void)
{
// setup la obiectele care ne intereseaza

	std::vector<Triangle> vector_gol = { };

	// position - coltul stanga jos, deci: size - width creste catre dreapta, height creste in sus
	obiecte.push_back(RectangleWithTriangles("patrat1", glm::vec2(0.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), vector_gol));
	obiecte.push_back(RectangleWithTriangles("patrat2", glm::vec2(0.7f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), vector_gol));
	obiecte.push_back(RectangleWithTriangles("patrat3", glm::vec2(2.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), vector_gol));
	obiecte.push_back(RectangleWithTriangles("barna", glm::vec2(1.5f, 0.1f), glm::vec2(0.2f, 0.05f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), vector_gol));

	std::vector<Triangle> triunghiuri_normale = {
		Triangle(glm::vec2(0.082506376, 0.070870861), glm::vec2(0.10207519, 0.072518547), glm::vec2(0.091139676, 0.099430756), glm::vec4(0.87, 0.67, 0.53, 1.0)),
		Triangle(glm::vec2(0.065581991, 0.092026342), glm::vec2(0.11899957, 0.09561202), glm::vec2(0.094013934, 0.13433731), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.1169134, 0.094670779), glm::vec2(0.13486618, 0.11688404), glm::vec2(0.11000849, 0.10895073), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.066941985, 0.091497455), glm::vec2(0.075101957, 0.10612496), glm::vec2(0.051302041, 0.11476848), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.052359815, 0.11318182), glm::vec2(0.07457307, 0.13116398), glm::vec2(0.047599831, 0.12666844), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.13074087, 0.11371071), glm::vec2(0.13962616, 0.12178826), glm::vec2(0.11741291, 0.13909729), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.094081445, 0.10842183), glm::vec2(0.075630845, 0.13974457), glm::vec2(0.11053739, 0.14121282), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.072457521, 0.12957732), glm::vec2(0.07827528, 0.13645285), glm::vec2(0.081977488, 0.12904843), glm::vec4(0.87, 0.67, 0.53, 1.0)),
		Triangle(glm::vec2(0.11899958, 0.13803952), glm::vec2(0.11106628, 0.1385684), glm::vec2(0.11582626, 0.14597282), glm::vec4(0.87, 0.67, 0.53, 1.0)),
		Triangle(glm::vec2(0.075630844, 0.13909729), glm::vec2(0.095728551, 0.14015506), glm::vec2(0.0814486, 0.17770604), glm::vec4(0.16, 0.13, 0.04, 1.0)),
		Triangle(glm::vec2(0.092555224, 0.14015506), glm::vec2(0.11106627, 0.14121283), glm::vec2(0.10683517, 0.17559049), glm::vec4(0.16, 0.13, 0.04, 1.0)),
		Triangle(glm::vec2(0.099430759, 0.17347494), glm::vec2(0.11318182, 0.17400383), glm::vec2(0.10842184, 0.20203484), glm::vec4(0.16, 0.13, 0.04, 1.0)),
		Triangle(glm::vec2(0.07668862, 0.17506161), glm::vec2(0.08726636, 0.17611938), glm::vec2(0.081448601, 0.19886152), glm::vec4(0.16, 0.13, 0.04, 1.0)),
		Triangle(glm::vec2(0.081448601, 0.19727486), glm::vec2(0.072986408, 0.20838149), glm::vec2(0.088853021, 0.20520817), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.10842184, 0.19991929), glm::vec2(0.10101742, 0.2078526), glm::vec2(0.11688403, 0.21049703), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.077746393, 0.071928635), glm::vec2(0.093084117, 0.072457522), glm::vec2(0.088324133, 0.06558199), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.086737472, 0.072457522), glm::vec2(0.1100085, 0.074044183), glm::vec2(0.096786325, 0.06558199), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.083035263, 0.070341974), glm::vec2(0.10471963, 0.07087086), glm::vec2(0.093613003, 0.057648684), glm::vec4(0.47, 0.27, 0.13, 1.0))
	};

	obiecte.push_back(RectangleWithTriangles("indianajones", glm::vec2(0.0f, -0.6f), glm::vec2(0.1f, 0.5f), glm::vec4(0.7f, 0.5f, 0.5f, 0.5f), triunghiuri_normale));

	///// !!!!!1 PROVIZORIU - TREB SCHIMBAT VECTOR GOL CU ACTUAL TRIUNGHIURI, TREB SCOASE DIN SCRIPTU PYTHON
	obiecte.push_back(RectangleWithTriangles("indianajonesbici", glm::vec2(0.0f, -0.6f), glm::vec2(0.1f, 0.5f), glm::vec4(0.5f, 0.7f, 0.5f, 0.0f), vector_gol));
	obiecte.push_back(RectangleWithTriangles("indianajonesbarna", glm::vec2(0.0f, -0.6f), glm::vec2(0.1f, 0.5f), glm::vec4(0.5f, 0.5f, 0.7f, 0.0f), vector_gol));



	
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



		// RENDERING Polygons
		for (int h = 0; h < obiecte.at(g).getTriunghiuri().size(); h++)
		{
			// setup vbo, vao, ibo
			glGenVertexArrays(1, obiecte.at(g).getTriunghiuri().at(h).getVAO());
			glGenBuffers(1, obiecte.at(g).getTriunghiuri().at(h).getVBO());
			glGenBuffers(1, obiecte.at(g).getTriunghiuri().at(h).getIBO());

			// bind VAO
			glBindVertexArray(*(obiecte.at(g).getTriunghiuri().at(h).getVAO()));

			// bind VBO
			glBindBuffer(GL_ARRAY_BUFFER, *(obiecte.at(g).getTriunghiuri().at(h).getVBO()));
			glBufferData(GL_ARRAY_BUFFER, sizeof(obiecte.at(g).getTriunghiuri().at(h).getVertices()[0]) * 9, obiecte.at(g).getTriunghiuri().at(h).getVertices(), GL_STATIC_DRAW);

			// bind IBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(obiecte.at(g).getTriunghiuri().at(h).getIBO()));
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(obiecte.at(g).getTriunghiuri().at(h).getIndices()[0]) * 3, obiecte.at(g).getTriunghiuri().at(h).getIndices(), GL_STATIC_DRAW);


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

	}









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

		
		// click to whip
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			whipping = 1;
			std::cout << "whip 1" << std::endl;
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			whipping = 0;
			std::cout << "whip 0" << std::endl;
		}

		// if whipping and close enough, start swinging animation
		if (whipping && isWhipHittingBar()) {

			swinging = 1;
			std::cout << "swinging" << std::endl;
		}

		/// PROVIZORIU
		if (jumping == 1 && swinging == 1)
			swinging = 0;
		


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





		// Applying gravity when not jumping and not on ground
		if (!jumping && !checkCollision()) {
			camera_pos_y += gravity;  // Apply gravity (fall down)
		}



		// tine cont si de translatii laterale si verticale
		trans = glm::translate(glm::mat4(1.0f), glm::vec3(camera_pos_x, camera_pos_y, 0.0));



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



			// randare speciala pt modelele lui indiana jones
			if (obiecte.at(j).getName() == "indianajones" || obiecte.at(j).getName() == "indianajonesbici"
				|| obiecte.at(j).getName() == "indianajonesbarna")
			{
				// aplicam peste transformarea terenului o transformare care nu se misca
				unsigned int transformLoc2 = glGetUniformLocation(programID, "transform");
				glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(locplayer));
			}

			// shaderu de culoare
			unsigned int transformColor = glGetUniformLocation(programID, "color");
			glUniform4fv(transformColor, 1, glm::value_ptr(obiecte.at(j).getColor()));
				


			// i.fs.d. stare, nu randam poligoanele care corespund altor stari
			if (swinging == 0 && whipping == 0 && obiecte.at(j).getName() != "indianajonesbici" && obiecte.at(j).getName() != "indianajonesbarna") {
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (whipping == 1 && obiecte.at(j).getName() != "indianajones" && obiecte.at(j).getName() != "indianajonesbarna") {
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (swinging == 1 && obiecte.at(j).getName() != "indianajones" && obiecte.at(j).getName() != "indianajonesbici") {
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}



			// randat TRIUNGHIURILE obiectelor
			for (int k = 0; k < obiecte.at(j).getTriunghiuri().size(); k++)
			{
				// bind VAO
				glBindVertexArray(*(obiecte.at(j).getTriunghiuri().at(k).getVAO()));

				// shaderu de pozitie
				unsigned int transformLoc = glGetUniformLocation(programID, "transform");
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));



				// randare speciala pt modelele lui indiana jones
				if (obiecte.at(j).getName() == "indianajones" || obiecte.at(j).getName() == "indianajonesbici"
					|| obiecte.at(j).getName() == "indianajonesbarna")
				{
					glm::mat4 rotationMatrix = glm::mat4(
						1, 0, 0, 0,
						0, -1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1
					);

					// Apply the rotation to the locplayer matrix
					glm::mat4 transformedLocplayer = rotationMatrix * locplayer;

					// aplicam peste transformarea terenului o transformare care nu se misca
					unsigned int transformLoc2 = glGetUniformLocation(programID, "transform");
					glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(transformedLocplayer));
				}

				// shaderu de culoare
				unsigned int transformColor = glGetUniformLocation(programID, "color");
				glUniform4fv(transformColor, 1, glm::value_ptr(obiecte.at(j).getTriunghiuri().at(k).getColor()));



				// i.fs.d. stare, nu randam poligoanele care corespund altor stari
				if (swinging == 0 && whipping == 0 && obiecte.at(j).getName() != "indianajonesbici" && obiecte.at(j).getName() != "indianajonesbarna") {
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
				if (whipping == 1 && obiecte.at(j).getName() != "indianajones" && obiecte.at(j).getName() != "indianajonesbarna") {
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
				if (swinging == 1 && obiecte.at(j).getName() != "indianajones" && obiecte.at(j).getName() != "indianajonesbici") {
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}


			}

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


