#include "ar-fighter/FightSceneLogic.h"

// Game Engine Core
#include "game-engine/Core/GL/GL.h"
#include "game-engine/Core/GL/GLTexture.h"

// Game Engine Device
#include <game-engine/Device/System.h>

// Game Engine Scene
#include "game-engine/Scene/Scene.h"
#include "game-engine/Scene/SceneManager.h"

// Game Engine Graphics
#include <game-engine/Modules/Graphics/Graphics.h>
#include <game-engine/Modules/Graphics/CameraEntity.h>
#include <game-engine/Modules/Graphics/DirectionalLightProperty.h>

// Game Engine AR Module
#include <game-engine/Modules/AR/AR.h>
#include <game-engine/Modules/AR/AREntity.h>
#include <game-engine/Modules/AR/ARTracker.h>

// Game Engine Physics
#include <game-engine/Modules/Physics/Physics.h>

// Game Engine GUI
#include <game-engine/Modules/GUI/GUI.h>
#include <game-engine/Modules/GUI/GUIProperty.h>
#include <game-engine/Modules/GUI/GUIRectangle.h>
#include <game-engine/Modules/GUI/GUIAnalog.h>
#include <game-engine/Modules/GUI/GUIProgressBar.h>

// Game Engine AI
#include <game-engine/Modules/AI/AIFSM.h>
#include <game-engine/Modules/AI/AIDT.h>

#include "game-engine/GameObject.h"

#include "ar-fighter/Game_Objects/Character.h"
#include "ar-fighter/Game_Objects/Y_Bot.h"
#include "ar-fighter/Game_Objects/X_Bot.h"

// Game Engine Util
#include "game-engine/Util/TimeUtil.h"
#include "game-engine/Util/TextureUtil.h"

// Game AI
#include "ar-fighter/AI/FSM/AIFSMStateCombat.h"
#include "ar-fighter/AI/FSM/AIFSMStateLose.h"
#include "ar-fighter/AI/FSM/AIFSMStateMoveToPlayer.h"
#include "ar-fighter/AI/FSM/AIFSMStateWin.h"

FightSceneLogic::FightSceneLogic(Scene *scene)
    :SceneLogic(scene),
    state(GameState::PROMPT_TRACKER),
    player(NULL),
    opponent(NULL),
    arHandler(NULL),
    trackStartUI(NULL),
    trackConfirmUI(NULL),
    playCharacterInfoUI(NULL),
    playControlUI(NULL),
    pauseUI(NULL),
    quitUI(NULL),
    playerWonUI(NULL),
    opponentWonUI(NULL),
    countdown3UI(NULL),
    countdown2UI(NULL),
    countdown1UI(NULL),
    countdownFightUI(NULL),
    playerWalkController(NULL),
    playerHealthBar(NULL),
    opponentHealthBar(NULL),
    playerPortrait(NULL),
    opponentPortrait(NULL),
    pauseButton(NULL),
    resumeButton(NULL)
{
    
}

void FightSceneLogic::initialise()
{
    initialiseScene();
    initialiseGame();
}

void FightSceneLogic::initialiseGame()
{
    countDownFlag = true;
    timeElapsed = 0.0;
    
    // Transform the players to their start positions and make them face eachother
    
    player->translateOW(-150.0f, 0.0f, 0.0f);
    //player->rotateOW(glm::radians(90.0f), 0.0f, 1.0f, 0.0f);
    
    opponent->translateOW(150.0f, 0.0f, 0.0f);
    
    player->faceTarget(opponent->getPosition());
    opponent->faceTarget(player->getPosition());
    
    //opponent->rotateOW(glm::radians(-90.0f), 0.0f, 1.0f, 0.0f);
}

