#pragma once

// All the global things that EVERYTHING might need.
// Before you add something here, ask yourself: "is it REALLY global??"

#include "cDebugRender.h"

extern cDebugRenderer* g_pDebugRenderer;

extern bool g_drawDebugLightSpheres;

void g_DrawDebugSphere(glm::vec3 position, float scale, glm::vec4 colourRGBA);



