#ifndef _XBOT_H
#define _XBOT_H

#include "ar-fighter/Game_Objects/Character.h"

class XBot : public Character
{
public:
    XBot(unsigned short collisionMask, unsigned short collidesWithMask) : Character("x_bot", collisionMask, collidesWithMask, glm::vec4(138.0f/255.0f, 62.0f/255.0f, 53.0f/255.0f, 1.0f))
    {
    }
    
    //void initialise();
    
    //void walk(const float &x, const float &z);
    //void idle();
};

#endif /* _XBOT_H */
