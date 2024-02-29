/*
 * Created on Jun 30, 2005
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.judm.gen.pane;

import java.awt.Cursor;
import java.io.File;

import edu.vanderbilt.isis.judm.gen.log.GenLogger;
import edu.vanderbilt.isis.judm.gen.util.Utils;


/**
 * @author nsomogyi
 *
 */
public class EclipsePanel extends LibPanel {
    /**
     * @param _logger
     */
    public EclipsePanel(GenLogger _logger) {
        super(_logger);
        generateButton.setToolTipText(
            " Generates the domain-specific eclipse project ");
        libProjLabel.setText(" Project name: ");
    }

    protected void generate() {
        if ((libProjField.getText().trim().compareTo("") == 0) ||
                (libMetaField.getText().trim().compareTo("") == 0) ||
                (libOutField.getText().trim().compareTo("") == 0)) {
            Utils.displayError(this, "Error",
                "<html><body><i>UML meta model</i>, <i>Output directory</i>, and <i>Project " +
                "name</i> <br> parameters are required!</body></html>");

            return;
        }

        setCursor(new Cursor(Cursor.WAIT_CURSOR));

        try {
            String[] cmd = new String[8];
            cmd[0] = "cmd.exe";
            cmd[1] = "/C";
            cmd[2] = "%UDM_PATH%\\etc\\java\\eclipseAPI";
            cmd[3] = libProjField.getText();
            cmd[4] = libMetaField.getText();
            cmd[5] = libParamsField.getText();
            cmd[6] = new File(cmd[4]).getParent();
            cmd[7] = libOutField.getText();

            Utils.executeCommand(cmd, logger);
        } catch (Throwable t) {
            t.printStackTrace();
        } finally {
            setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
        }
    }
}
