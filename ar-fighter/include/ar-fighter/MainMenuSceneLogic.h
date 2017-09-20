#ifndef _MAINMENUSCENELOGIC_H
#define _MAINMENUSCENELOGIC_H

#include "game-engine/Core/Main/SceneLogic.h"

#include "game-engine/Core/Main/Engine.h"

#include "ar-fighter/Game_Objects/Character.h"

class MainMenuSceneLogic : public SceneLogic
{
public:
    
    enum class GameState
    {
        UNINITIALISED,
        INITIALISED,
        IDLE,
        START
    };
    
    GameState state;
    
    // To house GUI stuff
    GameObject *mainUI;
    
    MainMenuSceneLogic(Scene *scene) : SceneLogic(scene), state(GameState::UNINITIALISED)
    {
        
    }
    
    void startButton();
    
protected:
    
    void initialise();
    void deinitialise();
    void update();
    void draw();
};

#endif /* _MAINMENUSCENELOGIC_H */
