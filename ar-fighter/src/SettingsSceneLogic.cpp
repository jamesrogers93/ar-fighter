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

// Game Engine GUI
#include <game-engine/Modules/GUI/GUI.h>
#include <game-engine/Modules/GUI/GUIAnalog.h>

#include <game-engine/GameObject.h>

#include "ar-fighter/Game_Objects/Y_Bot.h"
#include "ar-fighter/Game_Objects/X_Bot.h"
YBot *test;

static unsigned int counter = 0;
void SettingsSceneLogic::update()
{
    // Update Animation module
    Engine::getInstance().update(CoreModuleType::CM_ANIMATION, false);
    
    /*std::cout << counter << std::endl;
    if(counter == 300)
    {
        //test->getAnimator()->switchAnimation("y_bot_walk_x");
        
        test->getAnimator()->getAnimationController1()->setReverse(false);
        test->getAnimator()->getAnimationController2()->setReverse(false);
    }
    else
    {
        counter++;
    }*/
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
    AnimationModule *aModule = static_cast<AnimationModule*>(Engine::getInstance().getCoreModule(CoreModuleType::CM_ANIMATION));
    
    
    YBot *yBot = new YBot();
    yBot->initialise();
    yBot->translate(-150.0, 0.0, 0.0);
    mScene->addEntity(yBot);
    
    //yBot->walk(1.0, 1.0);
    yBot->idle();
    test = yBot;
    
    
    // DELETE
    GUIAnalog *guiProperty = new GUIAnalog("Analog", 200.0f, 150.0f, glm::vec2(300.0, 300.0));
    guiProperty->setColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f));
    guiProperty->setDrawOnTouched(0.0, (float)System::screenWidth / 2.0f, (float)System::screenHeight, 0.0);
    
    test->addProperty(guiProperty);
    
    
    
    
    
    // X_Bot
    XBot *xBot = new XBot();
    xBot->initialise();
    xBot->translate(150.0, 0.0, 0.0);
    mScene->addEntity(xBot);
    
    //yBot->walk(1.0, 1.0);
    xBot->idle();
    
    /*// X_Bot
    
    //
    // IMPORTING ASSETS
    //
    
    // Load in stuff for the character
    
    // Skeleton
    EntityImporter entityImporterXBot;
    entityImporterXBot.ImportAsynchronously(System::assetsPath + "entites/X_Bot/X_Bot_Skeleton.jmpEntity");
    
    // Properties
    PropertyImporter propertyImporterXBot1;
    propertyImporterXBot1.ImportAsynchronously(System::assetsPath + "properties/X_Bot/X_Bot_Surface.jmpProperty");
    
    PropertyImporter propertyImporterXBot2;
    propertyImporterXBot2.ImportAsynchronously(System::assetsPath + "properties/X_Bot/X_Bot_Joints.jmpProperty");
    
    // Materials
    MaterialImporter materialImporterXBot1;
    materialImporterXBot1.ImportAsynchronously(System::assetsPath + "materials/X_Bot/X_Bot_Surface.jmpMat");
    
    MaterialImporter materialImporterXBot2;
    materialImporterXBot2.ImportAsynchronously(System::assetsPath + "materials/X_Bot/X_Bot_Joints.jmpMat");
    
    // Meshes
    MeshImporter meshImporterXBot1;
    meshImporterXBot1.ImportAsynchronously(System::assetsPath + "meshes/X_Bot/X_Bot_Surface.jmpMesh");
    
    MeshImporter meshImporterXBot2;
    meshImporterXBot2.ImportAsynchronously(System::assetsPath + "meshes/X_Bot/X_Bot_Joints.jmpMesh");
    
    // Animations
    AnimationImporter animationImporterXBot1;
    animationImporterXBot1.ImportAsynchronously(System::assetsPath + "animations/X_Bot/X_Bot_neutral_idle.jmpAnimation");
    
    /AnimationImporter animationImporterXBot2;
    //animationImporterXBot2.ImportAsynchronously(System::assetsPath + "animations/X_Bot/punch.jmpAnimation");
    
    // Wait till all assets are loaded in
    entityImporterXBot.join();
    propertyImporterXBot1.join();
    propertyImporterXBot2.join();
    materialImporterXBot1.join();
    materialImporterXBot2.join();
    meshImporterXBot1.join();
    meshImporterXBot2.join();
    animationImporterXBot1.join();
    //animationImporterXBot2.join();
    
    
    
    //
    // ANIMATIONS
    //
    
    // Add animations to the animation module
    aModule->addAnimation(animationImporterXBot1.getImportedObject()->getName(), animationImporterXBot1.getImportedObject());
    //aModule->addAnimation(animationImporterXBot2.getImportedObject()->getName(), animationImporterXBot2.getImportedObject());
    
    
    
    
    //
    // CHARACTER OBJECT
    //
    
    // Create animatable mesh properties
    AnimatableMeshProperty *meshPropertyXBot1 = (AnimatableMeshProperty*)propertyImporterXBot1.getImportedObject();
    meshPropertyXBot1->setShaderKey("basic");   // Set shader key
    meshPropertyXBot1->linkJoints(entityImporterXBot.getImportedObject()); // Link to imported skeleton
    
    AnimatableMeshProperty *meshPropertyXBot2 = (AnimatableMeshProperty*)propertyImporterXBot2.getImportedObject();
    meshPropertyXBot2->setShaderKey("basic");
    meshPropertyXBot2->linkJoints(entityImporterXBot.getImportedObject());
    
    // Create animator property
    AnimatorProperty *animatorPropertyXBot1 = new AnimatorProperty("opponent_animator", (JointEntity*)entityImporterXBot.getImportedObject());
    //animatorPropertyXBot1->play(AnimatorProperty::ANIMATOR_1, "x_bot_neutral_idle", true, 1.0f, false);
    
    // Create the game object, character
    GameObject *opponent = new GameObject("opponent");
    opponent->translate(150.0, 0.0, 0.0);
    //character->scale(0.02, 0.02, 0.02);
    opponent->addChild(entityImporterXBot.getImportedObject()); // Add joint entity to the game object
    opponent->addProperty(propertyImporterXBot1.getImportedObject()); // Add mesh property to the game object
    opponent->addProperty(propertyImporterXBot2.getImportedObject()); // Add mesh property to the game object
    opponent->addProperty(animatorPropertyXBot1);                      // Add animator property to the game object
    mScene->addEntity(opponent);
    
    //
    // MESHES
    //
    
    // Add the meshes to the graphics module
    MeshGL *meshGLXBot1 = MeshGL::loadMeshGL(meshImporterXBot1.getImportedObject()->getVertices(),
                                         meshImporterXBot1.getImportedObject()->getIndices());
    MeshGL *meshGLXBot2 = MeshGL::loadMeshGL(meshImporterXBot2.getImportedObject()->getVertices(),
                                         meshImporterXBot2.getImportedObject()->getIndices());
    
    gModule->addMesh(meshImporterXBot1.getImportedObject()->getName(), meshGLXBot1);
    gModule->addMesh(meshImporterXBot2.getImportedObject()->getName(), meshGLXBot2);
    
    
    
    
    
    //
    // MATERIALS
    //
    
    // Add the materials to the graphics module
    gModule->addMaterial(materialImporterXBot1.getImportedObject()->getName(), materialImporterXBot1.getImportedObject());
    gModule->addMaterial(materialImporterXBot2.getImportedObject()->getName(), materialImporterXBot2.getImportedObject());*/
    
    
    
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
