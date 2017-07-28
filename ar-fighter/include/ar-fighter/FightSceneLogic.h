#ifndef _FIGHTSCENELOGIC_H
#define _FIGHTSCENELOGIC_H

#include "game-engine/Scene/SceneLogic.h"

#include "game-engine/Core/Engine/Engine.h"

class FightSceneLogic : public SceneLogic
{
public:
    FightSceneLogic(Scene *scene) : SceneLogic(scene)
    {
        
    }
    
    void initialise()
    {}
    
    void update()
    {
        // Update Animation module
        Engine::getInstance().update(CoreModuleType::CM_ANIMATION, false);
    }
    
    void draw()
    {
        jmpGLClearColour(0.65, 0.65, 0.65, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Engine *engine = &Engine::getInstance();
        
        // Update AR first
        engine->update(CoreModuleType::CM_AR);
        
        // Update Graphics second
        engine->update(CoreModuleType::CM_GRAPHICS);
    }
    
    void leftButtonPressed()
    {}
    
    void rightButtonPressed()
    {}
    
    void punchButtonPressed()
    {}
};

#endif /* _FIGHTSCENELOGIC_H */
