#include "ar-fighter/Game_Objects/Character.h"

// Game Engine Core
#include "game-engine/Core/Engine/Engine.h"

// Game Engine Device
#include <game-engine/Device/System.h>

#include <game-engine/Modules/CoreModule.h>

// Game Engine Util
#include <game-engine/Util/TimeUtil.h>
// Game Engine Graphics
#include <game-engine/Modules/Graphics/Graphics.h>
#include <game-engine/Modules/Graphics/AnimatableMeshProperty.h>
#include <game-engine/Modules/Graphics/Mesh.h>
#include <game-engine/Modules/Graphics/Material.h>

// Game Engine Animation
#include <game-engine/Modules/Animation/AnimationModule.h>
#include <game-engine/Modules/Animation/AnimatorProperty.h>
#include <game-engine/Modules/Animation/Animation.h>
#include <game-engine/Modules/Animation/JointEntity.h>

// Game Engine Physics
#include  <game-engine/Modules/Physics/PhysicsProperty.h>
#include  <game-engine/Modules/Physics/PhysicsSphere.h>

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

// Bullet
#include <btBulletCollisionCommon.h>

const float Character::MAX_HEALTH = 100.0f;
const float Character::DAMAGE_INFLICT = 30.0f;

Character::Character(const std::string &name, unsigned short collisionMask, unsigned short collidesWithMask, const glm::vec4 &colourTheme)
    : GameObject(name),
    mAnimator(NULL),
    state(IDLE),
    collisionMask(collisionMask),
    collidesWithMask(collidesWithMask),
    mThemeColour(colourTheme),
    maxHealth(MAX_HEALTH),
    health(MAX_HEALTH),
    damageInflict(DAMAGE_INFLICT),
    canDealDamage(false)
{
    /*physicsMeshMap =
    {
        {name + "_head", headPhysicsMeshProperty},
        {name + "_torso", torsoPhysicsMeshProperty},
        {name + "_lefthand", leftHandPhysicsMeshProperty},
        {name + "_righthand", rightHandPhysicsMeshProperty}
    };*/
    physicsMeshesInitialsed = false;
    speed = 1.0f;
}

void Character::walk(const float &x, const float &z)
{
    
    if(mAnimator != NULL)
    {
        
        // We need to workout the values to give the animation controllers based on the direction
        bool xReverse = x < 0.0;
        bool zReverse = z < 0.0;
        
        // We will use this as the speed of the animation
        float length = glm::length(glm::vec2(x, z)) * speed;;
        
        // Now we need to workout the alphas
        // Normalise alphas to sum of 1
        float alphaA = fabs(x);
        float alphaB = fabs(z);
        float sum = alphaA + alphaB;
        alphaA = alphaA / sum;
        alphaB = alphaB / sum;
        
        mAnimator->getAnimationController1()->setAnimation(name + "_walk_x+");
        mAnimator->getAnimationController1()->setLoop(true);
        mAnimator->getAnimationController1()->setSpeed(length);
        mAnimator->getAnimationController1()->setReverse(xReverse);
        //mAnimator->getAnimationController1()->resetElapsedTime();
        mAnimator->getAnimationController1()->setAlpha(alphaA);
        mAnimator->getAnimationController1()->play();
        
        mAnimator->getAnimationController2()->setAnimation(name + "_walk_z+");
        mAnimator->getAnimationController2()->setLoop(true);
        mAnimator->getAnimationController2()->setSpeed(length);
        mAnimator->getAnimationController2()->setReverse(zReverse);
        //mAnimator->getAnimationController2()->resetElapsedTime();
        mAnimator->getAnimationController2()->setAlpha(alphaB);
        mAnimator->getAnimationController2()->play();
        
        
        // Now translate character
        
        // Get normalised direction of player movement
        glm::vec3 direction = glm::normalize(glm::vec3(x, 0.0, z));
        
        // Get the translation distance
        glm::vec3 t = glm::vec3(20.0f, 0.0f, 50.0);
        
        // Multiply by direction to translate in direction of walking direction
        t *= direction;
        
        // Multiply by length (speed) to account for slower walking speeds
        t *= length;
        
        walkTranslation = t;
        
        // Set the state
        state = WALKING;
        
        // Finally translate
        //translate(t);
    }
}