void FightSceneLogic::update()
{
    // Update Animation module
    Engine::getInstance().update(CoreModuleType::CM_ANIMATION, false);
    
    // Update Physics module
    Engine::getInstance().update(CoreModuleType::CM_PHYSICS, false);
    
    // Update AI module
    if(state == GameState::PLAYING)
    {
        if(!countDownFlag)
        {
            Engine::getInstance().update(CoreModuleType::CM_AI, false);
        }
        
    }
    
    if(state == GameState::PROMPT_TRACK_CONFIRM)
    {
        // Enable these in scene
        mScene->prepare(player);
        mScene->prepare(opponent);
        mScene->prepare(trackConfirmUI);
            
        // Disable these in scene
        mScene->unPrepare(trackStartUI);
            
        state = GameState::PROMPTING_TRACK_CONFIRM;
    }
    else if(state == GameState::STARTED_TRACKER)
    {
        // Enable these in scene
        mScene->prepare(playCharacterInfoUI);
        mScene->prepare(playControlUI);
        
        // Disable these in scene
        mScene->unPrepare(trackConfirmUI);
        
        state = GameState::PLAYING;
    }
    else if(state == GameState::PROMPT_TRACKER)
    {
        player->translateOW(-150.0f, 0.0f, 0.0f);
        //player->rotateOW(glm::radians(90.0f), 0.0f, 1.0f, 0.0f);
        
        opponent->translateOW(150.0f, 0.0f, 0.0f);
        
        player->faceTarget(opponent->getPosition());
        opponent->faceTarget(player->getPosition());
        
        // Enable these in scene
        mScene->prepare(trackStartUI);
        
        // Disable these in scene
        mScene->unPrepare(player);
        mScene->unPrepare(opponent);
        mScene->unPrepare(playCharacterInfoUI);
        mScene->unPrepare(playControlUI);
        mScene->unPrepare(trackConfirmUI);
        mScene->unPrepare(pauseUI);
        mScene->unPrepare(quitUI);
        mScene->unPrepare(playerWonUI);
        mScene->unPrepare(opponentWonUI);
        mScene->unPrepare(countdown3UI);
        mScene->unPrepare(countdown2UI);
        mScene->unPrepare(countdown1UI);
        mScene->unPrepare(countdownFightUI);
        
        state = GameState::PROMPTING_TRACKER;
    }
    else if (state == GameState::PLAYING)
    {
        if(countDownFlag)
        {
            countDown();
        }
        else
        {
            gameplay();
        }
    }
    else if (state == GameState::PAUSING)
    {
        player->state = Character::IDLE;
        player->getAnimator()->getAnimationController1()->stop();
        player->getAnimator()->getAnimationController2()->stop();
        
        opponent->state = Character::IDLE;
        opponent->getAnimator()->getAnimationController1()->stop();
        opponent->getAnimator()->getAnimationController2()->stop();
        
        // Enable these in scene
        mScene->prepare(pauseUI);
        mScene->prepare(quitUI);
        
        // Disable these in scene
        //mScene->unPrepare(playCharacterInfoUI);
        mScene->unPrepare(playControlUI);
        mScene->unPrepare(countdown3UI);
        mScene->unPrepare(countdown2UI);
        mScene->unPrepare(countdown1UI);
        mScene->unPrepare(countdownFightUI);
        
        state = GameState::PAUSED;
    }
    else if (state == GameState::RESUMING)
    {
        // Enable these in scene
        //mScene->prepare(playCharacterInfoUI);
        mScene->prepare(playControlUI);
        
        // Disable these in scene
        mScene->unPrepare(pauseUI);
        mScene->unPrepare(quitUI);
        
        state = GameState::PLAYING;
    }
    else if(state == GameState::QUIT)
    {
        // Set up the next scene
        Scene *scene = SceneManager::getInstance().getScene("main-menu");
        
        Physics *p = &Physics::getInstance();
        mScene->deinitialise();
        
        //Graphics *g = &Graphics::getInstance();
        //GUI *gu = &GUI::getInstance();
        //AR *a = &AR::getInstance();
        
        
        scene->initialise();
        SceneManager::getInstance().makeActiveScene("main-menu");
        return;
    }
    else if (state == GameState::PLAYER_WON)
    {
        
        //player->win();
        //opponent->lose();
        
        // Enable these in scene
        mScene->prepare(playerWonUI);
        mScene->prepare(quitUI);
        
        // Disable these in scene
        //mScene->unPrepare(playCharacterInfoUI);
        mScene->unPrepare(pauseUI);
        mScene->unPrepare(playControlUI);
        mScene->unPrepare(countdown3UI);
        mScene->unPrepare(countdown2UI);
        mScene->unPrepare(countdown1UI);
        mScene->unPrepare(countdownFightUI);
    }
    else if (state == GameState::OPPONENT_WON)
    {
        //opponent->win();
        //player->lose();
        
        // Enable these in scene
        mScene->prepare(opponentWonUI);
        mScene->prepare(quitUI);
        
        // Disable these in scene
        //mScene->unPrepare(playCharacterInfoUI);
        mScene->unPrepare(pauseUI);
        mScene->unPrepare(playControlUI);
        mScene->unPrepare(countdown3UI);
        mScene->unPrepare(countdown2UI);
        mScene->unPrepare(countdown1UI);
        mScene->unPrepare(countdownFightUI);
    }
    
    // Check to see if we're not tracking and not prompting user to track
    if( !arHandler->getTracker()->isTracking() && state != GameState::PROMPTING_TRACKER)
    {
        if(state == GameState::PLAYER_WON || state == GameState::OPPONENT_WON)
        {
            state = GameState::QUIT;
        }
        else
            state = GameState::PROMPT_TRACKER;
    }
    
}

void FightSceneLogic::gameplay()
{
    player->faceTarget(opponent->getPosition());
    opponent->faceTarget(player->getPosition());
}

