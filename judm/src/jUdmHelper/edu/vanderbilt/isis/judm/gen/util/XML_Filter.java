/*
 * Created on Jun 29, 2005
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.judm.gen.util;

import java.io.File;

import javax.swing.filechooser.FileFilter;


/**
 *  XML file filter.
*
*  @author nsomogyi
 */
public class XML_Filter extends FileFilter {
    // Accept all directories and xml and xme files.
    public boolean accept(File f) {
        if (f.isDirectory()) {
            return true;
        }

        String fn = f.getAbsolutePath().toLowerCase().trim();

        if (fn.endsWith(".xml")) {
            return true;
        }

        return false;
    }

    //The description of this filter
    public String getDescription() {
        return "UML meta model files (*.xml)";
    }
}
