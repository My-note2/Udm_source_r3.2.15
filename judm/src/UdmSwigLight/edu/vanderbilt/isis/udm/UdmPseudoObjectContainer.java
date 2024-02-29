/**
 * Created on Jan 6, 2005
 */
package edu.vanderbilt.isis.udm;

import java.util.logging.Logger;

import edu.vanderbilt.isis.udm.swig.UdmPseudoObjectS;

/**
 * Equivalent of SWIG generated UdmPseudoObjectS container for UdmPseudoObjects
 * 
 * @author Himanshu Neema
 */
public class UdmPseudoObjectContainer {
    private static final Logger log = Logger.getLogger(UdmPseudoObjectContainer.class.getName());
    private edu.vanderbilt.isis.udm.swig.UdmPseudoObjectS swigContainer;

    /**
     * Initializes a container from the provided array.
     */
    public UdmPseudoObjectContainer(UdmPseudoObject[] items) {
        this(items.length);
        for (int i = 0; i < items.length; i++) {
            setAt(i, items[i]);
        }
    }
    
    /**
     * Initializes a container from the provided array.
     *
     * @throws ClassCastException if an Object in items isn't a UdmPseudoObject
     */
    public UdmPseudoObjectContainer(Object[] items) {
        this(items.length);
        for (int i = 0; i < items.length; i++) {
            setAt(i, (UdmPseudoObject)items[i]);
        }
    }
    
    /**
     * A package private method
     */
    UdmPseudoObjectContainer(UdmPseudoObjectS swigContainer) {
        if (swigContainer == null) {
            throw new IllegalArgumentException("Null parameter");
        }
        this.swigContainer = swigContainer;
    }

    /**
     * A package private method
     */
    UdmPseudoObjectS getInternal() {
        return swigContainer;
    }

    public UdmPseudoObjectContainer(int length) {
        log.finer("new UdmPseudoObjectS(" + length + ");");
        swigContainer = new UdmPseudoObjectS(length);
    }

    public UdmPseudoObjectContainer(UdmPseudoObjectContainer from) {
        log.finer("new UdmPseudoObjectS(fromContainer);");
        swigContainer = new UdmPseudoObjectS(from.getInternal());
    }

    public void clone(UdmPseudoObjectContainer from) {
        log.finer("UdmPseudoObjectS.__assign__(fromContainer);");
        swigContainer = swigContainer.__assign__(from.getInternal());
    }

    public void setOverflow(boolean overflow) {
        log.finer("UdmPseudoObjectS.setOverflow(" + overflow + ");");
        swigContainer.setOverflow(overflow);
    }

    public boolean getOverflow() {
        log.finer("UdmPseudoObjectS.getOverflow();");
        return swigContainer.getOverflow();
    }

    public void setAt(int index, UdmPseudoObject item) {
        if (index < 0 || index > getLength()) {
            throw new IllegalArgumentException("Invalid index: " + index
                    + " for container of size " + getLength());
        }
        edu.vanderbilt.isis.udm.swig.UdmPseudoObject newItem = item.getInternal();
        log.finer("UdmPseudoObjectS.SetAt(" + index + ",newItem);");
        swigContainer.SetAt(index, newItem);
    }

    public int getLength() {
        log.finer("UdmPseudoObjectS.GetLength();");
        return swigContainer.GetLength();
    }

    public UdmPseudoObject getAt(int index) {
        if (index < 0 || index > getLength() - 1) {
            throw new IllegalArgumentException("Invalid index: " + index
                    + " for container of size " + getLength());
        }
        log.finer("UdmPseudoObjectS.get(" + index + ");");
        edu.vanderbilt.isis.udm.swig.UdmPseudoObject item = swigContainer.get(index);
        return new UdmPseudoObject(item);
    }
}