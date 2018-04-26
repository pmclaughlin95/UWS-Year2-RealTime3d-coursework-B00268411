#include "InputHandler.h"

void InputHandler::handleInput(world* world)
{
	SDL_Event sdlevent;
	
	const	Uint8* keys = SDL_GetKeyboardState(NULL);
	
	while (SDL_PollEvent(&sdlevent)) {
		switch (sdlevent.type) {
			if (sdlevent.type == SDL_QUIT)
				world->running = false;
		case SDL_KEYDOWN:

			switch (sdlevent.key.keysym.sym) {
			case SDLK_w:
				world->movePlayer = 0.1;		//Sets velocity of player
				world->currentAnim = 1;			//Triggers run animation
				break;
			case SDLK_a:
				world->rotatePlayer = 2;
				break;
			case SDLK_d:
				world->rotatePlayer = -2;
				break;
			case SDLK_SPACE:					//Triggers jump animation
				world->currentAnim = 6;
				break;
			case SDLK_1:						//Triggers attack animation
				world->currentAnim = 2;
				break;
			case SDLK_2:						//Triggers salute animation
				world->currentAnim = 8;
				break;
			}
			break;

		case SDL_KEYUP:                
			switch (sdlevent.key.keysym.sym) {

			case SDLK_w:
				world->movePlayer = 0;
				world->currentAnim = 0;
				break;

			case SDLK_a:
				world->rotatePlayer = 0;
				break;

			case SDLK_d:
				world->rotatePlayer = 0;
				break;

			case SDLK_SPACE:
				world->currentAnim = 0;
				break;

			case SDLK_1:
				world->currentAnim = 0;
				break;

			case SDLK_2:
				world->currentAnim = 0;
				break;
			}
			break;

		}
	}

	//deal with any collisions
	world->collisionResolve();

	//updates player movements
	PlayerRotate(world);
	world->at = world->pos;
	world->eye = moveForward(world->at, world->r, -4.0f);

}


glm::vec3 InputHandler::moveForward(glm::vec3 cam, GLfloat angle, GLfloat d)
{
	return glm::vec3(cam.x + d*std::sin(angle*DEG_TO_RADIAN), cam.y, cam.z - d*std::cos(angle*DEG_TO_RADIAN));
}

glm::vec3 InputHandler::rotate(glm::vec3 pos, GLfloat angle, GLfloat d)
{
	return glm::vec3(pos.x + d*std::cos(angle*DEG_TO_RADIAN), pos.y, pos.z + d*std::sin(angle*DEG_TO_RADIAN));
}

void InputHandler::PlayerRotate(world *world)
{
	world->r -= world->rotatePlayer;
}

void InputHandler::PlayerMove(world *world)
{
	world->pos = moveForward(world->pos, world->r, world->movePlayer);
}
