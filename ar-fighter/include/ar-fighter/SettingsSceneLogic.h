#ifndef _SETTINGSSCENELOGIC_H
#define _SETTINGSSCENELOGIC_H

#include "game-engine/Core/Main/SceneLogic.h"

#include "game-engine/Core/Main/Engine.h"

#include "ar-fighter/Game_Objects/Character.h"

class GUIRectangle;

class SettingsSceneLogic : public SceneLogic
{
public:
    
    enum class GameState
    {
        UNINITIALISED,
        INITIALISED,
        IDLE,
        PLAY
    };
    GameState state;
    
    Character *player;
    Character *opponent;
    
    std::string difficulty;
    
    // To house GUI stuff
    GameObject *settingsUI;
    GUIRectangle *highlightPtr;
    
    SettingsSceneLogic(Scene *scene);
    
    void playButton();
    
    void switchPlayerRight()
    {}
    
    void switchPlayerLeft()
    {}
    
    void switchOpponentRight()
    {}
    
    void switchOpponentLeft()
    {}
    
    void easyDifficulty();
    void hardDifficulty();
    
protected:
    void initialise();
    void deinitialise();
    void update();
    void draw();
};

#endif /* _SETTINGSSCENELOGIC_H */
