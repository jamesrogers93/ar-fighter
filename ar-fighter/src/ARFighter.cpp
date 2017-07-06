#include "ar-fighter/ARFighter.h"

// STD
#include <vector>
#include <utility>

// Game Engine
#include <game-engine/Core/Engine/Engine.h>

// Game Engine Device
#include <game-engine/Core/Device/System.h>
#include <game-engine/Core/Device/CameraCapture.h>
#include <game-engine/Core/Device/Gyroscope.h>
#include <game-engine/Core/Device/GLContext.h>

// Game Engine Graphics
#include <game-engine/Core/Modules/CoreModule.h>
#include <game-engine/Core/Modules/Graphics/Graphics.h>
#include <game-engine/Core/Modules/Graphics/GraphicsRead.h>
#include <game-engine/Core/Modules/Graphics/GeometryEntity.h>
#include <game-engine/Core/Modules/Graphics/CameraEntity.h>
#include <game-engine/Core/Modules/Graphics/PointLightEntity.h>
#include <game-engine/Core/Modules/Graphics/DirectionalLightEntity.h>
#include <game-engine/Core/Modules/Graphics/Geometry.h>
#include <game-engine/Core/Modules/Graphics/Material.h>
#include <game-engine/Core/Modules/Graphics/Vertex.h>
#include <game-engine/Core/Modules/Graphics/Shader.h>
#include <game-engine/Core/Modules/Graphics/Shaders.h>

// Game Engine AR Core Module
#include <game-engine/Core/Modules/AR/AR.h>
#include <game-engine/Core/Modules/AR/AREntity.h>
#include <game-engine/Core/Modules/AR/ARTracker.h>

// Game Engine Scene
#include <game-engine/Core/Scene/SceneManager.h>
#include <game-engine/Core/Scene/Scene.h>

ENode *TEST;
AREntity *TESTTRACK;

Scene* ARFighter::createScene()
{
    Scene* scene = new Scene("Fight");
    
    // Create Geometry entity
    // Here we are referencing a 'cube' geometric object, a 'wooden-box' material and a 'basic' shader
    GeometryEntity* entity = new GeometryEntity("origin", 0.0, 0.0, 0.0, "cube", "white", "basic");
    entity->initialise();
    entity->scale(10.0, 10.0, 10.0);
    scene->addEntity(entity);
    
    GeometryEntity* entity1 = new GeometryEntity("x", 0.0, 0.0, 0.0, "cylinder", "red", "basic");
    entity1->initialise();
    entity1->rotate(glm::radians(90.0), 0.0, 1.0, 0.0, 0);
    entity->addChild(entity1);
    
    GeometryEntity* entity2 = new GeometryEntity("y", 0.0, 0.0, 0.0, "cylinder", "green", "basic");
    entity2->initialise();
    entity2->rotate(glm::radians(-90.0), 1.0, 0.0, 0.0, 0);
    entity->addChild(entity2);
    
    GeometryEntity* entity3 = new GeometryEntity("z", 0.0, 0.0, 0.0, "cylinder", "blue", "basic");
    entity3->initialise();
    entity->addChild(entity3);
    
    // But we must create a geometric object (This automatically adds the geometry to the Graphics module)
    GraphicsRead::readJMPFile(this->assetsPath + "cube.jmp");
    GraphicsRead::readJMPFile(this->assetsPath + "axis.jmp");

    // and create a material object
    Material *m1 = new Material(glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(0.0, 0.0, 0.0, 1.0), 32);
    Material *m2 = new Material(glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec4(0.0, 0.0, 0.0, 1.0), 32);
    Material *m3 = new Material(glm::vec4(0.0, 1.0, 0.0, 1.0), glm::vec4(0.0, 0.0, 0.0, 1.0), 32);
    Material *m4 = new Material(glm::vec4(0.0, 0.0, 1.0, 1.0), glm::vec4(0.0, 0.0, 0.0, 1.0), 32);
    
    // and a shader object
    std::vector<std::pair<GLint, std::string> > vertexAttribs;
    vertexAttribs.push_back(std::make_pair(ATTRIB_POSITION, "position"));
    vertexAttribs.push_back(std::make_pair(ATTRIB_NORMAL, "normal"));
    vertexAttribs.push_back(std::make_pair(ATTRIB_TEXCOORD, "texCoord"));
    
    std::vector<std::string> uniformNames;
    CameraEntity::fillUniformNames(uniformNames);
    Material::fillUniformNames(uniformNames);
    PointLightEntity::fillUniformNames(uniformNames);
    DirectionalLightEntity::fillUniformNames(uniformNames);
    GeometryEntity::fillUniformNames(uniformNames);
    //Shader *s = Shader::loadShaderFromString(basicVertex, basicFragment, vertexAttribs, uniformNames);
    Shader *s = Shader::loadShaderFromFile(this->assetsPath + "basic.vert", this->assetsPath + "basic.frag", vertexAttribs, uniformNames);
    
    // Put the geometry, material and shader in the graphics object
    //Graphics *gModule = Engine::getInstance().getCoreModule<Graphics>(CM_GRAPHICS);
    Graphics *gModule = static_cast<Graphics*>(Engine::getInstance().getCoreModule(CM_GRAPHICS));
    //gModule->addGeometry("cube", g);
    gModule->addMaterial("white", m1);
    gModule->addMaterial("red", m2);
    gModule->addMaterial("green", m3);
    gModule->addMaterial("blue", m4);
    gModule->addShader("basic", s);
    
    // Now some lights to light the scene
    //PointLightEntity *l1 = new PointLightEntity("light1", glm::vec3(0.0, 3.0, 0.0), glm::vec3(0.0, 0.0, 0.2), glm::vec3(0.0, 0.0, 0.7), glm::vec3(0.0, 0.0, 0.9), Attenuation::ATT_100);
    //l1->initialise();
    /*PointLightEntity *l2 = new PointLightEntity("light2", glm::vec3(-2.0, 5.0, -10.0), glm::vec3(0.2,0.0,0.0), glm::vec3(0.7,0.0,0.0), glm::vec3(0.6,0.0,0.0), Attenuation::ATT_100);
    l2->initialise();*/
    DirectionalLightEntity *l3 = new DirectionalLightEntity("light3", glm::normalize(glm::vec3(0.0, -5.0, 10.0)), glm::vec3(1.0,1.0,1.0), glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,0.0,0.0));
    l3->initialise();
    
    // Add the graphics entity and light entity to the scene
    scene->addEntity(l3);
    
    // Create and add an AR entity
    AREntity *arEntity = new AREntity("backcamera", glm::vec3(0.0, 0.0, 10.0));
    scene->addEntity(arEntity);
    arEntity->initialise();
    arEntity->startCapture();
    CameraCapture::getInstance().addDelegate(arEntity);

    // Also need to add a camera entity to the scene
    CameraEntity *camEntity = new CameraEntity("myCamera", CameraEntity::perspectiveMatrix(640, 480, 47.0), 0.0, 0.0, 0.0);
    Graphics::getInstance().setActiveCameraEntity("myCamera");
    camEntity->initialise();
    arEntity->addChild(camEntity);
    
    TEST = entity;
    TESTTRACK = arEntity;

    return scene;
}

