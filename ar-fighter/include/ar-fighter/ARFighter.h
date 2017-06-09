#ifndef _ARFIGHTER_H
#define _ARFIGHTER_H

// Game Engine
#include <game-engine/Core/Engine/Engine.h>

class ARFighter
{
public:
    
    ARFighter& getInstance()
    {
        static ARFighter instance;
        
        return instance;
    }
    
    void initalise();
    void deinitalise();
    
    void update();
    void draw();
    
private:
    ARFighter(){}
    ~ARFighter();

};

#endif /* _ARFIGHTER_H */
