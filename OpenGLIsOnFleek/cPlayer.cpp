#include "cPlayer.h"
#include "cScoreSystem.h"

extern cScoreSystem* g_ScoreSystem;

cPlayer::cPlayer() : 
	spawnTime(glfwGetTime()),
	playerID(0)
{

}

cPlayer::cPlayer(sPhsyicsProperties* thePlayer)
{
	this->thePhysics = thePlayer;
}

cPlayer::~cPlayer()
{

}

void cPlayer::respawn(int health, glm::vec3 position)
{
	thePhysics->position = position;
	thePhysics->health = health;

	g_ScoreSystem->updateScore(this);

	// reset spawn time
	spawnTime = glfwGetTime();
}
