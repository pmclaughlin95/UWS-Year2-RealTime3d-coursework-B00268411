#pragma once
#include <stack>
#include "rt3d.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "rt3dObjLoader.h"
#include "md2model.h"
#include "BoundingCircle.h"
#include "BoundingBox.h"
#include "Collectable.h"
#include "Rnd.h"
#include "InputHandler.h"

#include <iostream>
using namespace std;

class InputHandler;
class boundingBox;

#define DEG_TO_RADIAN 0.017453293



class world
{
public:
	world();
	void gameLoop(SDL_Window * hWindow);

	bool running;

	void draw(SDL_Window * window);
	void update(void);


	//initfunctions
	void initVariables();		//initialises all variable, textures, materials, lights etc including the shadrer programme and stuff
	void init(void);
	void inittextures();

	//Draw functions
	void drawPlayerCharacter();
	void drawGroundAndBuildings();
	void drawSkybox();
	void drawCollectable();

	//utilities
	GLuint loadBitmap(char *fname);

	//Collision Tests
	bool collisionTest(boundingBox* a, boundingBox* b);
	bool collectableCollisionTest(boundingBox*a, boundingBox*b);
	void collisionResolve();
	
	// Object functions
	void loadBasicCube();					//This is being used for every cube in the game (ground, sun...)
	void loadBoundaries();					
	void drawFence();		
	void loadBuilding();
	void loadCollectable();
 	void resetObjectData();					//we need to reset the variables we use to load the objects in before theyre loaded
	void initPlayerModel();

	

	// Variables made public for use by the input handler
	//movements
	GLfloat rotatePlayer;
	GLfloat movePlayer;
	GLfloat r;
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 pos;
	int currentAnim;

	
private:
	InputHandler *inputHandler;

	//Collectable
	int score;
	Collectable *collectable[20];
	bool showCollectable;
	
	//Collisions
	boundingBox *PlayerAABB;
	boundingBox *buildingAABB[6];
	boundingBox *collectableAABB[20];
	
	//object data
	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	GLuint size;
	float meshIndexCount;
	int objectIndexCount;
	float buildingIndexCount;
	float collectableIndexCount;
	GLuint cubeVertCount = 8;
	GLuint cubeIndexCount = 36;

	//rendering stuff
	GLuint ShaderProgram;
	GLuint lightShaderProgram;
	GLuint skyboxShaderProgram;
	std::stack<glm::mat4> mvStack;

	GLuint meshObjects[17];
	GLuint textures[10];
	glm::vec4 lightPos;
	

	//camera variables
	glm::vec3 up;

	//perspective variables
	GLfloat fov;
	GLfloat aspect;
	GLfloat near;
	GLfloat far;

	//Character model variables
	md2model tmpModel;
	GLuint md2VertCount;
	

	//Lights and materials

	rt3d::lightStruct sunLight = {
		{ 1, 1, 1, 0.5f }, 
		{ 5, 5, 5, 0.5 }, 
		{ 0, 0, 0,1 }, 
		{ 0.0f, 5, -3, 1.0f }  
	};
	
	rt3d::materialStruct sunMaterial = {
		{ 1 ,1, 1, 0.3f }, 
		{ 0.8f, 0.5f, 0.5f, 0.3f }, 
		{ 1.0f, 0.8f, 0.8f, 0.3f }, 
		2.0f  
	};

	rt3d::materialStruct skyboxMaterial = {
		{ 0.15f, 0.15f, 0.15f, 1 }, 
		{ 0.5f, 0.5f, 0.5f, 0.5f }, 
		{ 0, 0, 0, 0 }, 
		1  
	};

	rt3d::materialStruct material4 = {
		{ 0.19225f, 0.19225f, 0.19225f, 1 }, 
		{ 0.50754f, 0.50754f, 0.50754f, 1 }, 
		{ 0.508273f, 0.508273f, 0.508273f, 1 }, 
		0.1f  
	};

	rt3d::materialStruct material6 = {
		{ 0.19125f, 0.0735f, 0.0225f, 1 }, 
		{ 0.7038f, 0.27048f, 0.0828f, 1 }, 
		{ 0.256777f, 0.137622f, 0.086014f, 1 }, 
		1.0f  
	};

	rt3d::lightStruct light0 = {
		{ 0.2f, 0.2f, 0.2f, 1.0f }, 
		{ 0.7f, 0.7f, 0.7f, 1.0f }, 
		{ 0.8f, 0.8f, 0.8f, 1.0f }, 
		{ 0, 0, 0, 1.0f }  
	};

	rt3d::lightStruct light1 = {
		{ 0.3f, 0.3f, 0.3f, 1.0f }, 
		{ 1.0f, 1.0f, 1.0f, 1.0f }, 
		{ 1.0f, 1.0f, 1.0f, 1.0f }, 
		{ -10.0f, 10.0f, 10.0f, 1.0f }  
	};

	

	rt3d::materialStruct material2 = { 
		{ 0.4f, 0.2f, 0.2f, 0.3f }, 
		{ 0.8f, 0.5f, 0.5f, 0.3f }, 
		{ 1.0f, 0.8f, 0.8f, 0.3f }, 
		2.0f  
	};

	rt3d::materialStruct material1 = {
		{ 0.38f, 0.98f, 0.26f, 1.0f }, 
		{ 0.38f, 0.98f, 0.26f, 1.0f }, 
		{ 0.8f, 0.8f, 0.8f, 1.0f }, 
		1.0f  
	};

	rt3d::materialStruct material0 = { 
		{ 0.19225f	, 0.19225f	, 0.19225f	, 1 }, 
		{ 0.50754f	, 0.50754f	, 0.50754f, 1 }, 
		{ 0.508273f	, 0.508273f	, 0.508273f	, 1 }, 
		50  
	};

	rt3d::materialStruct material11 = {
		{ 0.2f, 0.4f, 0.2f, 1.0f }, 
		{ 0.5f, 1.0f, 0.5f, 1.0f }, 
		{ 0.0f, 0.1f, 0.0f, 1.0f }, 
		2.0f  
	};

	rt3d::materialStruct materialGold = {		
		{ 0.24725f, 0.1995f, 0.0745f, 1.0f }, 
		{ 0.75164f, 0.60648f, 0.22648f, 1.0f }, 
		{ 0.628281f, 0.555802f, 0.366065f, 1.0f }, 
		10  // shininess
	};
	
	rt3d::materialStruct material3 = {		
		{ 0, 0.05f, 0, 1.0f }, 
		{ 0.4f, 0.5f, 0.4f, 1.0f }, 
		{ 0.04f, 0.4f, 0.04f, 1.0f }, 
		2.0f  
	};


};
