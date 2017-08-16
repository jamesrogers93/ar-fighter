#ifndef _YBOT_H
#define _YBOT_H

#include "ar-fighter/Game_Objects/Character.h"

class YBot : public Character
{
public:
    YBot(unsigned short collisionMask, unsigned short collidesWithMask) : Character("y_bot", collisionMask, collidesWithMask)
    {
    }
    
    //void initialise();
  
    //void walk(const float &x, const float &z);
    //void idle();
};

#endif /* _YBOT_H */
