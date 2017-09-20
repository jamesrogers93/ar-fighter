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
    
    static const float MAX_HEALTH;
    static const float DAMAGE_INFLICT;
    
    enum CharacterState { IDLE, WALKING, PUNCHING, BLOCKING, WIN, LOSE};
    CharacterState state;
    
    Character(const std::string &name, unsigned short collisionMask, unsigned short collidesWithMask, const glm::vec4 &colourTheme);
    
    virtual void initialise();
    virtual void initialisePhysics();
    
    virtual void walk(const float &x, const float &z);
    virtual void idle();// {}
    virtual void punch();
    virtual void kick();
    virtual void block();
    virtual void win();
    virtual void lose();
    
    void actionCallback();
    
    virtual void beforeCollisionCallback(PhysicsProperty *obj);
    
    virtual void bodyCollisionCallback(PhysicsProperty *obA, PhysicsProperty *obB, btManifoldPoint* contactPoint);
    virtual void headCollisionCallback(PhysicsProperty *obA, PhysicsProperty *obB, btManifoldPoint* contactPoint);
    virtual void torsoCollisionCallback(PhysicsProperty *obA, PhysicsProperty *obB, btManifoldPoint* contactPoint);
    virtual void fistCollisionCallback(PhysicsProperty *obA, PhysicsProperty *obB, btManifoldPoint* contactPoint);
    
    
    AnimatorProperty* getAnimator() { return mAnimator; }
    
    
    void setPunchedCallback(std::function<void()> func) { mPunchedCallback = func; }
    void punchedCallback() { mPunchedCallback(); }
    
    void update();
    
    void setOpponent(Character *opponent) { this->opponent = opponent; }
    Character* getOpponent() { return this->opponent; }
    
    const float& getMaxHealth() { return maxHealth; }
    const float& getHealth() { return health; }
    const float& getDamageInflict() { return damageInflict; }
    const bool& getCanDealDamage() { return canDealDamage; }
    const glm::vec4& getColourTheme() { return mThemeColour; }
    const bool isAlive() { return health > 0.0f; }
    
    void setSpeed(const float &speed) { this->speed = speed; }
    void setDifficulty(const int difficulty) { this->difficulty = difficulty; }
    int getDifficulty() { return difficulty; }
    
    void takeDamage(const float &damage) { health -= damage; if( health < 0) health = 0; }
    void damageDealt() { canDealDamage = false; }
    
protected:
    
    Character *opponent;
    
    // Player properties
    float maxHealth;
    float health;
    float damageInflict;
    bool canDealDamage;
    float speed;
    glm::vec4 mThemeColour;
    int difficulty;
    
    // Handles to the properties
    AnimatorProperty *mAnimator;
    MeshProperty *headPhysicsMeshProperty;
    MeshProperty *torsoPhysicsMeshProperty;
    MeshProperty *rightHandPhysicsMeshProperty;
    MeshProperty *leftHandPhysicsMeshProperty;
    MeshProperty *bodyPhysicsMeshProperty;
    
    bool physicsMeshesInitialsed;
    
    unsigned short collisionMask;
    unsigned short collidesWithMask;
    
    std::unordered_map<std::string, MeshProperty*> physicsMeshMap;

    
    glm::vec3 walkTranslation;
    
    // ACtion callbacks
    std::function<void()> mPunchedCallback;
    
};

#endif /* _CHARACTER_H */
