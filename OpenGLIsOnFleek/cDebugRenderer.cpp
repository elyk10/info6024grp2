#include "cDebugRender.h"
#include "OpenGLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "Basic Shader Manager/cShaderManager.h"
#include "cVAOManager/cVAOManager.h"

// This is "file global" here. We will look at a "pointer to implementation" pattern 
//  later to avoid this (if we need to)
cShaderManager* cDebugRenderer_pShaderManager = NULL;
cVAOManager* cDebugRenderer_pMeshManager = NULL;

GLuint cDebugRenderer_vertex_buffer = 0;
GLuint cDebugRenderer_VAO = 0;

cDebugRenderer::cDebugRenderer()
{
    this->m_sizeOfLineVertexBuffer = 0;
    this->m_pLinesVerticesToDraw = NULL;
}


// https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats
float getRandFloat(float a, float b) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}


bool cDebugRenderer::Initialize()
{
    cDebugRenderer_pShaderManager = new cShaderManager();

    cDebugRenderer_pShaderManager->setBasePath("assets/shaders");

    cShaderManager::cShader vertexShader;
    vertexShader.fileName = "debugObject_vertexShader.glsl";

    cShaderManager::cShader fragmentShader;
    fragmentShader.fileName = "debugObject_fragmentShader01.glsl";

    if (!cDebugRenderer_pShaderManager->createProgramFromFile("debugObjectShader", vertexShader, fragmentShader))
    {
        this->m_lastError = "ERROR: " + cDebugRenderer_pShaderManager->getLastError();
        return false;
    }

    GLuint debugShaderProgramID = cDebugRenderer_pShaderManager->getIDFromFriendlyName("debugObjectShader");


    // Allocate a C++/CPU side array to hold our lines
    // const unsigned int LINE_VERTEX_BUFFER_SIZE = 20'000;
    this->m_pLinesVerticesToDraw = new sLineVertex_Buffer[LINE_VERTEX_BUFFER_SIZE];
    // Clear this to zero
    // (you can also do a loop, which is what memset() really uses)
    memset(this->m_pLinesVerticesToDraw, 0, 
           LINE_VERTEX_BUFFER_SIZE * sizeof(sLineVertex_Buffer));
    // But we don't have anything in there, yet
    this->m_sizeOfLineVertexBuffer = 0;

    // *********************************************************************************
    // HACK:
    const float MAX_LINE_POSITION = 1000.0f;

    for ( int count = 0; count != 500; count++ )
    {
        glm::vec3 lineStart = glm::vec3(getRandFloat(-MAX_LINE_POSITION, MAX_LINE_POSITION),
                                        getRandFloat(-MAX_LINE_POSITION, MAX_LINE_POSITION),
                                        getRandFloat(-MAX_LINE_POSITION, MAX_LINE_POSITION));

        glm::vec3 lineEnd = glm::vec3(getRandFloat(-MAX_LINE_POSITION, MAX_LINE_POSITION),
                                      getRandFloat(-MAX_LINE_POSITION, MAX_LINE_POSITION),
                                      getRandFloat(-MAX_LINE_POSITION, MAX_LINE_POSITION));

        glm::vec4 lineColour = glm::vec4(getRandFloat(0.0f, 1.0f),
                                         getRandFloat(0.0f, 1.0f),
                                         getRandFloat(0.0f, 1.0f),
                                         1.0f);

        this->AddLine(lineStart, lineEnd, lineColour);
    }

    // *********************************************************************************


    glGenVertexArrays(1, &cDebugRenderer_VAO);
// "Bind" this buffer:
// - aka "make this the 'current' VAO buffer
    glBindVertexArray(cDebugRenderer_VAO);

    // NOTE: OpenGL error checks have been omitted for brevity
    glGenBuffers(1, &cDebugRenderer_vertex_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, cDebugRenderer_vertex_buffer);

    // Copies the data from the C++/CPU side to the GPU, overwriting whatever is there
    unsigned int numberOfBytesToCopy = sizeof(sLineVertex_Buffer) * LINE_VERTEX_BUFFER_SIZE;

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(numberOfBytesToCopy),
                 this->m_pLinesVerticesToDraw,  // Pointer to the buffer on the CPU/RAM
                 GL_DYNAMIC_DRAW);



    GLint vPosition_location = glGetAttribLocation(debugShaderProgramID, "vPosition");
    GLint vColour_location = glGetAttribLocation(debugShaderProgramID, "vColour");


    glEnableVertexAttribArray(vPosition_location);
    glVertexAttribPointer(vPosition_location, 4,
                          GL_FLOAT, 
                          GL_FALSE,
                          sizeof(sLineVertex_Buffer),                   // Stride
                          (void*) offsetof(sLineVertex_Buffer,x) );     // Offset in bytes to "x"

    glEnableVertexAttribArray(vColour_location);
    glVertexAttribPointer(vColour_location, 4,
                          GL_FLOAT, 
                          GL_FALSE,
                          sizeof(sLineVertex_Buffer), 
                          (void*)offsetof(sLineVertex_Buffer, r));     // Offset in bytes to "r"


    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(vPosition_location);
    glDisableVertexAttribArray(vColour_location);


    // Load the other debug objects
    cDebugRenderer_pMeshManager = new cVAOManager();
    cDebugRenderer_pMeshManager->setBasePath("assets/models");

    sModelDrawInfo sphereMeshInfo;
    cDebugRenderer_pMeshManager->LoadModelIntoVAO("Sphere_1_unit_Radius.ply", sphereMeshInfo, debugShaderProgramID);

    sModelDrawInfo cubeMeshInfo;
    cDebugRenderer_pMeshManager->LoadModelIntoVAO("Cube_1x1x1_xyz_n_rgba.ply", cubeMeshInfo, debugShaderProgramID);

    // We're good to go
	return true;
}

