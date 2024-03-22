#include "cPhysics.h"


#include <iostream>

extern glm::vec3 g_cameraEye;
extern glm::vec3 g_cameraTarget;
extern AudioManager* m_Audio;
// HACK:
void g_DrawDebugSphere(glm::vec3 position, float scale, glm::vec4 colourRGBA);

int bong = -1;
int launcher = -1;

void cPhysics::Update(double deltaTime)
{
//	for ( unsigned int index = 0; index !=  this->m_vec_pPhysicalProps.size(); index++ )
//	{
//		sPhsyicsProperties* pObject = this->m_vec_pPhysicalProps[index];
//		
//		// ...and so on...
//
//	}

	// Perform the iteration loop
	for (sPhsyicsProperties* pObject : this->m_vec_pPhysicalProps)
	{
		// Infinite mass? 
		if (pObject->inverse_mass >= 0.0f)
		{
			if (pObject->friendlyName == "player") {
				if (pObject->health <= 0) {
					bong = m_Audio->PlayAudio("assets/audio/Bong.mp3", pObject->position);
					pObject->position = glm::vec3(0.0f, 5.0f, -20.0f );
					pObject->health = 1000;
					for (sPhsyicsProperties* pSubObject : this->m_vec_pPhysicalProps) {
						if (pSubObject->friendlyName == "lavaTrap") {
							pSubObject->setRotationFromEuler(glm::vec3(0.0f));
						}
						else if (pSubObject->friendlyName == "launcher") {
							pSubObject->position.y = 0.0f;
						}
					}
				}
				else {

				}
			}
			if (m_Audio->GetChannelPlaying(bong)) {
				unsigned int pbPos;
				m_Audio->GetPlaybackPosition(bong, pbPos);
				unsigned int length = m_Audio->GetData("assets/audio/Bong.mp3");
				if (pbPos >= 1800) {
					m_Audio->StopAudio(bong);
				}
			}
			if (m_Audio->GetChannelPlaying(launcher)) {
				unsigned int pbPos;
				m_Audio->GetPlaybackPosition(launcher, pbPos);
				unsigned int length = m_Audio->GetData("assets/audio/stonescrape.mp3");
				std::cout << pbPos << std::endl;
				if (pbPos >= 4500) {
					m_Audio->StopAudio(launcher);
				}
			}
			// Explicit forward Euler "integration step"
			//		NewVelocity = LastVel + (Accel * DeltaTime)
			//		NewPosition = LastPos + (Vel * DeltaTime)	

			// Update the "old" position
			if (pObject->friendlyName == "launcher") {
				for (sPhsyicsProperties* pSubObject : this->m_vec_pPhysicalProps) {
					if (pSubObject->friendlyName == "player") {
						pSubObject->position.y += pObject->position.y;
					}
				}
				if (pObject->position.y > 10.0f) {
					pObject->velocity.y = -25.0f;
				}
				else if (pObject->position.y < -0.1f) {
					pObject->velocity.y = 0.0f;
					pObject->position.y = 0.0f;
				}
			}
			pObject->oldPosition = pObject->position;

			// Velocity change is based on the acceleration over this time frame 
			// This part: (Accel * DeltaTime)
			glm::vec3 deltaVelocityThisFrame = pObject->acceleration * (float)deltaTime;

			// Update the velocity based on this delta velocity
			// Then this part: NewVelocity = LastVel + ...
			pObject->velocity += deltaVelocityThisFrame;


			// Position change is based on the velocity over this time frame
			// This part: (Vel * DeltaTime)	
			glm::vec3 deltaPosition = pObject->velocity * (float)deltaTime;

			// ...then this part: NewPosition = LastPos + ...
			// Upatate the position based on this delta position
	//		pCurrentMesh->pPhysProps->position += deltaPosition;
			pObject->position.x += deltaPosition.x;
			pObject->position.y += deltaPosition.y;
			pObject->position.z += deltaPosition.z;

			//if (pObject->friendlyName == "player") {
			//	g_cameraEye += deltaPosition;
			//	g_cameraTarget += deltaPosition;
			//}

		}//if (pObject->inverse_mass >= 0.0f)

	}//for (sPhsyicsProperties* pObject





	// See which object is colliding with which object...
	for (sPhsyicsProperties* pObjectA : this->m_vec_pPhysicalProps )
	{
		//object will never move, no point checking for collision as the moving ones will do that
		if (pObjectA->inverse_mass < 0.01f) {
			continue;
		}
		bool touchingHole = false;
		for (sPhsyicsProperties* pObjectB : this->m_vec_pPhysicalProps )
		{
			//if (pObjectB->friendlyName != "floor") {
			//	continue;
			//}
			// Are "A" and "B" the same object
			if (pObjectA == pObjectB)
			{
				// Yup, so skip this
				continue;
			}
			if (touchingHole) {
				continue;
			}
// We could do the matrix this way...
//			if ((pObjectA->shapeType == sPhsyicsProperties::SPHERE) &&
//				(pObjectA->shapeType == sPhsyicsProperties::PLANE))
//			{
//
//			}// Sphere - Plane
//
//			if ((pObjectA->shapeType == sPhsyicsProperties::SPHERE) &&
//				(pObjectA->shapeType == sPhsyicsProperties::SPHERE))
//			{
//
//			}// Sphere - Sphere

			// What's the collision? 
			switch (pObjectA->shapeType)
			{
			case sPhsyicsProperties::SPHERE:
				switch (pObjectB->shapeType)
				{
				case sPhsyicsProperties::SPHERE:
					// Sphere - Sphere
					break;
				case sPhsyicsProperties::PLANE:
					// Shpere - Plane
					break;
				case sPhsyicsProperties::TRIANGLE:
					// Sphere - Triangle
					break;
				case sPhsyicsProperties::AABB:
					if (this->m_Sphere_AABB_IntersectionTest(pObjectA, pObjectB)) {
						if (pObjectB->friendlyName == "hole") {
							touchingHole = true;
							continue;
						}
						if (pObjectB->friendlyName == "lavaTrap") {
							touchingHole = true;
							pObjectB->adjustRoationAngleFromEuler((glm::vec3(-0.75f, 0.0f, 0.0f) * (float)deltaTime));
							continue;
						}
						if (pObjectB->friendlyName == "launcher") {
							pObjectB->velocity = glm::vec3(0.0f, 25.0f, 0.0f);
							pObjectA->position.y += 0.75f;
							pObjectA->velocity = glm::vec3(0.0f, 50.0f, 0.0f);
							launcher = m_Audio->PlayAudio("assets/audio/stonescrape.mp3", pObjectB->position);
							touchingHole = true;
							continue;
						}
						if (pObjectB->friendlyName == "lava") {
							pObjectA->health -= 200 * deltaTime;
							std::cout << "Hp: " << pObjectA->health << std::endl;
							pObjectA->position.y = -19.25f;
						}
						if (pObjectB->friendlyName == "floor") {
							sPhsyicsProperties::sAABB* pAABB_Detail = (sPhsyicsProperties::sAABB*)(pObjectB->pShape);
							if (pObjectA->position.y + 1.f < pAABB_Detail->minXYZ.y) {
								pObjectA->position.y = pAABB_Detail->minXYZ.y;
							}
							else {
								pObjectA->position.y = pAABB_Detail->maxXYZ.y;
							}
						}
						else {
							pObjectA->position = pObjectA->oldPosition;
						}
						if (glm::length(pObjectA->velocity) > 30.0f) {
			
							pObjectA->health -= glm::length(pObjectA->velocity) * 0.5f;
							std::cout << "Hp: " << pObjectA->health << std::endl;
							pObjectA->velocity = glm::vec3(0.0f);
						}
					}
					break;
				case sPhsyicsProperties::CAPSULE:
					// Sphere - Capsule
					break;
				case sPhsyicsProperties::MESH_OF_TRIANGLES_INDIRECT:
					// Sphere - Mesh triangle (indirect)
					if ( this->m_Sphere_TriMeshIndirect_IntersectionTest( pObjectA, pObjectB))
					{
						std::cout << "Hazzah!" << std::endl;
					}
					break;
				case sPhsyicsProperties::MESH_OF_TRIANGLES_LOCAL_VERTICES:
					// Shpere - Mesh (local vertices)
					break;
				}//switch (pObjectB->shapeType)
				break;

//			case sPhsyicsProperties::PLANE:
//				break;
//			case sPhsyicsProperties::TRIANGLE:
//				break;
// ??			case sPhsyicsProperties::AABB:
// ??			break;

			case sPhsyicsProperties::CAPSULE:
				switch (pObjectB->shapeType)
				{
				case sPhsyicsProperties::SPHERE:
					// Capsule - Sphere
					break;
				case sPhsyicsProperties::PLANE:
					// Capsule - Plane
					break;
				case sPhsyicsProperties::TRIANGLE:
					// Capsule - Triangle
					break;
				case sPhsyicsProperties::AABB:
					// Capsule - AABB
					break;
				case sPhsyicsProperties::CAPSULE:
					// Capsule - Capsule
					break;
				case sPhsyicsProperties::MESH_OF_TRIANGLES_INDIRECT:
					// Capsule - Mesh triangle (indirect)
					break;
				case sPhsyicsProperties::MESH_OF_TRIANGLES_LOCAL_VERTICES:
					// Capsule - Mesh (local vertices)
					break;
				}//switch (pObjectB->shapeType)
				break;

//			case sPhsyicsProperties::MESH_OF_TRIANGLES_INDIRECT:
//				switch (pObjectB->shapeType)
//				{
//				case sPhsyicsProperties::SPHERE:
//					break;
//				case sPhsyicsProperties::PLANE:
//					break;
//				case sPhsyicsProperties::TRIANGLE:
//					break;
//				case sPhsyicsProperties::AABB:
//					break;
//				case sPhsyicsProperties::CAPSULE:
//					break;
//				case sPhsyicsProperties::MESH_OF_TRIANGLES_INDIRECT:
//					break;
//				case sPhsyicsProperties::MESH_OF_TRIANGLES_LOCAL_VERTICES:
//					break;
//				}//switch (pObjectB->shapeType)
//				break;
//
//			case sPhsyicsProperties::MESH_OF_TRIANGLES_LOCAL_VERTICES:
//				switch (pObjectB->shapeType)
//				{
//				case sPhsyicsProperties::SPHERE:
//					break;
//				case sPhsyicsProperties::PLANE:
//					break;
//				case sPhsyicsProperties::TRIANGLE:
//					break;
//				case sPhsyicsProperties::AABB:
//					break;
//				case sPhsyicsProperties::CAPSULE:
//					break;
//				case sPhsyicsProperties::MESH_OF_TRIANGLES_INDIRECT:
//					break;
//				case sPhsyicsProperties::MESH_OF_TRIANGLES_LOCAL_VERTICES:
//					break;
//				}//switch (pObjectB->shapeType)
//				break;
// 
			//UNKNOWN_OR_UNDEFINED

			}//switch (pObjectA->shapeType)

		}//for (sPhsyicsProperties* pObjectB

	}//for (sPhsyicsProperties* pObjectA...


	// Upadate the draw locations (and orientations) for all assocuated meshes
	for (sPhsyicsProperties* pObject : this->m_vec_pPhysicalProps)
	{
		if (pObject->pTheAssociatedMesh)
		{
			if (pObject->friendlyName == "player") {
				glm::vec3 cameraEyeDisplacement = ::g_cameraEye - g_cameraTarget;
				::g_cameraEye = pObject->position + cameraEyeDisplacement;
				::g_cameraTarget = pObject->position;
			}
			pObject->pTheAssociatedMesh->setDrawPosition(pObject->position - glm::vec3(0.0f, 0.25f, 0.0f));
//			pObject->pTheAssociatedMesh->setDrawOrientation(pObject->orientation);
			pObject->pTheAssociatedMesh->setDrawOrientation(pObject->get_qOrientation());
		}
	}//for (sPhsyicsProperties* pObjectA


	// HACK:
	if ( ! this->m_vecCollisionsThisFrame.empty())
	{
		std::cout << "BREAK ME!" << std::endl;
		
		std::cout << this->m_vecCollisionsThisFrame.size() << std::endl;
		for (sCollisionEvent col:  this->m_vecCollisionsThisFrame)
		{
			std::cout
				<< col.pObjectA->getShapeTypeAsString()
				<< " hit "
				<< col.pObjectB->getShapeTypeAsString()
				<< std::endl;
		}
		std::cout << "******************************************" << std::endl;
		// HACK:
		this->m_vecCollisionsThisFrame.clear();
	}

	return;
}
