#ifndef _ARFIGHTER_H
#define _ARFIGHTER_H

// Game Engine
//#include <game-engine/Core/Engine/Engine.h>

class SceneManager;

class ARFighter
{
public:
    
    static ARFighter& getInstance()
    {
        static ARFighter instance;
        
        return instance;
    }
    
    void initalise(const unsigned int &screenWidth, const unsigned int &screenHeight);
    void deinitalise();
    
    void update();
    void draw();
    
private:
    ARFighter()
    {
        sceneManager = 0;
    }
    ~ARFighter(){}

    SceneManager *sceneManager;
    
    unsigned int screenWidth, screenHeight;
};

#endif /* _ARFIGHTER_H */
