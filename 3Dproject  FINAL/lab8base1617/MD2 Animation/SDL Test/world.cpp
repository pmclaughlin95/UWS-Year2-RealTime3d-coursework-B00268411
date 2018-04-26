#include "world.h"
using namespace std;

world::world()
{
	init();
}

void world::gameLoop(SDL_Window * hWindow)
{
	update();
	draw(hWindow); 
}

void world::init(void)
{
	//Load objects
	loadBoundaries();
	loadBasicCube();
	loadBuilding();
	loadCollectable();
	

	//initialise shaders
	//skyboxShaderProgram = rt3d::initShaders("textured.vert", "textured.frag");		//This was supposed to be used with the new cubemapped skybox.
	lightShaderProgram = rt3d::initShaders("gouraud.vert  ", "simple.frag");			
	ShaderProgram = rt3d::initShaders("phong-tex.vert", "phong-tex.frag");	
	
	//fixed light source
	rt3d::setLight(lightShaderProgram, sunLight);
	lightPos = { 0, 5, -3, 5.0f }; 
	rt3d::setLightPos(lightShaderProgram, glm::value_ptr(lightPos));

	//initial variables
	initVariables();
	mvStack.push(glm::mat4(1.0));
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
}

void world::inittextures()
{
	textures[0] = loadBitmap("grass.bmp");
	textures[4] = loadBitmap("gold.bmp");
	textures[5] = loadBitmap("scratches.bmp");
	textures[6] = loadBitmap("sky1.bmp");
	textures[7] = loadBitmap("building1.bmp");
	textures[9] = loadBitmap("road.bmp");
}

void world::initPlayerModel()
{
	cout << "player initialised" << endl;
	textures[11] = loadBitmap("hayden.bmp");
	meshObjects[11] = tmpModel.ReadMD2Model("hayden-tris.MD2");
	md2VertCount = tmpModel.getVertDataCount();
}

void world::initVariables()
{
	score = 0;
	//perspective
	fov = float(90.0f*DEG_TO_RADIAN);
	aspect = 800 / 600;
	near = 1.0f;
	far = 280.0f;

	//camera
	eye = { 0.0f, 1.0f, 4.0f };
	at = { 0.0f, 1.0f, 3.0f };
	up = { 0.0f, 1.0f, 0.0f };
	pos = { 0.0f, 3.0f, 5.0f };

	//movements
	r = 0;
	rotatePlayer = 0;
	movePlayer = 0;

	//Character
	tmpModel;
	currentAnim = 0;
	md2VertCount = 0;

	//initialise
	initPlayerModel();
	inittextures();

	//objects creation
	inputHandler - new InputHandler();

	PlayerAABB = new boundingBox(glm::vec3(pos.x - 0.2, pos.y - 3, pos.z - 0.2), glm::vec3(pos.x + 0.2, pos.y, pos.z + 0.2));

	buildingAABB[0] = new boundingBox(glm::vec3(31, -1, -14), glm::vec3(51.9, 10, 12));
	buildingAABB[1] = new boundingBox(glm::vec3(31, -1, -94), glm::vec3(51.9, 10, -68));
	buildingAABB[2] = new boundingBox(glm::vec3(31, -1, -174), glm::vec3(51.9, 10, -148));

	buildingAABB[3] = new boundingBox(glm::vec3(111, -1, -14), glm::vec3(132, 10, 12));
	buildingAABB[4] = new boundingBox(glm::vec3(110, -1, -94), glm::vec3(132, 10, -68));
	buildingAABB[5] = new boundingBox(glm::vec3(111, -1, -174), glm::vec3(132, 10, -148));

	//Collectable objects
	for (int i = 0; i < 19; i++)
	{
		glm::vec3 position(rnd(10, 150), 2, rnd(10, -150));
		collectable[i] = new Collectable(position);
		collectableAABB[i] = new boundingBox(collectable[i]->position - glm::vec3(2, 2, 2), collectable[i]->position + glm::vec3(2, 2, 2));
	}
}


