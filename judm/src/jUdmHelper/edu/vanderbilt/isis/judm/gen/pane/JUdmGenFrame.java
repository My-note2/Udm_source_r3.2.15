/*
 * Created on Jun 29, 2005
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.judm.gen.pane;

import edu.vanderbilt.isis.judm.gen.log.GenLogger;

import java.awt.BorderLayout;
import java.awt.HeadlessException;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;

import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JTabbedPane;
import javax.swing.KeyStroke;


/**
 * @author nsomogyi
 *
 */
public class JUdmGenFrame extends JFrame implements ActionListener {
    protected JTabbedPane tabbedPane = null;
    protected LibPanel libPanel = null;
    protected EclipsePanel eclipsePanel = null;
    protected DashboardPanel dashboardPanel = null;
    protected GenLogger logger = null;

    /**
     * @param arg0
     * @throws java.awt.HeadlessException
     */
    public JUdmGenFrame(String arg0) throws HeadlessException {
        super(arg0);
        initFrame();
    }

    protected void initFrame() {
        //Suggest that the L&F (rather than the system)
        //decorate all windows.
        //setDefaultLookAndFeelDecorated(true);
        setDefaultCloseOperation(JUdmGenFrame.EXIT_ON_CLOSE);

        initMenu();
        initTabbedPane();
        getContentPane().add(tabbedPane, BorderLayout.NORTH);
        getContentPane().add(logger, BorderLayout.CENTER);
    }

    protected void initTabbedPane() {
        logger = new GenLogger();
        tabbedPane = new JTabbedPane();
        initLibPane();
        initEclipsePane();
        initDashboardPane();

        tabbedPane.addTab(" Library ", null, libPanel,
            "Generates the domain-specific library");
        tabbedPane.setMnemonicAt(0, KeyEvent.VK_L);

        tabbedPane.addTab(" Eclipse project ", null, eclipsePanel,
            "Generates the domain-specific eclipse project");
        tabbedPane.setMnemonicAt(0, KeyEvent.VK_E);

        tabbedPane.addTab(" Dashboard ", null, dashboardPanel,
            "Generates the data type for BioSpice");
        tabbedPane.setMnemonicAt(0, KeyEvent.VK_D);
    }

    protected void initLibPane() {
        libPanel = new LibPanel(logger);
    }

    protected void initEclipsePane() {
        eclipsePanel = new EclipsePanel(logger);
    }

    protected void initDashboardPane() {
        dashboardPanel = new DashboardPanel(logger);
    }

    protected void initMenu() {
        JMenuBar menuBar = new JMenuBar();

        JMenu fileMenu = new JMenu(" File");
        fileMenu.setMnemonic(KeyEvent.VK_F);
        menuBar.add(fileMenu);

        JMenuItem exitItem = new JMenuItem(" Exit", KeyEvent.VK_X);
        exitItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_X,
                ActionEvent.ALT_MASK));
        exitItem.addActionListener(this);
        exitItem.setActionCommand("exit");
        fileMenu.add(exitItem);

        JMenu helpMenu = new JMenu(" Help");
        helpMenu.setMnemonic(KeyEvent.VK_H);
        menuBar.add(helpMenu);

        JMenuItem aboutItem = new JMenuItem(" About", KeyEvent.VK_A);
        aboutItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F1, 0));
        aboutItem.addActionListener(this);
        aboutItem.setActionCommand("about");
        helpMenu.add(aboutItem);

        setJMenuBar(menuBar);
    }

    /**
     * @param arg0
     */
    public void actionPerformed(ActionEvent arg0) {
        String action = arg0.getActionCommand();

        if (action.compareTo("exit") == 0) {
            System.exit(0);
        } else if (action.compareTo("about") == 0) {
        }
    }
}
