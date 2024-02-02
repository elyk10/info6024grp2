// PhysicsStuff.cpp

#include "OpenGLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <vector>

//#include "cMesh.h"
#include "sPhsyicsProperties.h"

#include <iostream>

#include "cGlobal.h"

#include <limits.h>		// Lots of limits, etc. for all the data types
#include <float.h>		// Limits for floating point

#include "cVAOManager/cVAOManager.h"
// This is defined in main
extern cVAOManager* g_pMeshManager;

//extern std::vector< cMesh* > g_vec_pMeshesToDraw;

// This has no access to the vector of meshes
extern std::vector< sPhsyicsProperties* > g_vec_pPhysicalProps;

//// Using this so that I can see the locations of the "closest point to the triangle"
//void DrawObject(cMesh* pCurrentMesh, glm::mat4 matModel,
//				glm::mat4 matProjection, glm::mat4 matView,
//				GLuint shaderProgramID);			// OpenGL common 

// reutrns NULL if we DIDN'T find the object
//cMesh* findObjectByFriendlyName(std::string friendlyNameToFind)
//{
//	for ( unsigned int index = 0; index != ::g_vec_pMeshesToDraw.size(); index++ )
//	{
//		// This it? 
//		cMesh* pCurrentMesh = ::g_vec_pMeshesToDraw[index];
//
//		if (pCurrentMesh->friendlyName == friendlyNameToFind)
//		{
//			return pCurrentMesh;
//		}
//	}
//
//	// Didn't find it
//	return NULL;
//}

glm::vec3 ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);
bool TestSphereTriangle(float sphereRadius, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2, glm::vec3 sphereCentre);

