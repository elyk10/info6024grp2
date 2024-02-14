#pragma once

#include "cMesh.h"

class cPlayer
{
public:
	cPlayer();
	cPlayer(cMesh* playerMesh);

	~cPlayer();

	cMesh* theMesh;
	float speed;
	glm::vec3 moveDir;

	glm::quat initialOrientation;
};