void FightSceneLogic::countDown()
{
    // Prepare countdown
    static bool ready = true;
    if(ready)
    {
        
    }
    
    // Double buffer update flag
    static bool update1 = true;
    static bool update2 = false;

    
    if(timeElapsed > 4.0)
    {
        countDownFlag = false;
        mScene->unPrepare(countdownFightUI);
    }
    else if(timeElapsed > 3.0)
    {
        // Prepare fight gui
        if(update2)
        {
            std::cout << "Fight!" << std::endl;
            
            mScene->prepare(countdownFightUI);
            mScene->unPrepare(countdown1UI);
            
            update2 = false;
            update1 = true;
        }
    }
    else if(timeElapsed > 2.0)
    {
        // Prepare 1 gui
        if(update1)
        {
            std::cout << "1" << std::endl;
            
            mScene->prepare(countdown1UI);
            mScene->unPrepare(countdown2UI);
            
            update1 = false;
            update2 = true;
        }
    }
    else if(timeElapsed > 1.0)
    {
        // Prepare 2 gui
        if(update2)
        {
            std::cout << "2" << std::endl;
            
            mScene->prepare(countdown2UI);
            mScene->unPrepare(countdown3UI);
            
            update2 = false;
            update1 = true;
        }
    }
    else
    {
        // Prepare 3 gui
        if(update1)
        {
            std::cout << "3" << std::endl;
            
            mScene->prepare(countdown3UI);
            
            update1 = false;
            update2 = true;
        }
    }
    
    timeElapsed += timeSinceLastUpdate;
    
}

void FightSceneLogic::playerPunched()
{
    if(player->state == Character::CharacterState::BLOCKING)
    {
        return;
    }
    
    std::cout << "Player punched!" << std::endl;
    
    if(opponent->getCanDealDamage())
    {
        // Update health
        player->takeDamage(opponent->getDamageInflict());
    
        // Update health progress bar
        playerHealthBar->setProgression(player->getHealth() / player->getMaxHealth());
        
        opponent->damageDealt();
    }
    
    if(!player->isAlive())
    {
        state = GameState::OPPONENT_WON;
        player->lose();
        opponent->win();
    }
}

void FightSceneLogic::opponentPunched()
{
    if(opponent->state == Character::CharacterState::BLOCKING)
    {
        return;
    }
    
    std::cout << "Opponent punched!" << std::endl;
    
    if(player->getCanDealDamage())
    {
        // Update health
        opponent->takeDamage(player->getDamageInflict());
    
        // Update health progress bar
        opponentHealthBar->setProgression(opponent->getHealth() / opponent->getMaxHealth());
        
        player->damageDealt();
    }
    
    if(!opponent->isAlive())
    {
        state = GameState::PLAYER_WON;
        player->win();
        opponent->lose();
    }
}