void DoPhysicUpdate(double deltaTime)
{

	//// Find the debug sphere
	//cMesh* pDebugSphere = findObjectByFriendlyName("DEBUG_SPHERE");


//	for (unsigned int meshIndex = 0; meshIndex != ::g_vec_pMeshesToDraw.size(); meshIndex++)
	for (unsigned int meshIndex = 0; meshIndex != ::g_vec_pPhysicalProps.size(); meshIndex++)
	{

		sPhsyicsProperties* pCurrentPhysObject = ::g_vec_pPhysicalProps[meshIndex];

//		if (pCurrentMesh->pPhysProps == NULL )
//		{
//			// Skip this.
//			continue;
//		}

// ***********************************************************************
		// Explicit forward Euler "integration step"

//		NewVelocity = LastVel + (Accel * DeltaTime)
//		NewPosition = LastPos + (Vel * DeltaTime)	

		// Velocity change is based on the acceleration over this time frame 
//		// This part: (Accel * DeltaTime)
		glm::vec3 deltaVelocityThisFrame = pCurrentPhysObject->acceleration * (float)deltaTime;

		// Update the velocity based on this delta velocity
		// Then this part: NewVelocity = LastVel + ...
		pCurrentPhysObject->velocity += deltaVelocityThisFrame;


		// Position change is based on the velocity over this time frame
		// This part: (Vel * DeltaTime)	
		glm::vec3 deltaPosition = pCurrentPhysObject->velocity * (float)deltaTime;

		// ...then this part: NewPosition = LastPos + ...
		// Upatate the position based on this delta position
//		pCurrentMesh->pPhysProps->position += deltaPosition;
		pCurrentPhysObject->transInfo.position.x += deltaPosition.x;
		pCurrentPhysObject->transInfo.position.y += deltaPosition.y;
		pCurrentPhysObject->transInfo.position.z += deltaPosition.z;
// ***********************************************************************


// ***********************************************************************
	// HACK:Collision detection and response

	// Our ground object is at -10.0f

	// See if our sphere "hits" the ground
	// For spheres, if the ground <= radius of the sphere, it's "hit"
	// Our model has radius of 1.0f


//    ____        _                         ____  _                  
//   / ___| _ __ | |__   ___ _ __ ___      |  _ \| | __ _ _ __   ___ 
//   \___ \| '_ \| '_ \ / _ \ '__/ _ \_____| |_) | |/ _` | '_ \ / _ \
//    ___) | |_) | | | |  __/ | |  __/_____|  __/| | (_| | | | |  __/
//   |____/| .__/|_| |_|\___|_|  \___|     |_|   |_|\__,_|_| |_|\___|
//         |_|                                                       
//
// 		const float GROUND_LOCATION_Y = -10.0f;
//		const float GROUND_LOCATION_Y = 0.0f;
//
//		// Is this a sphere? 
//		if (pCurrentMesh->friendlyName == "Sphere")
//		{
//			if ( (pCurrentMesh->pPhysProps->position.y - 1.0f) <= GROUND_LOCATION_Y)
//			{
//				// "Invert" the velocity
//				// Velocity goes "up" +ve.
//				float newVel = fabs(pCurrentMesh->pPhysProps->velocity.y);
//
//				pCurrentMesh->pPhysProps->velocity.y = newVel;
//
//			}
//		}

		// Update the draw location with the physics location
//		pCurrentMesh->drawPosition = pCurrentMesh->pPhysProps->position;

		// Copy the "physics objects" info to to the mesh
//		pCurrentPhysObject->pTheAssociatedMesh->transInfo = pCurrentPhysObject->transInfo;

		pCurrentPhysObject->pTheAssociatedMesh->setTransformInfo(pCurrentPhysObject->transInfo);

		// Can't do this anymore:
//		pCurrentPhysObject->pTheAssociatedMesh->meshName = "Bunny.ply"


		// Is this a sphere? 
		if (pCurrentMesh->friendlyName == "Sphere")
		{
			// Find the ground object
			cMesh* pTheGround = findObjectByFriendlyName("Ground");

			float sphereRadius = 1.0f;		

			// Get all the triangle from this "ground" model
			sModelDrawInfo groundMeshInfo;
			glm::vec3 closestPointToTriangle = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
			float closestDistanceSoFar = FLT_MAX;
			glm::vec3 closestTriangleVertices[3] = { glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f) };
			unsigned int indexOfClosestTriangle = INT_MAX;


			//    _____ _           _        _                     _     _        _                   _      
			//   |  ___(_)_ __   __| |   ___| | ___  ___  ___  ___| |_  | |_ _ __(_) __ _ _ __   __ _| | ___ 
			//   | |_  | | '_ \ / _` |  / __| |/ _ \/ __|/ _ \/ __| __| | __| '__| |/ _` | '_ \ / _` | |/ _ \
			//   |  _| | | | | | (_| | | (__| | (_) \__ \  __/\__ \ |_  | |_| |  | | (_| | | | | (_| | |  __/
			//   |_|   |_|_| |_|\__,_|  \___|_|\___/|___/\___||___/\__|  \__|_|  |_|\__,_|_| |_|\__, |_|\___|
			//                                                                                  |___/        
			if (::g_pMeshManager->FindDrawInfoByModelName(pTheGround->meshName, groundMeshInfo))
			{
				// Which triangle is closest to this sphere (right now)
				for ( unsigned int index = 0; index != groundMeshInfo.numberOfIndices; index += 3 )
				{	
					glm::vec3 verts[3];

					verts[0].x = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index] ].x;
					verts[0].y = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index] ].y;
					verts[0].z = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index] ].z;

					verts[1].x = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 1] ].x;
					verts[1].y = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 1] ].y;
					verts[1].z = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 1] ].z;

					verts[2].x = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 2] ].x;
					verts[2].y = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 2] ].y;
					verts[2].z = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 2] ].z;

					// Transform this object in world space using the same technique we did to render it
					// 
					// Here's the key line: 	
					// 
					//		vertexWorldPos = matModel * vec4( vPos.xyz, 1.0f);
					// 
					// THIS BLOCK OF CODE IS FROM DrawObject()

					glm::mat4 matModel = glm::mat4(1.0f);

					// Translation
					glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
															glm::vec3(pTheGround->drawPosition.x,
																	  pTheGround->drawPosition.y,
																	  pTheGround->drawPosition.z));

					   // Rotation matrix generation
					glm::mat4 matRotateX = glm::rotate(glm::mat4(1.0f),
													   pTheGround->orientation.x, // (float)glfwGetTime(),
													   glm::vec3(1.0f, 0.0, 0.0f));


					glm::mat4 matRotateY = glm::rotate(glm::mat4(1.0f),
													   pTheGround->orientation.y, // (float)glfwGetTime(),
													   glm::vec3(0.0f, 1.0, 0.0f));

					glm::mat4 matRotateZ = glm::rotate(glm::mat4(1.0f),
													   pTheGround->orientation.z, // (float)glfwGetTime(),
													   glm::vec3(0.0f, 0.0, 1.0f));

					   // Scaling matrix
					glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
													glm::vec3(pTheGround->scale,
															  pTheGround->scale,
															  pTheGround->scale));
					//--------------------------------------------------------------

					// Combine all these transformation
					matModel = matModel * matTranslate;

					matModel = matModel * matRotateX;
					matModel = matModel * matRotateY;
					matModel = matModel * matRotateZ;

					matModel = matModel * matScale;


					// vertexWorldPos = matModel * vec4(vPos.xyz, 1.0f);

					glm::vec4 vertsWorld[3];
					vertsWorld[0] = (matModel * glm::vec4(verts[0], 1.0f));
					vertsWorld[1] = (matModel * glm::vec4(verts[1], 1.0f));
					vertsWorld[2] = (matModel * glm::vec4(verts[2], 1.0f));

					// And make sure you multiply the normal by the inverse transpose
					// OR recalculate it right here! 

					// ******************************************************

					glm::vec3 thisTriangleClosestPoint = ClosestPtPointTriangle(pCurrentMesh->drawPosition,
																				vertsWorld[0], vertsWorld[1], vertsWorld[2]);

					// Is this the closest so far
					float distanceToThisTriangle = glm::distance(thisTriangleClosestPoint, pCurrentMesh->drawPosition);

					if (distanceToThisTriangle < closestDistanceSoFar)
					{
						// this one is closer
						closestDistanceSoFar = distanceToThisTriangle;
						// Make note of the triangle index
						indexOfClosestTriangle = index;
						// 
						closestTriangleVertices[0] = vertsWorld[0];
						closestTriangleVertices[1] = vertsWorld[1];
						closestTriangleVertices[2] = vertsWorld[2];
					}


				} //for ( unsigned int index...

				//    ____  _     _                  _     _ _     _        _                   _     ___ 
				//   |  _ \(_) __| | __      _____  | |__ (_) |_  | |_ _ __(_) __ _ _ __   __ _| | __|__ \
				//   | | | | |/ _` | \ \ /\ / / _ \ | '_ \| | __| | __| '__| |/ _` | '_ \ / _` | |/ _ \/ /
				//   | |_| | | (_| |  \ V  V /  __/ | | | | | |_  | |_| |  | | (_| | | | | (_| | |  __/_| 
				//   |____/|_|\__,_|   \_/\_/ \___| |_| |_|_|\__|  \__|_|  |_|\__,_|_| |_|\__, |_|\___(_) 
				//                                                                        |___/           
