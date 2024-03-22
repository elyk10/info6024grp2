#pragma once

// sqlite things
#include "sqlite/sqlite3.h"


#include <string>
class cScoreDAO
{
public:

	cScoreDAO();

	~cScoreDAO();

	bool init(std::string dbFileName);

	void shutdown();

	void setScore(int playerID, int playerScore);

	int getScore(int playerID);

private:
	sqlite3* m_db;
};

