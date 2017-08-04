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
    
    void initialise(const unsigned int &screenWidth, const unsigned int &screenHeight, const std::string &assetsPath);
    void deinitialise();
    
    void update();
    void draw();
    
    void touchDown(const float &x, const float &y);
    void touchMove(const float &x, const float &y);
    void touchUp(const float &x, const float &y);
    
    void setUpSettingsScene();
    void setUpFightScene();
    
    
    //void setAssetsPath(const std::string &path) {   this->assetsPath = path;    }
    
private:
    ARFighter()
    {
        sceneManager = 0;
    }
    ~ARFighter(){}

    SceneManager *sceneManager;
    
    unsigned int screenWidth, screenHeight;
    
    //std::string assetsPath;
    
    Scene* createSettingsScene();
};

#endif /* _ARFIGHTER_H */
