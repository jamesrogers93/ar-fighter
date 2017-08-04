#include "ar-fighter/Game_Objects/Y_Bot.h"

// Game Engine Core
#include "game-engine/Core/Engine/Engine.h"

// Game Engine Device
#include <game-engine/Device/System.h>

#include <game-engine/Modules/CoreModule.h>

// Game Engine Graphics
#include <game-engine/Modules/Graphics/Graphics.h>
#include <game-engine/Modules/Graphics/AnimatableMeshProperty.h>
#include <game-engine/Modules/Graphics/Mesh.h>
#include <game-engine/Modules/Graphics/Material.h>

// Game Engine Animation
#include <game-engine/Modules/Animation/AnimationModule.h>
#include <game-engine/Modules/Animation/AnimatorProperty.h>
#include <game-engine/Modules/Animation/Animation.h>

// Game Engine Importers
#include <game-engine/Entity/EntityImporter.h>
#include <game-engine/Entity/PropertyImporter.h>
#include <game-engine/Modules/Graphics/MaterialImporter.h>
#include <game-engine/Modules/Graphics/MeshImporter.h>
#include <game-engine/Modules/Animation/AnimationImporter.h>

// STD
#include <math.h>

// GLM
#include <glm/glm.hpp>

void YBot::walk(const float &x, const float &z)
{
    
    if(mAnimator != NULL)
    {
        
        // We need to workout the values to give the animation controllers based on the direction
        bool xReverse = x < 0.0;
        bool zReverse = z < 0.0;
    
        //We will use this as the speed of the animation
        float length = glm::length(glm::vec2(x, z));
        
        // Now we need to workout the alphas
        // Normalise alphas to sum of 1
        float alphaA = fabs(x);
        float alphaB = fabs(z);
        float sum = alphaA + alphaB;
        alphaA = alphaA / sum;
        alphaB = alphaB / sum;
        
        mAnimator->getAnimationController1()->setAnimation("y_bot_walk_x+");
        mAnimator->getAnimationController1()->setLoop(true);
        mAnimator->getAnimationController1()->setSpeed(length);
        mAnimator->getAnimationController1()->setReverse(xReverse);
        mAnimator->getAnimationController1()->resetElapsedTime();
        mAnimator->getAnimationController1()->setAlpha(alphaA);
        mAnimator->getAnimationController1()->play();
        
        mAnimator->getAnimationController2()->setAnimation("y_bot_walk_z+");
        mAnimator->getAnimationController2()->setLoop(true);
        mAnimator->getAnimationController2()->setSpeed(length);
        mAnimator->getAnimationController2()->setReverse(zReverse);
        mAnimator->getAnimationController2()->resetElapsedTime();
        mAnimator->getAnimationController2()->setAlpha(alphaB);
        mAnimator->getAnimationController2()->play();
    }
}

void YBot::idle()
{
    if(mAnimator != NULL)
    {
        mAnimator->getAnimationController1()->setAnimation("y_bot_neutral_idle");
        mAnimator->getAnimationController1()->setLoop(true);
        mAnimator->getAnimationController1()->setSpeed(1.0f);
        mAnimator->getAnimationController1()->setReverse(false);
        mAnimator->getAnimationController1()->resetElapsedTime();
        mAnimator->getAnimationController1()->play();
        
        mAnimator->getAnimationController2()->stop();
    }
}

void YBot::initialise()
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
    entityImporterYBot.join();
    
    // Properties
    PropertyImporter propertyImporterYBot1;
    propertyImporterYBot1.ImportAsynchronously(System::assetsPath + "properties/Y_Bot/Y_Bot_Surface.jmpProperty");
    
    PropertyImporter propertyImporterYBot2;
    propertyImporterYBot2.ImportAsynchronously(System::assetsPath + "properties/Y_Bot/Y_Bot_Joints.jmpProperty");
    
    propertyImporterYBot1.join();
    propertyImporterYBot2.join();
    
    // Materials
    MaterialImporter materialImporterYBot1;
    materialImporterYBot1.ImportAsynchronously(System::assetsPath + "materials/Y_Bot/Y_Bot_Surface.jmpMat");
    
    MaterialImporter materialImporterYBot2;
    materialImporterYBot2.ImportAsynchronously(System::assetsPath + "materials/Y_Bot/Y_Bot_Joints.jmpMat");
    
    materialImporterYBot1.join();
    materialImporterYBot2.join();
    
    // Meshes
    MeshImporter meshImporterYBot1;
    meshImporterYBot1.ImportAsynchronously(System::assetsPath + "meshes/Y_Bot/Y_Bot_Surface.jmpMesh");
    
    MeshImporter meshImporterYBot2;
    meshImporterYBot2.ImportAsynchronously(System::assetsPath + "meshes/Y_Bot/Y_Bot_Joints.jmpMesh");
    
    
    meshImporterYBot1.join();
    meshImporterYBot2.join();
    
    
    // Animations
    AnimationImporter animationImporterYBot1;
    animationImporterYBot1.ImportAsynchronously(System::assetsPath + "animations/Y_Bot/Y_Bot_walk_z+.jmpAnimation");
    
    AnimationImporter animationImporterYBot2;
    animationImporterYBot2.ImportAsynchronously(System::assetsPath + "animations/Y_Bot/Y_Bot_walk_x+.jmpAnimation");
    
    animationImporterYBot1.join();
    animationImporterYBot2.join();
    
    AnimationImporter animationImporterYBot3;
    animationImporterYBot3.ImportAsynchronously(System::assetsPath + "animations/Y_Bot/Y_Bot_neutral_idle.jmpAnimation");
    
    animationImporterYBot3.join();
    
    //
    // ANIMATIONS
    //
    
    // Add animations to the animation module
    aModule->addAnimation(animationImporterYBot1.getImportedObject()->getName(), animationImporterYBot1.getImportedObject());
    aModule->addAnimation(animationImporterYBot2.getImportedObject()->getName(), animationImporterYBot2.getImportedObject());
    aModule->addAnimation(animationImporterYBot3.getImportedObject()->getName(), animationImporterYBot3.getImportedObject());
    //aModule->addAnimation(animationImporterYBot4.getImportedObject()->getName(), animationImporterYBot4.getImportedObject());
    
    
    
    
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
    AnimatorProperty *animatorPropertyYBot1 = new AnimatorProperty("y_bot_animator", (JointEntity*)entityImporterYBot.getImportedObject());
    mAnimator = animatorPropertyYBot1;
    
    // Create the game object, character
    //GameObject *character = new GameObject("player");
    //character->translate(-150.0, 0.0, 0.0);
    //character->scale(0.02, 0.02, 0.02);
    addChild(entityImporterYBot.getImportedObject()); // Add joint entity to the game object
    addProperty(propertyImporterYBot1.getImportedObject()); // Add mesh property to the game object
    addProperty(propertyImporterYBot2.getImportedObject()); // Add mesh property to the game object
    addProperty(animatorPropertyYBot1);                      // Add animator property to the game object
    //mScene>addEntity(character);
    
    
    
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
}