void Character::idle()
{
    if(mAnimator != NULL)
    {
        mAnimator->getAnimationController1()->setAnimation(name + "_neutral_idle");
        mAnimator->getAnimationController1()->setLoop(true);
        mAnimator->getAnimationController1()->setSpeed(1.0f);
        mAnimator->getAnimationController1()->setReverse(false);
        mAnimator->getAnimationController1()->resetElapsedTime();
        mAnimator->getAnimationController1()->play();
        
        mAnimator->getAnimationController2()->stop();
        
        state = IDLE;
    }
}

void Character::punch()
{
    if(state == PUNCHING || state == BLOCKING)
    {
        return;
    }
        
    if(mAnimator != NULL)
    {
        std::function<void(void)> callback = std::bind(&Character::actionCallback, this);
        mAnimator->getAnimationController1()->setCallback(callback);
        mAnimator->getAnimationController1()->setAnimation(name + "_punch_4");
        mAnimator->getAnimationController1()->setLoop(false);
        mAnimator->getAnimationController1()->setSpeed(2.0f);
        mAnimator->getAnimationController1()->setReverse(false);
        mAnimator->getAnimationController1()->resetElapsedTime();
        mAnimator->getAnimationController1()->play();
        
        mAnimator->getAnimationController2()->stop();
        
        state = PUNCHING;
        canDealDamage = true;
    }
}

void Character::kick()
{
    if(mAnimator != NULL)
    {
        
    }
}

void Character::block()
{
    if(state == PUNCHING || state == BLOCKING)
    {
        return;
    }
    
    if(mAnimator != NULL)
    {
        std::function<void(void)> callback = std::bind(&Character::actionCallback, this);
        mAnimator->getAnimationController1()->setCallback(callback);
        mAnimator->getAnimationController1()->setAnimation(name + "_block");
        mAnimator->getAnimationController1()->setLoop(false);
        mAnimator->getAnimationController1()->setSpeed(2.0f);
        mAnimator->getAnimationController1()->setReverse(false);
        mAnimator->getAnimationController1()->resetElapsedTime();
        mAnimator->getAnimationController1()->play();
        
        mAnimator->getAnimationController2()->stop();
        
        state = BLOCKING;
        canDealDamage = false;
    }
}

void Character::win()
{
    if(mAnimator != NULL)
    {
        mAnimator->getAnimationController1()->setAnimation(name + "_win");
        mAnimator->getAnimationController1()->setLoop(false);
        mAnimator->getAnimationController1()->setSpeed(1.0f);
        mAnimator->getAnimationController1()->setReverse(false);
        mAnimator->getAnimationController1()->resetElapsedTime();
        mAnimator->getAnimationController1()->play();
        
        mAnimator->getAnimationController2()->stop();
        
        state = WIN;
    }
}

void Character::lose()
{
    if(mAnimator != NULL)
    {
        mAnimator->getAnimationController1()->setAnimation(name + "_die");
        mAnimator->getAnimationController1()->setLoop(false);
        mAnimator->getAnimationController1()->setSpeed(1.0f);
        mAnimator->getAnimationController1()->setReverse(false);
        mAnimator->getAnimationController1()->resetElapsedTime();
        mAnimator->getAnimationController1()->play();
        
        mAnimator->getAnimationController2()->stop();
        
        state = LOSE;
    }
}

void Character::actionCallback()
{
    state = IDLE;
}

