#ifndef _FIGHTSCENELOGIC_H
#define _FIGHTSCENELOGIC_H

#include "game-engine/Scene/SceneLogic.h"

#include "game-engine/Core/Engine/Engine.h"

class Character;
class AREntity;
class GameObject;
class GUIProperty;
class GUIAnalog;
class GUIProgressBar;

class FightSceneLogic : public SceneLogic
{
public:
    
    enum class GameState { PROMPT_TRACKER, PROMPTING_TRACKER, STARTED_TRACKER, PROMPT_TRACK_CONFIRM, PROMPTING_TRACK_CONFIRM, PLAYING};
    GameState state;
    
    Character *player;
    Character *opponent;
    AREntity *arHandler;
    
    // To house GUI stuff
    GameObject *trackStartUI;
    GameObject *trackConfirmUI;
    GameObject *playingUI;
    
    // Countdown GUI
    GameObject *countdown3UI;
    GameObject *countdown2UI;
    GameObject *countdown1UI;
    GameObject *countdownFightUI;
    
    // Referece to player Analog control
    GUIAnalog *playerWalkController;
    
    // Pointer to health bars
    GUIProgressBar *playerHealthBar;
    GUIProgressBar *opponentHealthBar;
    
    // Pointer to portaits
    GUIProperty *playerPortrait;
    GUIProperty *opponentPortrait;
    
    // Gameplay vars
    bool countDownFlag;
    double timeElapsed;
    
    FightSceneLogic(Scene *scene) : SceneLogic(scene), state(GameState::PROMPT_TRACKER)
    {
    }
    
    void initialise();
    void initialiseScene();
    void initialiseGame();
    void update();
    void draw();
    
    // Gameplay functions
    void countDown();
    void gameplay();
    
    // Gameplay control functions
    void walkAnalogMove();
    void walkAnalogUp();
    void punchButtonUp();
    void kickButtonUp();
    void blockButtonUp();
    void trackStart();
    void trackConfirmYes();
    void trackConfirmNo();
};

#endif /* _FIGHTSCENELOGIC_H */
