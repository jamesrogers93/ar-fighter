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

#include "game-engine/GameObject.h"

#include "ar-fighter/Game_Objects/Character.h"
#include "ar-fighter/Game_Objects/Y_Bot.h"

// Game Engine Util
#include "game-engine/Util/TextureUtil.h"

void onAnalogDown()
{
    std::cout << "Analog down" << std::endl;
}

void onAnalogMove()
{
    std::cout << "Analog move" << std::endl;
}

void onAnalogUp()
{
    std::cout << "Analog up" << std::endl;
}

void FightSceneLogic::initialise()
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
    GameObject *trackingState = new GameObject("track-state");
    GameObject *playingState = new GameObject("play-state");
    
    ///// For trackingState
    
    // Button to start AR tracking
    GUIProperty *aRTrackButton = new GUIProperty("ar-track-button");
    std::function<void(void)> arCallbackFunc = std::bind(&FightSceneLogic::track, this);
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
    trackingState->addProperty(aRTrackButton);
    
    ///// For playingState
    
    // Analog to control player movement
    GUIAnalog *playerAnalogControl = new GUIAnalog("player-walk-control", 150.0f, 100.0f);
    playerAnalogControl->setColourUp(glm::vec4(0.5f, 0.5f, 0.5f, 0.8f), glm::vec4(0.7f, 0.7f, 0.7f, 0.8f));
    playerAnalogControl->setColourDown(glm::vec4(0.5f, 0.5f, 0.5f, 0.8f), glm::vec4(0.7f, 0.7f, 0.7f, 0.8f));
    playerAnalogControl->setDrawOnTouched(0.0, (float)System::screenWidth / 2.0f, (float)System::screenHeight, 0.0);
    std::function<void(void)> walkMoveFunc = std::bind(&FightSceneLogic::walkAnalogMove, this);
    std::function<void(void)> walkUpFunc = std::bind(&FightSceneLogic::walkAnalogUp, this);
    playerAnalogControl->setCallbackOnTouchMove(walkMoveFunc);
    playerAnalogControl->setCallbackOnTouchUp(walkUpFunc);
    playingState->addProperty(playerAnalogControl);
    playerWalkController = playerAnalogControl;
    
    // Prepare the tracking and playing state game objects
    mScene->prepare(trackingState);
    mScene->prepare(playingState);
    
    // Store our own pointer to the tracking and playing state game objects
    // Store reference
    trackingGameObject = trackingState;
    playingGameObject = playingState;
    
}

void FightSceneLogic::walkAnalogMove()
{
    //std::cout << "Analog activated" << std::endl;
    
    // Get the analog gui property
    
    // Get the offset
    glm::vec2 offset = playerWalkController->getOffset();
    std::cout << "Analog x: " << offset.x << " y: " << offset.y << std::endl;
    
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

void FightSceneLogic::track()
{
    arHandler->startTracking();
    
    
    state = GameState::STARTED_TRACKER;
    
    //GUIProperty *button = (GUIProperty*)arTrackButton->getProperty("track-button");
}

void FightSceneLogic::update()
{
    // Update Animation module
    Engine::getInstance().update(CoreModuleType::CM_ANIMATION, false);
    
    if(state == GameState::STARTED_TRACKER)
    {
        mScene->prepare(player);
        mScene->prepare(opponent);
        mScene->prepare(playingGameObject);
        
        mScene->unPrepare(trackingGameObject);
        
        state = GameState::PLAYING;
    }
    else if(state == GameState::PROMPT_TRACKER)
    {
        mScene->unPrepare(player);
        mScene->unPrepare(opponent);
        mScene->unPrepare(playingGameObject);
        
        mScene->prepare(trackingGameObject);
        
        state = GameState::PROMPTING_TRACKER;
    }
    
    if(arHandler->getTracker()->isTracking())
    {
        
    }
    else
    {
        state = GameState::PROMPT_TRACKER;
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
