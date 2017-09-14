#ifndef _AIFSMSTATECOMBAT_H
#define _AIFSMSTATECOMBAT_H

#include <game-engine/Modules/AI/AIFSMState.h>

class AIFSMStateCombat : public AIFSMState
{
private:
    Character *character;
    
public:
    
    AIFSMStateCombat(Entity *parent) : AIFSMState(parent)
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
            
            if(distance > 150)
            {
                return "move-to-player";
            } else if(character->getOpponent()->state == Character::CharacterState::PUNCHING)
            {
                character->block();
            } else
            {
                character->punch();
            }
        }
        
        return "combat";
    }
};

#endif /* _AIFSMSTATECOMBAT_H */
