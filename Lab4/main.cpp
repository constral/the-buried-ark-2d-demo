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
	glm::vec2 polygons_offset;
	glm::vec2 polygons_scale;

public:
	RectangleWithTriangles(
		std::string auxname,
		glm::vec2 auxposition,
		glm::vec2 auxsize,
		glm::vec4 auxcolor,
		const std::vector<Triangle>& externalTriangles,
		glm::vec2 auxpolygons_offset,
		glm::vec2 auxpolygons_scale
	) : Rectangle(auxname, auxposition, auxsize, auxcolor),
		triunghiuri(externalTriangles),
		polygons_offset(auxpolygons_offset),
		polygons_scale(auxpolygons_scale)
	{
	}



	std::vector<Triangle>& getTriunghiuri()
	{
		return triunghiuri;
	}
	glm::vec2 getPolygonsOffset()
	{
		return polygons_offset;
	}
	glm::vec2 getPolygonsScale()
	{
		return polygons_scale;
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
bool dead = 0;
bool hitboxes_visible = 1;


//jump stuff
float deltaJumpTime = 0.0f;
float firstJumpFrame = 0.0f;
float shortjumpDuration = 1.0f;		// seconds
float longjumpDuration = 2.7f;		// seconds
float jumpDuration = shortjumpDuration;
float jumpHeight = 0.2f;

float razaBiciului = 0.5f;
float swingDuration = 1.0f; // seconds



const float gravity = 0.002f;




//verifica daca biciul loveste bara
bool isWhipHittingBar()
{
	Rectangle* barna_ptr = getRectangleByName("barna");
	Rectangle* indiana_ptr = getRectangleByName("indianajones");
	

	// tinem cont numai de pozitia pe x, ca si asa a fost o mojicie aia cu sistemu de referinta la indiana jones
	float distance = (camera_pos_x + barna_ptr->getPosition().x);
	
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
		return true;

	return false;
}




int main(void)
{
// controls
	std::cout << "move left / right - A, D" << std::endl;
	std::cout << "jump - space" << std::endl;
	std::cout << "use whip - left click" << std::endl;
	std::cout << "the whip can be used around a pole in order to help you jump farther." << std::endl;

// setup la obiectele care ne intereseaza

	// poligoanele
	std::vector<Triangle> vector_gol = { };

	std::vector<Triangle> triunghiuri_normale2 = {
		Triangle(glm::vec2(0.075862082, 0.027681666), glm::vec2(0.118390812, 0.031131224), glm::vec2(0.094624756, 0.087474044), glm::vec4(0.87, 0.67, 0.53, 1.0)),
		Triangle(glm::vec2(0.039080468, 0.071972324), glm::vec2(0.155172416, 0.079479224), glm::vec2(0.10087135600000001, 0.160553652), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.150638562, 0.077508658), glm::vec2(0.189655178, 0.124013864), glm::vec2(0.135632182, 0.10740485599999999), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.04203613, 0.07086505800000001), glm::vec2(0.059770126, 0.101488886), glm::vec2(0.00804598, 0.11958477399999999), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.0103448308, 0.116262978), glm::vec2(0.0586207, 0.153910034), glm::vec2(0.0, 0.144498274), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.180689678, 0.117370252), glm::vec2(0.2, 0.134281234), glm::vec2(0.151724142, 0.17051905199999998), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.101018078, 0.10629756600000001), glm::vec2(0.060919552, 0.171874172), glm::vec2(0.13678163599999998, 0.174948072), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.054022996, 0.150588244), glm::vec2(0.06666668, 0.164982712), glm::vec2(0.074712656, 0.14948097000000002), glm::vec4(0.87, 0.67, 0.53, 1.0)),
		Triangle(glm::vec2(0.155172438, 0.168304512), glm::vec2(0.13793106800000002, 0.16941177200000002), glm::vec2(0.14827589, 0.184913512), glm::vec4(0.87, 0.67, 0.53, 1.0)),
		Triangle(glm::vec2(0.06091955, 0.17051905199999998), glm::vec2(0.104597718, 0.172733572), glm::vec2(0.073563228, 0.2513495), glm::vec4(0.16, 0.13, 0.04, 1.0)),
		Triangle(glm::vec2(0.097701154, 0.172733572), glm::vec2(0.137931046, 0.174948092), glm::vec2(0.12873563400000002, 0.24692042), glm::vec4(0.16, 0.13, 0.04, 1.0)),
		Triangle(glm::vec2(0.112643694, 0.24249137999999998), glm::vec2(0.142528752, 0.24359862000000002), glm::vec2(0.13218393, 0.30228375999999996), glm::vec4(0.16, 0.13, 0.04, 1.0)),
		Triangle(glm::vec2(0.063218406, 0.24581318), glm::vec2(0.086206914, 0.24802771999999998), glm::vec2(0.07356323000000001, 0.29564016), glm::vec4(0.16, 0.13, 0.04, 1.0)),
		Triangle(glm::vec2(0.07356323000000001, 0.29231836), glm::vec2(0.055172422, 0.31557098), glm::vec2(0.08965519000000001, 0.30892738000000003), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.13218393, 0.29785468), glm::vec2(0.11609197, 0.3144637), glm::vec2(0.15057473200000002, 0.32), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.065517254, 0.029896198), glm::vec2(0.098850594, 0.031003464), glm::vec2(0.088505762, 0.016609000000000002), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.085057486, 0.031003464), glm::vec2(0.135632204, 0.034325263999999994), glm::vec2(0.106896568, 0.016609000000000002), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.077011508, 0.026574398), glm::vec2(0.12413795, 0.027681664), glm::vec2(0.100000016, 0.0), glm::vec4(0.47, 0.27, 0.13, 1.0))
	};

	std::vector<Triangle> triunghiuri_bici = {
		Triangle(glm::vec2(0.067894612, 0.028739514), glm::vec2(0.10027623199999999, 0.030631457999999997), glm::vec2(0.08393841, 0.08818638799999999), glm::vec4(0.87, 0.67, 0.53, 1.0)),
		Triangle(glm::vec2(0.041314942, 0.07465450800000001), glm::vec2(0.12965043199999998, 0.07789832199999999), glm::vec2(0.090883854, 0.16146274), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.126149974, 0.076084418), glm::vec2(0.157164866, 0.12141475), glm::vec2(0.115662608, 0.10671266), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.043524776, 0.073431088), glm::vec2(0.057907948, 0.10357653600000001), glm::vec2(0.019197324600000003, 0.123697524), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.19779143600000001, 0.10790987199999999), glm::vec2(0.20695750000000002, 0.108032354), glm::vec2(0.198692522, 0.145826126), glm::vec4(0.10, 0.10, 0.10, 1.0)),
		Triangle(glm::vec2(0.02747172, 0.12760289), glm::vec2(0.0072332652, 0.17543868399999998), glm::vec2(0.010940107599999999, 0.11837249), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.157062064, 0.10834094), glm::vec2(0.155912158, 0.133604232), glm::vec2(0.2020993, 0.12691292799999998), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.089358512, 0.106883086), glm::vec2(0.060903570000000004, 0.17433189999999998), glm::vec2(0.118572504, 0.174609276), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.0, 0.173156604), glm::vec2(0.0121531548, 0.18716639399999999), glm::vec2(0.0189855568, 0.17127524600000002), glm::vec4(0.87, 0.67, 0.53, 1.0)),
		Triangle(glm::vec2(0.2088365, 0.13822475399999998), glm::vec2(0.19849088599999998, 0.125500014), glm::vec2(0.21573218, 0.12174931), glm::vec4(0.87, 0.67, 0.53, 1.0)),
		Triangle(glm::vec2(0.060862690000000004, 0.172968814), glm::vec2(0.094079426, 0.17357583), glm::vec2(0.07289706, 0.25380414), glm::vec4(0.16, 0.13, 0.04, 1.0)),
		Triangle(glm::vec2(0.088845224, 0.17383171), glm::vec2(0.119444868, 0.174566664), glm::vec2(0.11463704599999999, 0.24730216), glm::vec4(0.16, 0.13, 0.04, 1.0)),
		Triangle(glm::vec2(0.102290308, 0.24344414), glm::vec2(0.12500524400000002, 0.24344914), glm::vec2(0.11892426399999999, 0.30286218000000004), glm::vec4(0.16, 0.13, 0.04, 1.0)),
		Triangle(glm::vec2(0.06487876, 0.24861916), glm::vec2(0.082392886, 0.24999378), glm::vec2(0.07423314, 0.29835449999999997), glm::vec4(0.16, 0.13, 0.04, 1.0)),
		Triangle(glm::vec2(0.074132928, 0.29501321999999996), glm::vec2(0.060876498, 0.31908448), glm::vec2(0.08684710200000001, 0.31112258000000004), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.118790646, 0.29840714), glm::vec2(0.107078546, 0.31571056), glm::vec2(0.133416562, 0.32), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.060110114000000006, 0.031350836), glm::vec2(0.085442152, 0.031227903999999997), glm::vec2(0.077156618, 0.0171328378), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.074973742, 0.031739642), glm::vec2(0.11345809400000001, 0.033204554000000004), glm::vec2(0.091114486, 0.0164505258), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.06873356800000001, 0.027583116), glm::vec2(0.10453401, 0.026948446), glm::vec2(0.08537926400000001, 0.0), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.08111652, 0.060358624), glm::vec2(0.09881595400000001, 0.053366819999999995), glm::vec2(0.09785089000000001, 0.058786768), glm::vec4(0.40, 0.40, 0.40, 1.0)),
		Triangle(glm::vec2(0.09628915, 0.05130927), glm::vec2(0.10550735400000001, 0.049701892), glm::vec2(0.103123494, 0.058494121999999996), glm::vec4(0.70, 0.70, 0.70, 1.0)),
		Triangle(glm::vec2(0.10245882199999999, 0.045738366), glm::vec2(0.111677024, 0.044130986), glm::vec2(0.109293148, 0.052923228), glm::vec4(0.70, 0.70, 0.70, 1.0)),
		Triangle(glm::vec2(0.10617911599999999, 0.038629906), glm::vec2(0.11539732, 0.037022528), glm::vec2(0.113013458, 0.04581476), glm::vec4(0.70, 0.70, 0.70, 1.0)),
		Triangle(glm::vec2(0.1126362, 0.033796616), glm::vec2(0.12185438999999999, 0.03218925), glm::vec2(0.11947052600000001, 0.04098148), glm::vec4(0.70, 0.70, 0.70, 1.0)),
		Triangle(glm::vec2(0.094664638, 0.05419793), glm::vec2(0.10142171999999999, 0.05292119), glm::vec2(0.09969560799999999, 0.060103864), glm::vec4(1.00, 0.40, 0.00, 1.0))
	};

	std::vector<Triangle> triunghiuri_barna = {
		Triangle(glm::vec2(0.0095071592, 0.090469138), glm::vec2(0.03990156, 0.06559357199999999), glm::vec2(0.061662574, 0.11946256400000001), glm::vec4(0.87, 0.67, 0.53, 1.0)),
		Triangle(glm::vec2(0.01465429, 0.14440859), glm::vec2(0.096353136, 0.075195288), glm::vec2(0.114383208, 0.165586866), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.09204711, 0.076747812), glm::vec2(0.148750114, 0.083653902), glm::vec2(0.102010734, 0.10686599), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.0158709822, 0.141755876), glm::vec2(0.0479515, 0.15140140600000002), glm::vec2(0.0258076, 0.19694515999999998), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.176816164, 0.041231446), glm::vec2(0.185157828, 0.033898376), glm::vec2(0.20166826000000002, 0.067277776), glm::vec4(0.10, 0.10, 0.10, 1.0)),
		Triangle(glm::vec2(0.035743764, 0.19300525600000001), glm::vec2(0.047825884, 0.24316818), glm::vec2(0.014986808800000001, 0.199868602), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.14036837, 0.074504648), glm::vec2(0.155349052, 0.093275914), glm::vec2(0.19274839600000002, 0.051164014), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.078403354, 0.12827845), glm::vec2(0.095512652, 0.198942634), glm::vec2(0.147682116, 0.15245782800000002), glm::vec4(0.42, 0.36, 0.33, 1.0)),
		Triangle(glm::vec2(0.040996736, 0.24638892), glm::vec2(0.058922376, 0.24816326), glm::vec2(0.053931058, 0.23237674000000003), glm::vec4(0.87, 0.67, 0.53, 1.0)),
		Triangle(glm::vec2(0.2020103, 0.054026748), glm::vec2(0.186244372, 0.05195234), glm::vec2(0.196695596, 0.03778536), glm::vec4(0.87, 0.67, 0.53, 1.0)),
		Triangle(glm::vec2(0.094611584, 0.198013158), glm::vec2(0.124944226, 0.17155420200000002), glm::vec2(0.15671284600000002, 0.24535614), glm::vec4(0.16, 0.13, 0.04, 1.0)),
		Triangle(glm::vec2(0.120387362, 0.175971762), glm::vec2(0.148441608, 0.15172158200000002), glm::vec2(0.19022270800000002, 0.20697772), glm::vec4(0.16, 0.13, 0.04, 1.0)),
		Triangle(glm::vec2(0.17664498599999998, 0.21424746), glm::vec2(0.19712768600000002, 0.19586418), glm::vec2(0.2293142, 0.24274279999999998), glm::vec4(0.16, 0.13, 0.04, 1.0)),
		Triangle(glm::vec2(0.146196268, 0.24818512), glm::vec2(0.16285832, 0.23497885999999998), glm::vec2(0.186163266, 0.27573524), glm::vec4(0.16, 0.13, 0.04, 1.0)),
		Triangle(glm::vec2(0.183954496, 0.27345684), glm::vec2(0.18726432199999998, 0.30118604), glm::vec2(0.20563108, 0.27454132), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.22636914, 0.2397049), glm::vec2(0.22678038, 0.26140472), glm::vec2(0.253246, 0.24311426), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.00414438, 0.09861444400000001), glm::vec2(0.026905492, 0.07802236600000001), glm::vec2(0.0104988014, 0.07477551), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.0177917574, 0.0868575), glm::vec2(0.053417561999999995, 0.056740452), glm::vec2(0.022650444, 0.062995338), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.0095303718, 0.088973414), glm::vec2(0.041405214, 0.0595462), glm::vec2(0.0070496698, 0.056020728), glm::vec4(0.47, 0.27, 0.13, 1.0)),
		Triangle(glm::vec2(0.041475026, 0.102095342), glm::vec2(0.052999664, 0.08283088999999999), glm::vec2(0.055565936, 0.08743953), glm::vec4(0.40, 0.40, 0.40, 1.0)),
		Triangle(glm::vec2(0.04941701, 0.083423224), glm::vec2(0.056708918, 0.07482636), glm::vec2(0.060134113999999995, 0.08296491), glm::vec4(0.70, 0.70, 0.70, 1.0)),
		Triangle(glm::vec2(0.051447436000000006, 0.074495066), glm::vec2(0.058739344000000006, 0.0658982), glm::vec2(0.062164541999999996, 0.07403675200000001), glm::vec4(0.70, 0.70, 0.70, 1.0)),
		Triangle(glm::vec2(0.050294704, 0.066463776), glm::vec2(0.057586612, 0.057866914), glm::vec2(0.061011802000000004, 0.06600545599999999), glm::vec4(0.70, 0.70, 0.70, 1.0)),
		Triangle(glm::vec2(0.053051932, 0.057823844), glm::vec2(0.060343832, 0.049227004), glm::vec2(0.06376902400000001, 0.057365547999999995), glm::vec4(0.70, 0.70, 0.70, 1.0)),
		Triangle(glm::vec2(0.049783836, 0.086778122), glm::vec2(0.055066454, 0.08040693), glm::vec2(0.058064186, 0.08687640399999999), glm::vec4(1.00, 0.40, 0.00, 1.0))
	};

	std::vector<Triangle> triunghiuri_coloana = {
		Triangle(glm::vec2(0.200f, 0.950f), glm::vec2(0.300f, 0.950f), glm::vec2(0.250f, 0.900f), glm::vec4(0.61f, 0.48f, 0.34f, 1.0f)),
		Triangle(glm::vec2(0.200f, 0.950f), glm::vec2(0.250f, 0.900f), glm::vec2(0.150f, 0.900f), glm::vec4(0.55f, 0.42f, 0.30f, 1.0f)),
		Triangle(glm::vec2(0.300f, 0.950f), glm::vec2(0.350f, 0.900f), glm::vec2(0.250f, 0.900f), glm::vec4(0.48f, 0.37f, 0.26f, 1.0f)),

		Triangle(glm::vec2(0.230f, 0.900f), glm::vec2(0.270f, 0.900f), glm::vec2(0.250f, 0.700f), glm::vec4(0.83f, 0.65f, 0.46f, 1.0f)),
		Triangle(glm::vec2(0.230f, 0.900f), glm::vec2(0.250f, 0.700f), glm::vec2(0.200f, 0.700f), glm::vec4(0.72f, 0.57f, 0.37f, 1.0f)),
		Triangle(glm::vec2(0.270f, 0.900f), glm::vec2(0.300f, 0.700f), glm::vec2(0.250f, 0.700f), glm::vec4(0.64f, 0.49f, 0.31f, 1.0f)),

		Triangle(glm::vec2(0.220f, 0.700f), glm::vec2(0.280f, 0.700f), glm::vec2(0.250f, 0.680f), glm::vec4(0.45f, 0.33f, 0.21f, 1.0f)),
		Triangle(glm::vec2(0.220f, 0.700f), glm::vec2(0.250f, 0.680f), glm::vec2(0.190f, 0.680f), glm::vec4(0.42f, 0.30f, 0.18f, 1.0f)),
		Triangle(glm::vec2(0.280f, 0.700f), glm::vec2(0.310f, 0.680f), glm::vec2(0.250f, 0.680f), glm::vec4(0.36f, 0.26f, 0.16f, 1.0f)),

		Triangle(glm::vec2(0.230f, 0.680f), glm::vec2(0.270f, 0.680f), glm::vec2(0.250f, 0.660f), glm::vec4(0.45f, 0.33f, 0.21f, 1.0f)),
		Triangle(glm::vec2(0.230f, 0.680f), glm::vec2(0.250f, 0.660f), glm::vec2(0.210f, 0.660f), glm::vec4(0.42f, 0.30f, 0.18f, 1.0f)),
		Triangle(glm::vec2(0.270f, 0.680f), glm::vec2(0.290f, 0.660f), glm::vec2(0.250f, 0.660f), glm::vec4(0.36f, 0.26f, 0.16f, 1.0f)),

		Triangle(glm::vec2(0.200f, 0.660f), glm::vec2(0.300f, 0.660f), glm::vec2(0.250f, 0.630f), glm::vec4(0.61f, 0.48f, 0.34f, 1.0f)),
		Triangle(glm::vec2(0.200f, 0.660f), glm::vec2(0.250f, 0.630f), glm::vec2(0.150f, 0.630f), glm::vec4(0.55f, 0.42f, 0.30f, 1.0f)),
		Triangle(glm::vec2(0.300f, 0.660f), glm::vec2(0.350f, 0.630f), glm::vec2(0.250f, 0.630f), glm::vec4(0.48f, 0.37f, 0.26f, 1.0f))
	};

	std::vector<Triangle> triunghiuri_tzepi = {
		Triangle(glm::vec2(-0.1f, 0.2f), glm::vec2(0.1f, 0.2f), glm::vec2(0.0f, 0.4f), glm::vec4(0.7, 0.7, 0.7, 1.0)),  // Spike 1
		Triangle(glm::vec2(0.3f, 0.2f), glm::vec2(0.1f, 0.2f), glm::vec2(0.2f, 0.4f), glm::vec4(0.7, 0.7, 0.7, 1.0)),  // Spike 1
		Triangle(glm::vec2(0.5f, 0.2f), glm::vec2(0.3f, 0.2f), glm::vec2(0.4f, 0.4f), glm::vec4(0.7, 0.7, 0.7, 1.0)),  // Spike 1
	};

	// position - coltul stanga jos, deci: size - width creste catre dreapta, height creste in sus
	// si multe altele...
	obiecte.push_back(RectangleWithTriangles("patrat1", glm::vec2(0.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), vector_gol, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
	obiecte.push_back(RectangleWithTriangles("patrat2", glm::vec2(0.7f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), vector_gol, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
	obiecte.push_back(RectangleWithTriangles("patrat3", glm::vec2(2.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), vector_gol, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
	obiecte.push_back(RectangleWithTriangles("barna", glm::vec2(1.5f, 0.1f), glm::vec2(0.2f, 0.05f), glm::vec4(0.61f, 0.45f, 0.16f, 1.0f), vector_gol, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)));

	obiecte.push_back(RectangleWithTriangles("coloana1", glm::vec2(0.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), triunghiuri_coloana, glm::vec2(0.625f, -1.5f), glm::vec2(0.5f, 1.5f)));
	obiecte.push_back(RectangleWithTriangles("coloana2", glm::vec2(0.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), triunghiuri_coloana, glm::vec2(0.825f, -1.5f), glm::vec2(0.5f, 1.5f)));
	obiecte.push_back(RectangleWithTriangles("coloana3", glm::vec2(0.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), triunghiuri_coloana, glm::vec2(1.025f, -1.5f), glm::vec2(0.5f, 1.5f)));
	
	obiecte.push_back(RectangleWithTriangles("coloana4", glm::vec2(0.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), triunghiuri_coloana, glm::vec2(1.925f, -1.5f), glm::vec2(0.5f, 1.5f)));
	obiecte.push_back(RectangleWithTriangles("coloana5", glm::vec2(0.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), triunghiuri_coloana, glm::vec2(2.125f, -1.5f), glm::vec2(0.5f, 1.5f)));
	obiecte.push_back(RectangleWithTriangles("coloana6", glm::vec2(0.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), triunghiuri_coloana, glm::vec2(2.325f, -1.5f), glm::vec2(0.5f, 1.5f)));
	
	obiecte.push_back(RectangleWithTriangles("coloana7", glm::vec2(0.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), triunghiuri_coloana, glm::vec2(-0.075f, -1.5f), glm::vec2(0.5f, 1.5f)));
	obiecte.push_back(RectangleWithTriangles("coloana8", glm::vec2(0.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), triunghiuri_coloana, glm::vec2(0.325f, -1.5f), glm::vec2(0.5f, 1.5f)));
	obiecte.push_back(RectangleWithTriangles("coloana9", glm::vec2(0.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), triunghiuri_coloana, glm::vec2(0.125f, -1.5f), glm::vec2(0.5f, 1.5f)));
	
	obiecte.push_back(RectangleWithTriangles("tzepi1", glm::vec2(0.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), triunghiuri_tzepi, glm::vec2(0.5f, -1.8f), glm::vec2(0.5f, 1.5f)));
	obiecte.push_back(RectangleWithTriangles("tzepi2", glm::vec2(0.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), triunghiuri_tzepi, glm::vec2(1.2f, -1.8f), glm::vec2(0.5f, 1.5f)));
	obiecte.push_back(RectangleWithTriangles("tzepi3", glm::vec2(0.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), triunghiuri_tzepi, glm::vec2(1.5f, -1.8f), glm::vec2(0.5f, 1.5f)));
	obiecte.push_back(RectangleWithTriangles("tzepi4", glm::vec2(0.0f, -1.0f), glm::vec2(0.5f, 0.4f), glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), triunghiuri_tzepi, glm::vec2(1.8f, -1.8f), glm::vec2(0.5f, 1.5f)));


	obiecte.push_back(RectangleWithTriangles("indianajones", glm::vec2(0.0f, -0.6f), glm::vec2(0.1f, 0.5f), glm::vec4(0.7f, 0.5f, 0.5f, 0.0f), triunghiuri_normale2, glm::vec2(-0.05f, 0.1f), glm::vec2(1.05f, 1.5f)));
	obiecte.push_back(RectangleWithTriangles("indianajonesbici", glm::vec2(0.0f, -0.6f), glm::vec2(0.1f, 0.5f), glm::vec4(0.5f, 0.7f, 0.5f, 0.0f), triunghiuri_bici, glm::vec2(-0.05f, 0.1f), glm::vec2(1.25f, 1.5f)));
	obiecte.push_back(RectangleWithTriangles("indianajonesbarna", glm::vec2(0.0f, -0.6f), glm::vec2(0.1f, 0.5f), glm::vec4(0.5f, 0.5f, 0.7f, 0.0f), triunghiuri_barna, glm::vec2(-0.10f, 0.15f), glm::vec2(1.05f, 1.5f)));




	
// setup la opengl	
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "IJ The Buried Ark", NULL, NULL);
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
	glClearColor(0.20, 0.14, 0.05, 1.0);
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



	// separate setup for whip
	float lineVertices[] = {
		0.0f, -0.6f, 0.0f, // First point
		1.5f, 0.1f, 0.0f  // Second point
	};

	unsigned int lineVAO, lineVBO;

	// Generate and bind VAO
	glGenVertexArrays(1, &lineVAO);
	glBindVertexArray(lineVAO);

	// Generate and bind VBO
	glGenBuffers(1, &lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

	// Define the vertex attribute for position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind VAO for safety
	glBindVertexArray(0);









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
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			whipping = 0;
		}

		// if whipping and close enough, start swinging animation
		if (whipping && isWhipHittingBar()) {

			swinging = 1;
		}

		// correction
		if (swinging == 1)
		{
			whipping = 0;
		}


		


		// space to jump, ONLY on ground, only after a jump finishes
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && jumping == 0 && (checkCollision() || swinging == 1))
		{
			jumping = 1;
			firstJumpFrame = glfwGetTime();
		}

		// if jumping => animation
		if (jumping == 1) {

			if (swinging == 1)
				jumpDuration = longjumpDuration;

			float currentJumpFrame = glfwGetTime();
			deltaJumpTime = currentJumpFrame - firstJumpFrame;
			float normalizedTime = (deltaJumpTime / jumpDuration*2) * 3.1456f;

			const float radius = jumpHeight;
			float cosinus = cos(normalizedTime);		// values (-1, 1)
			camera_pos_y = (-1.0f + cosinus) * radius;	// values (0, jumpHeight)

			// stop jumping animation when duration expires
			if (deltaJumpTime > jumpDuration)
			{
				jumping = 0;
				if (swinging == 1)
					swinging = 0;
					jumpDuration = shortjumpDuration;
					
			}
		}





		// Applying gravity when not jumping and not colliding with ground
		if (!jumping && !checkCollision()) {
			camera_pos_y += gravity;
		}

		// daca se intinde prea mult biciul ca n-ai sarit cum trebe, se rupe biciul si cazi
		if (camera_pos_y > razaBiciului)
			swinging = 0;

		// death condition (falling below a certain depth)
		if (camera_pos_y > 0.8f)
			dead = 1;

		// tine cont si de translatii laterale si verticale
		/// .. DACA nu e mort, in care caz se pune stop
		if(dead == 0)
			trans = glm::translate(glm::mat4(1.0f), glm::vec3(camera_pos_x, camera_pos_y, 0.0));



		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);



		// randat obiectele (cu hitboxurile lor)
		//// nested aici e si randarea triunghiurilor
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
				

			
			// nu randam hitboxurile lu indiana
			if (obiecte.at(j).getName() != "indianajones" && obiecte.at(j).getName() != "indianajonesbici" && obiecte.at(j).getName() != "indianajonesbarna") {
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}



			// randat TRIUNGHIURILE obiectelor
			for (int k = 0; k < obiecte.at(j).getTriunghiuri().size(); k++)
			{
				// bind VAO
				glBindVertexArray(*(obiecte.at(j).getTriunghiuri().at(k).getVAO()));



				// shaderu de pozitie (mai mult un MVP)
				// tine cont de offseturi si scaling

				glm::mat4 transformare(1.0f);
				glm::vec3 offset = glm::vec3(obiecte.at(j).getPolygonsOffset().x, obiecte.at(j).getPolygonsOffset().y, 0.0f);
				glm::vec3 scale = glm::vec3(obiecte.at(j).getPolygonsScale().x, obiecte.at(j).getPolygonsScale().y, 1.0f);

				// Apply translation
				transformare = glm::translate(trans, offset);

				// Apply scale
				transformare = glm::scale(transformare, scale);

				unsigned int transformLoc = glGetUniformLocation(programID, "transform");
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformare));



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

					// Apply translation
					transformare = glm::translate(locplayer, offset);

					// Apply scale
					transformare = glm::scale(transformare, scale);

					// Apply the rotation to the locplayer matrix
					glm::mat4 transformedLocplayer = rotationMatrix * transformare;

					// aplicam peste transformarea terenului o transformare care nu se misca
					unsigned int transformLoc2 = glGetUniformLocation(programID, "transform");
					glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(transformedLocplayer));
				}



				// shaderu de culoare
				unsigned int transformColor = glGetUniformLocation(programID, "color");
				glUniform4fv(transformColor, 1, glm::value_ptr(obiecte.at(j).getTriunghiuri().at(k).getColor()));



				// i.fs.d. stare, nu randam poligoanele care corespund altor stari
				// plus ca, daca e indiana jones mort, nu il randam pe el
				if (swinging == 0 && whipping == 0 && obiecte.at(j).getName() != "indianajonesbici" && obiecte.at(j).getName() != "indianajonesbarna") {

					if (obiecte.at(j).getName() == "indianajones" && dead == 0 || obiecte.at(j).getName() != "indianajones") {
						glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
					}
				}
				if (whipping == 1 && obiecte.at(j).getName() != "indianajones" && obiecte.at(j).getName() != "indianajonesbarna") {
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
				if (swinging == 1 && obiecte.at(j).getName() != "indianajones" && obiecte.at(j).getName() != "indianajonesbici") {
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}


			}

		}
	


		// randat sfoara biciului
		if (swinging == 1)
		{
			lineVertices[0] = -1.0f * (camera_pos_x - 0.1f);
			lineVertices[1] = -1.0f * (camera_pos_y + 0.2f);
			glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lineVertices), lineVertices);

			// Bind the line VAO
			glBindVertexArray(lineVAO);

			// Set uniforms if necessary (like transform or color)
			unsigned int transformLoc = glGetUniformLocation(programID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

			unsigned int colorLoc = glGetUniformLocation(programID, "color");
			glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);  // Example: Red color

			// Draw the line
			glDrawArrays(GL_LINES, 0, 2);

			// Unbind VAO
			glBindVertexArray(0);
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


