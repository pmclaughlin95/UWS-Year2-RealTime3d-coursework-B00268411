#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "rt3d.h"

struct boundingCircle {

public:
	~boundingCircle(){}

	boundingCircle(glm::vec2 position, double radius);
	void update(glm::vec2 position, double radius);


	float getRadii() { return radius; }
	glm::vec2 getPosition() { return position; }

	void collision();
	void draw();


private:
	glm::vec2 position;
	double radius;

};

