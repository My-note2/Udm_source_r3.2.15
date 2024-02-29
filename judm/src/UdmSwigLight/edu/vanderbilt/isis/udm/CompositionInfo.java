/**
 * Created on Jan 6, 2005
 */
package edu.vanderbilt.isis.udm;

import java.util.logging.Logger;

import edu.vanderbilt.isis.udm.swig.cint_string;

/**
 * Contains information about the parent and child relationships (roles)
 * 
 * @author Himanshu Neema
 */
public class CompositionInfo {
    private static final Logger log = Logger.getLogger(CompositionInfo.class.getName());
    private edu.vanderbilt.isis.udm.swig.CompositionInfo swigCompInfo;
    
    /**
     * A package private method
     */
    edu.vanderbilt.isis.udm.swig.CompositionInfo getInternal() {
        return swigCompInfo;
    }

    public CompositionInfo(String pr, String cr) {
        log.finer("new CompositionInfo(\"" + pr + "\",\"" + cr + "\");");
        swigCompInfo = new edu.vanderbilt.isis.udm.swig.CompositionInfo(pr, cr);
    }

    public void setParentRole(String parentRole) {
        cint_string swigParentRole = new cint_string(parentRole);
        log.finer("CompositionInfo.setParentRole(new cint_string(\"" + swigParentRole + "\"));");
        swigCompInfo.setParentRole(swigParentRole);
    }

    public String getParentRole() {
        log.finer("cint_string swigParentRole = CompositionInfo.getParentRole();");
        cint_string swigParentRole = swigCompInfo.getParentRole();
        if (swigParentRole == null) {
            return null;
        }
        log.finer("swigParentRole.buffer();");
        return swigParentRole.buffer();
    }

    public void setChildRole(String childRole) {
        cint_string swigChildRole = new cint_string(childRole);
        log.finer("CompositionInfo.setChildRole(new cint_string(\"" + childRole + "\"));");
        swigCompInfo.setChildRole(swigChildRole);
    }

    public String getChildRole() {
        log.finer("cint_string swigChildRole = CompositionInfo.getChildRole();");
        cint_string swigChildRole = swigCompInfo.getChildRole();
        if (swigChildRole == null) {
            return null;
        }
        log.finer("swigChildRole.buffer();");
        return swigChildRole.buffer();
    }
}
