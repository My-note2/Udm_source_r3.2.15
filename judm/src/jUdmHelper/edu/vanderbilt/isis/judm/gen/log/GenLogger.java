/*
 * Created on Jun 29, 2005
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.judm.gen.log;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.LayoutManager;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;


/**
 * @author nsomogyi
 *
 */
public class GenLogger extends JPanel implements ActionListener {
    protected JTextArea logger = null;
    protected JButton clearButton = null;

    /**
     *
     */
    public GenLogger() {
        super(new BorderLayout(5, 5));
        initLogger();
    }

    /**
     * @param arg0
     */
    public GenLogger(boolean arg0) {
        super(arg0);
        initLogger();
    }

    /**
     * @param arg0
     */
    public GenLogger(LayoutManager arg0) {
        super(arg0);
        initLogger();
    }

    /**
     * @param arg0
     * @param arg1
     */
    public GenLogger(LayoutManager arg0, boolean arg1) {
        super(arg0, arg1);
        initLogger();
    }

    protected void initLogger() {
        logger = new JTextArea(5, 20);
        logger.setEditable(false);
        logger.setLineWrap(true);
        logger.setWrapStyleWord(true);

        JScrollPane scroll = new JScrollPane(logger,
                JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
                JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

        scroll.setBorder(BorderFactory.createTitledBorder(
                BorderFactory.createLoweredBevelBorder(), " Logger "));

        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT, 15, 5));
        clearButton = new JButton(" Clear ");
        clearButton.setMnemonic(KeyEvent.VK_C);
        clearButton.setToolTipText(" Clears the Logger area ");
        clearButton.addActionListener(this);
        buttonPanel.add(clearButton);

        add(scroll, BorderLayout.CENTER);
        add(buttonPanel, BorderLayout.SOUTH);
    }

    /* (non-Javadoc)
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent arg0) {
        clear();
    }

    public void log(String text) {
        logger.append(text);
    }

    protected void clear() {
        logger.setText("");
        logger.setCaretPosition(0);
    }
}