std::string cDebugRenderer::getLastError(void)
{
    std::string theLastError = this->m_lastError;
    this->m_lastError = "";
    return theLastError;
}

void cDebugRenderer::ShutDown(void)
{
    delete cDebugRenderer_pShaderManager;
}

void cDebugRenderer::AddLine(glm::vec3 startXYZ, glm::vec3 endXYZ, glm::vec4 RGBA)
{
    // Next line to add is at this->m_sizeOfLineVertexBuffer

    this->m_pLinesVerticesToDraw[this->m_sizeOfLineVertexBuffer + 0].x = startXYZ.x;
    this->m_pLinesVerticesToDraw[this->m_sizeOfLineVertexBuffer + 0].y = startXYZ.y;
    this->m_pLinesVerticesToDraw[this->m_sizeOfLineVertexBuffer + 0].z = startXYZ.z;

    this->m_pLinesVerticesToDraw[this->m_sizeOfLineVertexBuffer + 0].r = RGBA.r;
    this->m_pLinesVerticesToDraw[this->m_sizeOfLineVertexBuffer + 0].g = RGBA.g;
    this->m_pLinesVerticesToDraw[this->m_sizeOfLineVertexBuffer + 0].b = RGBA.b;
    this->m_pLinesVerticesToDraw[this->m_sizeOfLineVertexBuffer + 0].a = RGBA.a;

    this->m_pLinesVerticesToDraw[this->m_sizeOfLineVertexBuffer + 1].x = endXYZ.x;
    this->m_pLinesVerticesToDraw[this->m_sizeOfLineVertexBuffer + 1].y = endXYZ.y;
    this->m_pLinesVerticesToDraw[this->m_sizeOfLineVertexBuffer + 1].z = endXYZ.z;

    this->m_pLinesVerticesToDraw[this->m_sizeOfLineVertexBuffer + 1].r = RGBA.r;
    this->m_pLinesVerticesToDraw[this->m_sizeOfLineVertexBuffer + 1].g = RGBA.g;
    this->m_pLinesVerticesToDraw[this->m_sizeOfLineVertexBuffer + 1].b = RGBA.b;
    this->m_pLinesVerticesToDraw[this->m_sizeOfLineVertexBuffer + 1].a = RGBA.a;

    // Increment by two
    this->m_sizeOfLineVertexBuffer += 2;

    return;
}


