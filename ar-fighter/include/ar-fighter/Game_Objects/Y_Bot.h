#ifndef _YBOT_H
#define _YBOT_H

#include "ar-fighter/Game_Objects/Character.h"

class YBot : public Character
{
public:
    YBot() : Character("y_bot")
    {
    }
    
    void initialise();
  
    void walk();
};

#endif /* _YBOT_H */