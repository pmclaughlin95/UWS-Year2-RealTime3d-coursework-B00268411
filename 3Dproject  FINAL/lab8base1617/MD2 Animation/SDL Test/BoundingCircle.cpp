#include "BoundingCircle.h"


boundingCircle::boundingCircle(glm::vec2 position, double radius)
{

	position = this->position;
	radius = this->radius;

}


void boundingCircle::update(glm::vec2 position, double radius)
{
	position = this->position;

	if(radius != NULL)				// radius will often not need updated
	radius = this->radius;
}


void boundingCircle::collision()
{




}




void boundingCircle::draw() 
{

	

}