/*
 * Created on Jun 30, 2005
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.judm.gen.pane;

import edu.vanderbilt.isis.judm.gen.Main;
import edu.vanderbilt.isis.judm.gen.log.GenLogger;
import edu.vanderbilt.isis.judm.gen.util.Utils;

import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.event.ActionEvent;

import java.io.File;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JTextField;


/**
 * @author nsomogyi
 *
 */
public class DashboardPanel extends LibPanel {
    protected JTextField dashboardHomeField = null;
    protected JButton dashboardHomeButton = null;
    protected JTextField versionField = null;
    protected JCheckBox dashAnalyzer = null; 

    /**
     * @param _logger
     */
    public DashboardPanel(GenLogger _logger) {
        super(_logger);
        initDashboardPanel();
    }

    private void initDashboardPanel() {
        generateButton.setToolTipText(
            " Generates the domain-specific data type for Dashboard ");
        libProjLabel.setText(" Type name: ");
        libOutField.setText(Main.CURRENT_DIR);

        dashboardHomeField = new JTextField(35);

        dashboardHomeButton = new JButton("...");
        dashboardHomeButton.setPreferredSize(new Dimension(25, 20));
        dashboardHomeButton.setMaximumSize(new Dimension(25, 20));
        dashboardHomeButton.setMinimumSize(new Dimension(25, 20));
        dashboardHomeButton.setToolTipText("Browse Dashboard home");
        dashboardHomeButton.addActionListener(this);
        dashboardHomeButton.setActionCommand("home");

        versionField = new JTextField(5);
        versionField.setText("1.0.0,0");
        
        // disable analyzer generation option
        dashAnalyzer = new JCheckBox(" Create example analyzer ", false);
        dashAnalyzer.setEnabled(false);
        libParamsField.setEnabled(false);
        libParamsButton.setEnabled(false);
        libMetaButton.setEnabled(false);
        libMetaField.setEnabled(false);

        gbc.gridwidth = 1;
        gbc.insets = new Insets(7, 10, 2, 5);
        gbc.gridx = 0;
        gbc.gridy = 0;
        add(new JLabel(" Dashboard home: "), gbc);
        gbc.gridy = 4;
        add(new JLabel(" Version: "), gbc);

        gbc.insets = new Insets(7, 5, 2, 5);
        gbc.weightx = 1.0;
        gbc.gridx = 1;
        gbc.gridy = 0;
        gbc.gridwidth = 1;
        add(dashboardHomeField, gbc);
        gbc.gridy = 4;
        add(versionField, gbc);

        gbc.insets = new Insets(7, 5, 2, 10);
        gbc.weightx = 0;
        gbc.gridx = 2;
        gbc.gridy = 0;
        add(dashboardHomeButton, gbc);

        gbc.insets = new Insets(2, 5, 2, 0);
        gbc.gridx = 1;
        gbc.gridy = 6;
        add(dashAnalyzer, gbc);

        if (new File(Main.DASHBOARD_HOME).exists()) {
            dashboardHomeField.setText(Main.DASHBOARD_HOME);
            dashboardHomeField.setCaretPosition(0);
        }
    }

    /* (non-Javadoc)
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent arg0) {
        String action = arg0.getActionCommand();

        if (action.compareTo("home") == 0) {
            home();
        } else {
            super.actionPerformed(arg0);
        }
    }

    private void home() {
        String res = Utils.getOutDir(dashboardHomeField.getText(),
                DashboardPanel.this);

        if (res != null) {
            dashboardHomeField.setText(res);
        }
    }

    protected void generate() {
        if ((dashboardHomeField.getText().trim().compareTo("") == 0) ||
                (libProjField.getText().trim().compareTo("") == 0) ||
                (libOutField.getText().trim().compareTo("") == 0)) {
            Utils.displayError(this, "Error",
                "<html><body><i>Dashboard home</i>, <i>Type name</i>, and <i>Output directory</i>" +
                "<br> parameters are required!</body></html>");

            return;
        }

        setCursor(new Cursor(Cursor.WAIT_CURSOR));

        try {
            String[] cmd = new String[8];
            cmd[0] = "cmd.exe";
            cmd[1] = "/C";
            cmd[2] = "%UDM_PATH%\\etc\\java\\biospiceAPI";
            cmd[3] = libProjField.getText();
            cmd[4] = libProjField.getText().toLowerCase();
            cmd[5] = versionField.getText();
            cmd[6] = dashboardHomeField.getText();
            cmd[7] = libOutField.getText();

            Utils.executeCommand(cmd, logger);
        } catch (Throwable t) {
            t.printStackTrace();
        } finally {
            setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
        }
    }
}
