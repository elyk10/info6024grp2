#pragma once

#include "cVAOManager/cVAOManager.h"	// For loading cubes, spheres, etc.

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include <string>
#include <vector>

class cDebugRenderer
{
public:
	cDebugRenderer();

	bool Initialize(void);
	void ShutDown(void);

	// Only being called by the main render loop
	// It's NOT being called by the objects that add drawing items
	void RenderDebugObjects(double deltaTime,
							glm::mat4 matView, glm::mat4 matProjection);

// ***************************************************************************
// Objects that want to add items call these methods
	void AddLine(glm::vec3 startXYZ, glm::vec3 endXYZ, glm::vec4 RGBA);

	// Later
	void AddSphere(glm::vec3 location, float radius, glm::vec4 RGBA);
	void AddCube(glm::vec3 centreLocation, float size, glm::vec4 RGBA);
// ***************************************************************************

	std::string getLastError(void);

private:
	std::string m_lastError;

	// Here is the array that will hold the line vertices
	struct sLineVertex_Buffer
	{
		float x, y, z, w;
		float r, g, b, a;
	};

	// Default is maximum of 10,000 lines
	const unsigned int LINE_VERTEX_BUFFER_SIZE = 20'000;
	unsigned int m_sizeOfLineVertexBuffer;
	sLineVertex_Buffer* m_pLinesVerticesToDraw;

	// The list of items to draw 
	struct sSphere
	{
		glm::vec3 centreXYZ;	float radius;	glm::vec4 RGBA;
	};

	struct sCube
	{
		glm::vec3 centreXYZ;	float size;		glm::vec4 RGBA;
	};

	struct sArrow
	{
		glm::vec3 centreXYZ;	glm::vec3 direction;	glm::vec4 RGBA;
	};

	std::vector<sSphere>	m_vecSpheresToDraw;
	std::vector<sCube>		m_vecCubesToDraw;
	std::vector<sArrow>		m_vecArrowsToDraw;

};
