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
    Character(const std::string &name) : GameObject(name), mAnimator(NULL)
    {}
    
    virtual void initialise() = 0;
    
    virtual void walk(const float &x, const float &z){}
    virtual void idle() {}
    
    AnimatorProperty* getAnimator() { return mAnimator; }
    
protected:
    
    // Handles to the properties
    AnimatorProperty *mAnimator;

};

#endif /* _CHARACTER_H */
