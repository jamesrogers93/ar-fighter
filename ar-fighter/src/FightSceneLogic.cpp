#include "ar-fighter/FightSceneLogic.h"

// Game Engine Core
#include "game-engine/Core/GL/GL.h"
#include "game-engine/Core/GL/GLTexture.h"

// Game Engine Device
#include <game-engine/Device/System.h>

// Game Engine Scene
#include "game-engine/Scene/Scene.h"

// Game Engine Graphics
#include <game-engine/Modules/Graphics/Graphics.h>
#include <game-engine/Modules/Graphics/CameraEntity.h>

// Game Engine AR Module
#include <game-engine/Modules/AR/AR.h>
#include <game-engine/Modules/AR/AREntity.h>
#include <game-engine/Modules/AR/ARTracker.h>

// Game Engine GUI
#include <game-engine/Modules/GUI/GUI.h>
#include <game-engine/Modules/GUI/GUIProperty.h>
#include <game-engine/Modules/GUI/GUIRectangle.h>
#include <game-engine/Modules/GUI/GUIAnalog.h>
#include <game-engine/Modules/GUI/GUIProgressBar.h>

#include "game-engine/GameObject.h"

#include "ar-fighter/Game_Objects/Character.h"
#include "ar-fighter/Game_Objects/Y_Bot.h"

// Game Engine Util
#include "game-engine/Util/TimeUtil.h"
#include "game-engine/Util/TextureUtil.h"

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

void FightSceneLogic::initialiseScene()
{
    // Here we will add other stuff to the scene
    
    
    // We need to add an AR node to the scene and make the camera it's child
    AREntity *arEntity = new AREntity("backcamera");
    arEntity->translate(0.0, 0.0, 300.0);
    this->mScene->addEntity(arEntity);
    arEntity->initialise();
    arEntity->startCapture();
    CameraCapture::getInstance().addDelegate(arEntity);
    AR::getInstance().setActiveAREntity("backcamera");
    arHandler = arEntity;
    
    this->mScene->removeEntity("glcamera");
    
    CameraEntity *camEntity = new CameraEntity("glCamera", CameraEntity::perspectiveMatrix(System::screenWidth, System::screenHeight, 47.0));
    //camEntity->translate(0.0, 100.0, 300.0);
    camEntity->initialise();
    Graphics::getInstance().setActiveCameraEntity("glCamera");
    arEntity->addChild(camEntity);
    
    
    // We need 2 game objects to house gui stuff for the tracking inisalising state, and the game playing state
    this->trackStartUI = new GameObject("track-start-ui");
    this->trackConfirmUI = new GameObject("track-confirm-ui");
    this->playingUI = new GameObject("playing-ui");
    
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
        glm::vec2 bounds(300.0f, 126.6f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2(((float)System::screenWidth / 2.0f), 100.0f));
        //guiRectangle->setColourUp(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        guiRectangle->setColourDown(glm::vec4(-0.2f, -0.2f, -0.2f, 0.0f));
        Texture *texture = Texture::loadFromFile("textures/button_track.png", true);
    
        if(texture != NULL)
        {
            GLTexture glTexture = GLTexture::loadFromData(*texture);
        
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
        
            delete texture;
        }
    
        aRTrackButton->addShape(guiRectangle);
        trackStartUI->addProperty(aRTrackButton);
    }
    
    ///// For trackConfirmUI
    
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
            GLTexture glTexture = GLTexture::loadFromData(*texture);
            
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
            GLTexture glTexture = GLTexture::loadFromData(*texture);
            
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
                GLTexture glTexture = GLTexture::loadFromData(*texture);
                
                guiRectangle->setMapUp(glTexture);
                guiRectangle->setMapDown(glTexture);
                
                delete texture;
            }
            
            playerPortrait->addShape(guiRectangle);
            playingUI->addProperty(playerPortrait);
        
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

            playerHealthBar->setColourUp(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            playerHealthBar->setProgression(0.25f);
            
            playingUI->addProperty(playerHealthBar);
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
                GLTexture glTexture = GLTexture::loadFromData(*texture);
                
                guiRectangle->setMapUp(glTexture);
                guiRectangle->setMapDown(glTexture);
                
                delete texture;
            }
            
            opponentPortrait->addShape(guiRectangle);
            playingUI->addProperty(opponentPortrait);
            
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
            
            opponentHealthBar->setColourUp(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            opponentHealthBar->setReverse(true);
            opponentHealthBar->setProgression(0.25f);
            
            playingUI->addProperty(opponentHealthBar);
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
        playingUI->addProperty(playerAnalogControl);
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
            GLTexture glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButtonPunch->addShape(guiRectangle);
        playingUI->addProperty(guiButtonPunch);
    }
    
    // Player kick button
    {
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
            GLTexture glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        playingUI->addProperty(guiButton);
    }
    
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
            GLTexture glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        playingUI->addProperty(guiButton);
    }
    
    
    // The count down game objects
    countdown3UI = new GameObject("countdown-3");
    countdown2UI = new GameObject("countdown-2");
    countdown1UI = new GameObject("countdown-1");
    countdownFightUI = new GameObject("countdown-fight");
    
    // Countdown 3
    {
        GUIProperty *guiButton = new GUIProperty("countdown-3");
        
        glm::vec2 bounds(300.0f, 300.f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2((float)System::screenWidth*0.5f, (float)System::screenHeight*0.5f));
        
        Texture *texture = Texture::loadFromFile("textures/3-300x300.png", true);
        
        if(texture != NULL)
        {
            GLTexture glTexture = GLTexture::loadFromData(*texture);
            
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
            GLTexture glTexture = GLTexture::loadFromData(*texture);
            
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
            GLTexture glTexture = GLTexture::loadFromData(*texture);
            
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
            GLTexture glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        countdownFightUI->addProperty(guiButton);
    }
    
    // Prepare the tracking and playing state game objects
    mScene->prepare(trackStartUI);
    mScene->prepare(trackConfirmUI);
    mScene->prepare(playingUI);
    
}

void FightSceneLogic::update()
{
    // Update Animation module
    Engine::getInstance().update(CoreModuleType::CM_ANIMATION, false);
    
    // Update Physics module
    Engine::getInstance().update(CoreModuleType::CM_PHYSICS, false);
    
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
        mScene->prepare(playingUI);
        
        // Disable these in scene
        mScene->unPrepare(trackConfirmUI);
        
        state = GameState::PLAYING;
    }
    else if(state == GameState::PROMPT_TRACKER)
    {
        // Enable these in scene
        mScene->prepare(trackStartUI);
        
        // Disable these in scene
        mScene->unPrepare(player);
        mScene->unPrepare(opponent);
        mScene->unPrepare(playingUI);
        mScene->unPrepare(trackConfirmUI);
        
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
    
    // Check to see if we're not tracking and not prompting user to track
    if( !arHandler->getTracker()->isTracking() && state != GameState::PROMPTING_TRACKER)
    {
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

void FightSceneLogic::walkAnalogMove()
{
    // Get the offset
    glm::vec2 offset = playerWalkController->getOffset();
    
    // Call the player walk function with the analog offset
    player->walk(-offset.x, offset.y);
}

void FightSceneLogic::walkAnalogUp()
{
    player->idle();
}

void FightSceneLogic::punchButtonUp()
{
    player->punch();
}

void FightSceneLogic::kickButtonUp()
{
    player->kick();
}

void FightSceneLogic::blockButtonUp()
{
    player->block();
}

void FightSceneLogic::trackStart()
{
    arHandler->startTracking();
    
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
