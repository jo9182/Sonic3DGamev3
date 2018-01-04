#include <glad/glad.h>

#include "../entities.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "echut.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../engineTester/main.h"
#include "../../entities/player.h"
#include "../../toolbox/maths.h"
#include "../camera.h"
#include "../../collision/collisionmodel.h"
#include "../../collision/collisionchecker.h"

#include <list>
#include <iostream>
#include <algorithm>

std::list<TexturedModel*> EC_Hut::models;
CollisionModel* EC_Hut::cmOriginal;

EC_Hut::EC_Hut()
{
	
}

EC_Hut::EC_Hut(float x, float y, float z, float rotY)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	this->rotX = 0;
	this->rotY = rotY;
	this->rotZ = 0;
	this->scale = 1;
	this->visible = true;
	updateTransformationMatrix();

	collideModelOriginal = EC_Hut::cmOriginal;
	collideModelTransformed = loadCollisionModel("Models/EmeraldCoast/", "Hut");

	CollisionChecker::addCollideModel(collideModelTransformed);

	updateCollisionModelWithZ();
}

void EC_Hut::step()
{
	if (abs(getX() - Global::gameCamera->getPosition()->x) > ENTITY_RENDER_DIST)
	{
		setVisible(false);
	}
	else
	{
		if (abs(getZ() - Global::gameCamera->getPosition()->z) > ENTITY_RENDER_DIST)
		{
			setVisible(false);
		}
		else
		{
			setVisible(true);
		}
	}
}

std::list<TexturedModel*>* EC_Hut::getModels()
{
	return &EC_Hut::models;
}

void EC_Hut::loadStaticModels()
{
	if (EC_Hut::models.size() > 0)
	{
		return;
	}

	std::fprintf(stdout, "Loading EC_Hut static models...\n");

	std::list<TexturedModel*>* newModels = loadObjModel("res/Models/EmeraldCoast/", "Hut.obj");
	for (auto newModel : (*newModels))
	{
		EC_Hut::models.push_back(newModel);
	}
	delete newModels;
	Global::countDelete++;


	if (EC_Hut::cmOriginal == nullptr)
	{
		EC_Hut::cmOriginal = loadCollisionModel("Models/EmeraldCoast/", "Hut");
	}
}

void EC_Hut::deleteStaticModels()
{
	std::fprintf(stdout, "Deleting EC_Hut static models...\n");
	for (auto model : EC_Hut::models)
	{
		model->deleteMe();
		delete model;
		Global::countDelete++;
	}

	EC_Hut::models.clear();

	if (EC_Hut::cmOriginal != nullptr)
	{
		EC_Hut::cmOriginal->deleteMe();
		delete EC_Hut::cmOriginal;
		EC_Hut::cmOriginal = nullptr;
	}
}
