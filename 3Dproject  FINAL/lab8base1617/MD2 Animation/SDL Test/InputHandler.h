#pragma once
#include "world.h"
class world;


class InputHandler
{
public:
	InputHandler(void){}
	void handleInput(world* world);

	//Forward motion
	glm::vec3 moveForward(glm::vec3 cam, GLfloat angle, GLfloat d);

	//Rotate
	glm::vec3 rotate(glm::vec3 pos, GLfloat angle, GLfloat d);

	//Values applied in moveForward and Rotate
	void PlayerRotate(world *world);

	void PlayerMove(world *world);
	
	

};
