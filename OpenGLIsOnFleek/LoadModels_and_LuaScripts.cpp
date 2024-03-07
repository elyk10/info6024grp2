#include "cMesh.h"
//#include "sPhsyicsProperties.h"
#include "cVAOManager/cVAOManager.h"
#include "cPhysics.h"
#include "cLayoutLoader.h"
#include <vector>
#include "LuaBrain/cLuaBrain.h"
#include <iostream>

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

    int randomi = rand() % 10;
    int randomj = rand() % 10;

    int launcheri = rand() % 10;
    int launcherj = rand() % 10;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (i == randomi && j == randomj) {
                cMesh* floor = new cMesh();
                //floor->alpha_transparency = 1.0f;
                floor->drawPosition = glm::vec3(50 * i, 0.0f, 50 * j);
                floor->setRotationFromEuler(glm::vec3(0.0f));
                floor->drawScale = glm::vec3(0.1f);
                floor->meshName = "SM_Env_Dwarf_Floor_02.ply";
                floor->bIsVisible = true;
                floor->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
                floor->textureRatios[0] = 1.0f;
                ::g_vec_pMeshesToDraw.push_back(floor);

                std::cout << "Lava trap at x:" << floor->drawPosition.x << " y: " << floor->drawPosition.y << " z: " << floor->drawPosition.z << std::endl;

                sPhsyicsProperties* pLavaTrap = new sPhsyicsProperties();
                pLavaTrap->shapeType = sPhsyicsProperties::AABB;
                pLavaTrap->friendlyName = "lavaTrap";
                pLavaTrap->pTheAssociatedMesh = floor;
                pLavaTrap->position = floor->drawPosition;
                pLavaTrap->setRotationFromEuler(glm::vec3(0.0f));
                pLavaTrap->setShape(new sPhsyicsProperties::sAABB(glm::vec3(50 * (i - 1), -2.0f, 50 * (j - 1)), glm::vec3(50 * i, 0.25f, 50 * j)));
                ::g_pPhysics->AddShape(pLavaTrap);

                cMesh* lava = new cMesh();
                //floor->alpha_transparency = 1.0f;
                lava->drawPosition = glm::vec3(50 * i, -20.0f, 50 * j);
                lava->setRotationFromEuler(glm::vec3(0.0f));
                lava->drawScale = glm::vec3(0.1f);
                lava->meshName = "SM_Env_Dwarf_Floor_02.ply";
                lava->bIsVisible = true;
                lava->textureName[0] = "lava.bmp";
                lava->textureRatios[0] = 1.0f;
                ::g_vec_pMeshesToDraw.push_back(lava);

                sPhsyicsProperties* pLava = new sPhsyicsProperties();
                pLava->shapeType = sPhsyicsProperties::AABB;
                pLava->friendlyName = "lava";
                pLava->pTheAssociatedMesh = lava;
                pLava->setRotationFromEuler(glm::vec3(0.0f));
                pLava->position = lava->drawPosition;
                pLava->setShape(new sPhsyicsProperties::sAABB(glm::vec3(50 * (i - 1), -20.0f, 50 * (j - 1)), glm::vec3(50 * i, -19.25f, 50 * j)));
                ::g_pPhysics->AddShape(pLava);
            }
            else if (i == launcheri && j == launcherj) {
                cMesh* floor = new cMesh();
                //floor->alpha_transparency = 1.0f;
                floor->drawPosition = glm::vec3(50 * i, 0.0f, 50 * j);
                floor->setRotationFromEuler(glm::vec3(0.0f));
                floor->drawScale = glm::vec3(0.1f);
                floor->meshName = "SM_Env_Dwarf_Floor_02.ply";
                floor->bIsVisible = true;
                floor->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
                floor->textureRatios[0] = 1.0f;
                ::g_vec_pMeshesToDraw.push_back(floor);

                std::cout << "Launcher at x:" << floor->drawPosition.x << " y: " << floor->drawPosition.y << " z: " << floor->drawPosition.z << std::endl;

                sPhsyicsProperties* pLauncher = new sPhsyicsProperties();
                pLauncher->shapeType = sPhsyicsProperties::AABB;
                pLauncher->friendlyName = "launcher";
                pLauncher->pTheAssociatedMesh = floor;
                pLauncher->position = floor->drawPosition;
                pLauncher->setRotationFromEuler(glm::vec3(0.0f));
                pLauncher->setShape(new sPhsyicsProperties::sAABB(glm::vec3(50 * (i - 1) + 5, -2.0f, 50 * (j - 1) + 5), glm::vec3(50 * i - 5, 0.25f, 50 * j - 5)));
                ::g_pPhysics->AddShape(pLauncher);

            }
            else {
                cMesh* floor = new cMesh();
                //floor->alpha_transparency = 1.0f;
                floor->drawPosition = glm::vec3(50 * i, 0.0f, 50 * j);
                floor->setRotationFromEuler(glm::vec3(0.0f));
                floor->drawScale = glm::vec3(0.1f);
                floor->meshName = "SM_Env_Dwarf_Floor_02.ply";
                floor->bIsVisible = true;
                floor->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
                floor->textureRatios[0] = 1.0f;
                ::g_vec_pMeshesToDraw.push_back(floor);
            }
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (i < 6 && i > 3 && j < 6 && j > 3) {
                continue;
            }
            cMesh* floor = new cMesh();
            //floor->alpha_transparency = 1.0f;
            floor->drawPosition = glm::vec3(50 * i, 50.0f, 50 * j);
            floor->setRotationFromEuler(glm::vec3(0.0f));
            floor->drawScale = glm::vec3(0.1f);
            floor->meshName = "SM_Env_Dwarf_Floor_02.ply";
            floor->bIsVisible = true;
            floor->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
            floor->textureRatios[0] = 1.0f;
            ::g_vec_pMeshesToDraw.push_back(floor);
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (i < 7 && i > 2 && j < 7 && j > 2) {
                continue;
            }
            cMesh* floor = new cMesh();
            //floor->alpha_transparency = 1.0f;
            floor->drawPosition = glm::vec3(50 * i, 100.0f, 50 * j);
            floor->setRotationFromEuler(glm::vec3(0.0f));
            floor->drawScale = glm::vec3(0.1f);
            floor->meshName = "SM_Env_Dwarf_Floor_02.ply";
            floor->bIsVisible = true;
            floor->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
            floor->textureRatios[0] = 1.0f;
            ::g_vec_pMeshesToDraw.push_back(floor);
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 3; j++) {
            cMesh* wall = new cMesh();
            //floor->alpha_transparency = 1.0f;
            wall->drawPosition = glm::vec3(50 * i, j * 50.0f, 450.0f);
            wall->setRotationFromEuler(glm::vec3(0.0f));
            wall->drawScale = glm::vec3(0.1f);
            wall->meshName = "SM_Env_Dwarf_Wall_01.ply";
            wall->bIsVisible = true;
            wall->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
            wall->textureRatios[0] = 1.0f;
            ::g_vec_pMeshesToDraw.push_back(wall);
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 3; j++) {
            cMesh* wall = new cMesh();
            //floor->alpha_transparency = 1.0f;
            wall->drawPosition = glm::vec3(50 * i - 50.0f, j * 50.0f, -50.0f);
            wall->setRotationFromEuler(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
            wall->drawScale = glm::vec3(0.1f);
            wall->meshName = "SM_Env_Dwarf_Wall_01.ply";
            wall->bIsVisible = true;
            wall->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
            wall->textureRatios[0] = 1.0f;
            ::g_vec_pMeshesToDraw.push_back(wall);
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 3; j++) {
            cMesh* wall = new cMesh();
            //floor->alpha_transparency = 1.0f;
            wall->drawPosition = glm::vec3(450.0f, j * 50.0f, 50 * i - 50.0f);
            wall->setRotationFromEuler(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
            wall->drawScale = glm::vec3(0.1f);
            wall->meshName = "SM_Env_Dwarf_Wall_01.ply";
            wall->bIsVisible = true;
            wall->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
            wall->textureRatios[0] = 1.0f;
            ::g_vec_pMeshesToDraw.push_back(wall);
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 3; j++) {
            cMesh* wall = new cMesh();
            //floor->alpha_transparency = 1.0f;
            wall->drawPosition = glm::vec3(-50.0f, j * 50.0f, 50 * i);
            wall->setRotationFromEuler(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
            wall->drawScale = glm::vec3(0.1f);
            wall->meshName = "SM_Env_Dwarf_Wall_01.ply";
            wall->bIsVisible = true;
            wall->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
            wall->textureRatios[0] = 1.0f;
            ::g_vec_pMeshesToDraw.push_back(wall);
        }
    }

    cMesh* stairs = new cMesh();
    //floor->alpha_transparency = 1.0f;
    stairs->drawPosition = glm::vec3(150.0f, 0.0f, 175.0f);
    stairs->setRotationFromEuler(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
    stairs->drawScale = glm::vec3(0.1f);
    stairs->meshName = "SM_Env_Dwarf_Stairs_01.ply";
    stairs->bIsVisible = true;
    stairs->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
    stairs->textureRatios[0] = 1.0f;
    ::g_vec_pMeshesToDraw.push_back(stairs);

    cMesh* stairs2 = new cMesh();
    //floor->alpha_transparency = 1.0f;
    stairs2->drawPosition = glm::vec3(200.0f, 25.0f, 175.0f);
    stairs2->setRotationFromEuler(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
    stairs2->drawScale = glm::vec3(0.1f);
    stairs2->meshName = "SM_Env_Dwarf_Stairs_01.ply";
    stairs2->bIsVisible = true;
    stairs2->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
    stairs2->textureRatios[0] = 1.0f;
    ::g_vec_pMeshesToDraw.push_back(stairs2);


    cMesh* stairs3 = new cMesh();
    //floor->alpha_transparency = 1.0f;
    stairs3->drawPosition = glm::vec3(200.0f, 50.0f, 100.0f);
    stairs3->setRotationFromEuler(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
    stairs3->drawScale = glm::vec3(0.1f);
    stairs3->meshName = "SM_Env_Dwarf_Stairs_01.ply";
    stairs3->bIsVisible = true;
    stairs3->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
    stairs3->textureRatios[0] = 1.0f;
    ::g_vec_pMeshesToDraw.push_back(stairs3);

    cMesh* stairs4 = new cMesh();
    //floor->alpha_transparency = 1.0f;
    stairs4->drawPosition = glm::vec3(250.0f, 75.0f, 100.0f);
    stairs4->setRotationFromEuler(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
    stairs4->drawScale = glm::vec3(0.1f);
    stairs4->meshName = "SM_Env_Dwarf_Stairs_01.ply";
    stairs4->bIsVisible = true;
    stairs4->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
    stairs4->textureRatios[0] = 1.0f;
    ::g_vec_pMeshesToDraw.push_back(stairs4);

    cMesh* stairs5 = new cMesh();
    //floor->alpha_transparency = 1.0f;
    stairs5->drawPosition = glm::vec3(150.0f, 75.0f, 300.0f);
    stairs5->setRotationFromEuler(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
    stairs5->drawScale = glm::vec3(0.1f);
    stairs5->meshName = "SM_Env_Dwarf_Stairs_01.ply";
    stairs5->bIsVisible = true;
    stairs5->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
    stairs5->textureRatios[0] = 1.0f;
    ::g_vec_pMeshesToDraw.push_back(stairs5);

    cMesh* stairs6 = new cMesh();
    //floor->alpha_transparency = 1.0f;
    stairs6->drawPosition = glm::vec3(200.0f, 50.0f, 300.0f);
    stairs6->setRotationFromEuler(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
    stairs6->drawScale = glm::vec3(0.1f);
    stairs6->meshName = "SM_Env_Dwarf_Stairs_01.ply";
    stairs6->bIsVisible = true;
    stairs6->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
    stairs6->textureRatios[0] = 1.0f;
    ::g_vec_pMeshesToDraw.push_back(stairs6);

    sPhsyicsProperties* pfirstFloor = new sPhsyicsProperties();
    pfirstFloor->shapeType = sPhsyicsProperties::AABB;
    pfirstFloor->friendlyName = "floor";

    pfirstFloor->setShape(new sPhsyicsProperties::sAABB(glm::vec3(-50.0f, -2.3f, -50.0f), glm::vec3(450.0f, 0.5f, 450.0f)));
    ::g_pPhysics->AddShape(pfirstFloor);

    sPhsyicsProperties* pSecondFloorHole = new sPhsyicsProperties();
    pSecondFloorHole->shapeType = sPhsyicsProperties::AABB;
    pSecondFloorHole->friendlyName = "hole";
    pSecondFloorHole->setShape(new sPhsyicsProperties::sAABB(glm::vec3(150.0f, 50.0f, 150.0f), glm::vec3(250.0f, 52.5f, 250.0f)));
    ::g_pPhysics->AddShape(pSecondFloorHole);

    sPhsyicsProperties* pSecondFloor = new sPhsyicsProperties();
    pSecondFloor->shapeType = sPhsyicsProperties::AABB;
    pSecondFloor->friendlyName = "floor";
    pSecondFloor->setShape(new sPhsyicsProperties::sAABB(glm::vec3(-50.0f, 50.f, -50.0f), glm::vec3(450.0f, 52.5f, 450.0f)));
    ::g_pPhysics->AddShape(pSecondFloor);

    sPhsyicsProperties* pThirdFloorHole = new sPhsyicsProperties();
    pThirdFloorHole->shapeType = sPhsyicsProperties::AABB;
    pThirdFloorHole->friendlyName = "hole";
    pThirdFloorHole->setShape(new sPhsyicsProperties::sAABB(glm::vec3(100.0f, 101.7f, 100.0f), glm::vec3(300.0f, 102.0f, 300.0f)));
    ::g_pPhysics->AddShape(pThirdFloorHole);

    sPhsyicsProperties* pThirdFloor = new sPhsyicsProperties();
    pThirdFloor->shapeType = sPhsyicsProperties::AABB;
    pThirdFloor->friendlyName = "floor";
    pThirdFloor->setShape(new sPhsyicsProperties::sAABB(glm::vec3(-50.0f, 101.7f, -50.0f), glm::vec3(450.0f, 102.0f, 450.0f)));
    ::g_pPhysics->AddShape(pThirdFloor);

    sPhsyicsProperties* pFirstWall = new sPhsyicsProperties();
    pFirstWall->shapeType = sPhsyicsProperties::AABB;

    pFirstWall->setShape(new sPhsyicsProperties::sAABB(glm::vec3(-50.0f, -2.5f, 0.0f), glm::vec3(-45.6f, 150.0f, 450.0f)));
    ::g_pPhysics->AddShape(pFirstWall);

    sPhsyicsProperties* pSecondWall = new sPhsyicsProperties();
    pSecondWall->shapeType = sPhsyicsProperties::AABB;

    pSecondWall->setShape(new sPhsyicsProperties::sAABB(glm::vec3(-50.0f, -2.5f, -50.0f), glm::vec3(450.0f, 150.0f, -45.6f)));
    ::g_pPhysics->AddShape(pSecondWall);

    sPhsyicsProperties* pThirdWall = new sPhsyicsProperties();
    pThirdWall->shapeType = sPhsyicsProperties::AABB;

    pThirdWall->setShape(new sPhsyicsProperties::sAABB(glm::vec3(450.0f, -2.5f, -50.0f), glm::vec3(455.6f, 150.0f, 450.0f)));
    ::g_pPhysics->AddShape(pThirdWall);


    sPhsyicsProperties* pFourthWall = new sPhsyicsProperties();
    pFourthWall->shapeType = sPhsyicsProperties::AABB;

    pFourthWall->setShape(new sPhsyicsProperties::sAABB(glm::vec3(-50.0f, -2.5f, 450.0f), glm::vec3(450.0f, 150.0f, 455.6f)));
    ::g_pPhysics->AddShape(pFourthWall);

    //for (int i = 0; i < layout->modelPositions.size(); i++)
    //{
    //    if (layout->filesToLoad[i] != "camera")
    //    {
    //        cMesh* pTempMesh = new cMesh();
    //        pTempMesh->meshName = layout->filesToLoad[i];
    //        
    //        pTempMesh->drawPosition = layout->modelPositions[i];
    //        pTempMesh->adjustRoationAngleFromEuler(layout->modelRotation[i]);
    //        pTempMesh->setUniformDrawScale(layout->modelScale[i]);

    //        pTempMesh->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
    //        pTempMesh->textureRatios[0] = 1.0f;


    //        /*if (layout->filesToLoad[i] == "Sphere_1_unit_Radius.ply")
    //        {
    //            pTempMesh->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //            pTempMesh->bUseDebugColours = true;
    //        }*/
    //        if (layout->filesToLoad[i] == "SM_Env_GrassPatch_01.ply")
    //        {
    //            if ((int)getRandomFloat(0.0f, 10.0f) == 5)
    //            {
    //                pTempMesh->textureName[1] = "sand.bmp";
    //                pTempMesh->textureRatios[0] = getRandomFloat(0.7f, 1.0f);
    //                pTempMesh->textureRatios[1] = 1.0f - pTempMesh->textureRatios[0];
    //            }
    //        }
    //        if (layout->filesToLoad[i] == "Big_Flat_Mesh_256x256_12_5_xyz_n_rgba_uv.ply")
    //        {
    //            pTempMesh->friendlyName = "ground";
    //            pTempMesh->textureName[0] = "sand.bmp";
    //            pTempMesh->textureName[1] = "landShape.bmp";
    //            pTempMesh->textureRatios[1] = 0.0f;
    //            
    //        }
    //        if (layout->filesToLoad[i] == "SM_Item_Bottle_01.ply")
    //        {
    //            pTempMesh->alpha_transparency = 0.7f;
    //        }
    //        if (layout->filesToLoad[i] == "SM_Item_Bottle_02.ply")
    //        {
    //            pTempMesh->alpha_transparency = 0.6f;
    //        }
    //        if (layout->filesToLoad[i] == "SM_Item_Bottle_03.ply")
    //        {
    //            pTempMesh->alpha_transparency = 0.8f;
    //        }
    //        if (layout->filesToLoad[i] == "SM_Item_Bottle_04.ply")
    //        {
    //            pTempMesh->alpha_transparency = 0.68f;
    //        }
    //        if (layout->filesToLoad[i] == "Big_Flat_Mesh_256x256_04_8K_xyz_n_rgba_uv.ply")//"Big_Flat_Mesh_256x256_12_5_xyz_n_rgba_uv2.ply")
    //        {
    //            pTempMesh->alpha_transparency = 0.9f;
    //            pTempMesh->textureName[1] = "fire.bmp";
    //            pTempMesh->textureName[0] = "flames.bmp";
    //            pTempMesh->textureRatios[1] = 1.0f; 
    //            pTempMesh->textureRatios[0] = 0.0f; 
    //            pTempMesh->friendlyName = "flames";
    //        }

    //        ::g_vec_pMeshesToDraw.push_back(pTempMesh);
    //    }

    //} 

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
