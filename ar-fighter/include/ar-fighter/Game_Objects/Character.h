#ifndef _CHARACTER_H
#define _CHARACTER_H

#include <game-engine/GameObject.h>

class AnimatorProperty;
class AnimatableMeshProperty;

// A character object must have:
// - 1 Skeleton child entity
// - 1 Animator Property
// - Atleast 1 animatable mesh property

class Character : public GameObject
{
public:
    
    enum CharacterState { IDLE, WALKING, PUNCHING};
    CharacterState state;
    
    Character(const std::string &name) : GameObject(name), mAnimator(NULL), state(IDLE)
    {}
    
    virtual void initialise();
    
    virtual void walk(const float &x, const float &z);
    virtual void idle();// {}
    virtual void punch();
    
    AnimatorProperty* getAnimator() { return mAnimator; }
    
    void update();
    
protected:
    
    // Handles to the properties
    AnimatorProperty *mAnimator;

    
    glm::vec3 walkTranslation;
    
};

#endif /* _CHARACTER_H */
