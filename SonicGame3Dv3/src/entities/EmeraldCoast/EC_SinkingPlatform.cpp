#include <glad/glad.h>

#include "../entities.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "ecsinkingplatform.h"
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

std::list<TexturedModel*> EC_SinkingPlatform::models;
CollisionModel* EC_SinkingPlatform::cmOriginal;

EC_SinkingPlatform::EC_SinkingPlatform()
{
	
}

EC_SinkingPlatform::EC_SinkingPlatform(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	surfaceHeight = y;
	position.z = z;
	rotX = 0;
	rotY = 0;
	rotZ = 0;
	scale = 1;
	visible = false;
	updateTransformationMatrix();

	collideModelOriginal = EC_SinkingPlatform::cmOriginal;
	collideModelTransformed = loadCollisionModel("Models/EmeraldCoast/", "SinkingPlatform");

	CollisionChecker::addCollideModel(collideModelTransformed);

	updateCollisionModel();
}

void EC_SinkingPlatform::step()
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

			if (collideModelTransformed->playerIsOn)
			{
				if (getY() > surfaceHeight - 20.0f)
				{
					increasePosition(0, -0.25f, 0);
				}
			}
			else if (getY() < surfaceHeight)
			{
				increasePosition(0, 0.35f, 0);
				if (getY() > surfaceHeight)
				{
					setY(surfaceHeight);
				}
			}

			updateCMJustPosition();
			updateTransformationMatrix();
		}
	}
}

std::list<TexturedModel*>* EC_SinkingPlatform::getModels()
{
	return &EC_SinkingPlatform::models;
}

void EC_SinkingPlatform::loadStaticModels()
{
	if (EC_SinkingPlatform::models.size() > 0)
	{
		return;
	}

	std::fprintf(stdout, "Loading EC_SinkingPlatform static models...\n");

	std::list<TexturedModel*>* newModels = loadObjModel("res/Models/EmeraldCoast/", "SinkingPlatform.obj");
	for (auto newModel : (*newModels))
	{
		EC_SinkingPlatform::models.push_back(newModel);
	}
	delete newModels;
	Global::countDelete++;


	if (EC_SinkingPlatform::cmOriginal == nullptr)
	{
		EC_SinkingPlatform::cmOriginal = loadCollisionModel("Models/EmeraldCoast/", "SinkingPlatform");
	}
}

void EC_SinkingPlatform::deleteStaticModels()
{
	std::fprintf(stdout, "Deleting EC_SinkingPlatform static models...\n");
	for (auto model : EC_SinkingPlatform::models)
	{
		model->deleteMe();
		delete model;
		Global::countDelete++;
	}

	EC_SinkingPlatform::models.clear();

	if (EC_SinkingPlatform::cmOriginal != nullptr)
	{
		EC_SinkingPlatform::cmOriginal->deleteMe();
		delete EC_SinkingPlatform::cmOriginal;
		Global::countDelete++;
		EC_SinkingPlatform::cmOriginal = nullptr;
	}
}