void Character::initialise()
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
    entityImporterYBot.ImportAsynchronously(System::assetsPath + "entites/" + name + "/" + name + "_Skeleton.jmpEntity");
    entityImporterYBot.join();
    
    // Properties
    PropertyImporter propertyImporterYBot1;
    propertyImporterYBot1.ImportAsynchronously(System::assetsPath + "properties/" + name + "/" + name + "_Surface.jmpProperty");
    
    PropertyImporter propertyImporterYBot2;
    propertyImporterYBot2.ImportAsynchronously(System::assetsPath + "properties/" + name + "/" + name + "_Joints.jmpProperty");
    
    propertyImporterYBot1.join();
    propertyImporterYBot2.join();
    
    // Materials
    MaterialImporter materialImporterYBot1;
    materialImporterYBot1.ImportAsynchronously(System::assetsPath + "materials/" + name + "/" + name + "_Surface.jmpMat");
    
    MaterialImporter materialImporterYBot2;
    materialImporterYBot2.ImportAsynchronously(System::assetsPath + "materials/" + name + "/" + name + "_Joints.jmpMat");
    
    materialImporterYBot1.join();
    materialImporterYBot2.join();
    
    // Meshes
    MeshImporter meshImporterYBot1;
    meshImporterYBot1.ImportAsynchronously(System::assetsPath + "meshes/" + name + "/" + name + "_Surface.jmpMesh");
    
    MeshImporter meshImporterYBot2;
    meshImporterYBot2.ImportAsynchronously(System::assetsPath + "meshes/" + name + "/" + name + "_Joints.jmpMesh");
    
    
    meshImporterYBot1.join();
    meshImporterYBot2.join();
    
    
    // Animations
    AnimationImporter animationImporterYBot1;
    animationImporterYBot1.ImportAsynchronously(System::assetsPath + "animations/" + name + "/" + name + "_walk_z+.jmpAnimation");
    
    AnimationImporter animationImporterYBot2;
    animationImporterYBot2.ImportAsynchronously(System::assetsPath + "animations/" + name + "/" + name + "_walk_x+.jmpAnimation");
    
    animationImporterYBot1.join();
    animationImporterYBot2.join();
    
    
    
    AnimationImporter animationImporterYBot3;
    animationImporterYBot3.ImportAsynchronously(System::assetsPath + "animations/" + name + "/" + name + "_neutral_idle.jmpAnimation");
    
    animationImporterYBot3.join();
    
    
    
    AnimationImporter animationImporterYBot4;
    animationImporterYBot4.ImportAsynchronously(System::assetsPath + "animations/" + name + "/" + name + "_punch_1.jmpAnimation");
    
    AnimationImporter animationImporterYBot5;
    animationImporterYBot5.ImportAsynchronously(System::assetsPath + "animations/" + name + "/" + name + "_punch_2.jmpAnimation");

    animationImporterYBot4.join();
    animationImporterYBot5.join();
    
    
    
    AnimationImporter animationImporterYBot6;
    animationImporterYBot6.ImportAsynchronously(System::assetsPath + "animations/" + name + "/" + name + "_punch_3.jmpAnimation");
    
    AnimationImporter animationImporterYBot7;
    animationImporterYBot7.ImportAsynchronously(System::assetsPath + "animations/" + name + "/" + name + "_punch_4.jmpAnimation");
    
    animationImporterYBot6.join();
    animationImporterYBot7.join();
    
    
    
    AnimationImporter animationImporterYBot8;
    animationImporterYBot8.ImportAsynchronously(System::assetsPath + "animations/" + name + "/" + name + "_block.jmpAnimation");
    
    AnimationImporter animationImporterYBot9;
    animationImporterYBot9.ImportAsynchronously(System::assetsPath + "animations/" + name + "/" + name + "_win.jmpAnimation");
    
    animationImporterYBot8.join();
    animationImporterYBot9.join();
    
    
    AnimationImporter animationImporterYBot10;
    animationImporterYBot10.ImportAsynchronously(System::assetsPath + "animations/" + name + "/" + name + "_die.jmpAnimation");
    
    animationImporterYBot10.join();
    
    
    //
    // ANIMATIONS
    //
    
    // Add animations to the animation module
    if(!aModule->addAnimation(animationImporterYBot1.getImportedObject()->getName(), animationImporterYBot1.getImportedObject()))
    {
        delete animationImporterYBot1.getImportedObject();
    }
    if(!aModule->addAnimation(animationImporterYBot2.getImportedObject()->getName(), animationImporterYBot2.getImportedObject()))
    {
        delete animationImporterYBot2.getImportedObject();
    }
    if(!aModule->addAnimation(animationImporterYBot3.getImportedObject()->getName(), animationImporterYBot3.getImportedObject()))
    {
        delete animationImporterYBot3.getImportedObject();
    }
    if(!aModule->addAnimation(animationImporterYBot4.getImportedObject()->getName(), animationImporterYBot4.getImportedObject()))
    {
        delete animationImporterYBot4.getImportedObject();
    }
    if(!aModule->addAnimation(animationImporterYBot5.getImportedObject()->getName(), animationImporterYBot5.getImportedObject()))
    {
        delete animationImporterYBot5.getImportedObject();
    }
    if(!aModule->addAnimation(animationImporterYBot6.getImportedObject()->getName(), animationImporterYBot6.getImportedObject()))
    {
        delete animationImporterYBot6.getImportedObject();
    }
    if(!aModule->addAnimation(animationImporterYBot7.getImportedObject()->getName(), animationImporterYBot7.getImportedObject()))
    {
        delete animationImporterYBot7.getImportedObject();
    }
    if(!aModule->addAnimation(animationImporterYBot8.getImportedObject()->getName(), animationImporterYBot8.getImportedObject()))
    {
        delete animationImporterYBot8.getImportedObject();
    }
    if(!aModule->addAnimation(animationImporterYBot9.getImportedObject()->getName(), animationImporterYBot9.getImportedObject()))
    {
        delete animationImporterYBot9.getImportedObject();
    }
    if(!aModule->addAnimation(animationImporterYBot10.getImportedObject()->getName(), animationImporterYBot10.getImportedObject()))
    {
        delete animationImporterYBot10.getImportedObject();
    }
    //aModule->addAnimation(animationImporterYBot4.getImportedObject()->getName(), animationImporterYBot4.getImportedObject());
    
    
    
    
    //
    // CHARACTER OBJECT
    //
    
    // Create animatable mesh properties
    AnimatableMeshProperty *meshPropertyYBot1 = (AnimatableMeshProperty*)propertyImporterYBot1.getImportedObject();
    meshPropertyYBot1->setShaderKey("lighting");   // Set shader key
    meshPropertyYBot1->linkJoints((JointEntity*)entityImporterYBot.getImportedObject()); // Link to imported skeleton
    
    AnimatableMeshProperty *meshPropertyYBot2 = (AnimatableMeshProperty*)propertyImporterYBot2.getImportedObject();
    meshPropertyYBot2->setShaderKey("lighting");
    meshPropertyYBot2->linkJoints((JointEntity*)entityImporterYBot.getImportedObject());
    
    // Create animator property
    AnimatorProperty *animatorPropertyYBot1 = new AnimatorProperty(name + "_animator", (JointEntity*)entityImporterYBot.getImportedObject());
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
    
    if(!gModule->addMesh(meshImporterYBot1.getImportedObject()->getName(), meshGLYBot1))
    {
        delete meshGLYBot1;
    }
    
    if(!gModule->addMesh(meshImporterYBot2.getImportedObject()->getName(), meshGLYBot2))
    {
        delete meshGLYBot2;
    }
    
    delete meshImporterYBot1.getImportedObject();
    delete meshImporterYBot2.getImportedObject();
    
    
    
    //
    // MATERIALS
    //
    
    // Add the materials to the graphics module
    if(!gModule->addMaterial(materialImporterYBot1.getImportedObject()->getName(), materialImporterYBot1.getImportedObject()))
    {
        delete materialImporterYBot1.getImportedObject();
    }
    
    if(!gModule->addMaterial(materialImporterYBot2.getImportedObject()->getName(), materialImporterYBot2.getImportedObject()))
    {
        delete materialImporterYBot2.getImportedObject();
    }
    
}


