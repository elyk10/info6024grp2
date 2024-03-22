#pragma once
#include "cScoreDAO.h"
#include "cPlayer.h"

class cScoreSystem
{
public:
	cScoreSystem();

	~cScoreSystem();

	void updateScore(cPlayer* thePlayer);

private:

	int calculateScore(cPlayer* thePlayer);

	cScoreDAO* m_scoreDAO;
};