void ARFighter::initalise(const unsigned int &screenWidth, const unsigned int &screenHeight)
{
    
    // Initalise System which contains information about the device
    System::screenWidth = screenWidth;
    System::screenHeight = screenHeight;
    
    // Initialise OpenGL context
    GLContext::getInstance().initialise();
    GLContext::getInstance().makeCurrentContext();
    
    // Initialise Camera
    CameraCapture::getInstance().setResolution(CameraCapture::RES_640x480);
    CameraCapture::getInstance().initialise();
    
    // Initialise Gyroscope
    Gyroscope::getInstance().initialise();
    
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    
    // Get instance of the engine.
    Engine *engine = &Engine::getInstance();
    
    // Get instance of the scene manager.
    this->sceneManager = &SceneManager::getInstance();
    
    // Add the modules we want to use in this game to the engine.
    engine->addCoreModule(&Graphics::getInstance());
    engine->addCoreModule(&AR::getInstance());
    
    // Configure the graphics module.
    Graphics::getInstance().initalise();
    Graphics::getInstance().enableBackfaceCulling();
    
    // Configure the ar module
    AR::getInstance().initalise();
    
    // Add our scenes to the engine.
    
    // Character Selection Scene
    this->sceneManager->addScene(createScene());
    
    // Set the active scene
    this->sceneManager->makeActiveScene("Fight");
    
    AR::getInstance().setActiveAREntity("backcamera");
    
    // Enemy Selection Scene
    
    // Fighting Scene
    
    
}

void ARFighter::deinitalise()
{
    
}

void ARFighter::update()
{
    SceneManager::getInstance().update();
    
    /*if(TEST != 0)
    {
        TEST->rotate(glm::radians(1.0), glm::vec3(0.0, 0.0, 0.0));
    }*/
}

void ARFighter::pitch()
{
    if(TEST != 0)
    {
        TEST->rotate(glm::radians(5.0), glm::vec3(1.0, 0.0, 0.0),0);
    }
}

void ARFighter::yaw()
{
    if(TEST != 0)
    {
        TEST->rotate(glm::radians(5.0), glm::vec3(0.0, 1.0, 0.0),0);
    }
}


//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>
void ARFighter::roll()
{
    //static float degrees = 5.0f;
    
    if(TEST != 0)
    {
        TEST->rotate(glm::radians(5.0), glm::vec3(0.0, 0.0, 1.0), 0);
        
        
        /*glm::mat4 T = glm::translate(glm::mat4(), glm::vec3(3.0,0.0, 0.0));
        
        glm::fquat Q = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0, 1.0,0.0));
        Q = glm::normalize(Q);
        glm::mat4 R = glm::mat4_cast(Q);
        
        glm::fquat Q1 = glm::angleAxis(glm::radians(degrees++), glm::vec3(0.0, 0.0, 1.0));
        Q1 = glm::normalize(Q1);
        glm::mat4 R1 = glm::mat4_cast(Q1);
        
        //glm::mat4 R = glm::rotate(glm::mat4(), glm::radians(degrees+=5), glm::vec3(1.0, 0.0, 0.0));
        
        TEST->localModel = R1 * T * R;
        
        if(degrees > 360.0f)
            degrees = 0.0f;*/
    }
}

void ARFighter::track()
{
    if(TESTTRACK != 0)
    {
        TESTTRACK->startTracking();
    }
}

void ARFighter::draw()
{
    Engine *engine = &Engine::getInstance();
    
    // Update AR first
    std::vector<CoreModuleType> moduleAR;
    moduleAR.push_back(CM_AR);
    engine->updateIncluding(moduleAR);
    
    // Update Graphics second
    std::vector<CoreModuleType> moduleGrap;
    moduleGrap.push_back(CM_GRAPHICS);
    engine->updateIncluding(moduleGrap);
    
}
