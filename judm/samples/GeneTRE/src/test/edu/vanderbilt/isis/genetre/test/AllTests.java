/*
 * Created on Apr 14, 2005
 */
package edu.vanderbilt.isis.genetre.test;

import junit.framework.Test;
import junit.framework.TestSuite;

/**
 * @author nsomogyi
 */
public class AllTests extends TestSuite {
    public static Test suite() { 
        TestSuite suite= new TestSuite();
        suite.addTestSuite(GeneTREFileInstanceTest.class);
        suite.addTestSuite(GeneTREStringInstanceTest.class);
        return suite;
    }
    
    public static void main(String[] args) {
        junit.textui.TestRunner.run(suite());
    }
}
