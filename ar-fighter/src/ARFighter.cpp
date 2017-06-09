#include "ar-fighter/ARFighter.h"

// GameEngine Core Modules
#include <game-engine/Core/Modules/Graphics/Graphics.h>

void ARFighter::initalise()
{
    
    Engine *engine = &Engine::getInstance();
    
    engine->addCoreModule(&Graphics::getInstance());
}

void ARFighter::deinitalise()
{
    
}

void ARFighter::update()
{
    
}

void ARFighter::draw()
{
    
}
