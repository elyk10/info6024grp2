#pragma once
#include <string>
#include "glm/vec3.hpp"
#include "OpenGLCommon.h"

// rapidjson
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"


// load player and scene information from JSON file

class cJSONLoader
{
public:

	cJSONLoader(std::string basePath, GLuint shaderProgramID);

	~cJSONLoader();


	bool loadPlayer(std::string fileName);

	bool loadScene(std::string fileName);

	void setShaderProgramID(GLuint id);

	void setBasePath(std::string basepath);

private:

	std::string m_BasePath;
	
	GLuint m_ShaderProgramID;


	// helper functions
	bool getStringJSON(const rapidjson::Value& data, std::string name, std::string& outputString);

	bool getFloatJSON(const rapidjson::Value& data, std::string name, float& outputFloat);

	bool getGLMVec3JSON(const rapidjson::Value& jsonVector3, std::string name, glm::vec3& outputVec);

	
};

