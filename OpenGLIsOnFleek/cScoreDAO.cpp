#include "cScoreDAO.h"
#include <cstdio>

cScoreDAO::cScoreDAO() :
    m_db(NULL)
{
}

cScoreDAO::~cScoreDAO()
{
	if (m_db)
	{
		delete m_db;
	}
}

bool cScoreDAO::init(std::string dbFileName) 
{
	// error value essentially
	int rc;

	// error message output
	char* errorMessage = 0;

	// open the database
	rc = sqlite3_open(dbFileName.c_str(), &m_db);

	// check for errors
	if (rc)
	{
		printf("Database failed to open: %s\n", sqlite3_errmsg(m_db));
		sqlite3_close(m_db);
		return false;
	}

	printf("Score DB opened successfully\n");

	// ensure highScores table exists, create table if it does not
	std::string checkTableQuery("SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'highScore';");

	char** queryTableResult = 0;

	int rows, columns;

	rc = sqlite3_get_table(m_db, checkTableQuery.c_str(), &queryTableResult, &rows, &columns, &errorMessage);

	if (rc != SQLITE_OK)
	{
		printf("Failed to check table existance. SQL Error: %s\n", errorMessage);
		sqlite3_free(errorMessage);
		return false;
	}

	// add one to rows because the names of the columns are first
	int resultSize = (rows + 1) * columns;
	
	// can free this immediately, really only need to check the result size here
	sqlite3_free_table(queryTableResult);


	if (resultSize < 2) // table has not been created yet (idk why or how this would be more than 2, but just in case)
	{
		// create the table
		std::string createTableQuery("CREATE TABLE highScore (id INTEGER PRIMARY KEY, score INTEGER NOT NULL);");

		rc = sqlite3_get_table(m_db, createTableQuery.c_str(), &queryTableResult, &rows, &columns, &errorMessage);

		if (rc != SQLITE_OK)
		{
			printf("Failed to create highScore table. SQL Error: %s\n", errorMessage);
			sqlite3_free(errorMessage);
			return false;
		}
	}

	sqlite3_free(errorMessage);

	return true;
}

void cScoreDAO::shutdown()
{
	if (m_db)
	{
		sqlite3_close(m_db);
	}
}

void cScoreDAO::setScore(int playerID, int playerScore)
{
	// error value essentially
	int rc;

	// error message output
	char* errorMessage = 0;


	// first check if playerID exists
	char checkIdQuery[128];
	sprintf_s(checkIdQuery, sizeof(checkIdQuery), "SELECT * FROM highScore WHERE id = %d;", playerID);

	char** queryTableResult = 0;

	int rows, columns;

	rc = sqlite3_get_table(m_db, checkIdQuery, &queryTableResult, &rows, &columns, &errorMessage);

	if (rc != SQLITE_OK)
	{
		printf("Failed to check player id existance. SQL Error: %s\n", errorMessage);
		sqlite3_free(errorMessage);
		return;
	}
	sqlite3_free_table(queryTableResult);

	sqlite3_free(errorMessage);

	// add one to rows because the names of the columns are first
	int resultSize = (rows + 1) * columns;

	if (resultSize != 0)	// this player already exists in the table
	{
		char updateScoreQuery[128];
		sprintf_s(updateScoreQuery, sizeof(updateScoreQuery), "UPDATE highScore SET score = %d WHERE id = %d;", playerScore, playerID);
		
		rc = sqlite3_get_table(m_db, updateScoreQuery, &queryTableResult, &rows, &columns, &errorMessage);

		if (rc != SQLITE_OK)
		{
			printf("Failed to update player score. SQL Error: %s\n", errorMessage);
			sqlite3_free(errorMessage);
			return;
		}


		sqlite3_free_table(queryTableResult);

		sqlite3_free(errorMessage);
	}
	else 
	{
		char insertScoreQuery[128];
		sprintf_s(insertScoreQuery, sizeof(insertScoreQuery), "INSERT INTO highScore (id, score) VALUES (%d, %d);", playerID, playerScore);

		rc = sqlite3_get_table(m_db, insertScoreQuery, &queryTableResult, &rows, &columns, &errorMessage);

		if (rc != SQLITE_OK)
		{
			printf("Failed to insert player score. SQL Error: %s\n", errorMessage);
			sqlite3_free(errorMessage);
			return;
		}


		sqlite3_free_table(queryTableResult);

		sqlite3_free(errorMessage);
	}

	return;
}

int cScoreDAO::getScore(int playerID)
{
	// error value essentially
	int rc;

	// error message output
	char* errorMessage = 0;


	// first check if playerID exists
	char checkIdQuery[128];
	sprintf_s(checkIdQuery, sizeof(checkIdQuery), "SELECT * FROM highScore WHERE id = %d;", playerID);

	char** queryTableResult = 0;

	int rows, columns;

	rc = sqlite3_get_table(m_db, checkIdQuery, &queryTableResult, &rows, &columns, &errorMessage);

	if (rc != SQLITE_OK)
	{
		printf("Failed to check player id existance. SQL Error: %s\n", errorMessage);
		sqlite3_free(errorMessage);
		return -1;
	}

	// add one to rows because the names of the columns are first
	int resultSize = (rows + 1) * columns;	

	if (resultSize == 0)
	{
		// played does not exist
		return -1;
	}

	int score = atoi(queryTableResult[3]);

	sqlite3_free_table(queryTableResult);

	sqlite3_free(errorMessage);

    return score;
}
