/*
 * Created on Apr 14, 2005
 */
package edu.vanderbilt.isis.genetf.test;

import junit.framework.Test;
import junit.framework.TestSuite;

/**
 * @author 
 */
public class AllTests extends TestSuite {
    public static Test suite() { 
        TestSuite suite= new TestSuite();
        suite.addTestSuite(GeneTFFileInstanceTest.class);
        suite.addTestSuite(GeneTFStringInstanceTest.class);
        return suite;
    }
    
    public static void main(String[] args) {
        junit.textui.TestRunner.run(suite());
    }
}
