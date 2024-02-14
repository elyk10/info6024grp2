#include "GLWF_CallBacks.h"

#include <string>
#include <iostream>
#include <vector>
#include "cMesh.h"
#include "cLightManager.h"
#include "cGlobal.h"
#include "cPlayer.h"

#include "cHiResTimer.h"

#include "LuaBrain/cLuaBrain.h"

extern int g_selectedMesh;// = 0;
extern std::vector< cMesh* > g_vec_pMeshesToDraw;

extern glm::vec3 g_cameraEye;
extern glm::vec3 g_cameraTarget;
extern glm::vec3 g_cameraFront;
extern glm::vec3 g_upVector;
extern double deltaTime;

extern cPlayer* thePlayer;

extern cLightManager* g_pTheLights;
extern int g_selectedLight;// = 0;

bool SaveVectorSceneToFile(std::string saveFileName);

extern bool wireMeshes = false;
bool cameraLock = false;
extern bool thirdPersonView;

// HACK:
extern float g_HeightAdjust; //= 10.0f;
extern glm::vec2 g_UVOffset;// = glm::vec2(0.0f, 0.0f);

// From main.cpp
extern cLuaBrain g_LuaBrain;

extern bool isMoving;
// Silly function binding example
//void ChangeTaylorSwiftTexture(std::string newTexture);

float CAMERA_MOVEMENT_SPEED = 1.0f;
const float OBJECT_MOVEMENT_SPEED = 0.01f;

const float LIGHT_MOVEMENT_SPEED = 1.0f;

cHiResTimer* p_HRTimer = new cHiResTimer(60);

double prevxpos;
double prevypos;
const float ANGLE_OFFSET_CAMERA = 0.008f;
const double mouseSensitivity = 0.05f;
float CAMERA_TARGET_OFFSET = 20.0f;

float yaw = 0.0f;
float pitch = 0.0f;
float roll = 0.0f;

bool firstMouse = true;


