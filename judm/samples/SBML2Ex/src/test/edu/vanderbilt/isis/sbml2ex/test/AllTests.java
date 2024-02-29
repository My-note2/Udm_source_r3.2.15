/**
 * Created on Feb 14, 2005
 */
package edu.vanderbilt.isis.sbml2ex.test;

import junit.framework.Test;
import junit.framework.TestSuite;

/**
 * @author Himanshu Neema
 */
public class AllTests extends TestSuite {
    public static Test suite() { 
        TestSuite suite= new TestSuite();
        suite.addTestSuite(ReadFileTest.class);
        suite.addTestSuite(ReadStringTest.class);
        return suite;
    }
    
    public static void main(String[] args) {
        junit.textui.TestRunner.run(suite());
    }
}