//				if (TestSphereTriangle(sphereRadius, 
//									   closestTriangleVertices[0], 
//									   closestTriangleVertices[1], 
//									   closestTriangleVertices[2], 
//									   pCurrentMesh->drawPosition) )
				// Compare the radius

				if (closestDistanceSoFar < sphereRadius )
				{
					// Hit it!
					// Take the normal of that triangle and bounce the sphere along it

					// How do we calculate the new direction after the "bounce"? 
					// 
					// Answer: it's based on the REFLECTION vector around the normal.
					// The sphere is travelling along its VELOCITY vector (at this moment)
					//	and will "bounce off" along an angle reflected by the normal

					// Calculate the current "direction" vector 
					// We're using the velocity
					glm::vec3 sphereDirection = pCurrentMesh->pPhysProps->velocity;
					// Normalize... 
					sphereDirection = glm::normalize(sphereDirection);

					// Calcualte the current normal from the TRANSFORMED vertices
					glm::vec3 edgeA = closestTriangleVertices[1] - closestTriangleVertices[0];
					glm::vec3 edgeB = closestTriangleVertices[2] - closestTriangleVertices[0];

					glm::vec3 triNormal = glm::normalize(glm::cross(edgeA, edgeB));

					// Calculate the reflection vector from the normal	
					// https://registry.khronos.org/OpenGL-Refpages/gl4/html/reflect.xhtml
					// 1st parameter is the "incident" vector
					// 2nd parameter is the "normal" vector
					glm::vec3 reflectionVec = glm::reflect(sphereDirection, triNormal);

					// Update the  velocity based on this reflection vector
					float sphereSpeed = glm::length(pCurrentMesh->pPhysProps->velocity);
					glm::vec3 newVelocity = reflectionVec * sphereSpeed;

					pCurrentMesh->pPhysProps->velocity = newVelocity;
				}

			}//if (::g_pMeshManager->FindDrawInfoByModelName..
			