void world::loadBasicCube()
{
	//cube
	rt3d::loadObj("cube.obj", verts, norms, tex_coords, indices);
	size = indices.size();
	meshIndexCount = size;
	meshObjects[5] = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
	resetObjectData();
}

void world::loadBoundaries()
{
	//Fence parameter
	rt3d::loadObj("fMence.obj", verts, norms, tex_coords, indices);
	size = indices.size();
	objectIndexCount = size;
	meshObjects[6] = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
	resetObjectData();
}

void world::loadBuilding()
{
	rt3d::loadObj("Building.obj", verts, norms, tex_coords, indices);
	size = indices.size();
	buildingIndexCount = size;
	meshObjects[12] = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), indices.size(), indices.data());
	resetObjectData();
}

void world::loadCollectable() {
	rt3d::loadObj("cube.obj", verts, norms, tex_coords, indices);
	size = indices.size();
	collectableIndexCount = size;
	meshObjects[16] = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), indices.size(), indices.data());
	resetObjectData();
	showCollectable = true;

}

void world::resetObjectData()
{
	verts.clear();
	norms.clear();
	tex_coords.clear();
	indices.clear();
}

//draws everything
void world::draw(SDL_Window * window) {

	

	// set up 
	glm::mat4 projection(1.0);
	projection = glm::perspective(fov, aspect, near, far);
	rt3d::setUniformMatrix4fv(ShaderProgram, "projection", glm::value_ptr(projection));
	glm::mat4 identity(1.0);

	glm::mat4 modelview(1.0);
	mvStack.push(modelview);
	mvStack.top() = glm::lookAt(eye, at, up);
	// clear the screen
	glClearColor(0.5, 0.5, 0.5, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glDisable(GL_DEPTH_TEST);
	drawSkybox();
	glEnable(GL_DEPTH_TEST); // enable depth testing
	
	drawPlayerCharacter();
	drawGroundAndBuildings();
	drawFence();

	drawCollectable();
	
	SDL_GL_SwapWindow(window); // swap buffers
}


void world::drawSkybox()
{
	
	GLfloat verts = 2;
	GLfloat skyBoxTexCoords[] = { 0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f };

	GLfloat skyBoxVerts[] = { -verts, -verts, -verts,
		-verts, verts, -verts,
		verts, verts, -verts,
		verts, -verts, -verts,
		-verts, -verts, verts,
		-verts, verts, verts,
		verts, verts, verts,
		verts, -verts, verts };

	GLuint skyBoxIndices[] =
	{ 0,1,2, 0,2,3, // back  
		1,0,5, 0,4,5, // left					
		6,3,2, 3,6,7, // right
		1,5,6, 1,6,2, // top
		0,3,4, 3,7,4, // bottom
		6,5,4, 7,6,4 }; // front


	meshObjects[2] = rt3d::createMesh(cubeVertCount, skyBoxVerts, nullptr,
		skyBoxVerts, skyBoxTexCoords, cubeIndexCount, skyBoxIndices);
	glBindTexture(GL_TEXTURE_2D, textures[6]);
	glm::mat3 mvRotOnlyMat3 = glm::mat3(mvStack.top());

	mvStack.push(glm::mat4(mvRotOnlyMat3));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0, 0, 0));
	rt3d::setUniformMatrix4fv(ShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::setMaterial(ShaderProgram, skyboxMaterial);
	rt3d::drawIndexedMesh(meshObjects[2], cubeIndexCount, GL_TRIANGLES);
	mvStack.pop();
	
}
void world::drawFence()
{
	glBindTexture(GL_TEXTURE_2D, textures[10]);
	for (int i = 0; i < 31; i++)
	{
		mvStack.push(mvStack.top());
		mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-20, 2.5, +16.8 - (i * 6.4)));
		mvStack.top() = glm::scale(mvStack.top(), glm::vec3(0.05, 0.05, 0.05));
		rt3d::setUniformMatrix4fv(ShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
		rt3d::setMaterial(ShaderProgram, material1);
		rt3d::drawIndexedMesh(meshObjects[6], objectIndexCount, GL_TRIANGLES);
		mvStack.pop();
	}
	for (int i = 0; i < 31; i++)
	{
		mvStack.push(mvStack.top());
		mvStack.top() = glm::translate(mvStack.top(), glm::vec3(179, 2.5, +16.8 - (i * 6.4)));
		mvStack.top() = glm::scale(mvStack.top(), glm::vec3(0.05, 0.05, 0.05));
		rt3d::setUniformMatrix4fv(ShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
		rt3d::setMaterial(ShaderProgram, material1);
		rt3d::drawIndexedMesh(meshObjects[6], objectIndexCount, GL_TRIANGLES);
		mvStack.pop();
	}

	for (int i = 0; i < 31; i++)
	{
		mvStack.push(mvStack.top());
		mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-16.5 + (i * 6.4), 2.5, +20));
		mvStack.top() = glm::scale(mvStack.top(), glm::vec3(0.05, 0.05, 0.05));
		mvStack.top() = glm::rotate(mvStack.top(), float(90 * DEG_TO_RADIAN), glm::vec3(0, 1, 0));
		rt3d::setUniformMatrix4fv(ShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
		rt3d::setMaterial(ShaderProgram, material1);
		rt3d::drawIndexedMesh(meshObjects[6], objectIndexCount, GL_TRIANGLES);
		mvStack.pop();
	}

	for (int i = 0; i < 31; i++)
	{
		mvStack.push(mvStack.top());
		mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-16.5 + (i * 6.4), 2.5, -178.5));
		mvStack.top() = glm::scale(mvStack.top(), glm::vec3(0.05, 0.05, 0.05));
		mvStack.top() = glm::rotate(mvStack.top(), float(90 * DEG_TO_RADIAN), glm::vec3(0, 1, 0));
		rt3d::setUniformMatrix4fv(ShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
		rt3d::setMaterial(ShaderProgram, material1);
		rt3d::drawIndexedMesh(meshObjects[6], objectIndexCount, GL_TRIANGLES);
		mvStack.pop();
	}

}

