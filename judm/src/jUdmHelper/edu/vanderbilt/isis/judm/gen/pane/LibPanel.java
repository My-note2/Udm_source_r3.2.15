/*
 * Created on Jun 29, 2005
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.judm.gen.pane;

import edu.vanderbilt.isis.judm.gen.log.GenLogger;
import edu.vanderbilt.isis.judm.gen.util.Utils;

import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;

import java.io.File;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;


/**
 * @author nsomogyi
 *
 */
public class LibPanel extends JPanel implements ActionListener {
    protected JTextField libMetaField = null;
    protected JButton libMetaButton = null;
    protected JTextField libOutField = null;
    protected JButton libOutButton = null;
    protected JLabel libProjLabel = null;
    protected JTextField libProjField = null;
    protected JTextField libParamsField = null;
    protected JButton libParamsButton = null;
    protected JButton generateButton = null;
    protected GenLogger logger = null;
    protected GridBagConstraints gbc = null;

    /**
     *
     */
    public LibPanel(GenLogger _logger) {
        super();
        logger = _logger;
        initLibPanel();
    }

    protected void initLibPanel() {
        setLayout(new GridBagLayout());

        libMetaField = new JTextField(25);

        libMetaButton = new JButton("...");
        libMetaButton.setPreferredSize(new Dimension(25, 20));
        libMetaButton.setMaximumSize(new Dimension(25, 20));
        libMetaButton.setMinimumSize(new Dimension(25, 20));
        libMetaButton.setToolTipText("Browse UML meta location");
        libMetaButton.addActionListener(this);
        libMetaButton.setActionCommand("meta");

        libOutField = new JTextField();

        libOutButton = new JButton("...");
        libOutButton.setPreferredSize(new Dimension(25, 20));
        libOutButton.setMaximumSize(new Dimension(25, 20));
        libOutButton.setMinimumSize(new Dimension(25, 20));
        libOutButton.setToolTipText("Browse output directory");
        libOutButton.addActionListener(this);
        libOutButton.setActionCommand("out");

        libProjField = new JTextField();

        libParamsField = new JTextField();

        libParamsButton = new JButton("...");
        libParamsButton.setPreferredSize(new Dimension(25, 20));
        libParamsButton.setMaximumSize(new Dimension(25, 20));
        libParamsButton.setMinimumSize(new Dimension(25, 20));
        libParamsButton.setToolTipText("Edit parameters");
        libParamsButton.addActionListener(this);
        libParamsButton.setActionCommand("edit");

        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT, 10, 5));
        generateButton = new JButton(" Generate ");
        generateButton.setMnemonic(KeyEvent.VK_G);
        generateButton.setToolTipText(" Generates the domain-specific library ");
        generateButton.addActionListener(this);
        generateButton.setActionCommand("gen");
        buttonPanel.add(generateButton);

        gbc = new GridBagConstraints();
        gbc.fill = GridBagConstraints.BOTH;

        gbc.insets = new Insets(2, 10, 2, 5);
        gbc.gridx = 0;
        gbc.gridy = 1;
        add(new JLabel(" UML meta model: "), gbc);
        gbc.gridy = 2;
        add(new JLabel(" Output directory: "), gbc);
        gbc.gridy = 3;
        add(libProjLabel = new JLabel(" Library name: "), gbc);
        gbc.gridy = 5;
        add(new JLabel(" UDM parameters: "), gbc);

        gbc.insets = new Insets(2, 5, 2, 5);
        gbc.weightx = 1.0;
        gbc.gridx = 1;
        gbc.gridy = 1;
        add(libMetaField, gbc);
        gbc.gridy = 2;
        add(libOutField, gbc);
        gbc.gridy = 3;
        add(libProjField, gbc);
        gbc.gridy = 5;
        add(libParamsField, gbc);

        gbc.insets = new Insets(2, 5, 2, 10);
        gbc.weightx = 0.0;
        gbc.gridx = 2;
        gbc.gridy = 1;
        add(libMetaButton, gbc);
        gbc.gridy = 2;
        add(libOutButton, gbc);
        gbc.gridy = 5;
        add(libParamsButton, gbc);
        gbc.insets = new Insets(2, 5, 2, 0);
        gbc.gridx = 1;
        gbc.gridy = 7;
        gbc.gridwidth = 2;
        add(buttonPanel, gbc);
    }

    /* (non-Javadoc)
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent arg0) {
        String action = arg0.getActionCommand();

        if (action.compareTo("gen") == 0) {
            generate();
        } else if (action.compareTo("meta") == 0) {
            metaDir();
        } else if (action.compareTo("out") == 0) {
            outDir();
        } else if (action.compareTo("edit") == 0) {
            edit();
        }
    }

    protected void generate() {
        if ((libProjField.getText().trim().compareTo("") == 0) ||
                (libMetaField.getText().trim().compareTo("") == 0) ||
                (libOutField.getText().trim().compareTo("") == 0)) {
            Utils.displayError(this, "Error",
                "<html><body><i>UML meta model</i>, <i>Output directory</i>, and <i>Library " +
                "name</i> <br> parameters are required!</body></html>");

            return;
        }

        setCursor(new Cursor(Cursor.WAIT_CURSOR));

        try {
            String[] cmd = new String[8];
            cmd[0] = "cmd.exe";
            cmd[1] = "/C";
            cmd[2] = "%UDM_PATH%\\etc\\java\\judmAPI";
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

    private void metaDir() {
        String res = Utils.getUmlMetaFile(libMetaField.getText(), LibPanel.this);

        if (res != null) {
            libMetaField.setText(res);
            libMetaField.setCaretPosition(0);

            if (libOutField.getText().trim().compareTo("") == 0) {
                try {
                    File f = new File(res);
                    String libName = f.getName();
                    
                    //  trailing .xml
                    libName = libName.substring(0, libName.length() - 4);

                    // trailing _udm
                    if (libName.toLowerCase().endsWith("_udm")) {
                        libName = libName.substring(0, libName.length() - 4);
                    }

                    //System.out.println("meta: " + libName);
                    libOutField.setText(f.getParent());
                    libProjField.setText(libName);
                } catch (Throwable t) {
                }
            }
        }
    }

    private void outDir() {
        String res = Utils.getOutDir(libOutField.getText(), LibPanel.this);

        if (res != null) {
            libOutField.setText(res);
            libOutField.setCaretPosition(0);
        }
    }

    private void edit() {
        String res = Utils.displayEditor(this, "Edit UDM parameters",
                libParamsField.getText());

        if (res != null) {
            libParamsField.setText(res);
        }
    }
}
