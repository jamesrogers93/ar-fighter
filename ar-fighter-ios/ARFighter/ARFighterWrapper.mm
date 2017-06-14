//
//  ARFighterInterface.m
//  ARFighter
//
//  Created by James Rogers on 13/06/2017.
//  Copyright © 2017 James Rogers. All rights reserved.
//

#import "ARFighterWrapper.h"
#import <ar-fighter/ARFighter.h>

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

- (void) initalise
{
    _instance->initalise();
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

@end