void FightSceneLogic::draw()
{
    jmpGLClearColour(0.65, 0.65, 0.65, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Engine *engine = &Engine::getInstance();
    
    // Update AR first
    engine->update(CoreModuleType::CM_AR);
    
    // Update Graphics second
    engine->update(CoreModuleType::CM_GRAPHICS);
    
    // Update the GUI
    engine->update(CoreModuleType::CM_GUI);
}

void FightSceneLogic::pause()
{
    if(state == GameState::PLAYING)
    {
        std::cout << "Paused!" << std::endl;
        state = GameState::PAUSING;
    }
}

void FightSceneLogic::resume()
{
    if(state == GameState::PAUSED)
    {
        std::cout << "Resuming!" << std::endl;
        state = GameState::RESUMING;
    }
}

void FightSceneLogic::quit()
{
    if(state == GameState::PAUSED || state == GameState::OPPONENT_WON || state == GameState::PLAYER_WON)
    {
        std::cout << "QUIT!" << std::endl;
        state = GameState::QUIT;
    }
}

void FightSceneLogic::retrack()
{
    if(state == GameState::PAUSED)
    {
        state = GameState::PROMPT_TRACKER;
    }
}

void FightSceneLogic::walkAnalogMove()
{
    // Get the offset
    if(!countDownFlag)
    {
        glm::vec2 offset = playerWalkController->getOffset();
        
        // Call the player walk function with the analog offset
        player->walk(-offset.x, offset.y);
    }
}

void FightSceneLogic::walkAnalogUp()
{
    // Get the offset
    if(!countDownFlag)
    {
        player->idle();
    }
}

void FightSceneLogic::punchButtonUp()
{
    // Get the offset
    if(!countDownFlag)
    {
        player->punch();
    }
}

void FightSceneLogic::kickButtonUp()
{
    // Get the offset
    if(!countDownFlag)
    {
        player->kick();
    }
}

void FightSceneLogic::blockButtonUp()
{
    // Get the offset
    if(!countDownFlag)
    {
        player->block();
    }
}

void FightSceneLogic::trackStart()
{
    arHandler->startTracking(glm::vec3(0.0f, 0.0f, 300.0f));
    
    state = GameState::PROMPT_TRACK_CONFIRM;
}

void FightSceneLogic::trackConfirmYes()
{
    state = GameState::STARTED_TRACKER;
}

void FightSceneLogic::trackConfirmNo()
{
    arHandler->stopTracking();
    
    state = GameState::PROMPT_TRACKER;
}

void FightSceneLogic::initialiseScene()
{
    unsigned short playerMask = 1;
    unsigned short opponentMask = 2;
    
    if(playerName == "y_bot")
    {
        // YBot
        YBot *yBot = new YBot(playerMask, opponentMask);
        yBot->initialise();
        yBot->initialisePhysics();
        yBot->translate(-150.0, 0.0, 0.0);
        mScene->addEntity(yBot);
        yBot->idle();
        player = yBot;
    }
    else if(playerName == "x_bot")
    {
        // X_Bot
        XBot *xBot = new XBot(playerMask, opponentMask);
        xBot->initialise();
        xBot->initialisePhysics();
        xBot->translate(150.0, 0.0, 0.0);
        
        mScene->addEntity(xBot);
        xBot->idle();
        player = xBot;
    }
    
    
    if(opponentName == "y_bot")
    {
        // YBot
        YBot *yBot = new YBot(opponentMask, playerMask);
        yBot->initialise();
        yBot->initialisePhysics();
        yBot->translate(-150.0, 0.0, 0.0);
        mScene->addEntity(yBot);
        yBot->idle();
        opponent = yBot;
    }
    else if(opponentName == "x_bot")
    {
        // X_Bot
        XBot *xBot = new XBot(opponentMask, playerMask);
        xBot->initialise();
        xBot->initialisePhysics();
        xBot->translate(150.0, 0.0, 0.0);
        mScene->addEntity(xBot);
        xBot->idle();
        opponent = xBot;
    }
    
    AIFSM *fsm = new AIFSM("opponent");
    
    AIFSMStateMoveToPlayer *state1  = new AIFSMStateMoveToPlayer(opponent);
    AIFSMStateCombat *state2        = new AIFSMStateCombat(opponent);
    AIFSMStateWin *state3           = new AIFSMStateWin(opponent);
    AIFSMStateLose *state4          = new AIFSMStateLose(opponent);
    
    fsm->addState("move-to-player", state1);
    fsm->addState("combat", state2);
    fsm->addState("win", state3);
    fsm->addState("lose", state4);
    
    fsm->setCurrentState("move-to-player");
    
    opponent->addProperty(fsm);
    
    if(difficulty == GameDifficulty::NORMAL)
    {
        opponent->setDifficulty(30);
    }
    else
    {
        opponent->setDifficulty(5);
    }
    
    // Add callback functions to characters when punched
    
    player->setPunchedCallback(std::bind(&FightSceneLogic::playerPunched, this));
    opponent->setPunchedCallback(std::bind(&FightSceneLogic::opponentPunched, this));
    
    player->setOpponent(opponent);
    opponent->setOpponent(player);
    
    player->setSpeed(2.0f);
    opponent->setSpeed(2.0f);
    
    // Here we will add other stuff to the scene
    
    
    //
    // AR
    //
    
    AREntity *arEntity = new AREntity("backcamera");
    arEntity->translate(0.0, 0.0, 300.0);
    this->mScene->addEntity(arEntity);
    arEntity->initialise();
    arEntity->startCapture();
    CameraCapture::getInstance().addDelegate(arEntity);
    AR::getInstance().setActiveAREntity("backcamera");
    arHandler = arEntity;
    
    //this->mScene->removeEntity("glcamera");
    
    
    //
    // CAMERA
    //
    
    CameraEntity *camEntity = new CameraEntity("glCamera", CameraEntity::perspectiveMatrix(System::screenWidth, System::screenHeight, 47.0));
    //camEntity->translate(0.0, 100.0, 300.0);
    camEntity->initialise();
    Graphics::getInstance().setActiveCameraEntity("glCamera");
    arEntity->addChild(camEntity);
    
    //
    // LIGHT
    //
    
    // Add a light to the scene
    GameObject *sun = new GameObject("sun");
    DirectionalLightProperty *light = new DirectionalLightProperty("light", glm::normalize(glm::vec3(0.0, -5.0, 10.0)), glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.6, 0.6, 0.6), glm::vec3(0.8, 0.8, 0.8));
    sun->addProperty(light);
    mScene->addEntity(sun);
    
    
    // We need 2 game objects to house gui stuff for the tracking inisalising state, and the game playing state
    this->trackStartUI = new GameObject("track-start-ui");
    this->trackConfirmUI = new GameObject("track-confirm-ui");
    this->playCharacterInfoUI = new GameObject("play-character-info-ui");
    this->playControlUI = new GameObject("play-control-ui");
    this->pauseUI = new GameObject("pause-ui");
    this->quitUI = new GameObject("quit-ui");
    this->playerWonUI = new GameObject("player-won-ui");
    this->opponentWonUI = new GameObject("opponent-won-ui");
    
    mScene->addEntity(trackStartUI);
    mScene->addEntity(trackConfirmUI);
    mScene->addEntity(playCharacterInfoUI);
    mScene->addEntity(playControlUI);
    mScene->addEntity(pauseUI);
    mScene->addEntity(quitUI);
    mScene->addEntity(playerWonUI);
    mScene->addEntity(opponentWonUI);
    
    
    
    // Some vars to help with ui layout
    float halfScreen = (float)System::screenWidth / 2.0f;
    float paddingSides = 25.0f;
    float paddingTop = 30.0f;
    float left = paddingSides;
    float right = (float)System::screenWidth - paddingSides;
    float top = (float)System::screenHeight - paddingTop;
    
    ///// For trackStartUI
    
    // Button to start AR tracking
    {
        GUIProperty *aRTrackButton = new GUIProperty("ar-track-button");
        
        aRTrackButton->isTouchable = true;
        std::function<void(void)> arCallbackFunc = std::bind(&FightSceneLogic::trackStart, this);
        aRTrackButton->setCallbackOnTouchUp(arCallbackFunc);
        glm::vec2 bounds(500.0f, 100.f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2(((float)System::screenWidth / 2.0f), 100.0f));
        //guiRectangle->setColourUp(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        guiRectangle->setColourDown(glm::vec4(-0.2f, -0.2f, -0.2f, 0.0f));
        Texture *texture = Texture::loadFromFile("textures/button_track.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        aRTrackButton->addShape(guiRectangle);
        trackStartUI->addProperty(aRTrackButton);
    }
    
    // Countdown 3
    {
        GUIProperty *guiButton = new GUIProperty("ar-indicator");
        
        glm::vec2 bounds(300.0f, 300.f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2((float)System::screenWidth*0.5f, (float)System::screenHeight*0.5f));
        
        Texture *texture = Texture::loadFromFile("textures/ar-indicator.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        trackStartUI->addProperty(guiButton);
    }
    
    ///// For trackConfirmUI
    
    // Confirm text
    {
        GUIProperty *guiButton = new GUIProperty("confirm-text");
        
        glm::vec2 bounds(433.0f, 55.0f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2(((float)System::screenWidth - 275.0f), 200.0f));
        
        Texture *texture = Texture::loadFromFile("textures/confirm.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        trackConfirmUI->addProperty(guiButton);
    }
    
    // AR Track confirm yes button
    {
        GUIProperty *aRTrackConfirmButtomYes = new GUIProperty("ar-track-confirm-button-yes");
        
        aRTrackConfirmButtomYes->isTouchable = true;
        std::function<void(void)> func = std::bind(&FightSceneLogic::trackConfirmYes, this);
        aRTrackConfirmButtomYes->setCallbackOnTouchUp(func);
        glm::vec2 bounds(200.0f, 50.f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2(((float)System::screenWidth - 150.0f), 100.0f));
        guiRectangle->setColourDown(glm::vec4(-0.2f, -0.2f, -0.2f, 0.0f));
        
        Texture *texture = Texture::loadFromFile("textures/button_yes.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        aRTrackConfirmButtomYes->addShape(guiRectangle);
        trackConfirmUI->addProperty(aRTrackConfirmButtomYes);
    }
    
    // AR Track confirm no button
    {
        GUIProperty *aRTrackConfirmButtomNo = new GUIProperty("ar-track-confirm-button-no");
        
        aRTrackConfirmButtomNo->isTouchable = true;
        std::function<void(void)> func = std::bind(&FightSceneLogic::trackConfirmNo, this);
        aRTrackConfirmButtomNo->setCallbackOnTouchUp(func);
        glm::vec2 bounds(200.0f, 50.f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2(((float)System::screenWidth - 400.0f), 100.0f));
        guiRectangle->setColourDown(glm::vec4(-0.2f, -0.2f, -0.2f, 0.0f));
        
        Texture *texture = Texture::loadFromFile("textures/button_no.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        aRTrackConfirmButtomNo->addShape(guiRectangle);
        trackConfirmUI->addProperty(aRTrackConfirmButtomNo);
    }
    
    ///// For playingUI
    
    {
        
        
        // Portraits are 15% width
        float portrait = (float)System::screenWidth * 0.15f;
        
        
        // Work out bar width
        // It will be from the portrait to 5% from middle
        float barWidth = halfScreen - portrait - paddingSides - ((float)System::screenWidth*0.05f);
        float barHeight = ((float)System::screenHeight*0.05f);
        
        float barPosition = barWidth * 0.5f;
        
        // Player portrait
        {
            playerPortrait = new GUIProperty("player-portrait");
            
            glm::vec2 bounds(portrait);
            GUIRectangle *guiRectangle = new GUIRectangle(bounds);
            guiRectangle->translateOW(
                                      glm::vec2(
                                                (portrait/2.0f) + paddingSides,
                                                top - (portrait/2.0f)
                                                )
                                      );
            //guiRectangle->translateOW(glm::vec2(200.0f));
            Texture *texture = Texture::loadFromFile("textures/portraits/y-bot-portrait.png", true);
            
            if(texture != NULL)
            {
                GLTexture *glTexture = GLTexture::loadFromData(*texture);
                
                guiRectangle->setMapUp(glTexture);
                guiRectangle->setMapDown(glTexture);
                
                delete texture;
            }
            
            playerPortrait->addShape(guiRectangle);
            playCharacterInfoUI->addProperty(playerPortrait);
            
        }
        
        // Player health bar
        {
            
            playerHealthBar = new GUIProgressBar("player-health-bar",
                                                 glm::vec2(barWidth, barHeight),
                                                 glm::vec2(
                                                           paddingSides + portrait + barPosition,
                                                           top - (barHeight/2.0f)
                                                           )
                                                 );
            
            playerHealthBar->setColourUp(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), player->getColourTheme());
            playerHealthBar->setProgression(1.0f);
            
            playCharacterInfoUI->addProperty(playerHealthBar);
        }
        
        // Opponent portrait
        {
            opponentPortrait = new GUIProperty("opponent-portrait");
            
            glm::vec2 bounds(portrait);
            GUIRectangle *guiRectangle = new GUIRectangle(bounds);
            guiRectangle->translateOW(
                                      glm::vec2(
                                                (float)System::screenWidth - (portrait/2.0f) - paddingSides,
                                                top - (portrait/2.0f)
                                                )
                                      );
            //guiRectangle->translateOW(glm::vec2(200.0f));
            Texture *texture = Texture::loadFromFile("textures/portraits/x-bot-portrait.png", true);
            
            if(texture != NULL)
            {
                GLTexture *glTexture = GLTexture::loadFromData(*texture);
                
                guiRectangle->setMapUp(glTexture);
                guiRectangle->setMapDown(glTexture);
                
                delete texture;
            }
            
            opponentPortrait->addShape(guiRectangle);
            playCharacterInfoUI->addProperty(opponentPortrait);
            
        }
        
        // Opponent health bar
        {
            opponentHealthBar = new GUIProgressBar("opponent-health-bar",
                                                   glm::vec2(barWidth, barHeight),
                                                   glm::vec2(
                                                             (float)System::screenWidth - paddingSides - portrait - barPosition,
                                                             top - (barHeight/2.0f)
                                                             )
                                                   );
            
            opponentHealthBar->setColourUp(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), opponent->getColourTheme());
            opponentHealthBar->setReverse(true);
            opponentHealthBar->setProgression(1.0f);
            
            playCharacterInfoUI->addProperty(opponentHealthBar);
        }
        
    }
    
    // Analog to control player movement
    {
        GUIAnalog *playerAnalogControl = new GUIAnalog("player-walk-control", 150.0f, 100.0f);
        
        playerAnalogControl->isTouchable = true;
        playerAnalogControl->setColourUp(glm::vec4(0.5f, 0.5f, 0.5f, 0.8f), glm::vec4(0.7f, 0.7f, 0.7f, 0.8f));
        playerAnalogControl->setColourDown(glm::vec4(0.5f, 0.5f, 0.5f, 0.8f), glm::vec4(0.7f, 0.7f, 0.7f, 0.8f));
        playerAnalogControl->setDrawOnTouched(0.0, (float)System::screenWidth / 2.0f, (float)System::screenHeight, 0.0);
        std::function<void(void)> walkMoveFunc = std::bind(&FightSceneLogic::walkAnalogMove, this);
        std::function<void(void)> walkUpFunc = std::bind(&FightSceneLogic::walkAnalogUp, this);
        playerAnalogControl->setCallbackOnTouchMove(walkMoveFunc);
        playerAnalogControl->setCallbackOnTouchUp(walkUpFunc);
        playControlUI->addProperty(playerAnalogControl);
        playerWalkController = playerAnalogControl;
    }
    
    // Player punch button
    {
        GUIProperty *guiButtonPunch = new GUIProperty("player-punch-control");
        
        guiButtonPunch->isTouchable = true;
        std::function<void(void)> func = std::bind(&FightSceneLogic::punchButtonUp, this);
        guiButtonPunch->setCallbackOnTouchUp(func);
        glm::vec2 bounds(200.0f, 100.f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2(((float)System::screenWidth - 150.0f), 100.0f));
        guiRectangle->setColourDown(glm::vec4(-0.2f, -0.2f, -0.2f, 0.0f));
        
        Texture *texture = Texture::loadFromFile("textures/button_punch.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButtonPunch->addShape(guiRectangle);
        playControlUI->addProperty(guiButtonPunch);
    }
    
    // Player kick button
    /*{
     GUIProperty *guiButton = new GUIProperty("player-kick-control");
     
     guiButton->isTouchable = true;
     std::function<void(void)> func = std::bind(&FightSceneLogic::kickButtonUp, this);
     guiButton->setCallbackOnTouchUp(func);
     glm::vec2 bounds(200.0f, 100.f);
     GUIRectangle *guiRectangle = new GUIRectangle(bounds);
     guiRectangle->translateOW(glm::vec2(((float)System::screenWidth - 400.0f), 100.0f));
     guiRectangle->setColourDown(glm::vec4(-0.2f, -0.2f, -0.2f, 0.0f));
     
     Texture *texture = Texture::loadFromFile("textures/button_kick.png", true);
     
     if(texture != NULL)
     {
     GLTexture *glTexture = GLTexture::loadFromData(*texture);
     
     guiRectangle->setMapUp(glTexture);
     guiRectangle->setMapDown(glTexture);
     
     delete texture;
     }
     
     guiButton->addShape(guiRectangle);
     playControlUI->addProperty(guiButton);
     }*/
    
    // Player block button
    {
        GUIProperty *guiButton = new GUIProperty("player-block-control");
        
        guiButton->isTouchable = true;
        std::function<void(void)> func = std::bind(&FightSceneLogic::blockButtonUp, this);
        guiButton->setCallbackOnTouchUp(func);
        glm::vec2 bounds(200.0f, 100.f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2(((float)System::screenWidth - 150.0f), 250.0f));
        guiRectangle->setColourDown(glm::vec4(-0.2f, -0.2f, -0.2f, 0.0f));
        
        Texture *texture = Texture::loadFromFile("textures/button_block.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        playControlUI->addProperty(guiButton);
    }
    
    // Pause button
    {
        pauseButton = new GUIProperty("pause-control");
        
        pauseButton->isTouchable = true;
        std::function<void(void)> func = std::bind(&FightSceneLogic::pause, this);
        pauseButton->setCallbackOnTouchUp(func);
        glm::vec2 bounds(200.0f, 100.f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2(((float)System::screenWidth - 150.0f), (float)System::screenHeight * 0.5f));
        guiRectangle->setColourDown(glm::vec4(-0.2f, -0.2f, -0.2f, 0.0f));
        
        Texture *texture = Texture::loadFromFile("textures/button_pause.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        pauseButton->addShape(guiRectangle);
        playControlUI->addProperty(pauseButton);
    }
    
    
    // The count down game objects
    countdown3UI = new GameObject("countdown-3");
    countdown2UI = new GameObject("countdown-2");
    countdown1UI = new GameObject("countdown-1");
    countdownFightUI = new GameObject("countdown-fight");
    
    mScene->addEntity(countdown3UI);
    mScene->addEntity(countdown2UI);
    mScene->addEntity(countdown1UI);
    mScene->addEntity(countdownFightUI);
    
    
    // Countdown 3
    {
        GUIProperty *guiButton = new GUIProperty("countdown-3");
        
        glm::vec2 bounds(300.0f, 300.f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2((float)System::screenWidth*0.5f, (float)System::screenHeight*0.5f));
        
        Texture *texture = Texture::loadFromFile("textures/3-300x300.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        countdown3UI->addProperty(guiButton);
    }
    
    // Countdown 2
    {
        GUIProperty *guiButton = new GUIProperty("countdown-2");
        
        glm::vec2 bounds(300.0f, 300.f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2((float)System::screenWidth*0.5f, (float)System::screenHeight*0.5f));
        
        Texture *texture = Texture::loadFromFile("textures/2-300x300.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        countdown2UI->addProperty(guiButton);
    }
    
    // Countdown 1
    {
        GUIProperty *guiButton = new GUIProperty("countdown-1");
        
        glm::vec2 bounds(300.0f, 300.f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2((float)System::screenWidth*0.5f, (float)System::screenHeight*0.5f));
        
        Texture *texture = Texture::loadFromFile("textures/1-300x300.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        countdown1UI->addProperty(guiButton);
    }
    
    // Countdown fight
    {
        GUIProperty *guiButton = new GUIProperty("countdown-fight");
        
        glm::vec2 bounds(615.0f, 137.0f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2((float)System::screenWidth*0.5f, (float)System::screenHeight*0.5f));
        
        Texture *texture = Texture::loadFromFile("textures/fight.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        countdownFightUI->addProperty(guiButton);
    }
    
    //// For pause UI
    
    
    // retrack button
    {
        GUIProperty *guiButton = new GUIProperty("retrack-control");
        
        guiButton->isTouchable = true;
        std::function<void(void)> func = std::bind(&FightSceneLogic::retrack, this);
        guiButton->setCallbackOnTouchUp(func);
        glm::vec2 bounds(500.0f, 100.f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2(((float)System::screenWidth * 0.5f), (float)System::screenHeight * 0.5f));
        guiRectangle->setColourDown(glm::vec4(-0.2f, -0.2f, -0.2f, 0.0f));
        
        Texture *texture = Texture::loadFromFile("textures/button_re-track.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        pauseUI->addProperty(guiButton);
    }
    
    // Resume button
    {
        resumeButton = new GUIProperty("resume-control");
        
        resumeButton->isTouchable = true;
        std::function<void(void)> func = std::bind(&FightSceneLogic::resume, this);
        resumeButton->setCallbackOnTouchUp(func);
        glm::vec2 bounds(500.0f, 100.f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2(((float)System::screenWidth * 0.5f), ((float)System::screenHeight * 0.5f) - (bounds.y) - 200.0f));
        guiRectangle->setColourDown(glm::vec4(-0.2f, -0.2f, -0.2f, 0.0f));
        
        Texture *texture = Texture::loadFromFile("textures/button_resume.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        resumeButton->addShape(guiRectangle);
        pauseUI->addProperty(resumeButton);
    }
    
    // Pause text
    {
        GUIProperty *guiButton = new GUIProperty("paused-text");
        
        glm::vec2 bounds(807.0f, 137.0f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2((float)System::screenWidth*0.5f, (float)System::screenHeight*0.75f));
        
        Texture *texture = Texture::loadFromFile("textures/paused.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        pauseUI->addProperty(guiButton);
    }
    
    
    
    
    // For quit UI
    
    // quit button
    {
        GUIProperty *guiButton = new GUIProperty("quit-control");
        
        guiButton->isTouchable = true;
        std::function<void(void)> func = std::bind(&FightSceneLogic::quit, this);
        guiButton->setCallbackOnTouchUp(func);
        glm::vec2 bounds(500.0f, 100.f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2(((float)System::screenWidth * 0.5f), ((float)System::screenHeight * 0.5f) - (bounds.y) - 50.0f));
        guiRectangle->setColourDown(glm::vec4(-0.2f, -0.2f, -0.2f, 0.0f));
        
        Texture *texture = Texture::loadFromFile("textures/button_quit.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        quitUI->addProperty(guiButton);
    }
    
    
    // For player won ui
    
    // Win text
    {
        GUIProperty *guiButton = new GUIProperty("win-text");
        
        glm::vec2 bounds(874.0f, 137.0f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2((float)System::screenWidth*0.5f, (float)System::screenHeight*0.75f));
        
        Texture *texture = Texture::loadFromFile("textures/win.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        playerWonUI->addProperty(guiButton);
    }
    
    // For opponent won ui
    
    // Lose text
    {
        GUIProperty *guiButton = new GUIProperty("lose-text");
        
        glm::vec2 bounds(1065.0f, 137.0f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2((float)System::screenWidth*0.5f, (float)System::screenHeight*0.75f));
        
        Texture *texture = Texture::loadFromFile("textures/lose.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        opponentWonUI->addProperty(guiButton);
    }
    
    // Prepare the tracking and playing state game objects
    // mScene->prepare(trackStartUI);
    //mScene->prepare(trackConfirmUI);
    //mScene->prepare(playCharacterInfoUI);
    //mScene->prepare(playControlUI);
    
}

void FightSceneLogic::deinitialise()
{
    state = GameState::PROMPT_TRACKER;
    player = NULL;
    opponent = NULL;
    arHandler = NULL;
    trackStartUI = NULL;
    trackConfirmUI = NULL;
    playCharacterInfoUI = NULL;
    playControlUI = NULL;
    pauseUI = NULL;
    quitUI = NULL;
    playerWonUI = NULL;
    opponentWonUI = NULL;
    countdown3UI = NULL;
    countdown2UI = NULL;
    countdown1UI = NULL;
    countdownFightUI = NULL;
    playerWalkController = NULL;
    playerHealthBar = NULL;
    opponentHealthBar = NULL;
    playerPortrait = NULL;
    opponentPortrait = NULL;
    pauseButton = NULL;
    resumeButton = NULL;
}
