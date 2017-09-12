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
    
    enum class GameState { PROMPT_TRACKER, PROMPTING_TRACKER, STARTED_TRACKER, PROMPT_TRACK_CONFIRM, PROMPTING_TRACK_CONFIRM, PLAYING, PAUSING, PAUSED, RESUMING, QUIT, PLAYER_WON, OPPONENT_WON};
    GameState state;
    
    std::string playerName;
    std::string opponentName;
    Character *player;
    Character *opponent;
    AREntity *arHandler;
    
    // To house GUI stuff
    GameObject *trackStartUI;
    GameObject *trackConfirmUI;
    GameObject *playCharacterInfoUI;
    GameObject *playControlUI;
    GameObject *pauseUI;
    GameObject *quitUI;
    GameObject *playerWonUI;
    GameObject *opponentWonUI;
    
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
    
    // Pause button
    GUIProperty *pauseButton;
    GUIProperty *resumeButton;
    
    // Gameplay vars
    bool countDownFlag;
    double timeElapsed;
    
    FightSceneLogic(Scene *scene);
    
    void initialiseScene();
    void initialiseGame();
    
    // Gameplay functions
    void countDown();
    void gameplay();
    void playerPunched();
    void opponentPunched();
    
    // Gameplay control functions
    void pause();
    void resume();
    void quit();
    void retrack();
    void walkAnalogMove();
    void walkAnalogUp();
    void punchButtonUp();
    void kickButtonUp();
    void blockButtonUp();
    void trackStart();
    void trackConfirmYes();
    void trackConfirmNo();
    
    
protected:
    void initialise();
    void deinitialise();
    void update();
    void draw();
    
};

#endif /* _FIGHTSCENELOGIC_H */
