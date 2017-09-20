#include "ar-fighter/MainMenuSceneLogic.h"

// Game Engine Core
#include "game-engine/Core/GL/GL.h"

// Game Engine Device
#include <game-engine/Device/System.h>

// Game Engine GUI
#include <game-engine/Modules/GUI/GUI.h>
#include <game-engine/Modules/GUI/GUIProperty.h>
#include <game-engine/Modules/GUI/GUIRectangle.h>

// Game Engine Scene
#include <game-engine/Scene/Scene.h>
#include <game-engine/Scene/SceneManager.h>

#include <game-engine/GameObject.h>

// AR Fighter
#include "ar-fighter/SettingsSceneLogic.h"

void MainMenuSceneLogic::initialise()
{
    
    GameObject *UI = new GameObject("main-menu-ui");
    
    
    // Text
    {
        GUIProperty *guiButton = new GUIProperty("main-text");
        
        guiButton->isTouchable = false;
        glm::vec2 bounds(1132.0f, 137.0f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2((float)System::screenWidth * 0.5f, (float)System::screenHeight * 0.75f));
        guiRectangle->setColourDown(glm::vec4(-0.2f, -0.2f, -0.2f, 0.0f));
        
        Texture *texture = Texture::loadFromFile("textures/ar-fighter.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        UI->addProperty(guiButton);
    }
    
    // Play button
    {
        GUIProperty *guiButton = new GUIProperty("start-button");
        
        guiButton->isTouchable = true;
        std::function<void(void)> func = std::bind(&MainMenuSceneLogic::startButton, this);
        guiButton->setCallbackOnTouchUp(func);
        glm::vec2 bounds(500.0f, 100.f);
        GUIRectangle *guiRectangle = new GUIRectangle(bounds);
        guiRectangle->translateOW(glm::vec2((float)System::screenWidth * 0.5f, (float)System::screenHeight * 0.25f));
        guiRectangle->setColourDown(glm::vec4(-0.2f, -0.2f, -0.2f, 0.0f));
        
        Texture *texture = Texture::loadFromFile("textures/button_start.png", true);
        
        if(texture != NULL)
        {
            GLTexture *glTexture = GLTexture::loadFromData(*texture);
            
            guiRectangle->setMapUp(glTexture);
            guiRectangle->setMapDown(glTexture);
            
            delete texture;
        }
        
        guiButton->addShape(guiRectangle);
        UI->addProperty(guiButton);
    }
    
    mScene->addEntity(UI);
    
    
    // Set state
    state = GameState::INITIALISED;
}

void MainMenuSceneLogic::deinitialise()
{
    
}

void MainMenuSceneLogic::update()
{
    if(state == GameState::START)
    {
        
        // Deinit the scene
        mScene->deinitialise();
        
        // Initialise the next scene and make it active
        SceneManager::getInstance().getScene("settings")->initialise();
        SceneManager::getInstance().makeActiveScene("settings");
    }
}

void MainMenuSceneLogic::draw()
{
    jmpGLClearColour(0.65, 0.65, 0.65, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Update the GUI
    Engine::getInstance().update(CoreModuleType::CM_GUI);
}

void MainMenuSceneLogic::startButton()
{
    if(state == GameState::INITIALISED)
    {
        std::cout << "Start!" << std::endl;
        state = GameState::START;
    }
    
}

