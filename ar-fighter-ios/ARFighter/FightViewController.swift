//
//  FightViewController.swift
//  ARFighter
//
//  Created by James Rogers on 13/06/2017.
//  Copyright © 2017 James Rogers. All rights reserved.
//

import GLKit
import OpenGLES

class FightViewController: GLKViewController
{
    var context: EAGLContext? = nil
    var game:   ARFighterWrapper? = nil
    
    deinit
    {
        self.tearDownGL()
        
        if EAGLContext.current() === self.context
        {
            EAGLContext.setCurrent(nil)
        }
        
    }
    
    override func viewDidLoad()
    {
        super.viewDidLoad()
        
        if self.context == nil
        {
            self.context = EAGLContext(api: .openGLES3)
        }
        
        if !(self.context != nil)
        {
            print("Failed to create ES context")
        }
        
        let view = self.view as! GLKView
        view.context = self.context!
        view.drawableDepthFormat = .format24

        
        self.setupGL()
        
        self.game = ARFighterWrapper()
        
        let screenSize = UIScreen.main.bounds
        self.game?.initalise(UInt32(screenSize.width), UInt32(screenSize.height))
    }
    
    override func didReceiveMemoryWarning()
    {
        super.didReceiveMemoryWarning()
        
        if self.isViewLoaded && (self.view.window != nil)
        {
            self.view = nil
         
            self.tearDownGL()
         
            if EAGLContext.current() === self.context
            {
                EAGLContext.setCurrent(nil)
            }
            self.context = nil
         
         }
    }

    
    func setupGL()
    {
        // Set current GL context
        EAGLContext.setCurrent(self.context)
    }
    
    func tearDownGL()
    {
        EAGLContext.setCurrent(self.context)
    }
    
    // MARK: - GLKView and GLKViewController delegate methods
    
    
    // Update view in here
    func update()
    {
        self.game?.update()
    }
    
    // Draw OpenGL content here
    override func glkView(_ view: GLKView, drawIn rect: CGRect)
    {
        self.game?.draw()
    }
}
