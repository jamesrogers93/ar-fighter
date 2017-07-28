//
//  ARFighterInterface.h
//  ARFighter
//
//  Created by James Rogers on 13/06/2017.
//  Copyright © 2017 James Rogers. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>

@interface ARFighterWrapper : NSObject

@property (readwrite, nonatomic) GLKView *view;

- (void) initialise:(unsigned int) screenWidth :(unsigned int) screenHeight;
- (void) deinitialise;
- (void) update: (double)deltaTime;
- (void) draw;

-(void) setUpSettingsScene;
-(void) setUpFightScene;

@end
