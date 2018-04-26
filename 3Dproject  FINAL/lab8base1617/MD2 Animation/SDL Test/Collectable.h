#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Collectable
{
public:
	Collectable(glm::vec3 pos);
	
	void setVisibility(bool visible) { visibility = visible; }

	//This is tested before drawn in World.cpp
	bool getVisibility() { return visibility; }

	glm::vec3 position;
	bool visibility;

};
