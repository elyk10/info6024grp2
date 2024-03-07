#pragma once

#include "cMesh.h"
#include "sPhsyicsProperties.h"

class cPlayer
{
public:
	cPlayer();
	cPlayer(sPhsyicsProperties* thePlayer);

	~cPlayer();

	sPhsyicsProperties* thePhysics;
	//cMesh* theMesh;
	float speed;
	glm::vec3 moveDir;

	glm::quat initialOrientation;
};
