/*
 * Created on Apr 14, 2005
 */
package edu.vanderbilt.isis.timeseries.test;

import junit.framework.Test;
import junit.framework.TestSuite;

/**
 * @author 
 */
public class AllTests extends TestSuite {
    public static Test suite() { 
        TestSuite suite= new TestSuite();
        suite.addTestSuite(TimeSeriesFileInstanceTest.class);
        suite.addTestSuite(TimeSeriesStringInstanceTest.class);
        return suite;
    }
    
    public static void main(String[] args) {
        junit.textui.TestRunner.run(suite());
    }
}
