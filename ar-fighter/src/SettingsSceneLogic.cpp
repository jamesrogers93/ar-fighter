#include "ar-fighter/SettingsSceneLogic.h"

#include "game-engine/Scene/Scene.h"
#include "game-engine/Scene/SceneManager.h"

// Game Engine Core
#include "game-engine/Core/GL/GL.h"

// Game Engine Device
#include <game-engine/Device/System.h>

#include <game-engine/Modules/CoreModule.h>

// Game Engine Graphics
#include <game-engine/Modules/Graphics/Graphics.h>
#include <game-engine/Modules/Graphics/AnimatableMeshProperty.h>
#include <game-engine/Modules/Graphics/CameraEntity.h>
#include <game-engine/Modules/Graphics/PointLightProperty.h>
#include <game-engine/Modules/Graphics/DirectionalLightProperty.h>
#include <game-engine/Modules/Graphics/Mesh.h>
#include <game-engine/Modules/Graphics/Material.h>
#include <game-engine/Modules/Graphics/Shader.h>

// Game Engine Animation
#include <game-engine/Modules/Animation/AnimationModule.h>
#include <game-engine/Modules/Animation/AnimatorProperty.h>
#include <game-engine/Modules/Animation/Animation.h>
#include <game-engine/Modules/Animation/AnimationImporter.h>

// Game Engine GUI
#include <game-engine/Modules/GUI/GUI.h>
#include <game-engine/Modules/GUI/GUIProperty.h>
#include <game-engine/Modules/GUI/GUIRectangle.h>

// Game Engine Importers
#include <game-engine/Entity/EntityImporter.h>
#include <game-engine/Entity/PropertyImporter.h>
#include <game-engine/Modules/Graphics/MaterialImporter.h>
#include <game-engine/Modules/Graphics/MeshImporter.h>

#include <game-engine/GameObject.h>

#include "ar-fighter/Game_Objects/Y_Bot.h"
#include "ar-fighter/Game_Objects/X_Bot.h"

#include "ar-fighter/FightSceneLogic.h"

SettingsSceneLogic::SettingsSceneLogic(Scene *scene)
    : SceneLogic(scene),
    state(GameState::UNINITIALISED),
    player(NULL),
    opponent(NULL),
    difficulty("normal"),
    settingsUI(NULL)
{
    
}

void SettingsSceneLogic::update()
{
    // Update Animation module
    Engine::getInstance().update(CoreModuleType::CM_ANIMATION, false);
    
    if(state == GameState::PLAY)
    {
        // Set up the next scene
        Scene *scene = SceneManager::getInstance().getScene("play");
        
        // Pass some settings from settings scene logic to the fight game scene logic
        FightSceneLogic *fightSceneLogic = (FightSceneLogic*)scene->getSceneLogic();
        fightSceneLogic->playerName = player->getName();
        fightSceneLogic->opponentName = opponent->getName();
        
        if(difficulty == "normal")
        {
            fightSceneLogic->difficulty = FightSceneLogic::GameDifficulty::NORMAL;
        }
        else
        {
            fightSceneLogic->difficulty = FightSceneLogic::GameDifficulty::HARD;
        }
        
        mScene->deinitialise();
        
        Graphics *g = &Graphics::getInstance();
        
        scene->initialise();
        SceneManager::getInstance().makeActiveScene("play");
    }
}

void SettingsSceneLogic::draw()
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

void SettingsSceneLogic::initialise()
{
    unsigned short playerMask = 1;
    unsigned short opponentMask = 2;
    
    // YBot
    YBot *yBot = new YBot(playerMask, opponentMask);
    yBot->initialise();
    yBot->translate(-150.0, 0.0, 0.0);
    mScene->addEntity(yBot);
    yBot->idle();
    player = yBot;
    
    // X_Bot
    XBot *xBot = new XBot(opponentMask, playerMask);
    xBot->initialise();
    xBot->translate(150.0, 0.0, 0.0);
    mScene->addEntity(xBot);
    xBot->idle();
    opponent = xBot;
    
    // To house UI elements
    this->settingsUI = new GameObject("settings-ui");
    mScene->addEntity(settingsUI);
    
    // Play button
    {
        GUIProperty *guiButton = new GUIProperty("player-block-control");
        
        guiButton->isTouchable = true;
        std::function<void(void)> func = std::bind(&SettingsSceneLogic::playButton, this);
        guiButton->setCallbackOnTouchUp(func);
        glm::vec2 bounds(200.0f, 100.f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2(((float)System::screenWidth * 0.5f), 100.0f));
        guiRectangle->setColourDown(glm::vec4(-0.2f, -0.2f, -0.2f, 0.0f));
        
        Texture *texture = Texture::loadFromFile("textures/button_play.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        settingsUI->addProperty(guiButton);
    }
    
    // Settings text
    {
        GUIProperty *guiButton = new GUIProperty("settings-text");
        
        glm::vec2 bounds(965.0f, 137.0f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2((float)System::screenWidth*0.5f, (float)System::screenHeight-150.0f));
        
        Texture *texture = Texture::loadFromFile("textures/settings.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        settingsUI->addProperty(guiButton);
    }
    
    // Player text
    {
        GUIProperty *guiButton = new GUIProperty("player-text");
        
        glm::vec2 bounds(404.0f, 70.0f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2((float)System::screenWidth*0.25f, (float)System::screenHeight-350.0f));
        
        Texture *texture = Texture::loadFromFile("textures/player.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        settingsUI->addProperty(guiButton);
    }
    
    // Opponent text
    {
        GUIProperty *guiButton = new GUIProperty("opponent-text");
        
        glm::vec2 bounds(533.0f, 70.0f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2((float)System::screenWidth*0.75f, (float)System::screenHeight-350.0f));
        
        Texture *texture = Texture::loadFromFile("textures/opponent.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        settingsUI->addProperty(guiButton);
    }
    
    //
    // LIGHT
    //
    
    // Add a light to the scene
    GameObject *sun = new GameObject("sun");
    DirectionalLightProperty *light = new DirectionalLightProperty("light", glm::normalize(glm::vec3(0.0, -5.0, 10.0)), glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.6, 0.6, 0.6), glm::vec3(0.8, 0.8, 0.8));
    sun->addProperty(light);
    mScene->addEntity(sun);
    

    //
    // CAMERA
    //
    
    // Also need to add a camera entity to the scene
    CameraEntity *camEntity = new CameraEntity("glCamera", CameraEntity::perspectiveMatrix(System::screenWidth, System::screenHeight, 47.0));
    camEntity->translate(0.0, 100.0, 300.0);
    camEntity->initialise();
    Graphics::getInstance().setActiveCameraEntity("glCamera");
    //arEntity->addChild(camEntity);
    mScene->addEntity(camEntity);
    
    state = GameState::INITIALISED;
}

void SettingsSceneLogic::deinitialise()
{
    state = GameState::UNINITIALISED;
    player = NULL;
    opponent = NULL;
    settingsUI = NULL;
}

void SettingsSceneLogic::playButton()
{
    std::cout << "Play!" << std::endl;
    
    state = GameState::PLAY;
    
    // No need to setup a new scene as we will use the scene in this one.
    
    /**/
}
