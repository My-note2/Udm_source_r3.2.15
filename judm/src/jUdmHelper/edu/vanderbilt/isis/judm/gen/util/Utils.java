/*
 * Created on Jun 29, 2005
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.judm.gen.util;

import edu.vanderbilt.isis.judm.gen.Main;
import edu.vanderbilt.isis.judm.gen.log.GenLogger;
import edu.vanderbilt.isis.judm.gen.log.StreamLogger;

import java.awt.Component;

import java.io.File;
import java.io.IOException;

import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;


/**
 * @author nsomogyi
 *
 */
public class Utils {
    /**
     * Pops up an "Open File" file chooser dialog that accepts only directories and
     * .xml files.
     * @param location Sets the file chooser to point to this directory
     * @param parent The parent component of the dialog, can be null
     * @return
     */
    public static String getUmlMetaFile(String location, Component parent) {
        // Create a file chooser
        final JFileChooser fc = new JFileChooser();
        fc.setCurrentDirectory(new File(Main.CURRENT_DIR));
        fc.setFileFilter(new XML_Filter());

        String currDir = Main.CURRENT_DIR;
        File cd = new File(location);

        if (cd.exists()) {
            currDir = cd.getAbsolutePath();
        }

        fc.setCurrentDirectory(new File(currDir));

        int returnVal = fc.showOpenDialog(parent);

        if (returnVal == JFileChooser.APPROVE_OPTION) {
            File file = fc.getSelectedFile();

            if (file.exists()) {
                return file.getAbsolutePath().trim();
            }
        }

        return null;
    }

    /**
     * Pops up an "Open File" file chooser dialog that accepts only directories.
     * @param location Sets the file chooser to point to this directory
     * @param parent The parent component of the dialog, can be null
     * @return
     */
    public static String getOutDir(String location, Component parent) {
        // Create a file chooser
        final JFileChooser fc = new JFileChooser();
        String currDir = Main.CURRENT_DIR;
        File cd = new File(location);

        if (cd.exists() && cd.isDirectory()) {
            currDir = cd.getAbsolutePath();
        }

        fc.setCurrentDirectory(new File(currDir));
        fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);

        int returnVal = fc.showOpenDialog(parent);

        if (returnVal == JFileChooser.APPROVE_OPTION) {
            File file = fc.getSelectedFile();

            if (file.exists() && file.isDirectory()) {
                return file.getAbsolutePath();
            }
        }

        return null;
    }

    public static void executeCommand(String[] cmd, GenLogger logger) {
        try {
            /*logger.log("Execing ");
            for (int i = 0; i < cmd.length; i++)
                logger.log(cmd[i] + " ");
            logger.log("\n");*/
            logger.log("Execute command... \n");

            Runtime rt = Runtime.getRuntime();
            Process proc = rt.exec(cmd);

            // any error message?
            StreamLogger errorGobbler = new StreamLogger(proc.getErrorStream(),
                    "ERROR", logger);

            // any output?
            StreamLogger outputGobbler = new StreamLogger(proc.getInputStream(),
                    "OUTPUT", logger);

            // kick them off
            errorGobbler.start();
            outputGobbler.start();

            // any error???
            int exitVal = proc.waitFor();
            logger.log("Exit value: " + exitVal + "\n");
        } catch (IOException ioEx) {
            logger.log("Error: " + ioEx.getMessage());
            ioEx.printStackTrace();
        } catch (InterruptedException ioEx) {
            logger.log("Error: " + ioEx.getMessage());
            ioEx.printStackTrace();
        }
    }

    public static void displayError(Component parent, String title, String msg) {
        JOptionPane.showMessageDialog(parent, msg, title,
            JOptionPane.ERROR_MESSAGE);
    }

    public static String displayEditor(Component parent, String title,
        String msg) {
        JTextArea editArea = new JTextArea(msg, 5, 30);
        editArea.setEditable(true);
        editArea.setLineWrap(true);

        JScrollPane scroll = new JScrollPane(editArea,
                JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
                JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

        final JOptionPane optionPane = new JOptionPane(scroll,
                JOptionPane.PLAIN_MESSAGE, JOptionPane.OK_CANCEL_OPTION);
        JDialog dialog = optionPane.createDialog(parent, title);
        dialog.setResizable(true);
        dialog.show();

        Object iValue = optionPane.getValue();

        if (iValue == null) {
            return null;
        }

        int value = ((Integer) optionPane.getValue()).intValue();

        if (value == JOptionPane.OK_OPTION) {
            return editArea.getText();
        }

        return null;
    }
}
