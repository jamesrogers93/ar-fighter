#include "ar-fighter/SettingsSceneLogic.h"

#include "game-engine/Scene/Scene.h"

// Game Engine Core
#include "game-engine/Core/GL/GL.h"

// Game Engine Device
#include <game-engine/Device/System.h>

#include <game-engine/Modules/CoreModule.h>

// Game Engine Graphics
#include <game-engine/Modules/Graphics/Graphics.h>
#include <game-engine/Modules/Graphics/AnimatableMeshProperty.h>
#include <game-engine/Modules/Graphics/CameraEntity.h>
#include <game-engine/Modules/Graphics/PointLightProperty.h>
#include <game-engine/Modules/Graphics/DirectionalLightProperty.h>
#include <game-engine/Modules/Graphics/Mesh.h>
#include <game-engine/Modules/Graphics/Material.h>
#include <game-engine/Modules/Graphics/Shader.h>

// Game Engine Animation
#include <game-engine/Modules/Animation/AnimationModule.h>
#include <game-engine/Modules/Animation/AnimatorProperty.h>
#include <game-engine/Modules/Animation/Animation.h>
#include <game-engine/Modules/Animation/AnimationImporter.h>

// Game Engine Importers
#include <game-engine/Entity/EntityImporter.h>
#include <game-engine/Entity/PropertyImporter.h>
#include <game-engine/Modules/Graphics/MaterialImporter.h>
#include <game-engine/Modules/Graphics/MeshImporter.h>

#include <game-engine/GameObject.h>

#include "ar-fighter/Game_Objects/Y_Bot.h"
#include "ar-fighter/Game_Objects/X_Bot.h"

void SettingsSceneLogic::update()
{
    // Update Animation module
    Engine::getInstance().update(CoreModuleType::CM_ANIMATION, false);
}

void SettingsSceneLogic::draw()
{
    jmpGLClearColour(0.65, 0.65, 0.65, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Engine *engine = &Engine::getInstance();
    
    // Update AR first
    engine->update(CoreModuleType::CM_AR);
    
    // Update Graphics second
    engine->update(CoreModuleType::CM_GRAPHICS);

    // Update the GUI
    engine->update(CoreModuleType::CM_GUI);
}

void SettingsSceneLogic::initialise()
{
    Graphics *gModule = static_cast<Graphics*>(Engine::getInstance().getCoreModule(CoreModuleType::CM_GRAPHICS));
    
    // YBot
    YBot *yBot = new YBot();
    yBot->initialise();
    yBot->translate(-150.0, 0.0, 0.0);
    mScene->addEntity(yBot);
    yBot->idle();
    player = yBot;
    
    // X_Bot
    XBot *xBot = new XBot();
    xBot->initialise();
    xBot->translate(150.0, 0.0, 0.0);
    mScene->addEntity(xBot);
    xBot->idle();
    opponent = xBot;
    
    
    //
    // SHADER
    //
    
    // Add a shader object to the graphics module
    std::vector<std::pair<GLint, std::string> > vertexAttribs;
    vertexAttribs.push_back(std::make_pair(ATTRIB_POSITION, "position"));
    vertexAttribs.push_back(std::make_pair(ATTRIB_NORMAL, "normal"));
    vertexAttribs.push_back(std::make_pair(ATTRIB_UV0, "uv0"));
    vertexAttribs.push_back(std::make_pair(ATTRIB_JOINT_ID, "joint_id"));
    vertexAttribs.push_back(std::make_pair(ATTRIB_JOINT_WEIGHT, "joint_weight"));
    
    std::vector<std::string> uniformNames;
    CameraEntity::fillUniformNames(uniformNames);
    Material::fillUniformNames(uniformNames);
    PointLightProperty::fillUniformNames(uniformNames);
    DirectionalLightProperty::fillUniformNames(uniformNames);
    AnimatableMeshProperty::fillUniformNames(uniformNames);
    //Shader *s = Shader::loadShaderFromFile(this->assetsPath + "shaders/basic.vert", this->assetsPath + "shaders/basic.frag", vertexAttribs, uniformNames);
    Shader *s = Shader::loadShaderFromFile(System::assetsPath + "shaders/animatable.vert", System::assetsPath + "shaders/basic.frag", vertexAttribs, uniformNames);
    gModule->addShader("basic", s);
    
    
    //
    // LIGHT
    //
    
    // Add a light to the scene
    GameObject *sun = new GameObject("sun");
    DirectionalLightProperty *light = new DirectionalLightProperty("light", glm::normalize(glm::vec3(0.0, -5.0, 10.0)), glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.6, 0.6, 0.6), glm::vec3(0.8, 0.8, 0.8));
    sun->addProperty(light);
    mScene->addEntity(sun);
    
    
    
    
    //
    // AR
    //
    
    // Create and add an AR entity
    /*AREntity *arEntity = new AREntity("backcamera");
     arEntity->translate(0.0, 0.0, 500.0);
     scene->addEntity(arEntity);
     arEntity->initialise();
     arEntity->startCapture();
     CameraCapture::getInstance().addDelegate(arEntity);
     AR::getInstance().setActiveAREntity("backcamera");*/
    
    
    
    
    //
    // CAMERA
    //
    
    // Also need to add a camera entity to the scene
    CameraEntity *camEntity = new CameraEntity("glCamera", CameraEntity::perspectiveMatrix(System::screenWidth, System::screenHeight, 47.0));
    camEntity->translate(0.0, 100.0, 300.0);
    camEntity->initialise();
    Graphics::getInstance().setActiveCameraEntity("glCamera");
    //arEntity->addChild(camEntity);
    mScene->addEntity(camEntity);
}
