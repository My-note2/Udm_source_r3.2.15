/*
 * Created on Jun 29, 2005
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.judm.gen;

import edu.vanderbilt.isis.judm.gen.pane.JUdmGenFrame;

import java.io.File;


/**
 * @author nsomogyi
 *
 */
public class Main {
    public static String CURRENT_DIR = "c:\\";
    public static String DASHBOARD_HOME = "C:\\Program Files\\Bio-SPICE\\Dashboard-6.0\\Dashboard";

    private static void createAndShowGUI() {
        //Create and set up the window.
        JUdmGenFrame frame = new JUdmGenFrame(" jUdm Helper");

        //Display the window.
        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }

    public static void main(String[] args) {
        //Schedule a job for the event-dispatching thread:
        //creating and showing this application's GUI.
        try {
            File dir1 = new File(".");
            //File dir2 = new File("..");

            CURRENT_DIR = dir1.getCanonicalPath();

            //System.out.println("Current dir : " + dir1.getCanonicalPath());
            //System.out.println("Parent  dir : " + dir2.getCanonicalPath());
        } catch (Exception e) {
            e.printStackTrace();
        }

        //System.out.println("CURRENT_DIR: " + CURRENT_DIR);
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    createAndShowGUI();
                }
            });
    }
}
