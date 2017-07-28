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
    
    @IBAction func leftButton(_ sender: UIButton)
    {
        
    }
    
    @IBAction func rightButton(_ sender: UIButton)
    {
        
    }
    
    @IBAction func punchButton(_ sender: UIButton)
    {
        
    }
    
    override func viewDidLoad()
    {
        super.viewDidLoad()

        self.game?.setUpFightScene()
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
