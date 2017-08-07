#include "ar-fighter/Game_Objects/X_Bot.h"

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

/*void XBot::walk(const float &x, const float &z)
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
        
        mAnimator->getAnimationController1()->setAnimation("x_bot_walk_x+");
        mAnimator->getAnimationController1()->setLoop(true);
        mAnimator->getAnimationController1()->setSpeed(length);
        mAnimator->getAnimationController1()->setReverse(xReverse);
        mAnimator->getAnimationController1()->resetElapsedTime();
        mAnimator->getAnimationController1()->setAlpha(alphaA);
        mAnimator->getAnimationController1()->play();
        
        mAnimator->getAnimationController2()->setAnimation("x_bot_walk_z+");
        mAnimator->getAnimationController2()->setLoop(true);
        mAnimator->getAnimationController2()->setSpeed(length);
        mAnimator->getAnimationController2()->setReverse(zReverse);
        mAnimator->getAnimationController2()->resetElapsedTime();
        mAnimator->getAnimationController2()->setAlpha(alphaB);
        mAnimator->getAnimationController2()->play();
    }
}*/

/*void XBot::idle()
{
    if(mAnimator != NULL)
    {
        mAnimator->getAnimationController1()->setAnimation("x_bot_neutral_idle");
        mAnimator->getAnimationController1()->setLoop(true);
        mAnimator->getAnimationController1()->setSpeed(1.0f);
        mAnimator->getAnimationController1()->setReverse(false);
        mAnimator->getAnimationController1()->resetElapsedTime();
        mAnimator->getAnimationController1()->play();
        
        mAnimator->getAnimationController2()->stop();
    }
}*/

/*void XBot::initialise()
{
    
    Graphics *gModule = static_cast<Graphics*>(Engine::getInstance().getCoreModule(CoreModuleType::CM_GRAPHICS));
    AnimationModule *aModule = static_cast<AnimationModule*>(Engine::getInstance().getCoreModule(CoreModuleType::CM_ANIMATION));
    
    // x_bot
    
    //
    // IMPORTING ASSETS
    //
    
    // Load in stuff for the character
    
    // Skeleton
    EntityImporter entityImporterXBot;
    entityImporterXBot.ImportAsynchronously(System::assetsPath + "entites/X_Bot/X_Bot_Skeleton.jmpEntity");
    entityImporterXBot.join();
    
    // Properties
    PropertyImporter propertyImporterXBot1;
    propertyImporterXBot1.ImportAsynchronously(System::assetsPath + "properties/X_Bot/X_Bot_Surface.jmpProperty");
    
    PropertyImporter propertyImporterXBot2;
    propertyImporterXBot2.ImportAsynchronously(System::assetsPath + "properties/X_Bot/X_Bot_Joints.jmpProperty");
    
    propertyImporterXBot1.join();
    propertyImporterXBot2.join();
    
    // Materials
    MaterialImporter materialImporterXBot1;
    materialImporterXBot1.ImportAsynchronously(System::assetsPath + "materials/X_Bot/X_Bot_Surface.jmpMat");
    
    MaterialImporter materialImporterXBot2;
    materialImporterXBot2.ImportAsynchronously(System::assetsPath + "materials/X_Bot/X_Bot_Joints.jmpMat");
    
    materialImporterXBot1.join();
    materialImporterXBot2.join();
    
    // Meshes
    MeshImporter meshImporterXBot1;
    meshImporterXBot1.ImportAsynchronously(System::assetsPath + "meshes/X_Bot/X_Bot_Surface.jmpMesh");
    
    MeshImporter meshImporterXBot2;
    meshImporterXBot2.ImportAsynchronously(System::assetsPath + "meshes/X_Bot/X_Bot_Joints.jmpMesh");
    
    
    meshImporterXBot1.join();
    meshImporterXBot2.join();
    
    
    // Animations
    AnimationImporter animationImporterXBot1;
    animationImporterXBot1.ImportAsynchronously(System::assetsPath + "animations/X_Bot/X_Bot_walk_z+.jmpAnimation");
    
    AnimationImporter animationImporterXBot2;
    animationImporterXBot2.ImportAsynchronously(System::assetsPath + "animations/X_Bot/X_Bot_walk_x+.jmpAnimation");
    
    animationImporterXBot1.join();
    animationImporterXBot2.join();
    
    AnimationImporter animationImporterXBot3;
    animationImporterXBot3.ImportAsynchronously(System::assetsPath + "animations/X_Bot/X_Bot_neutral_idle.jmpAnimation");
    
    animationImporterXBot3.join();
    
    //
    // ANIMATIONS
    //
    
    // Add animations to the animation module
    aModule->addAnimation(animationImporterXBot1.getImportedObject()->getName(), animationImporterXBot1.getImportedObject());
    aModule->addAnimation(animationImporterXBot2.getImportedObject()->getName(), animationImporterXBot2.getImportedObject());
    aModule->addAnimation(animationImporterXBot3.getImportedObject()->getName(), animationImporterXBot3.getImportedObject());
    //aModule->addAnimation(animationImporterXBot4.getImportedObject()->getName(), animationImporterXBot4.getImportedObject());
    
    
    
    
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
    AnimatorProperty *animatorPropertyXBot1 = new AnimatorProperty("x_bot_animator", (JointEntity*)entityImporterXBot.getImportedObject());
    mAnimator = animatorPropertyXBot1;
    
    // Create the game object, character
    //GameObject *character = new GameObject("player");
    //character->translate(-150.0, 0.0, 0.0);
    //character->scale(0.02, 0.02, 0.02);
    addChild(entityImporterXBot.getImportedObject()); // Add joint entity to the game object
    addProperty(propertyImporterXBot1.getImportedObject()); // Add mesh property to the game object
    addProperty(propertyImporterXBot2.getImportedObject()); // Add mesh property to the game object
    addProperty(animatorPropertyXBot1);                      // Add animator property to the game object
    //mScene>addEntity(character);
    
    
    
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
}*/
