#pragma once

#include "cMesh.h"
#include "sPhsyicsProperties.h"
#include "OpenGLCommon.h"

class cPlayer
{
public:
	cPlayer();
	cPlayer(sPhsyicsProperties* thePlayer);

	~cPlayer();

	void respawn(int health, glm::vec3 position);

	sPhsyicsProperties* thePhysics;
	//cMesh* theMesh;
	float speed;
	glm::vec3 moveDir;

	glm::quat initialOrientation;


	// score tracking
	unsigned int playerID;
	double spawnTime;
};