//			if ((pCurrentMesh->pPhysProps->position.y - sphereRadius) <= pTheGround->drawPosition.y)
//			{
//				// "Invert" the velocity
//				// Velocity goes "up" +ve.
//				float newVel = fabs(pCurrentMesh->pPhysProps->velocity.y);
//
//				pCurrentMesh->pPhysProps->velocity.y = newVel;
//
//			}
		}//if (pCurrentMesh->friendlyName == "Sphere")





// ***********************************************************************
//
		// HACK: Determine what is the closest position to each triangle in 
		//	the ground mesh
//
//		sModelDrawInfo groundMeshInfo;
//		if (::g_pMeshManager->FindDrawInfoByModelName("Flat_Grid_100x100.ply", groundMeshInfo))
//		{
//			// For each triangle, get the vertices, then pass them to the 
//			//	sphere-triangle collision function, returning the "closest point"
//			for ( unsigned int index = 0; index != groundMeshInfo.numberOfIndices; index += 3 )
//			{	
//				glm::vec3 verts[3];
//
//				verts[0].x = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index] ].x;
//				verts[0].y = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index] ].y;
//				verts[0].z = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index] ].z;
//
//				verts[1].x = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 1] ].x;
//				verts[1].y = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 1] ].y;
//				verts[1].z = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 1] ].z;
//
//				verts[2].x = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 2] ].x;
//				verts[2].y = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 2] ].y;
//				verts[2].z = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 2] ].z;
//
//				glm::vec3 closestPoint = ClosestPtPointTriangle(pCurrentMesh->drawPosition,
//																verts[0], verts[1], verts[2]);
//
//
////				::g_pDebugRenderer->AddLine(
//
////				// HACK:
////				if (index == 99 )
////				{
////					std::cout << closestPoint.x << ", " << closestPoint.y << ", " << closestPoint.z << std::endl;
////				}
//				//if (pDebugSphere != NULL)
////				if (pDebugSphere)			// NON zero => true, zero = false;
////				{
////					pDebugSphere->drawPosition = closestPoint;
////					pDebugSphere->scale = 0.5f;
////					pDebugSphere->bIsVisible = true;
////
////// HACK: 
////					extern glm::mat4 matProjection;    // "projection"
////					extern glm::mat4 matView;          // "view" or "camera"
////					extern GLuint shaderProgramID;
////
////					DrawObject(pDebugSphere, glm::mat4(1.0f), 
////							   matProjection, matView, shaderProgramID);
////
////					pDebugSphere->bIsVisible = false;
////				}
////
//			}//for ( unsigned int index = 0...
//		}//if (pMeshManager->FindDrawInfoByModelName(
//
//	}// for (unsigned int meshIndex

	}// For loop

	return;
}


//== = Section 5.2.7: ============================================================ =
//
// Returns true if sphere s intersects triangle ABC, false otherwise.
bool TestSphereTriangle(float sphereRadius, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2, 
					   glm::vec3 sphereCentre)
{
	// Find point P on triangle ABC closest to sphere center
	glm::vec3 closestPoint = ClosestPtPointTriangle(sphereCentre, vert0, vert1, vert2);

	// Sphere and triangle intersect if the (squared) distance from sphere
	// center to point p is less than the (squared) sphere radius
	glm::vec3 v = closestPoint - sphereCentre;

	bool isItIntersecting = false;

	if ( glm::dot(v, v) <= (sphereRadius * sphereRadius) )
	{
		isItIntersecting = true;
	}

	return isItIntersecting;
}


// From: Real-Time Collision Detection- Ericson, Christer- 9781558607323- Books - Amazon.ca
// https://www.amazon.ca/Real-Time-Collision-Detection-Christer-Ericson/dp/1558607323/ref=pd_lpo_sccl_1/137-6663593-0701065?pd_rd_w=YiI8A&content-id=amzn1.sym.687e7c56-2b08-4044-894f-bbad969cf967&pf_rd_p=687e7c56-2b08-4044-894f-bbad969cf967&pf_rd_r=JWS56NJC99QEH56TYFJX&pd_rd_wg=zBE6V&pd_rd_r=d611733e-ec29-4b30-bd70-89f092f1991a&pd_rd_i=1558607323&psc=1
// Chapter 5:

