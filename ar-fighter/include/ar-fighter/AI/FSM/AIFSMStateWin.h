#ifndef _AIFSMSTATEWIN_H
#define _AIFSMSTATEWIN_H

#include <game-engine/Modules/AI/AIFSMState.h>

class AIFSMStateWin : public AIFSMState
{
public:
    
    AIFSMStateWin(Entity *parent) : AIFSMState(parent)
    {
        
    }
    
    const std::string update()
    {
        return "win";
    }
};

#endif /* _AIFSMSTATEWIN_H */
