//
//  Screen_Main.swift
//  Excalibur
//
//  Created by Christian Baltzer on 23.06.19.
//  Copyright © 2019 Chaos With LEDs. All rights reserved.
//

import UIKit

class Screen_Main: UIViewController {

    @IBOutlet weak var Button: UIButton!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        let temp = Bluetooth_Funktionen.init()
        temp.senden(payload: 0)
    }
    

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
    }
    */

}
