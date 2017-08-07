#ifndef _FIGHTSCENELOGIC_H
#define _FIGHTSCENELOGIC_H

#include "game-engine/Scene/SceneLogic.h"

#include "game-engine/Core/Engine/Engine.h"

class Character;
class AREntity;
class GameObject;
class GUIAnalog;

class FightSceneLogic : public SceneLogic
{
public:
    
    enum class GameState { PROMPT_TRACKER, PROMPTING_TRACKER, STARTED_TRACKER, PLAYING};
    GameState state;
    
    Character *player;
    Character *opponent;
    AREntity *arHandler;
    
    // To house GUI stuff
    GameObject *trackingGameObject;
    GameObject *playingGameObject;
    
    // Referece to Analog button
    GUIAnalog *playerWalkController;
    
    FightSceneLogic(Scene *scene) : SceneLogic(scene), state(GameState::PROMPT_TRACKER)
    {
    }
    
    void initialise();
    void update();
    void draw();
    
    void walkAnalogMove();
    void walkAnalogUp();
    void punchButtonUp();
    void track();
};

#endif /* _FIGHTSCENELOGIC_H */