void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!cameraLock)
    {
        if (firstMouse)
        {
            prevxpos = xpos;
            prevypos = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - prevxpos;
        float yoffset = prevypos - ypos;
        prevxpos = xpos;
        prevypos = ypos;

        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 50.0f) //89.0f is base
            pitch = 50.0f;
        if (pitch < -9.0f) //-89.0f is base
            pitch = -9.0f;



        if (!thirdPersonView)
        {

            glm::vec3 direction;
            direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            direction.y = sin(glm::radians(pitch));
            direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            ::g_cameraFront = glm::normalize(direction);
            //std::cout << "CamereFront: " << g_cameraFront.x << ", " << g_cameraFront.y << ", " << g_cameraFront.z << std::endl;
        }
        else
        {

            glm::vec3 direction;
            direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            direction.y = sin(glm::radians(pitch));
            direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

            glm::vec3 temp = glm::vec3(0.0f);
            temp.x = direction.x;
            temp.z = direction.z;
            glm::normalize(temp);

            // adjust the angle of the player with the camera
            thePlayer->theMesh->setDrawOrientation(glm::quatLookAt(temp, glm::vec3(0.0f, 1.0f, 0.0f)));
            // thePlayer->forward = temp;

            ::g_cameraFront = glm::normalize(direction);
            direction = (glm::normalize(direction)) * CAMERA_TARGET_OFFSET;
            ::g_cameraEye = direction + thePlayer->theMesh->drawPosition;
            //std::cout << "CameraEye: " << g_cameraEye.x << ", " << g_cameraEye.y << ", " << g_cameraEye.z << std::endl;

        }
    }
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_F10 && action)
    {
        // Save the scene to a file
        SaveVectorSceneToFile("myscene.txt");
    }

    if (key == GLFW_KEY_H)
    {
        // Now I can't directly call this function, but we CAN call it through Lua
//        ChangeTaylorSwiftTexture("SpidermanUV_square.bmp");

//        g_LuaBrain.RunScriptImmediately("ChangeTaylorSwiftTexture()");
        g_LuaBrain.RunScriptImmediately("ChangeTaylorSwiftTexture('taylor-swift-jimmy-fallon.bmp')");

    }

    if (key == GLFW_KEY_J)
    {
        g_LuaBrain.RunScriptImmediately("AddMoveCommand('bathtub', -50, 15, -5, 13.5)");
    }

    if (key == GLFW_KEY_K)
    {
        g_LuaBrain.RunScriptImmediately("SetMeshPositionByFriendlyName('bathtub', 0.0, 0.0, 25)");
    }

    // This will move the object to the right, using Lua
    if (key == GLFW_KEY_M)
    {
        // bIsValid, x, y, z = GetMeshPositionByFriendlyName('bathtub')
        // SetMeshPositionByFriendlyName('bathtub', x, y, z );

        // We can move the object with this script:
        // (Lua if statement: https://www.lua.org/pil/4.3.1.html)
        // 
        // bIsValid, x, y, z = GetMeshPositionByFriendlyName('bathtub')
        // if bIsValid then
        //      x = x + 0.1
        //      SetMeshPositionByFriendlyName('bathtub', x, y, z )
        // end
        // 

        // this is annoying, but if it's more than one line, add a newline character "\n"
        // The downward slash at the end here tell C that the string continues.
        std::string luaScriptOneLine
            = "bIsValid, x, y, z = GetMeshPositionByFriendlyName('bathtub')\n"  \
            "if bIsValid then\n"                                                \
            "   x = x - 0.1\n"                                                  \
            "   y = y + 0.15\n"                                                  \
            "   SetMeshPositionByFriendlyName('bathtub', x, y, z )"             \
            "end";


        g_LuaBrain.RunScriptImmediately(luaScriptOneLine.c_str());
    }



    // Is the shift key down
    if (mods == GLFW_MOD_SHIFT)
    {
        // Shift key ONLY is down
        /*************************************************************************
        * external controls
        *************************************************************************/
        if (key == GLFW_KEY_L && action) // lock camera in position
        {
            if (cameraLock)
            {
                cameraLock = false;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else
            {
                cameraLock = true;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }

        if (key == GLFW_KEY_M && action) // switch to wire frame meshes
        {
            wireMeshes = !wireMeshes;

            for (auto mesh : ::g_vec_pMeshesToDraw)
            {
                mesh->bIsWireframe = wireMeshes;
            }
        }

        if (key == GLFW_KEY_P && action)
        {
            thirdPersonView = !thirdPersonView;
        }

    }

    // & will "mask" off the mod leaving the shift
    if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
    {
        // Adjust height of terrain
        if (key == GLFW_KEY_U)
        {
            ::g_HeightAdjust += 0.1;
        }
        if (key == GLFW_KEY_J)
        {
            ::g_HeightAdjust -= 0.1;
        }

        if (key == GLFW_KEY_LEFT)
        {
            ::g_UVOffset.x -= 0.01f;
        }
        if (key == GLFW_KEY_RIGHT)
        {
            ::g_UVOffset.x += 0.01f;
        }
        if (key == GLFW_KEY_UP)
        {
            ::g_UVOffset.y -= 0.01f;
        }
        if (key == GLFW_KEY_DOWN)
        {
            ::g_UVOffset.y += 0.01f;
        }


        // Shift key down (ignores other keys)

        if (key == GLFW_KEY_A)
        {
            ::g_pTheLights->theLights[::g_selectedLight].position.x -= LIGHT_MOVEMENT_SPEED;
        }
        if (key == GLFW_KEY_D)
        {
            ::g_pTheLights->theLights[::g_selectedLight].position.x += LIGHT_MOVEMENT_SPEED;
        }

        if (key == GLFW_KEY_W)
        {
            ::g_pTheLights->theLights[::g_selectedLight].position.z += LIGHT_MOVEMENT_SPEED;
        }
        if (key == GLFW_KEY_S)
        {
            ::g_pTheLights->theLights[::g_selectedLight].position.z -= LIGHT_MOVEMENT_SPEED;
        }


        if (key == GLFW_KEY_Q)
        {
            ::g_pTheLights->theLights[::g_selectedLight].position.y -= LIGHT_MOVEMENT_SPEED;
        }
        if (key == GLFW_KEY_E)
        {
            ::g_pTheLights->theLights[::g_selectedLight].position.y += LIGHT_MOVEMENT_SPEED;
        }

        // Linear attenuation 
        if (key == GLFW_KEY_1)
        {
            ::g_pTheLights->theLights[::g_selectedLight].atten.y *= 0.99f;      // Less 1%
        }
        if (key == GLFW_KEY_2)
        {
            ::g_pTheLights->theLights[::g_selectedLight].atten.y *= 1.01f;      // 1% more
        }

        // quadratic attenuation 
        if (key == GLFW_KEY_3)
        {
            ::g_pTheLights->theLights[::g_selectedLight].atten.z *= 0.99f;      // Less 1%
        }
        if (key == GLFW_KEY_4)
        {
            ::g_pTheLights->theLights[::g_selectedLight].atten.z *= 1.01f;      // 1% more
        }

        // 5 & 8 are outter angle
        if (key == GLFW_KEY_5)
        {
            ::g_pTheLights->theLights[::g_selectedLight].param1.z -= 0.1f;      // 0.1 degree
        }
        if (key == GLFW_KEY_8)
        {
            ::g_pTheLights->theLights[::g_selectedLight].param1.z += 0.1f;      // 0.1 degree
        }
        // 6 & 7 are inner angle
        if (key == GLFW_KEY_6)
        {
            ::g_pTheLights->theLights[::g_selectedLight].param1.y -= 0.1f;      // 0.1 degree
        }
        if (key == GLFW_KEY_7)
        {
            ::g_pTheLights->theLights[::g_selectedLight].param1.y += 0.1f;      // 0.1 degree
        }

        // quadratic attenuation 
        if (key == GLFW_KEY_9)
        {
            ::g_drawDebugLightSpheres = true;
        }
        if (key == GLFW_KEY_0)
        {
            ::g_drawDebugLightSpheres = false;
        }



    }//if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)

        // & Is ONLY Control down
//    if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
//    {
//        // Shift key down (ignores other keys)
//
//        if (key == GLFW_KEY_A )
//        {
//            ::g_vecMeshesToDraw[::g_selectedMesh].physProps.position.x -= OBJECT_MOVEMENT_SPEED;
//        }
//        if (key == GLFW_KEY_D )
//        {
//            ::g_vecMeshesToDraw[::g_selectedMesh].physProps.position.x += OBJECT_MOVEMENT_SPEED;
//        }
//
//        if (key == GLFW_KEY_W )
//        {
//            ::g_vecMeshesToDraw[::g_selectedMesh].physProps.position.z += OBJECT_MOVEMENT_SPEED;
//        }
//        if (key == GLFW_KEY_S )
//        {
//            ::g_vecMeshesToDraw[::g_selectedMesh].physProps.position.z -= OBJECT_MOVEMENT_SPEED;
//        }
//
//
//        if (key == GLFW_KEY_Q )
//        {
//            ::g_vecMeshesToDraw[::g_selectedMesh].physProps.position.y -= OBJECT_MOVEMENT_SPEED;
//        }
//        if (key == GLFW_KEY_E )
//        {
//            ::g_vecMeshesToDraw[::g_selectedMesh].physProps.position.y += OBJECT_MOVEMENT_SPEED;
//        }
//
//        // Select another model
//        if (key == GLFW_KEY_PAGE_UP)
//        {
//            ::g_selectedMesh++;
//            if (::g_selectedMesh > ::g_vec_pMeshesToDraw.size())
//            {
//                ::g_selectedMesh = 0;
//            }
//            std::cout << "Selcted model: " << ::g_selectedMesh << std::endl;
//        }
//        if (key == GLFW_KEY_PAGE_DOWN)
//        {
//            ::g_selectedMesh--;
//            if (::g_selectedMesh < 0)
//            {
//                ::g_selectedMesh = ((int)::g_vec_pMeshesToDraw.size() - 1);
//            }
//            std::cout << "Selcted model: " << ::g_selectedMesh << std::endl;
//        }
//    }


    // Nothing down
    if (mods == 0)
    {
        if (!cameraLock)
        {
            // Shift key is NOT down
            /*************************************************************************
            * movement controls
            *************************************************************************/

            if (!thirdPersonView) // first person
            {
                if (key == GLFW_KEY_A && action) // left
                {
                    ::g_cameraEye -= glm::normalize(glm::cross(::g_cameraFront, ::g_upVector)) * CAMERA_MOVEMENT_SPEED;
                }
                if (key == GLFW_KEY_D && action) // right
                {
                    ::g_cameraEye += glm::normalize(glm::cross(::g_cameraFront, ::g_upVector)) * CAMERA_MOVEMENT_SPEED;
                }
                if (key == GLFW_KEY_W && action) // forward
                {
                    ::g_cameraEye += CAMERA_MOVEMENT_SPEED * ::g_cameraFront;
                }
                if (key == GLFW_KEY_S && action) // backwards
                {
                    ::g_cameraEye -= CAMERA_MOVEMENT_SPEED * ::g_cameraFront;
                }


                if (key == GLFW_KEY_Q && action) // up
                {
                    ::g_cameraEye.y -= CAMERA_MOVEMENT_SPEED;
                }
                if (key == GLFW_KEY_E && action) // down
                {
                    ::g_cameraEye.y += CAMERA_MOVEMENT_SPEED;
                }
            }
            else // third person
            {
                deltaTime = p_HRTimer->getFrameTime();
                isMoving = false;
                if (key == GLFW_KEY_A && action) // left 
                {
                    isMoving = true;
                    glm::vec3 movement = glm::normalize(glm::cross(thePlayer->theMesh->drawPosition - ::g_cameraEye, ::g_upVector)) * thePlayer->speed * (float)deltaTime;
                    ::g_cameraEye.x -= movement.x;
                    ::g_cameraEye.z -= movement.z;
                    ::g_cameraTarget.x -= movement.x;
                    ::g_cameraTarget.z -= movement.z;

                    thePlayer->theMesh->drawPosition.x -= movement.x;
                    thePlayer->theMesh->drawPosition.z -= movement.z;

                    thePlayer->moveDir = movement * (-1.0f);
                }
                if (key == GLFW_KEY_D && action) // right 
                {
                    isMoving = true;
                    glm::vec3 movement = glm::normalize(glm::cross(thePlayer->theMesh->drawPosition - ::g_cameraEye, ::g_upVector)) * thePlayer->speed * (float)deltaTime;
                    ::g_cameraEye.x += movement.x;
                    ::g_cameraEye.z += movement.z;
                    ::g_cameraTarget.x += movement.x;
                    ::g_cameraTarget.z += movement.z;

                    thePlayer->theMesh->drawPosition.x += movement.x;
                    thePlayer->theMesh->drawPosition.z += movement.z;

                    thePlayer->moveDir = movement;
                }
                if (key == GLFW_KEY_W && action) // forward 
                {
                    isMoving = true;
                    glm::vec3 movement = glm::normalize(thePlayer->theMesh->drawPosition - ::g_cameraEye) * thePlayer->speed * (float)deltaTime;
                    ::g_cameraEye.x += movement.x;
                    ::g_cameraEye.z += movement.z;
                    ::g_cameraTarget.x += movement.x;
                    ::g_cameraTarget.z += movement.z;

                    thePlayer->theMesh->drawPosition.x += movement.x;
                    thePlayer->theMesh->drawPosition.z += movement.z;

                    thePlayer->moveDir = movement;
                }
                if (key == GLFW_KEY_S && action) // backwards 
                {
                    isMoving = true;
                    glm::vec3 movement = glm::normalize(thePlayer->theMesh->drawPosition - ::g_cameraEye) * thePlayer->speed * (float)deltaTime;
                    ::g_cameraEye.x -= movement.x;
                    ::g_cameraEye.z -= movement.z;
                    ::g_cameraTarget.x -= movement.x;
                    ::g_cameraTarget.z -= movement.z;

                    thePlayer->theMesh->drawPosition.x -= movement.x;
                    thePlayer->theMesh->drawPosition.z -= movement.z;

                    thePlayer->moveDir = movement * (-1.0f);
                }


                if (key == GLFW_KEY_Q && action) // up 
                {
                    ::g_cameraEye.y -= CAMERA_MOVEMENT_SPEED;
                    ::g_cameraTarget.y -= CAMERA_MOVEMENT_SPEED;
                }
                if (key == GLFW_KEY_E && action) // down 
                {
                    ::g_cameraEye.y += CAMERA_MOVEMENT_SPEED;
                    ::g_cameraTarget.y += CAMERA_MOVEMENT_SPEED;
                }
            }
        }


    }// if ( ( mods & GLFW_MOD_SHIFT ) == GLFW_MOD_SHIFT )



    return;
}