glm::vec3 ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 bc = c - b;

	// Compute parametric position s for projection P' of P on AB,
	// P' = A + s*AB, s = snom/(snom+sdenom)
	float snom = glm::dot(p - a, ab), sdenom = glm::dot(p - b, a - b);

	// Compute parametric position t for projection P' of P on AC,
	// P' = A + t*AC, s = tnom/(tnom+tdenom)
	float tnom = glm::dot(p - a, ac), tdenom = glm::dot(p - c, a - c);

	if (snom <= 0.0f && tnom <= 0.0f) return a; // Vertex region early out

	// Compute parametric position u for projection P' of P on BC,
	// P' = B + u*BC, u = unom/(unom+udenom)
	float unom = glm::dot(p - b, bc), udenom = glm::dot(p - c, b - c);

	if (sdenom <= 0.0f && unom <= 0.0f) return b; // Vertex region early out
	if (tdenom <= 0.0f && udenom <= 0.0f) return c; // Vertex region early out


	// P is outside (or on) AB if the triple scalar product [N PA PB] <= 0
	glm::vec3 n = glm::cross(b - a, c - a);
	float vc = glm::dot(n, glm::cross(a - p, b - p));
	// If P outside AB and within feature region of AB,
	// return projection of P onto AB
	if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
		return a + snom / (snom + sdenom) * ab;

	// P is outside (or on) BC if the triple scalar product [N PB PC] <= 0
	float va = glm::dot(n, glm::cross(b - p, c - p));
	// If P outside BC and within feature region of BC,
	// return projection of P onto BC
	if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
		return b + unom / (unom + udenom) * bc;

	// P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
	float vb = glm::dot(n, glm::cross(c - p, a - p));
	// If P outside CA and within feature region of CA,
	// return projection of P onto CA
	if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
		return a + tnom / (tnom + tdenom) * ac;

	// P must project inside face region. Compute Q using barycentric coordinates
	float u = va / (va + vb + vc);
	float v = vb / (va + vb + vc);
	float w = 1.0f - u - v; // = vc / (va + vb + vc)
	return u * a + v * b + w * c;
}

// The OG code from the book:
//Point ClosestPtPointTriangle(Point p, Point a, Point b, Point c)
//{
//	Vector ab = b - a;
//	Vector ac = c - a;
//	Vector bc = c - b;
//
//	// Compute parametric position s for projection P' of P on AB,
//	// P' = A + s*AB, s = snom/(snom+sdenom)
//	float snom = Dot(p - a, ab), sdenom = Dot(p - b, a - b);
//
//	// Compute parametric position t for projection P' of P on AC,
//	// P' = A + t*AC, s = tnom/(tnom+tdenom)
//	float tnom = Dot(p - a, ac), tdenom = Dot(p - c, a - c);
//
//	if (snom <= 0.0f && tnom <= 0.0f) return a; // Vertex region early out
//
//	// Compute parametric position u for projection P' of P on BC,
//	// P' = B + u*BC, u = unom/(unom+udenom)
//	float unom = Dot(p - b, bc), udenom = Dot(p - c, b - c);
//
//	if (sdenom <= 0.0f && unom <= 0.0f) return b; // Vertex region early out
//	if (tdenom <= 0.0f && udenom <= 0.0f) return c; // Vertex region early out
//
//
//	// P is outside (or on) AB if the triple scalar product [N PA PB] <= 0
//	Vector n = Cross(b - a, c - a);
//	float vc = Dot(n, Cross(a - p, b - p));
//	// If P outside AB and within feature region of AB,
//	// return projection of P onto AB
//	if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
//		return a + snom / (snom + sdenom) * ab;
//
//	// P is outside (or on) BC if the triple scalar product [N PB PC] <= 0
//	float va = Dot(n, Cross(b - p, c - p));
//	// If P outside BC and within feature region of BC,
//	// return projection of P onto BC
//	if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
//		return b + unom / (unom + udenom) * bc;
//
//	// P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
//	float vb = Dot(n, Cross(c - p, a - p));
//	// If P outside CA and within feature region of CA,
//	// return projection of P onto CA
//	if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
//		return a + tnom / (tnom + tdenom) * ac;
//
//	// P must project inside face region. Compute Q using barycentric coordinates
//	float u = va / (va + vb + vc);
//	float v = vb / (va + vb + vc);
//	float w = 1.0f - u - v; // = vc / (va + vb + vc)
//	return u * a + v * b + w * c;
//}