void cDebugRenderer::RenderDebugObjects(double deltaTime, glm::mat4 matView, glm::mat4 matProjection)
{
    
    // Switch to the debug object shader

    // Get the shader ID
    GLuint shaderID = cDebugRenderer_pShaderManager->getIDFromFriendlyName("debugObjectShader");

    // Switch to that shader
    glUseProgram(shaderID);

    
    // Update (re-copy) the CPU/C++ side array to the vertex buffer
//    glBindBuffer(GL_ARRAY_BUFFER, cDebugRenderer_vertex_buffer);

    // Copies the data from the C++/CPU side to the GPU, overwriting whatever is there
    unsigned int numberOfBytesToCopy = sizeof(sLineVertex_Buffer) * this->m_sizeOfLineVertexBuffer;

//    glBufferData(GL_ARRAY_BUFFER,
//                 sizeof(numberOfBytesToCopy),
//                 this->m_pLinesVerticesToDraw,  // Pointer to the buffer on the CPU/RAM
//                 GL_DYNAMIC_DRAW);

    // Overwrite the contents of an exiting buffer
//    glGetBufferSubData(GL_ARRAY_BUFFER,
//                       0,
//                       numberOfBytesToCopy,
//                       this->m_pLinesVerticesToDraw);



    GLint mvp_location = glGetUniformLocation(shaderID, "MVP");

    glm::mat4 matModel = glm::mat4(1.0f);

    glm::mat4 matMVP = matProjection * matView * matModel;

    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(matMVP));


    glBindVertexArray(cDebugRenderer_VAO);

    //Draw however many lines are in the buffer currently.
    glDrawArrays(GL_LINES, 0, this->m_sizeOfLineVertexBuffer);

    glBindVertexArray(0);


    // To "delete" all the lines in the buffer, just set this back to zero:
//    this->m_sizeOfLineVertexBuffer = 0;



    //for ( unsigned int index = 0; index != this->m_vecSpheresToDraw.size(); index++ )
    //{
    //    // For the debug objects, if you are drawing the loaded 3D models, 
    //    //  you'll have to do all the same transformation stuff you do with the main render loop

    //        //         mat4x4_identity(m);
    //    glm::mat4 matModel = glm::mat4(1.0f);

    //    sSphere currentSphere = this->m_vecSpheresToDraw[index];


    //    // Translation
    //    glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
    //                                            glm::vec3(currentSphere.centreXYZ.x,
    //                                                      currentSphere.centreXYZ.y,
    //                                                      currentSphere.centreXYZ.z));


    //       // Scaling matrix
    //    glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
    //                                    glm::vec3(currentSphere.radius,
    //                                              currentSphere.radius,
    //                                              currentSphere.radius));
    //    //--------------------------------------------------------------

    //    // Combine all these transformation
    //    matModel = matModel * matTranslate;

    //    matModel = matModel * matScale;

    //    glm::mat4 mvp = matProjection * matView * matModel;

    //    GLint mvp_location = glGetUniformLocation(shaderID, "MVP");

    //    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //    sModelDrawInfo modelInfo;
    //    if (cDebugRenderer_pMeshManager->FindDrawInfoByModelName("Sphere_1_unit_Radius.ply", modelInfo))
    //    {
    //        // Found it!!!

    //        glBindVertexArray(modelInfo.VAO_ID); 		//  enable VAO (and everything else)
    //        glDrawElements(GL_TRIANGLES,
    //                       modelInfo.numberOfIndices,
    //                       GL_UNSIGNED_INT,
    //                       0);
    //        glBindVertexArray(0); 			            // disable VAO (and everything else)

    //    }

    //}

    //// Get rid of the sphere
    //this->m_vecSpheresToDraw.clear();



    // Before we return, we'll have to switch back
    //glGetProgramiv
    return;
}
