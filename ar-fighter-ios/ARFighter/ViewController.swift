//
//  ViewController.swift
//  ARFighter
//
//  Created by James Rogers on 27/07/2017.
//  Copyright © 2017 James Rogers. All rights reserved.
//

import Foundation

import GLKit
import OpenGLES

class ViewController: GLKViewController
{
    var game: ARFighterWrapper? = nil
    
    deinit
    {
        //if EAGLContext.current() === self.context
        //{
            EAGLContext.setCurrent(nil)
        //}
    }
    
    func convertTouch(_ location: CGPoint) -> CGPoint
    {
        var point: CGPoint = location;
        point.y = self.view.frame.size.height - point.y
        
        point.x = point.x * UIScreen.main.scale;
        point.y = point.y * UIScreen.main.scale;
        return point
    }

    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        if let touch = touches.first
        {
            let location = convertTouch(touch.location(in: self.view))
            //print("Touch Began Location x: \(location.x) y: \(location.y)")
            
            game?.touchDown(Float(location.x), Float(location.y));
        }
        super.touchesBegan(touches, with: event)
    }
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        if let touch = touches.first
        {
            let location = convertTouch(touch.location(in: self.view))
            //print("Touch Moved Location x: \(location.x) y: \(location.y)")
            
            game?.touchMove(Float(location.x), Float(location.y));
        }
        super.touchesMoved(touches, with: event)
    }
    
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        if let touch = touches.first
        {
            let location = convertTouch(touch.location(in: self.view))
            //print("Touch End Location x: \(location.x) y: \(location.y)")
            
            game?.touchUp(Float(location.x), Float(location.y));
        }
        super.touchesEnded(touches, with: event)
    }
    
    override func viewDidLoad()
    {
        super.viewDidLoad()
        
        self.game = ARFighterWrapper()
        
        let screenSize = UIScreen.main.bounds
        let screenWidth = screenSize.width * UIScreen.main.scale
        let screenHeight = screenSize.height * UIScreen.main.scale
        
        self.game?.initialise(UInt32(screenWidth), UInt32(screenHeight))
        
        while GLContextIOS.currentContext() == nil
        {
            // Wait until thread has finished initalising the OpenGL loading
            // and rendering context in game.initialise
        }
        
        let view = self.view as! GLKView
        view.context =  GLContextIOS.currentContext()
        view.drawableDepthFormat = .format24
    }
    
    override func didReceiveMemoryWarning()
    {
        super.didReceiveMemoryWarning()
        
        if self.isViewLoaded && (self.view.window != nil)
        {
            self.view = nil
        }
    }
    
    // Update view in here
    func update()
    {
        //self.timeSinceLastUpdate
        self.game?.update(Double(self.timeSinceLastUpdate))
    }
    
    // Draw OpenGL content here
    override func glkView(_ view: GLKView, drawIn rect: CGRect)
    {
        self.game?.draw()
    }
}
