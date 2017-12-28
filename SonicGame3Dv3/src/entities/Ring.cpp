#include <glad/glad.h>

#include "entities.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "ring.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"

#include <list>
#include <iostream>

std::list<TexturedModel*> Ring::models;

Ring::Ring()
{

}

Ring::Ring(float x, float y, float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
}

void Ring::step()
{
	increaseRotation(0, 1, 0);
	updateTransformationMatrix();
}

std::list<TexturedModel*>* Ring::getModels()
{
	return &Ring::models;
}

void Ring::loadStaticModels()
{
	if (Ring::models.size() > 0)
	{
		//std::fprintf(stdout, "Ring models gone astray");
		return;
	}

	std::fprintf(stdout, "Loading ring static models...\n");

	std::list<TexturedModel*>* newModels = loadObjModel("res/Models/Ring/", "Ring.obj");
	for (auto newModel : (*newModels))
	{
		Ring::models.push_back(newModel);
	}
	delete newModels;
	Global::countDelete++;
}

void Ring::deleteStaticModels()
{
	std::fprintf(stdout, "Deleting ring static models...\n");
	for (auto model : Ring::models)
	{
		model->deleteMe(); //delete opengl ids
		delete model;
		Global::countDelete++;
	}

	Ring::models.clear();
}