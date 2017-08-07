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
    
    override func viewDidLoad()
    {
        super.viewDidLoad()

        
    }
    
    override func didReceiveMemoryWarning()
    {
        super.didReceiveMemoryWarning()
        
        if self.isViewLoaded && (self.view.window != nil)
        {
            self.view = nil
         }
    }
    
    func setUpGame()
    {
        self.game?.setUpFightScene()
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
