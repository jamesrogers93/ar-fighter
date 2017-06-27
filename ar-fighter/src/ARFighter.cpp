#include "ar-fighter/ARFighter.h"

// STD
#include <vector>
#include <utility>

// Game Engine
#include <game-engine/Core/Engine/Engine.h>

// GameEngine Core Modules
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

// Game Engine Scene
#include <game-engine/Core/Scene/SceneManager.h>
#include <game-engine/Core/Scene/Scene.h>

Scene* ARFighter::createScene()
{
    Scene* scene = new Scene("Fight");
    
    // Create Geometry entity
    // Here we are referencing a 'square' geometric object, a 'wooden-box' material and a 'basic' shader
    GeometryEntity* entity = new GeometryEntity("test", 0.0, 0.0, 0.0, "cube","wooden-box", "basic");
    
    // But we must create a geometric object (This automatically adds the geometry to the Graphics module)
    GraphicsRead::readJMPFile(this->assetsPath + "cube.jmp");

    // and create a material object
    Material *m = new Material(glm::vec4(0.6, 0.6, 0.6, 1.0), glm::vec4(1.0, 0.1, 0.1, 1.0), 32);
    
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
    gModule->addMaterial("wooden-box", m);
    gModule->addShader("basic", s);
    
    // Now some lights to light the scene
    PointLightEntity *l1 = new PointLightEntity("light1", glm::vec3(5.0, 0.5, 0.0), glm::vec3(0.0, 0.0, 0.2), glm::vec3(0.0, 0.0, 0.7), glm::vec3(0.0, 0.0, 0.9), Attenuation::ATT_100);
    PointLightEntity *l2 = new PointLightEntity("light2", glm::vec3(-2.0, 5.0, -10.0), glm::vec3(0.2,0.0,0.0), glm::vec3(0.7,0.0,0.0), glm::vec3(0.6,0.0,0.0), Attenuation::ATT_100);
    DirectionalLightEntity *l3 = new DirectionalLightEntity("light3", glm::normalize(glm::vec3(0.0, -5.0, 10.0)), glm::vec3(0.3,0.2,0.2), glm::vec3(0.7,0.0,0.0), glm::vec3(0.6,0.0,0.0));
    
    // Add the graphics entity and light entity to the scene
    scene->addEntity(entity);
    scene->addEntity(l1);
    scene->addEntity(l2);
    scene->addEntity(l3);
    
    // Also need to add a camera entity to the scene
    
    CameraEntity *camEntity = new CameraEntity("myCamera", CameraEntity::perspectiveMatrix(this->screenWidth, this->screenHeight), 2.0, 5.0, 10.0);
    Graphics::getInstance().setActiveCameraEntity("myCamera");
    scene->addEntity(camEntity);
    
    return scene;
}

void ARFighter::initalise(const unsigned int &screenWidth, const unsigned int &screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    
    // Get instance of the engine.
    Engine *engine = &Engine::getInstance();
    
    // Get instance of the scene manager.
    this->sceneManager = &SceneManager::getInstance();
    
    // Add the modules we want to use in this game to the engine.
    engine->addCoreModule(&Graphics::getInstance());
    
    // Configure the graphics module.
    Graphics::getInstance().enableBackfaceCulling();
    
    // Add our scenes to the engine.
    
    // Character Selection Scene
    this->sceneManager->addScene(createScene());
    
    // Set the active scene
    this->sceneManager->makeActiveScene("Fight");
    
    // Enemy Selection Scene
    
    // Fighting Scene
    
    
    
}

void ARFighter::deinitalise()
{
    
}

void ARFighter::update()
{
    SceneManager::getInstance().update();
}

void ARFighter::draw()
{
    Engine *engine = &Engine::getInstance();
    
    std::vector<CoreModuleType> modules;
    modules.push_back(CM_GRAPHICS);
    
    engine->updateIncluding(modules);
}
