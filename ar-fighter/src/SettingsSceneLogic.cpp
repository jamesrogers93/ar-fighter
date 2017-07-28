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
}

#include <fstream>
void SettingsSceneLogic::initialise()
{

    Graphics *gModule = static_cast<Graphics*>(Engine::getInstance().getCoreModule(CoreModuleType::CM_GRAPHICS));
    AnimationModule *aModule = static_cast<AnimationModule*>(Engine::getInstance().getCoreModule(CoreModuleType::CM_ANIMATION));
    
    
    
    // Y_Bot
    
    //
    // IMPORTING ASSETS
    //
    
    // Load in stuff for the character
    
    // Skeleton
    EntityImporter entityImporterYBot;
    entityImporterYBot.ImportAsynchronously(System::assetsPath + "entites/Y_Bot/Y_Bot_Skeleton.jmpEntity");
    
    // Properties
    PropertyImporter propertyImporterYBot1;
    propertyImporterYBot1.ImportAsynchronously(System::assetsPath + "properties/Y_Bot/Y_Bot_Surface.jmpProperty");
    
    PropertyImporter propertyImporterYBot2;
    propertyImporterYBot2.ImportAsynchronously(System::assetsPath + "properties/Y_Bot/Y_Bot_Joints.jmpProperty");
    
    // Materials
    MaterialImporter materialImporterYBot1;
    materialImporterYBot1.ImportAsynchronously(System::assetsPath + "materials/Y_Bot/Y_Bot_Surface.jmpMat");
    
    MaterialImporter materialImporterYBot2;
    materialImporterYBot2.ImportAsynchronously(System::assetsPath + "materials/Y_Bot/Y_Bot_Joints.jmpMat");
    
    // Meshes
    MeshImporter meshImporterYBot1;
    meshImporterYBot1.ImportAsynchronously(System::assetsPath + "meshes/Y_Bot/Y_Bot_Surface.jmpMesh");
    
    MeshImporter meshImporterYBot2;
    meshImporterYBot2.ImportAsynchronously(System::assetsPath + "meshes/Y_Bot/Y_Bot_Joints.jmpMesh");
    
    // Animations
    AnimationImporter animationImporterYBot1;
    animationImporterYBot1.ImportAsynchronously(System::assetsPath + "animations/Y_Bot/Y_Bot_neutral_idle.jmpAnimation");
    
    AnimationImporter animationImporterYBot2;
    animationImporterYBot2.ImportAsynchronously(System::assetsPath + "animations/Y_Bot/Y_Bot_punch.jmpAnimation");
    
    // Wait till all assets are loaded in
    entityImporterYBot.join();
    propertyImporterYBot1.join();
    propertyImporterYBot2.join();
    materialImporterYBot1.join();
    materialImporterYBot2.join();
    meshImporterYBot1.join();
    meshImporterYBot2.join();
    animationImporterYBot1.join();
    animationImporterYBot2.join();
    
    
    
    //
    // ANIMATIONS
    //
    
    // Add animations to the animation module
    aModule->addAnimation(animationImporterYBot1.getImportedObject()->getName(), animationImporterYBot1.getImportedObject());
    aModule->addAnimation(animationImporterYBot2.getImportedObject()->getName(), animationImporterYBot2.getImportedObject());
    
    
    
    
    //
    // CHARACTER OBJECT
    //
    
    // Create animatable mesh properties
    AnimatableMeshProperty *meshPropertyYBot1 = (AnimatableMeshProperty*)propertyImporterYBot1.getImportedObject();
    meshPropertyYBot1->setShaderKey("basic");   // Set shader key
    meshPropertyYBot1->linkJoints(entityImporterYBot.getImportedObject()); // Link to imported skeleton
    
    AnimatableMeshProperty *meshPropertyYBot2 = (AnimatableMeshProperty*)propertyImporterYBot2.getImportedObject();
    meshPropertyYBot2->setShaderKey("basic");
    meshPropertyYBot2->linkJoints(entityImporterYBot.getImportedObject());
    
    // Create animator property
    AnimatorProperty *animatorPropertyYBot1 = new AnimatorProperty("player_animator", (JointEntity*)entityImporterYBot.getImportedObject());
    animatorPropertyYBot1->play("y_bot_neutral_idle", true, 1.0f, false);
    
    // Create the game object, character
    GameObject *character = new GameObject("player");
    character->translate(-150.0, 0.0, 0.0);
    //character->scale(0.02, 0.02, 0.02);
    character->addChild(entityImporterYBot.getImportedObject()); // Add joint entity to the game object
    character->addProperty(propertyImporterYBot1.getImportedObject()); // Add mesh property to the game object
    character->addProperty(propertyImporterYBot2.getImportedObject()); // Add mesh property to the game object
    character->addProperty(animatorPropertyYBot1);                      // Add animator property to the game object
    mScene->addEntity(character);
    
    
    //
    // MESHES
    //
    
    // Add the meshes to the graphics module
    MeshGL *meshGLYBot1 = MeshGL::loadMeshGL(meshImporterYBot1.getImportedObject()->getVertices(),
                                         meshImporterYBot1.getImportedObject()->getIndices());
    MeshGL *meshGLYBot2 = MeshGL::loadMeshGL(meshImporterYBot2.getImportedObject()->getVertices(),
                                         meshImporterYBot2.getImportedObject()->getIndices());
    
    gModule->addMesh(meshImporterYBot1.getImportedObject()->getName(), meshGLYBot1);
    gModule->addMesh(meshImporterYBot2.getImportedObject()->getName(), meshGLYBot2);
    
    
    
    
    
    //
    // MATERIALS
    //
    
    // Add the materials to the graphics module
    gModule->addMaterial(materialImporterYBot1.getImportedObject()->getName(), materialImporterYBot1.getImportedObject());
    gModule->addMaterial(materialImporterYBot2.getImportedObject()->getName(), materialImporterYBot2.getImportedObject());
    
    
    
    // X_Bot
    
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
    
    /*AnimationImporter animationImporterXBot2;
    animationImporterXBot2.ImportAsynchronously(System::assetsPath + "animations/X_Bot/punch.jmpAnimation");*/
    
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
    animatorPropertyXBot1->play("x_bot_neutral_idle", true, 1.0f, false);
    
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
    gModule->addMaterial(materialImporterXBot2.getImportedObject()->getName(), materialImporterXBot2.getImportedObject());
    
    
    
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