void world::drawGroundAndBuildings()
{ 
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	rt3d::setMaterial(lightShaderProgram, material3);
	
	int columns;
	int rows;
	for (rows = 0; rows < 5; rows++)
	{
		if (rows == 0 || rows == 2 || rows == 4)
		{
			//pavement
			rt3d::setMaterial(ShaderProgram, material4);
			glBindTexture(GL_TEXTURE_2D, textures[9]);
			mvStack.push(mvStack.top());
			mvStack.top() = glm::translate(mvStack.top(), glm::vec3(40 * rows, -0.1f, 0));
			mvStack.top() = glm::scale(mvStack.top(), glm::vec3(20, 0.1, 20));
			rt3d::setUniformMatrix4fv(ShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
			rt3d::drawIndexedMesh(meshObjects[5], meshIndexCount, GL_TRIANGLES);
			mvStack.pop();
		}
		else {
			//grass
			rt3d::setMaterial(ShaderProgram, material0);
			glBindTexture(GL_TEXTURE_2D, textures[0]);
			mvStack.push(mvStack.top());
			mvStack.top() = glm::translate(mvStack.top(), glm::vec3(40 * rows, -0.1f, 0));
			mvStack.top() = glm::scale(mvStack.top(), glm::vec3(20, 0.1, 20));
			rt3d::setUniformMatrix4fv(ShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
			rt3d::drawIndexedMesh(meshObjects[5], meshIndexCount, GL_TRIANGLES);
			mvStack.pop();

			//buildings
			glBindTexture(GL_TEXTURE_2D, textures[7]);
			mvStack.push(mvStack.top());
			mvStack.top() = glm::translate(mvStack.top(), glm::vec3(40 * rows, -0.1f, 0));
			mvStack.top() = glm::scale(mvStack.top(), glm::vec3(10, 60, 10));
			mvStack.top() = glm::rotate(mvStack.top(), float(-23*DEG_TO_RADIAN), glm::vec3(0, 1, 0.0f));
			rt3d::setUniformMatrix4fv(ShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
			rt3d::setMaterial(ShaderProgram, material4);
			rt3d::drawIndexedMesh(meshObjects[12], buildingIndexCount, GL_TRIANGLES);
			mvStack.pop();
			glBindTexture(GL_TEXTURE_2D, textures[0]);

			glBindTexture(GL_TEXTURE_2D, textures[7]);
			mvStack.push(mvStack.top());
			mvStack.top() = glm::translate(mvStack.top(), glm::vec3(40 * rows, -0.1f, -80));
			mvStack.top() = glm::scale(mvStack.top(), glm::vec3(10, 60, 10));
			mvStack.top() = glm::rotate(mvStack.top(), float(-23 * DEG_TO_RADIAN), glm::vec3(0, 1, 0.0f));
			rt3d::setUniformMatrix4fv(ShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
			rt3d::setMaterial(ShaderProgram, material4);
			rt3d::drawIndexedMesh(meshObjects[12], buildingIndexCount, GL_TRIANGLES);
			mvStack.pop();
			glBindTexture(GL_TEXTURE_2D, textures[0]);

			glBindTexture(GL_TEXTURE_2D, textures[7]);
			mvStack.push(mvStack.top());
			mvStack.top() = glm::translate(mvStack.top(), glm::vec3(40 * rows, -0.1f,-160 ));
			mvStack.top() = glm::scale(mvStack.top(), glm::vec3(10, 60, 10));
			mvStack.top() = glm::rotate(mvStack.top(), float(-23 * DEG_TO_RADIAN), glm::vec3(0, 1, 0.0f));
			rt3d::setUniformMatrix4fv(ShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
			rt3d::setMaterial(ShaderProgram, material4);
			rt3d::drawIndexedMesh(meshObjects[12], buildingIndexCount, GL_TRIANGLES);
			mvStack.pop();
			glBindTexture(GL_TEXTURE_2D, textures[0]);
		}

		for (columns = 0; columns < 5; columns++)
		{
			//pavement
			mvStack.push(mvStack.top());
			mvStack.top() = glm::translate(mvStack.top(), glm::vec3(40 * rows, -0.1f, -40 * columns));
			mvStack.top() = glm::scale(mvStack.top(), glm::vec3(20, 0.1, 20));
			rt3d::setUniformMatrix4fv(ShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
			rt3d::drawIndexedMesh(meshObjects[5], meshIndexCount, GL_TRIANGLES);
			mvStack.pop();
			
		}
	}


	


	}

// Animate the md2 model, and update the mesh with new vertex data
void world::drawPlayerCharacter()
{
	
	tmpModel.Animate(currentAnim, 0.1);
	rt3d::updateMesh(meshObjects[11], RT3D_VERTEX, tmpModel.getAnimVerts(), tmpModel.getVertDataSize());

	glBindTexture(GL_TEXTURE_2D, textures[11]);
	mvStack.push(mvStack.top());

	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(pos.x, pos.y-0.85, pos.z));
	mvStack.top() = glm::rotate(mvStack.top(), float(-90.0f*DEG_TO_RADIAN), glm::vec3(1.0f, 0.0f, 0.0f));//sets character upright
	mvStack.top() = glm::rotate(mvStack.top(), float(90 * DEG_TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));
	mvStack.top() = glm::rotate(mvStack.top(), float(-r*DEG_TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));

	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(0.07, 0.07, 0.07));//scales character in its 3 dimensions
	rt3d::setUniformMatrix4fv(ShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::setMaterial(ShaderProgram, material0);
	rt3d::drawMesh(meshObjects[11], md2VertCount, GL_TRIANGLES);
	mvStack.pop();
	
}

//Draws collectables depending on whther they have been collected or not
void world::drawCollectable() {
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	rt3d::setMaterial(lightShaderProgram, material3);

	for (int i = 0; i < 19; i++)
	{
		if (collectableCollisionTest(PlayerAABB, collectableAABB[i]))
		{
			collectable[i]->setVisibility(false);
			cout << "collision" << endl;
			score += 1;
			cout << "SCORE:  " << score << endl;
		}
		

		if (collectable[i]->visibility == true)
		{
			glBindTexture(GL_TEXTURE_2D, textures[4]);
			mvStack.push(mvStack.top());
			mvStack.top() = glm::translate(mvStack.top(), collectable[i]->position);
			mvStack.top() = glm::scale(mvStack.top(), glm::vec3(0.5, 0.5, 0.5));
			mvStack.top() = glm::rotate(mvStack.top(), float(-2 * DEG_TO_RADIAN), glm::vec3(0, 1, 0.0f));
			rt3d::setUniformMatrix4fv(ShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
			rt3d::setMaterial(ShaderProgram, materialGold);
			rt3d::drawIndexedMesh(meshObjects[16], collectableIndexCount, GL_TRIANGLES);
			mvStack.pop();
			glBindTexture(GL_TEXTURE_2D, textures[0]);
			
		}
		
	}
		
	
}


//Handles collision and input
void world::update(void) {

	PlayerAABB->update(glm::vec3(pos.x, pos.y - 3, pos.z), glm::vec3(pos.x + 2, pos.y, pos.z - 2));

	inputHandler->handleInput(this);	
}

//AABB-AABB test
bool world::collisionTest(boundingBox* a, boundingBox* b) {	//Taken from real time collision detection chapter 4
	// Exit with no intersection if separated along an axis
	bool collision = true;

	if (a->max.x < b->min.x || a->min.x > b->max.x) return 0;


	if (a->max.y < b->min.y || a->min.y > b->max.y) return 0;


	if (a->max.z < b->min.z || a->min.z > b->max.z) return 0;


	if (collision)
		//cout << "collision!";

	return 1;

}	

bool world::collectableCollisionTest(boundingBox*a, boundingBox*b) {

	bool collision = true;

	if (a->max.x < b->min.x || a->min.x > b->max.x) return 0;


	if (a->max.y < b->min.y || a->min.y > b->max.y) return 0;


	if (a->max.z < b->min.z || a->min.z > b->max.z) return 0;	
	
	return 1;
}

//Stops player in event of collision
void world::collisionResolve()
{
	bool collision;
	int i;
	for (i = 0; i < 6; i++)
	{
		collision = collisionTest(PlayerAABB, buildingAABB[i]);

		if (!collision)
			inputHandler->PlayerMove(this);
		else
		{
			movePlayer -= 0.3;
			inputHandler->PlayerMove(this);
			movePlayer = 0;
		}
		if (pos.x > 177.5 || pos.x < -18)
		{
			movePlayer -= 0.3;
			inputHandler->PlayerMove(this);
			movePlayer = 0;
		}
		if (pos.z > 18.7 || pos.z < -176.7)
		{
			movePlayer -= 0.3;
			inputHandler->PlayerMove(this);
			movePlayer = 0;
		}

	}
}

//Utility
GLuint world::loadBitmap(char *fname)
{
	GLuint texID;
	glGenTextures(1, &texID); // generate texture ID
							  // load file - using core SDL library
	SDL_Surface *tmpSurface;
	tmpSurface = SDL_LoadBMP(fname);
	if (!tmpSurface)
	{
		std::cout << "Error loading bitmap" << std::endl;
	}

	// bind texture and set parameters
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_PixelFormat *format = tmpSurface->format;
	GLuint externalFormat, internalFormat;
	if (format->Amask) {
		internalFormat = GL_RGBA;
		externalFormat = (format->Rmask < format->Bmask) ? GL_RGBA : GL_BGRA;
	}
	else {
		internalFormat = GL_RGB;
		externalFormat = (format->Rmask < format->Bmask) ? GL_RGB : GL_BGR;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tmpSurface->w, tmpSurface->h, 0,
		externalFormat, GL_UNSIGNED_BYTE, tmpSurface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(tmpSurface); // texture loaded, free the temporary buffer
	return texID;	// return value of texture ID
}


 
