#ifndef _CHARACTER_H
#define _CHARACTER_H

// STD
#include <string>
#include <unordered_map>

#include <game-engine/GameObject.h>

class AnimatorProperty;
class AnimatableMeshProperty;
class MeshProperty;
class PhysicsProperty;
class btManifoldPoint;

// A character object must have:
// - 1 Skeleton child entity
// - 1 Animator Property
// - Atleast 1 animatable mesh property

class Character : public GameObject
{
public:
    
    enum CharacterState { IDLE, WALKING, PUNCHING};
    CharacterState state;
    
    Character(const std::string &name, unsigned short collisionMask, unsigned short collidesWithMask, const glm::vec4 &colourTheme);
    
    virtual void initialise();
    virtual void initialisePhysics();
    
    virtual void walk(const float &x, const float &z);
    virtual void idle();// {}
    virtual void punch();
    virtual void kick();
    virtual void block();
    
    virtual void beforeCollisionCallback(PhysicsProperty *obj);
    
    virtual void bodyCollisionCallback(PhysicsProperty *obA, PhysicsProperty *obB, btManifoldPoint* contactPoint);
    virtual void headCollisionCallback(PhysicsProperty *obA, PhysicsProperty *obB, btManifoldPoint* contactPoint);
    virtual void torsoCollisionCallback(PhysicsProperty *obA, PhysicsProperty *obB, btManifoldPoint* contactPoint);
    virtual void fistCollisionCallback(PhysicsProperty *obA, PhysicsProperty *obB, btManifoldPoint* contactPoint);
    
    
    AnimatorProperty* getAnimator() { return mAnimator; }
    
    
    void setPunchedCallback(std::function<void()> func) { mPunchedCallback = func; }
    void punchCallback() { mPunchedCallback(); }
    
    void update();
    
    
    const float& getMaxHealth() { return maxHealth; }
    const float& getHealth() { return health; }
    const float& getDamageInflict() { return damageInflict; }
    const bool& getCanDealDamage() { return canDealDamage; }
    const glm::vec4& getColourTheme() { return mThemeColour; }
    
    void takeDamage(const float &damage) { health -= damage; if( health < 0) health = 0; }
    void damageDealt() { canDealDamage = false; }
    
protected:
    
    // Player properties
    float maxHealth = 100.0f;
    float health = maxHealth;
    float damageInflict = 1.0f;
    bool canDealDamage = false;
    glm::vec4 mThemeColour;
    
    // Handles to the properties
    AnimatorProperty *mAnimator;
    MeshProperty *headPhysicsMeshProperty;
    MeshProperty *torsoPhysicsMeshProperty;
    MeshProperty *rightHandPhysicsMeshProperty;
    MeshProperty *leftHandPhysicsMeshProperty;
    MeshProperty *bodyPhysicsMeshProperty;
    
    
    unsigned short collisionMask;
    unsigned short collidesWithMask;
    
    std::unordered_map<std::string, MeshProperty*> physicsMeshMap;

    
    glm::vec3 walkTranslation;
    
    // ACtion callbacks
    std::function<void()> mPunchedCallback;
    
};

#endif /* _CHARACTER_H */
