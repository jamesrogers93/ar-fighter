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
    var game: ARFighterWrapper? = nil
    
    deinit
    {
        
    }
    
    @IBAction func pitchButton(_ sender: UIButton)
    {
        self.game?.pitch()
    }
    
    @IBAction func yawButton(_ sender: UIButton)
    {
         self.game?.yaw()
    }
    
    @IBAction func rollButton(_ sender: UIButton)
    {
         self.game?.roll()
    }
    
    @IBAction func trackButton(_ sender: UIButton)
    {
        self.game?.track()
    }
    
    override func viewDidLoad()
    {
        super.viewDidLoad()
        
        self.game = ARFighterWrapper()
        
        let screenSize = UIScreen.main.bounds
        let screenWidth = screenSize.width * UIScreen.main.scale
        let screenHeight = screenSize.height * UIScreen.main.scale
        
        self.game?.initalise(UInt32(screenWidth), UInt32(screenHeight))
        
        let view = self.view as! GLKView
        view.context = GLContextIOS.currentContext()
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
        self.game?.update()
    }
    
    // Draw OpenGL content here
    override func glkView(_ view: GLKView, drawIn rect: CGRect)
    {
        glClearColor(0.65, 0.65, 0.65, 1.0)
        glClear(GLenum(GL_COLOR_BUFFER_BIT) | GLenum(GL_DEPTH_BUFFER_BIT))
        
        self.game?.draw()
    }
}
