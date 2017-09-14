#include "ar-fighter/ARFighter.h"

// STD
#include <vector>
#include <utility>

// Game Engine Defines
#include <game-engine/Defines/OpenGL.h>

// Game Engine Core
#include <game-engine/Core/Engine/Engine.h>
#include <game-engine/Core/GL/GL.h>


// Game Engine Device
#include <game-engine/Device/System.h>
#include <game-engine/Device/CameraCapture.h>
#include <game-engine/Device/Gyroscope.h>
#include <game-engine/Core/GL/GLContext.h>

// Game Engine Graphics
#include <game-engine/Modules/CoreModule.h>
#include <game-engine/Modules/Graphics/Graphics.h>
#include <game-engine/Modules/Graphics/GraphicsRead.h>
#include <game-engine/Modules/Graphics/MeshProperty.h>
#include <game-engine/Modules/Graphics/AnimatableMeshProperty.h>
#include <game-engine/Modules/Graphics/CameraEntity.h>
#include <game-engine/Modules/Graphics/PointLightProperty.h>
#include <game-engine/Modules/Graphics/DirectionalLightProperty.h>
#include <game-engine/Modules/Graphics/Mesh.h>
#include <game-engine/Modules/Graphics/Material.h>
#include <game-engine/Modules/Graphics/Vertex.h>
#include <game-engine/Modules/Graphics/Shader.h>
#include <game-engine/Modules/Graphics/Shaders.h>

// Game Engine AR Module
#include <game-engine/Modules/AR/AR.h>
#include <game-engine/Modules/AR/AREntity.h>
#include <game-engine/Modules/AR/ARTracker.h>

// Game Engine Animation Module
#include <game-engine/Modules/Animation/AnimationModule.h>
#include <game-engine/Modules/Animation/AnimatorProperty.h>
#include <game-engine/Modules/Animation/Animation.h>
#include <game-engine/Modules/Animation/AnimationImporter.h>

// Game Engine Physics Module
#include <game-engine/Modules/Physics/Physics.h>

// Game Engine GUI Module
#include <game-engine/Modules/GUI/GUI.h>

// Game Engine AI Module
#include <game-engine/Modules/AI/AI.h>

// Game Engine Audio Module
#include <game-engine/Modules/Audio/Audio.h>

// Game Engine Scene
#include <game-engine/Scene/SceneManager.h>
#include <game-engine/Scene/Scene.h>

// Game Engine Importers
#include <game-engine/Entity/EntityImporter.h>
#include <game-engine/Entity/PropertyImporter.h>
#include <game-engine/Modules/Graphics/MaterialImporter.h>
#include <game-engine/Modules/Graphics/MeshImporter.h>

#include <game-engine/GameObject.h>

// ARFighter Scene Logic
#include "ar-fighter/MainMenuSceneLogic.h"
#include "ar-fighter/FightSceneLogic.h"
#include "ar-fighter/SettingsSceneLogic.h"

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


void ARFighter::initialise(const unsigned int &screenWidth, const unsigned int &screenHeight, const std::string &assetsPath)
{
    // initialise System which contains information about the device
    System::screenWidth = screenWidth;
    System::screenHeight = screenHeight;
    System::assetsPath = assetsPath;
    
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
    engine->addCoreModule(&AnimationModule::getInstance());
    engine->addCoreModule(&Physics::getInstance());
    engine->addCoreModule(&GUI::getInstance());
    engine->addCoreModule(&AI::getInstance());
    engine->addCoreModule(&Audio::getInstance());
    
    // Configure the graphics module.
    Graphics::getInstance().initialise();
    Graphics::getInstance().enableBackfaceCulling();
    Graphics::getInstance().enableBlend();
    
    // Configure the ar module
    AR::getInstance().initialise();
    
    // Configure physics module
    Physics::getInstance().initialise();
    
    // Configure GUI module
    GUI::getInstance().initialise();
    
    // Configure AI module
    AI::getInstance().initialise();
    
    // Configure Audio module
    Audio::getInstance().initialise();
    
    // Add some stuff to the engine
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
    Graphics::getInstance().addShader("lighting", s1);
    
    
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
    Graphics::getInstance().addShader("basic", s2);
    
    //
    //  Mesh
    //
    Graphics::getInstance().addMesh("sphere", createSphere(1.0f));
    
    
    // Set up the scenes that will be used
    Scene *scene = new Scene("main-menu");
    MainMenuSceneLogic *mainMenuSceneLogic = new MainMenuSceneLogic(scene);
    scene->setSceneLogic(mainMenuSceneLogic);
    this->sceneManager->addScene(scene);
    
    scene = new Scene("settings");
    SettingsSceneLogic *settingsSceneLogic = new SettingsSceneLogic(scene);
    scene->setSceneLogic(settingsSceneLogic);
    this->sceneManager->addScene(scene);
    
    scene = new Scene("play");
    FightSceneLogic *fightSceneLogic = new FightSceneLogic(scene);
    scene->setSceneLogic(fightSceneLogic);
    this->sceneManager->addScene(scene);
    
    // Initalise the main menu scene
    scene = this->sceneManager->getScene("main-menu");
    scene->initialise();
    this->sceneManager->makeActiveScene("main-menu");
    
}

