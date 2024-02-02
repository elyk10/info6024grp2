#include <string>
#include <fstream>>
#include "cMesh.h"
#include <vector>

#pragma once
class cLayoutLoader
{
public:
	cLayoutLoader();
	~cLayoutLoader();

	bool loadSourceFile(std::string fileToLoadFrom);
	void loadLayout();
	void setBasepath(std::string basepath);
	std::string getBasepath();

	std::vector<std::string> filesToLoad;
	std::vector<glm::vec3> modelPositions;
	std::vector<glm::vec3> modelRotation;
	std::vector<float> modelScale;

private:
	std::fstream fileLoaded;
	std::string m_basepath;


};

