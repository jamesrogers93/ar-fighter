#ifndef _AIFSMSTATEMOVETOPLAYER_H
#define _AIFSMSTATEMOVETOPLAYER_H

#include <game-engine/Modules/AI/AIFSMState.h>

#include <game-engine/Modules/Animation/AnimatorProperty.h>

#include <ar-fighter/Game_Objects/Character.h>

// GLM
#include <glm/glm.hpp>

class AIFSMStateMoveToPlayer : public AIFSMState
{
private:
    Character *character;
    
public:
    
    AIFSMStateMoveToPlayer(Entity *parent) : AIFSMState(parent)
    {
        character = static_cast<Character*>(parent);
    }
    
    const std::string update()
    {
        if(character != NULL)
        {
            
            float distance = glm::length(character->getPosition() - character->getOpponent()->getPosition());
            //character->getPosition();
            //character->getOpponent()->getPosition();
            
            if(distance > 100)
            {
                character->walk(0, 1);
            } else
            {
                character->state = Character::CharacterState::IDLE;
                character->getAnimator()->getAnimationController1()->stop();
                character->getAnimator()->getAnimationController2()->stop();
                return "combat";
            }
        }
        
        return "move-to-player";
    }
};

#endif /* _AIFSMSTATEMOVETOPLAYER_H */