void ARFighter::deinitialise()
{
    
}

void ARFighter::update()
{
    SceneManager::getInstance().update();
}

void ARFighter::draw()
{
    SceneManager::getInstance().draw();
}

void ARFighter::touchDown(const float &x, const float &y)
{
    GUI::getInstance().touchDown(x, y);
}

void ARFighter::touchMove(const float &x, const float &y)
{
    GUI::getInstance().touchMove(x, y);
}

void ARFighter::touchUp(const float &x, const float &y)
{
    GUI::getInstance().touchUp(x, y);
}

/*void ARFighter::pitch()
{
    if(TEST != 0)
    {
        //TEST->rotate(glm::radians(5.0), glm::vec3(1.0, 0.0, 0.0),0);
        
        const std::vector<Property*> *properties = &TEST->getProperties();
        
        for(unsigned int i = 0; i < properties->size(); i++)
        {
            if((*properties)[i]->getType() == Property::ANIMATOR)
            {
                AnimatorProperty *animProp = (AnimatorProperty*)(*properties)[i];
                animProp->play("punch", true);
            }
        }
    }
}*/

/*void ARFighter::yaw()
{
    if(TEST != 0)
    {
        //TEST->rotate(glm::radians(5.0), glm::vec3(0.0, 1.0, 0.0),0);
    }
}*/

/*void ARFighter::roll()
{
    
    if(TEST != 0)
    {
        //TEST->rotate(glm::radians(5.0), glm::vec3(0.0, 0.0, 1.0), 0);
    }
    
    //std::function<void()> myFunction = std::bind(task, 34, 93);
    //Task myTask = Task(myFunction);
    //dispatchQueue.sendToQueue(myTask);
    
}*/

/*void ARFighter::track()
{
    if(TESTTRACK != 0)
    {
        TESTTRACK->startTracking();
    }
}*/




// Create Geometry entity
// Here we are referencing a 'cube' geometric object, a 'wooden-box' material and a 'basic' shader
//MeshProperty* property = new MeshProperty("surface", "Alpha_SurfaceMesh", "mat1", "basic");
//property->initialise();
//property->scale(0.02, 0.02, 0.02);
//scene->addEntity(entity);

//MeshProperty* property1 = new MeshProperty("joints", "Alpha_JointsMesh", "mat2", "basic");
//property1->initialise();
//property1->scale(0.02, 0.02, 0.02);
//scene->addEntity(entity1);

//GraphicsRead::readJMPFile(this->assetsPath + "test.jmp");

//Material *m1 = new Material(glm::vec4(0.8, 0.1, 0.2, 1.0), glm::vec4(0.8, 0.1, 0.2, 1.0), 32);
//gModule->addMaterial("mat1", m1);

//Material *m2 = new Material(glm::vec4(0.4, 0.05, 0.1, 1.0), glm::vec4(0.4, 0.05, 0.1, 1.0), 32);
//gModule->addMaterial("mat2", m2);

/*GeometryEntity* entity = new GeometryEntity("origin", 0.0, 0.0, 0.0, "cube", "white", "basic");
 entity->initialise();
 //entity->scale(10.0, 10.0, 10.0);
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
 gModule->addMaterial("white", m1);
 gModule->addMaterial("red", m2);
 gModule->addMaterial("green", m3);
 gModule->addMaterial("blue", m4);*/

// and a shader object
/*std::vector<std::pair<GLint, std::string> > vertexAttribs;
 vertexAttribs.push_back(std::make_pair(ATTRIB_POSITION, "position"));
 vertexAttribs.push_back(std::make_pair(ATTRIB_NORMAL, "normal"));
 vertexAttribs.push_back(std::make_pair(ATTRIB_UV0, "texCoord"));
 
 std::vector<std::string> uniformNames;
 CameraEntity::fillUniformNames(uniformNames);
 Material::fillUniformNames(uniformNames);
 PointLightProperty::fillUniformNames(uniformNames);
 DirectionalLightProperty::fillUniformNames(uniformNames);
 MeshProperty::fillUniformNames(uniformNames);
 Shader *s = Shader::loadShaderFromFile(this->assetsPath + "basic.vert", this->assetsPath + "basic.frag", vertexAttribs, uniformNames);
 gModule->addShader("basic", s);*/

// Put the geometry, material and shader in the graphics object
//Graphics *gModule = Engine::getInstance().getCoreModule<Graphics>(CM_GRAPHICS);
//gModule->addGeometry("cube", g);

// Now some lights to light the scene
//PointLightEntity *l1 = new PointLightEntity("light1", glm::vec3(0.0, 3.0, 0.0), glm::vec3(0.0, 0.0, 0.2), glm::vec3(0.0, 0.0, 0.7), glm::vec3(0.0, 0.0, 0.9), Attenuation::ATT_100);
//l1->initialise();
/*PointLightEntity *l2 = new PointLightEntity("light2", glm::vec3(-2.0, 5.0, -10.0), glm::vec3(0.2,0.0,0.0), glm::vec3(0.7,0.0,0.0), glm::vec3(0.6,0.0,0.0), Attenuation::ATT_100);
 l2->initialise();*/
