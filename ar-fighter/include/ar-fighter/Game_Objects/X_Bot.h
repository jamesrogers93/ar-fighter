#ifndef _XBOT_H
#define _XBOT_H

#include "ar-fighter/Game_Objects/Character.h"

class XBot : public Character
{
public:
    XBot() : Character("x_bot")
    {
    }
    
    void initialise();
    
    void walk(const float &x, const float &z);
    void idle();
};

#endif /* _XBOT_H */
