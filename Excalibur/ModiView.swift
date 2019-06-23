//
//  ModiView.swift
//  ExcaliburApp
//
//  Created by Christian Baltzer on 17.06.19.
//  Copyright © 2019 Christian Baltzer. All rights reserved.
//

import UIKit

class ModiView: UITableViewController {
    
    let Modi = [
        "standart",
        "Bremse",
        "Heiligenschein",
        "Party",
        "Sound"
    ]
    let Detail = [
        "tut nichts/schaltet die Lichte aus",
        "Ein kleines Bremslicht 🔴",
        "Feel like an Angel😇",
        "UTZUTZUTZ 🎊 🎉",
        "See the rythme 🎶🎵"
    ]
    let Data = [
        0,
        1,
        2,
        3,
        4,
        5
    ]
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    
    
    // MARK: - Table view data source
    override func numberOfSections(in tableView: UITableView) -> Int {
        // #warning Incomplete implementation, return the number of sections
        return 1
    }
    
    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        // #warning Incomplete implementation, return the number of rows
        return Modi.count
    }
    
    
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "reuseIdentifier", for: indexPath)
        
        // Configure the cell...
        cell.textLabel?.text = Modi[indexPath.row]
        cell.detailTextLabel?.text = Detail[indexPath.row]
        
        return cell
    }
    
    // MARK: - Cell selected
    func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        let Funktion = Bluetooth_Funktionen.init()
        Funktion.senden(payload: indexPath.row)
    }
    
    
    /*
     // Override to support conditional editing of the table view.
     override func tableView(_ tableView: UITableView, canEditRowAt indexPath: IndexPath) -> Bool {
     // Return false if you do not want the specified item to be editable.
     return true
     }
     */
    
    /*
     // Override to support editing the table view.
     override func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCell.EditingStyle, forRowAt indexPath: IndexPath) {
     if editingStyle == .delete {
     // Delete the row from the data source
     tableView.deleteRows(at: [indexPath], with: .fade)
     } else if editingStyle == .insert {
     // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
     }
     }
     */
    
    /*
     // Override to support rearranging the table view.
     override func tableView(_ tableView: UITableView, moveRowAt fromIndexPath: IndexPath, to: IndexPath) {
     
     }
     */
    
    /*
     // Override to support conditional rearranging of the table view.
     override func tableView(_ tableView: UITableView, canMoveRowAt indexPath: IndexPath) -> Bool {
     // Return false if you do not want the item to be re-orderable.
     return true
     }
     */
    
    /*
     // MARK: - Navigation
     
     // In a storyboard-based application, you will often want to do a little preparation before navigation
     override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
     // Get the new view controller using segue.destination.
     // Pass the selected object to the new view controller.
     }
     */
    
}
