#include "cJSONLoader.h"
#include <iostream>
#include <fstream>

// extern
#include "cPlayer.h"
#include "cMesh.h"
#include "cVAOManager/cVAOManager.h"
#include "TextureManager/cBasicTextureManager.h"

extern cPlayer* thePlayer;
extern std::vector< cMesh* > g_vec_pMeshesToDraw;
extern cVAOManager* g_pMeshManager;
extern cBasicTextureManager* g_pTextureManager;


cJSONLoader::cJSONLoader(std::string basePath, GLuint shaderProgramID):
	m_BasePath(basePath),
	m_ShaderProgramID(shaderProgramID){}


cJSONLoader::~cJSONLoader(){}


bool cJSONLoader::loadPlayer(std::string fileName)
{
	using namespace rapidjson;

	errno_t err;
	FILE* fp;

	std::string filenameAndPath = m_BasePath + fileName;


	// Open file for reading, exit if unsuccessful
	err = fopen_s(&fp, filenameAndPath.c_str(), "r");

	if (err != 0)
	{
		std::cout << "Failed to open file:" << fileName << std::endl;
		return false;
	}

	// create read buffer for file
	// allocating it on the heap, because VS gives a warning about it being too large for the stack -- it's probably fine either way though
	size_t bufferSize = 65536;
	char* readbuffer = new char[bufferSize];


	// load the file into stream and parse the json
	rapidjson::FileReadStream is(fp, readbuffer, bufferSize);
	Document playerConfigFileObj;
	playerConfigFileObj.ParseStream(is);


	delete[] readbuffer;

	// ensure parse was successful
	if (playerConfigFileObj.HasParseError())
	{
		std::cout << "JSON parsing error" << std::endl;
		rapidjson::ParseErrorCode code = playerConfigFileObj.GetParseError();
		return false;
	}

	// close the file
	fclose(fp);

	// load the player information

	cMesh* playerMesh = new cMesh();


	if (playerConfigFileObj.HasMember("player") && playerConfigFileObj["player"].IsObject())
	{
		const Value& playerData = playerConfigFileObj["player"];

		// file name
		if (!getStringJSON(playerData, "meshFile", playerMesh->meshName)) 
		{
			std::cout << "Scene Loader: Failed to get mesh file name" << std::endl;
		}

		// friendly name
		if (!getStringJSON(playerData, "friendlyName", playerMesh->friendlyName))
		{
			std::cout << "Scene Loader: Failed to get friendly name" << std::endl;
		}

		// position
		if (!getGLMVec3JSON(playerData, "position", playerMesh->drawPosition))
		{
			std::cout << "Scene Loader: Failed to get player position" << std::endl;
		}

		// rotation
		glm::vec3 eulerRotation;
		if (!getGLMVec3JSON(playerData, "rotation", eulerRotation))
		{
			std::cout << "Scene Loader: Failed to get player rotation" << std::endl;
		}

		// scale
		playerMesh->adjustRoationAngleFromEuler(eulerRotation);

		if (!getGLMVec3JSON(playerData, "scale", playerMesh->drawScale))
		{
			std::cout << "Scene Loader: Failed to get player scale" << std::endl;
		}

		// movement speed

		if (!getFloatJSON(playerData, "movementSpeed", thePlayer->speed))
		{
			std::cout << "Scene Loader: Failed to get player speed" << std::endl;
		}

		// texture info

		if (playerData.HasMember("textureNames") && playerData["textureNames"].IsArray()
			&& playerData.HasMember("textureRatios") && playerData["textureRatios"].IsArray())
		{
			const Value& textureNamesArray = playerData["textureNames"];
			const Value& textureRatiosArray = playerData["textureRatios"];

			assert(textureNamesArray.Size() == textureRatiosArray.Size());


			for (int i = 0; i < textureNamesArray.Size(); i++)
			{
				assert(textureNamesArray[i].IsString());
				assert(textureRatiosArray[i].IsFloat());

				playerMesh->textureName[i] = textureNamesArray[i].GetString();
				playerMesh->textureRatios[i] = textureRatiosArray[i].GetFloat();
			}
		}

		g_vec_pMeshesToDraw.push_back(playerMesh);
		thePlayer->theMesh = playerMesh;
		thePlayer->moveDir = glm::vec3(0.f);
	}
	else
	{
		std::cout << "Failed to load player config -- object not found" << std::endl;
	}

	// load model into VAO
	sModelDrawInfo temp;

	if (!g_pMeshManager->LoadModelIntoVAO(playerMesh->meshName, temp, m_ShaderProgramID))
	{
		std::cout << "Scene Loader: failed to load player model into VAO" << std::endl;
	}
	std::string playerAnimationMesh = "playerModels/personPosed";
	for (int i = 1; i < 31; i++) {
		if (!g_pMeshManager->LoadModelIntoVAO(playerAnimationMesh + std::to_string(i) + ".ply", temp, m_ShaderProgramID))
		{
			std::cout << "Scene Loader: failed to load player model into VAO" << std::endl;
		}
	}
	// load texture(s)
	for (int i = 0; i < playerMesh->NUM_TEXTURES; i++)
	{
		if (playerMesh->textureName[i] != "")
		{
			if (!g_pTextureManager->Create2DTextureFromBMPFile(playerMesh->textureName[i], true))
			{
				std::cout << "Scene Loader: error creating texture: " << playerMesh->textureName[i] << std::endl;
			}
		}
	}

	return true;
}

bool cJSONLoader::loadScene(std::string fileName)
{
	// TODO
	return false;
}

void cJSONLoader::setShaderProgramID(GLuint id)
{
	m_ShaderProgramID = id;
}

void cJSONLoader::setBasePath(std::string basepath)
{
	m_BasePath = basepath;
}


bool cJSONLoader::getStringJSON(const rapidjson::Value& data, std::string name, std::string& outputString)
{
	if (data.HasMember(name.c_str()) && data[name.c_str()].IsString())
	{
		outputString = data[name.c_str()].GetString();
		return true;
	}

	return false;

}

bool cJSONLoader::getFloatJSON(const rapidjson::Value& data, std::string name, float& outputFloat)
{
	if (data.HasMember(name.c_str()) && data[name.c_str()].IsFloat())
	{
		outputFloat = data[name.c_str()].GetFloat();
		return true;
	}

	return false;
}


bool cJSONLoader::getGLMVec3JSON(const rapidjson::Value& data, std::string name, glm::vec3& outputVec)
{
	using namespace rapidjson;

	if (!(data.HasMember(name.c_str()) && data[name.c_str()].IsObject())) 
	{
		return false;
	}

	const Value& jsonVector3 = data[name.c_str()];

	if (!jsonVector3.HasMember("x") or !jsonVector3.HasMember("y") or !jsonVector3.HasMember("z"))
	{
		return false;
	}

	outputVec.x = jsonVector3["x"].GetFloat();
	outputVec.y = jsonVector3["y"].GetFloat();
	outputVec.z = jsonVector3["z"].GetFloat();


	return true;
}

