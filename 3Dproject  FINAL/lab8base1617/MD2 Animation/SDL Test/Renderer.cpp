//#include "Renderer.h"
//
//#define DEG_TO_RADIAN 0.017453293
//
//Renderer::Renderer()
//{	
//	//rt3d::setLight(shaderProgram, light0);
//	//rt3d::setMaterial(shaderProgram, material0);
//
//	//initialise essentials
//	//glEnable(GL_DEPTH_TEST);
//	glm::mat4 projection(1.0);
//	GLfloat scale(1.0f); 
//	glm::mat4 modelview(1.0); 
//
//	//mvpShaderProgram = rt3d::initShaders("phong.vert", "phong.frag");
//
//	glm::vec4 lightPos(-10.0f, 10.0f, 10.0f, 1.0f); //light position
//;
//	//Initialise other shit
//	initLights();
//	initMaterials();
//	initTextures();
//	initMeshes();
//
//	
//	//glEnable(GL_BLEND);
//	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//}
//
//
//
//
//void Renderer::draw(SDL_Window * window)
//{
//	glEnable(GL_CULL_FACE);
//	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	//set up projection
//	projection = glm::perspective(float(60.0f*DEG_TO_RADIAN), 800.0f / 600.0f, 1.0f, 50.0f);
////	rt3d::setUniformMatrix4fv(mvpShaderProgram, "projection", glm::value_ptr(projection));
//
//	//push in modelview
//	mvStack.push(modelview);
//
//
//	glDepthMask(GL_TRUE); // make sure depth test is on
//	// draw a cube for ground plane
//	glBindTexture(GL_TEXTURE_2D, textures[0]);
//	mvStack.push(mvStack.top());
//	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, -0.1f, 0.0f));
//	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(20.0f, 0.1f, 20.0f));
////	rt3d::setUniformMatrix4fv(mvpShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
////	rt3d::setMaterial(mvpShaderProgram, material0);
//	rt3d::drawIndexedMesh(meshObjects[0],6, GL_TRIANGLES);
//	mvStack.pop();
//
//	mvStack.pop(); // initial matrix
//	glDepthMask(GL_TRUE);
//
//    SDL_GL_SwapWindow(window); // swap buffers
//
//
//}
//
//GLuint Renderer::loadBitmap(char *fname) 
//{
//	GLuint texID;
//	glGenTextures(1, &texID); // generate texture ID
//
//	// load file - using core SDL library
//	SDL_Surface *tmpSurface;
//	tmpSurface = SDL_LoadBMP(fname);
//	if (!tmpSurface) {
//		std::cout << "Error loading bitmap" << std::endl;
//	}
//
//	// bind texture and set parameters
//	glBindTexture(GL_TEXTURE_2D, texID);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//	SDL_PixelFormat *format = tmpSurface->format;
//
//	GLuint externalFormat, internalFormat;
//	if (format->Amask) 
//	{
//		internalFormat = GL_RGBA;
//		externalFormat = (format->Rmask < format->Bmask) ? GL_RGBA : GL_BGRA;
//	}
//	else 
//	{
//		internalFormat = GL_RGB;
//		externalFormat = (format->Rmask < format->Bmask) ? GL_RGB : GL_BGR;
//	}
//
//	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tmpSurface->w, tmpSurface->h, 0,
//		externalFormat, GL_UNSIGNED_BYTE, tmpSurface->pixels);
//	glGenerateMipmap(GL_TEXTURE_2D);
//
//	SDL_FreeSurface(tmpSurface); // texture loaded, free the temporary buffer
//	return texID;	// return value of texture ID
//}
//
//void Renderer::initTextures()
//{
//	textures[0] = loadBitmap("fabric.bmp");
//	textures[1] = loadBitmap("hobgoblin2.bmp");
//}
//
//
//void Renderer::initMeshes()
//{
//	//eg
//	//meshObjects[0] = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
//	//meshObjects[1] = tmpModel.ReadMD2Model("tris.MD2");
//}
//
//
//
//void Renderer::initLights()
//{
//
//	rt3d::lightStruct light0 = {
//		{ 0.3f, 0.3f, 0.3f, 1.0f }, // ambient
//		{ 1.0f, 1.0f, 1.0f, 1.0f }, // diffuse
//		{ 1.0f, 1.0f, 1.0f, 1.0f }, // specular
//		{ -10.0f, 10.0f, 10.0f, 1.0f }  // position
//	};
//
//}
//
//void Renderer::initMaterials()
//{
//
//	rt3d::materialStruct material0 = {
//		{ 0.2f, 0.4f, 0.2f, 1.0f }, // ambient
//		{ 0.5f, 1.0f, 0.5f, 1.0f }, // diffuse
//		{ 0.0f, 0.1f, 0.0f, 1.0f }, // specular
//		2.0f						// shininess
//	};
//
//}