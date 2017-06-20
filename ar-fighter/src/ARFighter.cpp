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
    GeometryEntity* entity = new GeometryEntity("test", 0.0, 0.0, 0.0, "Cube","wooden-box", "basic");
    
    // But we must create a geometric object
    /*std::vector<Vertex3DPN> vertices;
    const float size = 0.5;
    vertices.push_back(Vertex3DPN( size,  size, 0.0, 0.0, 0.0, 1.0));
    vertices.push_back(Vertex3DPN( size, -size, 0.0, 0.0, 0.0, 1.0));
    vertices.push_back(Vertex3DPN(-size, -size, 0.0, 0.0, 0.0, 1.0));
    vertices.push_back(Vertex3DPN(-size,  size, 0.0, 0.0, 0.0, 1.0));
    
    std::vector<unsigned int> indices;
    indices.push_back(0); indices.push_back(1); indices.push_back(3);
    indices.push_back(1); indices.push_back(2); indices.push_back(3);
    
    Geometry *g = Geometry::loadGeometry(vertices, indices);*/
    
    GraphicsRead::readJMPFile(this->assetsPath + "cube.jmp");
    
    // and create a material object
    Material *m = new Material(glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec4(0.0, 0.0, 1.0, 1.0));
    
    // and a shader object
    std::vector<std::pair<GLint, std::string> > vertexAttribs;
    vertexAttribs.push_back(std::make_pair(ATTRIB_POSITION, "position"));
    vertexAttribs.push_back(std::make_pair(ATTRIB_NORMAL, "normal"));
    vertexAttribs.push_back(std::make_pair(ATTRIB_TEXCOORD, "texCoord"));
    
    std::vector<std::string> uniformNames;
    uniformNames.push_back("projection"); uniformNames.push_back("view");
    uniformNames.push_back("model");
    
    uniformNames.push_back("diffuseTexture"); uniformNames.push_back("specularTexture");
    uniformNames.push_back("diffuseSolid");   uniformNames.push_back("specularSolid");
    uniformNames.push_back("shininess");
    
    Shader *s = Shader::loadShaderFromString(basicVertex, basicFragment, vertexAttribs, uniformNames);
    
    // Put the geometry, material and shader in the graphics object
    //Graphics *gModule = Engine::getInstance().getCoreModule<Graphics>(CM_GRAPHICS);
    Graphics *gModule = static_cast<Graphics*>(Engine::getInstance().getCoreModule(CM_GRAPHICS));
    //gModule->addGeometry("square", g);
    gModule->addMaterial("wooden-box", m);
    gModule->addShader("basic", s);
    
    // Add the graphics entity to the scene
    scene->addEntity(entity);
    
    // Also need to add a camera entity to the scene
    
    CameraEntity *camEntity = new CameraEntity("myCamera", CameraEntity::perspectiveMatrix(this->screenWidth, this->screenHeight), 0.0, 0.0, -10.0);
    Graphics::getInstance().setActiveCameraEntity("myCamera");
    scene->addEntity(camEntity);
    
    // Update the scene to configure global model for each entity
    scene->update();
    
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
    
    // Add our scenes to the engine.
    
    // Character Selection Scene
    this->sceneManager->addScene(createScene());
    
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