void Character::initialisePhysics()
{
    //
    // Now set up some physics related properties
    //
    const float handPhysicsObjSize = 8;
    
    // Physics collision callbacks
    std::function<void(PhysicsProperty*, PhysicsProperty*, btManifoldPoint*)> bodyCallback = std::bind(&Character::bodyCollisionCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    
    std::function<void(PhysicsProperty*, PhysicsProperty*, btManifoldPoint*)> headCallback = std::bind(&Character::headCollisionCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    
    std::function<void(PhysicsProperty*, PhysicsProperty*, btManifoldPoint*)> torsoCallback = std::bind(&Character::torsoCollisionCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    
    std::function<void(PhysicsProperty*, PhysicsProperty*, btManifoldPoint*)> fistCallback = std::bind(&Character::fistCollisionCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    
    std::function<void(PhysicsProperty*)> beforeCollisionCallback = std::bind(&Character::beforeCollisionCallback, this, std::placeholders::_1);
    
    // Get the entites we want to attach the physics stuff to
    
    // Body
    // Get hips entity
    JointEntity* hipsParent = (JointEntity*)getDescendant(name + "_hips");
    GameObject *hips = new GameObject(name + "_hips_physics");
    hips->scale(glm::vec3(20.0f));
    hipsParent->addChild(hips);
    
    // Create and configure physics
    PhysicsShape *bodyShape = new PhysicsSphere(20.0f);
    PhysicsProperty *bodyPhysicsProperty = new PhysicsProperty(name + "_body", bodyShape);
    bodyPhysicsProperty->setCollisionCallback(bodyCallback);
    bodyPhysicsProperty->setBeforeCollisionTestCallback(beforeCollisionCallback);
    bodyPhysicsProperty->setCollisionFlags(bodyPhysicsProperty->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    //bodyPhysicsProperty->setMask(collisionMask, collidesWithMask);
    bodyPhysicsProperty->setMask(4, 4);
    hips->addProperty(bodyPhysicsProperty);
    
    // Create and configure graphical mesh
    //physicsMeshesInitialsed = true;
    /*bodyPhysicsMeshProperty = new MeshProperty(name + "_body");
    bodyPhysicsMeshProperty->setMaterialKey("no_collision");
    bodyPhysicsMeshProperty->setMeshKey("sphere");
    bodyPhysicsMeshProperty->setShaderKey("basic");
    bodyPhysicsMeshProperty->setDrawMode(GL_LINES);
    hips->addProperty(bodyPhysicsMeshProperty);
    physicsMeshMap[name + "_body"] = bodyPhysicsMeshProperty;*/
    
    // HEAD
    // Get head entity
    JointEntity* headParent = (JointEntity*)getDescendant(name + "_head");
    GameObject *head = new GameObject(name + "_head_physics");
    glm::vec3 t = glm::vec3(0.8f * headParent->getLocalBindTransform()[3]);
    head->translate(t);
    head->scale(glm::vec3(14.0f));
    headParent->addChild(head);
    
    // Create and configure physics
    PhysicsShape *headShape = new PhysicsSphere(14.0f);
    PhysicsProperty *headPhysicsProperty = new PhysicsProperty(name + "_head", headShape);
    headPhysicsProperty->setCollisionCallback(headCallback);
    headPhysicsProperty->setBeforeCollisionTestCallback(beforeCollisionCallback);
    headPhysicsProperty->setCollisionFlags(headPhysicsProperty->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    headPhysicsProperty->setMask(collisionMask, collidesWithMask);
    head->addProperty(headPhysicsProperty);
    
    // Create and configure graphical mesh
    /*headPhysicsMeshProperty = new MeshProperty(name + "_head");
    headPhysicsMeshProperty->setMaterialKey("no_collision");
    headPhysicsMeshProperty->setMeshKey("sphere");
    headPhysicsMeshProperty->setShaderKey("basic");
    headPhysicsMeshProperty->setDrawMode(GL_LINES);
    head->addProperty(headPhysicsMeshProperty);
    physicsMeshMap[name + "_head"] = headPhysicsMeshProperty;*/
    
    // Torso
    // Get spine entity
    JointEntity* spineParent = (JointEntity*)getDescendant(name + "_spine2");
    GameObject *torso = new GameObject(name + "_torso_physics");
    //glm::vec3 t = glm::vec3(0.8f * spineParent->getLocalBindTransform()[3]);
    //torso->translate(t);
    torso->scale(glm::vec3(18.0f));
    spineParent->addChild(torso);
    
    // Create and configure physics
    PhysicsShape *torsoShape = new PhysicsSphere(18.0f);
    PhysicsProperty *torsoPhysicsProperty = new PhysicsProperty(name + "_torso", torsoShape);
    torsoPhysicsProperty->setCollisionCallback(torsoCallback);
    torsoPhysicsProperty->setBeforeCollisionTestCallback(beforeCollisionCallback);
    torsoPhysicsProperty->setCollisionFlags(torsoPhysicsProperty->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    torsoPhysicsProperty->setMask(collisionMask, collidesWithMask);
    torso->addProperty(torsoPhysicsProperty);
    
    // Create and configure graphical mesh
    /*torsoPhysicsMeshProperty = new MeshProperty(name + "_torso");
    torsoPhysicsMeshProperty->setMaterialKey("no_collision");
    torsoPhysicsMeshProperty->setMeshKey("sphere");
    torsoPhysicsMeshProperty->setShaderKey("basic");
    torsoPhysicsMeshProperty->setDrawMode(GL_LINES);
    torso->addProperty(torsoPhysicsMeshProperty);
    physicsMeshMap[name + "_torso"] = torsoPhysicsMeshProperty;*/
    
    // LEFT HAND
    // Get left hand entity
    JointEntity* leftHandParent = (JointEntity*)getDescendant(name + "_lefthand");
    JointEntity* leftHandFinger = (JointEntity*)getDescendant(name + "_lefthandmiddle1");
    GameObject *leftHand = new GameObject(name + "_lefthand_physics");
     t = glm::vec3(0.5f * leftHandFinger->getLocalBindTransform()[3]);
    leftHand->translate(t);
    leftHand->scale(glm::vec3(handPhysicsObjSize));
    leftHandParent->addChild(leftHand);
    
    // Create and configure physics
    PhysicsShape *leftHandShape = new PhysicsSphere(handPhysicsObjSize);
    PhysicsProperty *leftHandPhysicsProperty = new PhysicsProperty(name + "_lefthand", leftHandShape);
    leftHandPhysicsProperty->setCollisionCallback(fistCallback);
    leftHandPhysicsProperty->setBeforeCollisionTestCallback(beforeCollisionCallback);
    leftHandPhysicsProperty->setCollisionFlags(leftHandPhysicsProperty->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    leftHandPhysicsProperty->setMask(collisionMask, collidesWithMask);
    leftHand->addProperty(leftHandPhysicsProperty);
    
    // Create and configure graphical mesh
    /*leftHandPhysicsMeshProperty = new MeshProperty(name + "_lefthand");
    leftHandPhysicsMeshProperty->setMaterialKey("no_collision");
    leftHandPhysicsMeshProperty->setMeshKey("sphere");
    leftHandPhysicsMeshProperty->setShaderKey("basic");
    leftHandPhysicsMeshProperty->setDrawMode(GL_LINES);
    leftHand->addProperty(leftHandPhysicsMeshProperty);
    physicsMeshMap[name + "_lefthand"] = leftHandPhysicsMeshProperty;*/
    
    // RIGHT HAND
    // Get right hand entity
    JointEntity* rightHandParent = (JointEntity*)getDescendant(name + "_righthand");
    JointEntity* rightHandFinger = (JointEntity*)getDescendant(name + "_righthandmiddle1");
    GameObject *rightHand = new GameObject(name + "_righthand_physics");
    t = glm::vec3(0.5f * rightHandFinger->getLocalBindTransform()[3]);
    rightHand->translate(t);
    rightHand->scale(glm::vec3(handPhysicsObjSize));
    rightHandParent->addChild(rightHand);
    
    // Create and configure physics
    PhysicsShape *rightHandShape = new PhysicsSphere(handPhysicsObjSize);
    PhysicsProperty *rightHandPhysicsProperty = new PhysicsProperty(name + "_righthand", rightHandShape);
    rightHandPhysicsProperty->setCollisionCallback(fistCallback);
    rightHandPhysicsProperty->setBeforeCollisionTestCallback(beforeCollisionCallback);
    rightHandPhysicsProperty->setCollisionFlags(rightHandPhysicsProperty->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    rightHandPhysicsProperty->setMask(collisionMask, collidesWithMask);
    rightHand->addProperty(rightHandPhysicsProperty);
    
    // Create and configure graphical mesh
    /*rightHandPhysicsMeshProperty = new MeshProperty(name + "_righthand");
    rightHandPhysicsMeshProperty->setMaterialKey("no_collision");
    rightHandPhysicsMeshProperty->setMeshKey("sphere");
    rightHandPhysicsMeshProperty->setShaderKey("basic");
    rightHandPhysicsMeshProperty->setDrawMode(GL_LINES);
    rightHand->addProperty(rightHandPhysicsMeshProperty);
    physicsMeshMap[name + "_righthand"] = rightHandPhysicsMeshProperty;*/
    
    // Create the materials needed and add them to the graphics module
    Material *noCollisionMaterial = new Material("no_collision", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 1.0f);
    Material *yesCollisionMaterial = new Material("yes_collision", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 1.0f);
    
    if(!Graphics::getInstance().addMaterial(noCollisionMaterial->getName(), noCollisionMaterial))
    {
        delete noCollisionMaterial;
    }
    if(!Graphics::getInstance().addMaterial(yesCollisionMaterial->getName(), yesCollisionMaterial))
    {
        delete yesCollisionMaterial;
    }
    
}

void Character::update()
{
    if(state == WALKING)
    {
        glm::vec3 t = (float)timeSinceLastUpdate * walkTranslation;
        translateLocalAxis(t);
    }
}

void Character::beforeCollisionCallback(PhysicsProperty *obj)
{
    if(physicsMeshesInitialsed)
    {
        auto it = physicsMeshMap.find(obj->getName());
    
        if(it != physicsMeshMap.end())
        {
            if(it->second != NULL)
            {
                it->second->setMaterialKey("no_collision");
                it->second->linkMaterialPtr();
            }
        }
    }
    
}

void Character::bodyCollisionCallback(PhysicsProperty *obA, PhysicsProperty *obB, btManifoldPoint* cp)
{
    /*static std::string bodyStr = "body";
    
    if (obA->getName().find(bodyStr) != std::string::npos && obB->getName().find(bodyStr) != std::string::npos)
    {
        std::cout << "Body collided!" << std::endl;
    }
    else
    {
        return;
    }*/
    
    std::cout << "Body collided!" << std::endl;
    
    // Resolve the collision
    double ptdist = cp->getDistance();
    translateLocalAxis(0.0, 0.0, ptdist * 0.5f);
    
    
    if(physicsMeshesInitialsed)
    {
        MeshProperty *meshProp;
        bool which = false;
    
        auto it = physicsMeshMap.find(obA->getName());
        if(it != physicsMeshMap.end())
        {
            if(it->second != NULL)
            {
                meshProp = it->second;
                which = true;
            }
        }
    
        it = physicsMeshMap.find(obB->getName());
        if(it != physicsMeshMap.end())
        {
            if(it->second != NULL)
            {
                meshProp = it->second;
                which = false;
            }
        }
    
        if(meshProp != NULL)
        {
            meshProp->setMaterialKey("yes_collision");
            meshProp->linkMaterialPtr();
        }
    }
}

void Character::headCollisionCallback(PhysicsProperty *obA, PhysicsProperty *obB, btManifoldPoint* contactPoint)
{
    std::cout << "Head collided!" << std::endl;
 
    if(physicsMeshesInitialsed)
    {
        auto it = physicsMeshMap.find(obA->getName());
    
        if(it != physicsMeshMap.end())
        {
            if(it->second != NULL)
            {
                it->second->setMaterialKey("yes_collision");
                it->second->linkMaterialPtr();
            }
        }
    
        it = physicsMeshMap.find(obB->getName());
    
        if(it != physicsMeshMap.end())
        {
            if(it->second != NULL)
            {
                it->second->setMaterialKey("yes_collision");
                it->second->linkMaterialPtr();
            }
        }
    }
}

void Character::torsoCollisionCallback(PhysicsProperty *obA, PhysicsProperty *obB, btManifoldPoint* contactPoint)
{
    std::cout << "Torso collided!" << std::endl;
    
    if(physicsMeshesInitialsed)
    {
        auto it = physicsMeshMap.find(obA->getName());
    
        if(it != physicsMeshMap.end())
        {
            if(it->second != NULL)
            {
                it->second->setMaterialKey("yes_collision");
                it->second->linkMaterialPtr();
            }
        }
    
        it = physicsMeshMap.find(obB->getName());
    
        if(it != physicsMeshMap.end())
        {
            if(it->second != NULL)
            {
                it->second->setMaterialKey("yes_collision");
                it->second->linkMaterialPtr();
            }
        }
    }
    
    // execute punched callback
    punchedCallback();
}

void Character::fistCollisionCallback(PhysicsProperty *obA, PhysicsProperty *obB, btManifoldPoint* contactPoint)
{
    std::cout << "Fist collided!" << std::endl;
    
    if(physicsMeshesInitialsed)
    {
        auto it = physicsMeshMap.find(obA->getName());
    
        if(it != physicsMeshMap.end())
        {
            if(it->second != NULL)
            {
                it->second->setMaterialKey("yes_collision");
                it->second->linkMaterialPtr();
            }
        }
    
        it = physicsMeshMap.find(obB->getName());
    
        if(it != physicsMeshMap.end())
        {
            if(it->second != NULL)
            {
                it->second->setMaterialKey("yes_collision");
                it->second->linkMaterialPtr();
            }
        }
    }
}
