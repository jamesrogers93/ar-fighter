//
//  ARFighterInterface.m
//  ARFighter
//
//  Created by James Rogers on 13/06/2017.
//  Copyright © 2017 James Rogers. All rights reserved.
//

#import "ARFighter-iOS.h"
#import <ar-fighter/ARFighter.h>


#import <string>

@interface ARFighterWrapper ()

@property (nonatomic, readonly) ARFighter* instance;

@end

@implementation ARFighterWrapper

- (id) init
{
    _instance = &ARFighter::getInstance();
    
    return self;
}

- (void) dealloc
{
    //delete _instance;
}

- (void) initialise:(unsigned int) screenWidth :(unsigned int) screenHeight
{
    NSBundle *b = [NSBundle mainBundle];
    NSString *dir = [b resourcePath];
    NSArray *parts = [NSArray arrayWithObjects:
                      dir, @"assets", (void *)nil];
    NSString *path = [NSString pathWithComponents:parts];
    const char *cpath = [path fileSystemRepresentation];
    std::string assetsPath(cpath);
    
    _instance->initialise(screenWidth, screenHeight, assetsPath+"/");
}

- (void) deinitialise
{
    _instance->deinitialise();
}

#import "game-engine/Util/TimeUtil.h"

- (void) update: (double)deltaTime
{
    timeSinceLastUpdate = deltaTime;
    _instance->update();
}

- (void) draw
{
    _instance->draw();
}

- (void) touchDown:(float) x :(float) y
{
    _instance->touchDown(x, y);
}

- (void) touchMove:(float) x :(float) y
{
    _instance->touchMove(x, y);
}

- (void) touchUp:(float) x :(float) y
{
    _instance->touchUp(x, y);
}

-(void) setUpSettingsScene
{
    _instance->setUpSettingsScene();
}

-(void) setUpFightScene
{
    _instance->setUpFightScene();
}

@end

