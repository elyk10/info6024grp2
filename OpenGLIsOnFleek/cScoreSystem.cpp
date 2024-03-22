#include "cScoreSystem.h"
#include <iostream>


cScoreSystem::cScoreSystem()
{
	m_scoreDAO = new cScoreDAO();
	m_scoreDAO->init("highScore.dat");
}

cScoreSystem::~cScoreSystem()
{
	m_scoreDAO->shutdown();
	delete m_scoreDAO;
}

void cScoreSystem::updateScore(cPlayer* thePlayer)
{
	int score = calculateScore(thePlayer);

	// check if this is a highscore for this player

	int currHighScore = m_scoreDAO->getScore(thePlayer->playerID);

	std::cout << "[Player " << thePlayer->playerID << "] Current score: " << score << " Highscore: " << currHighScore << std::endl;

	if (currHighScore < score || currHighScore == -1)
	{
		m_scoreDAO->setScore(thePlayer->playerID, score);
	}
}

int cScoreSystem::calculateScore(cPlayer* thePlayer)
{
	// score is currently just calculated based on time survived

	double currTime = glfwGetTime();

	double timeSurvived = currTime - thePlayer->spawnTime;

	int score = static_cast<int>(timeSurvived);

	return score;
}
