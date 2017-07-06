#ifndef _ARFIGHTER_H
#define _ARFIGHTER_H

#include <string>

// Game Engine
//#include <game-engine/Core/Engine/Engine.h>

class SceneManager;
class Scene;

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
    
    void setAssetsPath(const std::string &path) {   this->assetsPath = path;    }
    
    // DELETE THESE
    void pitch();
    void yaw();
    void roll();
    void track();
    
private:
    ARFighter()
    {
        sceneManager = 0;
    }
    ~ARFighter(){}

    SceneManager *sceneManager;
    
    unsigned int screenWidth, screenHeight;
    
    std::string assetsPath;
    
    Scene* createScene();
};

#endif /* _ARFIGHTER_H */
