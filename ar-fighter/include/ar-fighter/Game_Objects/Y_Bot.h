#ifndef _YBOT_H
#define _YBOT_H

#include "ar-fighter/Game_Objects/Character.h"

// GLM
#include <glm/glm.hpp>

class YBot : public Character
{
public:
    YBot(unsigned short collisionMask, unsigned short collidesWithMask) : Character("y_bot", collisionMask, collidesWithMask, glm::vec4(21.0f/255.0f, 51.0f/255.0f, 63.0f/255.0f, 1.0f))
    {
    }
    
    //void initialise();
  
    //void walk(const float &x, const float &z);
    //void idle();
};

#endif /* _YBOT_H */
