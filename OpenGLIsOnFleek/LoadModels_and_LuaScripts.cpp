#include "cMesh.h"
//#include "sPhsyicsProperties.h"
#include "cVAOManager/cVAOManager.h"
#include "cPhysics.h"
#include "cLayoutLoader.h"
#include <vector>
#include "LuaBrain/cLuaBrain.h"

extern std::vector< cMesh* > g_vec_pMeshesToDraw;
//extern std::vector< sPhsyicsProperties* > g_vec_pPhysicalProps;
extern cPhysics* g_pPhysics;    //

extern cMesh* g_pDebugSphereMesh;

extern cVAOManager* g_pMeshManager;

extern cLayoutLoader* layout;

// From main.cpp
extern cLuaBrain g_LuaBrain;
// Silly function binding example
void ChangeTaylorSwiftTexture(void);


float getRandomFloat(float a, float b);

void LoadLuaScripts(void)
{
//    g_LuaBrain.


    return;
}


bool LoadModels(void)
{
    LoadLuaScripts();

    ::g_pDebugSphereMesh = new cMesh();
    ::g_pDebugSphereMesh->meshName = "Sphere_1_unit_Radius.ply";
    ::g_pDebugSphereMesh->bIsWireframe = true;
    ::g_pDebugSphereMesh->bDoNotLight = true;
    ::g_pDebugSphereMesh->setUniformDrawScale(1.0f);
    ::g_pDebugSphereMesh->bIsVisible = false;
    ::g_pDebugSphereMesh->friendlyName = "DEBUG_SPHERE";
    // Note: we are NOT adding this to the vector of meshes


    for (int i = 0; i < layout->modelPositions.size(); i++)
    {
        if (layout->filesToLoad[i] != "camera")
        {
            cMesh* pTempMesh = new cMesh();
            pTempMesh->meshName = layout->filesToLoad[i];
            
            pTempMesh->drawPosition = layout->modelPositions[i];
            pTempMesh->adjustRoationAngleFromEuler(layout->modelRotation[i]);
            pTempMesh->setUniformDrawScale(layout->modelScale[i]);

            pTempMesh->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
            pTempMesh->textureRatios[0] = 1.0f;


            /*if (layout->filesToLoad[i] == "Sphere_1_unit_Radius.ply")
            {
                pTempMesh->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                pTempMesh->bUseDebugColours = true;
            }*/
            if (layout->filesToLoad[i] == "SM_Env_GrassPatch_01.ply")
            {
                if ((int)getRandomFloat(0.0f, 10.0f) == 5)
                {
                    pTempMesh->textureName[1] = "sand.bmp";
                    pTempMesh->textureRatios[0] = getRandomFloat(0.7f, 1.0f);
                    pTempMesh->textureRatios[1] = 1.0f - pTempMesh->textureRatios[0];
                }
            }
            if (layout->filesToLoad[i] == "Big_Flat_Mesh_256x256_12_5_xyz_n_rgba_uv.ply")
            {
                pTempMesh->friendlyName = "ground";
                pTempMesh->textureName[0] = "sand.bmp";
                pTempMesh->textureName[1] = "landShape.bmp";
                pTempMesh->textureRatios[1] = 0.0f;
                
            }
            if (layout->filesToLoad[i] == "SM_Item_Bottle_01.ply")
            {
                pTempMesh->alpha_transparency = 0.7f;
            }
            if (layout->filesToLoad[i] == "SM_Item_Bottle_02.ply")
            {
                pTempMesh->alpha_transparency = 0.6f;
            }
            if (layout->filesToLoad[i] == "SM_Item_Bottle_03.ply")
            {
                pTempMesh->alpha_transparency = 0.8f;
            }
            if (layout->filesToLoad[i] == "SM_Item_Bottle_04.ply")
            {
                pTempMesh->alpha_transparency = 0.68f;
            }
            if (layout->filesToLoad[i] == "Big_Flat_Mesh_256x256_04_8K_xyz_n_rgba_uv.ply")//"Big_Flat_Mesh_256x256_12_5_xyz_n_rgba_uv2.ply")
            {
                pTempMesh->alpha_transparency = 0.9f;
                pTempMesh->textureName[1] = "fire.bmp";
                pTempMesh->textureName[0] = "flames.bmp";
                pTempMesh->textureRatios[1] = 1.0f; 
                pTempMesh->textureRatios[0] = 0.0f; 
                pTempMesh->friendlyName = "flames";
            }

            ::g_vec_pMeshesToDraw.push_back(pTempMesh);
        }

    } 

//    cMesh* pBathtub = new cMesh();
//    pBathtub->meshName = "SM_Prop_Barrel_03 (2).ply";
//    pBathtub->friendlyName = "player"; 
//    pBathtub->setUniformDrawScale(3.0f);
////    pBathtub->setRotationFromEuler(glm::vec3(45.0f, 0.0f, 0.0f));
//    pBathtub->drawPosition = glm::vec3(0.0f, 30.0f, 161.0f);
//   // pBathtub->alpha_transparency = 0.2f;
//    // 
//    pBathtub->textureName[0] = "Texture_01_A.bmp";
//    pBathtub->textureRatios[0] = 1.0f;
//    ::g_vec_pMeshesToDraw.push_back(pBathtub);



    cMesh* pGroundMesh = new cMesh();
//    pGroundMesh->meshName = "Terrain_xyz_n_rgba_uv.ply";
//    pGroundMesh->meshName = "Big_Flat_Mesh_256x256_00_132K_xyz_n_rgba_uv.ply";    
//    pGroundMesh->meshName = "Big_Flat_Mesh_256x256_07_1K_xyz_n_rgba_uv.ply";    
    pGroundMesh->meshName = "Big_Flat_Mesh_256x256_12_5_xyz_n_rgba_uv.ply";    
    pGroundMesh->drawPosition.y = -50.0f;
    pGroundMesh->drawPosition.z = -50.0f;
    pGroundMesh->friendlyName = "ground";

    //pGroundMesh->bIsWireframe = true;
    //pGroundMesh->bDoNotLight = true;
    //pGroundMesh->wholeObjectDebugColourRGBA = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    //pGroundMesh->bUseDebugColours = true;
    //
//    pGroundMesh->textureName[0] = "NvF5e_height_map.bmp";
//    pGroundMesh->textureName[0] = "Blank_UV_Text_Texture.bmp";
    pGroundMesh->textureName[0] = "Texture_01_A.bmp";
    pGroundMesh->textureRatios[0] = 1.0f;

//    pGroundMesh->textureName[1] = "taylor-swift-jimmy-fallon.bmp";
//    pGroundMesh->textureRatios[1] = 0.0f;

    ::g_vec_pMeshesToDraw.push_back(pGroundMesh);




    const float MAX_SPHERE_LOCATION = 30.0f;
    const float MAX_VELOCITY = 1.0f;

    // Make a bunch of spheres...
    const unsigned int NUMBER_OF_SPHERES = 0;
    for (unsigned int count = 0; count != NUMBER_OF_SPHERES; count++)
    {
        cMesh* pSphereMesh = new cMesh();
//        pSphereMesh->meshName = "Sphere_1_unit_Radius.ply";
        pSphereMesh->meshName = "Sphere_1_unit_Radius_xyz_n_rgba_uv.ply";

        pSphereMesh->friendlyName = "Sphere";

        pSphereMesh->setUniformDrawScale(5.0f);

        // Add drawing mesh to the things to draw
        ::g_vec_pMeshesToDraw.push_back(pSphereMesh);

        // Now the physical object to match this
        sPhsyicsProperties* pSpherePhysProps = new sPhsyicsProperties();        // HEAP

        pSpherePhysProps->velocity.y = getRandomFloat(0.0f, MAX_VELOCITY);
        pSpherePhysProps->velocity.x = getRandomFloat(-MAX_VELOCITY, MAX_VELOCITY);
        pSpherePhysProps->velocity.z = getRandomFloat(-MAX_VELOCITY, MAX_VELOCITY);

        pSpherePhysProps->acceleration.y = (-9.81f / 5.0f);

        pSpherePhysProps->position.x = -10.0f;                      // getRandomFloat(-MAX_SPHERE_LOCATION, MAX_SPHERE_LOCATION);
        pSpherePhysProps->position.z = -10.0f;                        // getRandomFloat(-MAX_SPHERE_LOCATION, MAX_SPHERE_LOCATION);
        pSpherePhysProps->position.y = MAX_SPHERE_LOCATION; //  getRandomFloat(10.0f, MAX_SPHERE_LOCATION + 20.0f);

        // Mass of 10 somethings? kg?
        pSpherePhysProps->inverse_mass = 1.0f / 10.0f;


        pSpherePhysProps->shapeType = sPhsyicsProperties::SPHERE;

        // The rendered graphical object that moves with this physics object
        pSpherePhysProps->pTheAssociatedMesh = pSphereMesh;

//        sPhsyicsProperties::sSphere* pTemp = new sPhsyicsProperties::sSphere(1.0f);
//        pSpherePhysProps->setShape(pTemp);

        pSpherePhysProps->setShape( new sPhsyicsProperties::sSphere(5.0f) );
        ::g_pPhysics->AddShape(pSpherePhysProps);


    }//for ( unsigned int count...



    return true;
}
