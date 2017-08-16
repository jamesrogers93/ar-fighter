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

MeshGL* createSphere(const float &radius)
{
    int lats = 40;
    int longs = 40;
    int i, j;
    std::vector<VertexP> vertices;
    std::vector<GLuint> indices;
    int indicator = 0;
    for(i = 0; i <= lats; i++) {
        double lat0 = glm::pi<double>() * (-0.5 + (double) (i - 1) / lats);
        double z0  = sin(lat0);
        double zr0 =  cos(lat0);
        
        double lat1 = glm::pi<double>() * (-0.5 + (double) i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);
        
        for(j = 0; j <= longs; j++) {
            double lng = 2 * glm::pi<double>() * (double) (j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);
            
            VertexP v1;
            v1.position = glm::vec3(x * zr0 * radius, y * zr0 * radius, z0 * radius);
            vertices.push_back(v1);
            indices.push_back(indicator);
            indicator++;
            
            VertexP v2;
            v2.position = glm::vec3(x * zr1 * radius, y * zr1 * radius, z1 * radius);
            vertices.push_back(v2);
            indices.push_back(indicator);
            indicator++;
        }
        indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    }
    
    return MeshGL::loadMeshGL(vertices, indices);
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
    
    
    unsigned short yBotMask = 1;
    unsigned short xBotMask = 2;
    
    // YBot
    YBot *yBot = new YBot(yBotMask, xBotMask);
    yBot->initialise();
    yBot->translate(-150.0, 0.0, 0.0);
    mScene->addEntity(yBot);
    yBot->idle();
    player = yBot;
    
    // X_Bot
    XBot *xBot = new XBot(xBotMask, yBotMask);
    xBot->initialise();
    xBot->translate(150.0, 0.0, 0.0);
    mScene->addEntity(xBot);
    xBot->idle();
    opponent = xBot;
    
    
    //
    // SHADER
    //
    
    // Shader 1
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
    Shader *s1 = Shader::loadShaderFromFile(System::assetsPath + "shaders/animatable.vert", System::assetsPath + "shaders/lighting.frag", vertexAttribs, uniformNames);
    s1->setHasLighting(true);
    s1->setHasCamera(true);
    gModule->addShader("lighting", s1);
    
    
    // Shader 2
    vertexAttribs.clear();
    vertexAttribs.push_back(std::make_pair(ATTRIB_POSITION, "position"));
    
    uniformNames.clear();
    CameraEntity::fillUniformNames(uniformNames);
    Material::fillUniformNames(uniformNames);
    MeshProperty::fillUniformNames(uniformNames);

    Shader *s2 = Shader::loadShaderFromFile(System::assetsPath + "shaders/basic.vert", System::assetsPath + "shaders/basic.frag", vertexAttribs, uniformNames);
    s2->setHasLighting(false);
    s2->setHasCamera(true);
    gModule->addShader("basic", s2);
    
    //
    //  Mesh
    //
    gModule->addMesh("sphere", createSphere(1.0f));
    
    
    //
    // LIGHT
    //
    
    // Add a light to the scene
    GameObject *sun = new GameObject("sun");
    DirectionalLightProperty *light = new DirectionalLightProperty("light", glm::normalize(glm::vec3(0.0, -5.0, 10.0)), glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.6, 0.6, 0.6), glm::vec3(0.8, 0.8, 0.8));
    sun->addProperty(light);
    mScene->addEntity(sun);
    

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
