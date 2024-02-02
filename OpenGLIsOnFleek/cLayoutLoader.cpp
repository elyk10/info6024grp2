#include "cLayoutLoader.h"

cLayoutLoader::cLayoutLoader() {}

cLayoutLoader::~cLayoutLoader() { fileLoaded.close(); }

bool cLayoutLoader::loadSourceFile(std::string fileToLoadFrom)
{
	std::string fullFileName = this->m_basepath + "/" + fileToLoadFrom;

	fileLoaded.open(fullFileName);
	if (!fileLoaded.is_open())
		return false;
	else
		return true;
}

void cLayoutLoader::loadLayout()
{
	std::string tempString, inputLine;
	glm::vec3 tempVec;
	float tempFloat;
	int count = 0;
	while (std::getline(fileLoaded, inputLine))
	{
		for (int i = 0; i < inputLine.length(); i++)
		{
			if (inputLine[i] != ',')
			{
				tempString += inputLine[i];
			}
			else
			{
				if (count == 0)
				{
					filesToLoad.push_back(tempString);
					tempString.clear();
					count++;
				}
				else if (count == 1)
				{
					tempVec.x = std::stof(tempString);
					tempString.clear();
					count++;
				}
				else if (count == 2)
				{
					tempVec.y = std::stof(tempString);
					tempString.clear();
					count++;
				}
				else if (count == 3)
				{
					tempVec.z = std::stof(tempString);
					tempString.clear();
					modelPositions.push_back(tempVec);
					count++;
				}
				else if (count == 4)
				{
					tempVec.x = std::stof(tempString);
					tempString.clear();
					count++;
				}
				else if (count == 5)
				{
					tempVec.y = std::stof(tempString);
					tempString.clear();
					count++;
				}
				else if (count == 6)
				{
					tempVec.z = std::stof(tempString);
					tempString.clear();
					modelRotation.push_back(tempVec);
					//count = 0;
				}

			}
		}
		if (!tempString.empty())
		{
			tempFloat = std::stof(tempString);
			tempString.clear();
			modelScale.push_back(tempFloat);
			count = 0;


		}
	}

	fileLoaded.close();
}

void cLayoutLoader::setBasepath(std::string basepath)
{
	this->m_basepath = basepath;
}

std::string cLayoutLoader::getBasepath()
{
	return this->m_basepath;
}
