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
    
    NSBundle *b = [NSBundle mainBundle];
    NSString *dir = [b resourcePath];
    NSArray *parts = [NSArray arrayWithObjects:
                      dir, @"assets", (void *)nil];
    NSString *path = [NSString pathWithComponents:parts];
    const char *cpath = [path fileSystemRepresentation];
    std::string assetsPath(cpath);
    
    
    _instance->setAssetsPath(assetsPath+"/");
    
    return self;
}

- (void) dealloc
{
    //delete _instance;
}

- (void) initalise:(unsigned int) screenWidth :(unsigned int) screenHeight
{
    _instance->initalise(screenWidth, screenHeight);
}

- (void) deinitalise
{
    _instance->deinitalise();
}

- (void) update
{
    _instance->update();
}

- (void) draw
{
    _instance->draw();
}

-(void)pitch
{
    _instance->pitch();
}
-(void)yaw
{
    _instance->yaw();
}
-(void)roll
{
    _instance->roll();
}
-(void)track
{
    _instance->track();
}
@end

