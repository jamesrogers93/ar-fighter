#ifndef _SETTINGSSCENELOGIC_H
#define _SETTINGSSCENELOGIC_H

#include "game-engine/Scene/SceneLogic.h"

#include "game-engine/Core/Engine/Engine.h"

#include "ar-fighter/Game_Objects/Character.h"

class SettingsSceneLogic : public SceneLogic
{
public:
    
    Character *player;
    Character *opponent;
    
    SettingsSceneLogic(Scene *scene) : SceneLogic(scene)
    {
        
    }
    
    void initialise();
    
    void update();
    
    void draw();
    
    void switchPlayerRight()
    {}
    
    void switchPlayerLeft()
    {}
    
    void switchOpponentRight()
    {}
    
    void switchOpponentLeft()
    {}
    
    void easyDifficulty()
    {}
    
    void mediumDifficulty()
    {}
    
    void hardDifficulty()
    {}
};

#endif /* _SETTINGSSCENELOGIC_H */
