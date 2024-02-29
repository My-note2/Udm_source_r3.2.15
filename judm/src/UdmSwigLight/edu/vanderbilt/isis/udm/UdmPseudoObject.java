/**
 * Created on Jan 6, 2005
 */
package edu.vanderbilt.isis.udm;

import java.util.logging.Logger;

import edu.vanderbilt.isis.udm.swig.UdmBool;
import edu.vanderbilt.isis.udm.swig.UdmDouble;
import edu.vanderbilt.isis.udm.swig.UdmLong;
import edu.vanderbilt.isis.udm.swig.UdmPseudoObjectS;
import edu.vanderbilt.isis.udm.swig.VectorOfBools;
import edu.vanderbilt.isis.udm.swig.VectorOfDoubles;
import edu.vanderbilt.isis.udm.swig.VectorOfLongs;
import edu.vanderbilt.isis.udm.swig.VectorOfStrings;
import edu.vanderbilt.isis.udm.swig.cint_string;

/**
 * This wrapper for the UdmPseudoObject
 * 
 * @author Himanshu Neema
 */
public class UdmPseudoObject implements Comparable {
    private static final Logger log = Logger.getLogger(UdmPseudoObject.class.getName());
    private edu.vanderbilt.isis.udm.swig.UdmPseudoObject swigUPO;
    private static final int UNSPECIFIED_MODE = -1;
    
    protected Diagram metaDiagram;

    protected UdmPseudoObject() {
        log.finer("new UdmPseudoObject();");
        swigUPO = new edu.vanderbilt.isis.udm.swig.UdmPseudoObject();
    }

    /**
     * Enables initialization of meta class info (see initClassMetaInfo() method) before the
     * creation of the object
     */
    protected void setUPO(UdmPseudoObject upo) {
        swigUPO = upo.getInternal();
    }

    /**
     * @see java.lang.Object#equals(java.lang.Object)
     */
    public boolean equals(Object arg0) {
        if(!(arg0 instanceof UdmPseudoObject)) {
            return false;
        }
        return swigUPO.__equals__(((UdmPseudoObject) arg0).getInternal());
    }

    /**
     * @see java.lang.Comparable#compareTo(java.lang.Object)
     */
    public int compareTo(Object to) {
        if (!(to instanceof UdmPseudoObject)) {
            throw new ClassCastException("Argument of compareTo should be an instance of UdmPseudoObject");
        }

        long dn_id = getDataNetworkId();
        long to_dn_id = ((UdmPseudoObject) to).getDataNetworkId();

        if (dn_id == to_dn_id) {
            long diff = getObjectId() - ((UdmPseudoObject) to).getObjectId();
            if (diff < 0)
                return -1;
            else if (diff > 0)
                return 1;
            else
                return 0;
        }

        long diff = dn_id - to_dn_id;
        if (diff < 0)
            return -1;
        else if (diff > 0)
            return 1;
        else
            return 0;
    }

    /**
     * Overrides hashCode() to return Long(getObjectId()).hashCode().
     * Because the data network id is not used to construct the hash code,
     * objects from different data networks may have the same hash code.
     *
     * @see java.lang.Long#hashCode()
     */
    public int hashCode() {
		return new Long(getObjectId()).hashCode();
    }

    protected void setDiagram(Diagram metaDiagram) {
        this.metaDiagram = metaDiagram;
    }

    public Diagram getDiagram() {
        return metaDiagram;
    }

    /**
     * Must be called before actual creation of the object
     */
    protected UdmPseudoObject initClassMetaInfo(Diagram diagram, String type, String ns_path)
            throws UdmException {
        if (diagram == null) {
            throw new UdmException("Meta diagram not set");
        }
//        UdmPseudoObject metaClass = UdmHelper.initClassByName(diagram, type);
        UdmPseudoObject metaClass = metaDiagram.getMetaClass(type, ns_path);
        UdmHelper.checkNotNull(metaClass, "Couldn't init meta-class for '" + ns_path + "::" + type
                + "' by UPO_SetClass()");
        return metaClass;
    }

    public UdmPseudoObject(long dataNetworkId, long objectId) {
        log.finer("new UdmPseudoObject(" + dataNetworkId + "," + objectId + ");");
        swigUPO = new edu.vanderbilt.isis.udm.swig.UdmPseudoObject(dataNetworkId, objectId);
    }

    /**
     * A package private method
     */
    UdmPseudoObject(edu.vanderbilt.isis.udm.swig.UdmPseudoObject swigUPO) {
        if (swigUPO == null) {
            throw new IllegalArgumentException("Null parameter");
        }
        this.swigUPO = swigUPO;
    }

    /**
     * @param upo
     * @param diagram
     */
    public UdmPseudoObject(UdmPseudoObject upo, Diagram diagram) {
        setUPO(upo);
        setDiagram(diagram);
    }

    /**
     * A package private method
     */
    edu.vanderbilt.isis.udm.swig.UdmPseudoObject getInternal() {
        return swigUPO;
    }

    public long getObjectId() {
        log.finer("UdmPseudoObject._ob_id();");
        return swigUPO._ob_id();
    }

    public long getDataNetworkId() {
        log.finer("UdmPseudoObject._dn_id();");
        return swigUPO._dn_id();
    }

    public void clone(UdmPseudoObject from) {
        log.finer("UdmPseudoObject.__assign__(from);");
        swigUPO = swigUPO.__assign__(from.getInternal());
    }

    public String getType() throws UdmException {
        log.finer("cint_string swigType = new cint_string(\"\");");
        cint_string swigType = new cint_string("");
        log.finer("UdmPseudoObject.type(swigType);");
        boolean success = swigUPO.type(swigType);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::type() failed");
        log.finer("swigType.buffer();");
        return swigType.buffer();
    }

    public String getStereoType() throws UdmException {
        log.finer("cint_string swigStereoType = new cint_string(\"\");");
        cint_string swigStereoType = new cint_string("");
        log.finer("UdmPseudoObject.stereotype(swigStereoType);");
        boolean success = swigUPO.stereotype(swigStereoType);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::stereotype() failed");
        log.finer("swigStereoType.buffer();");
        return swigStereoType.buffer();
    }

    public long getUniqueId() throws UdmException {
        log.finer("UdmLong swigId = new UdmLong(0);");
        UdmLong swigId = new UdmLong(0);
        log.finer("UdmPseudoObject.uniqueId(swigId);");
        boolean success = swigUPO.uniqueId(swigId);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::uniqueId() failed");
        log.finer("swigId.getLongVal();");
        return swigId.getLongVal();
    }

    public UdmPseudoObject getParent(String parentRole) throws UdmException {
        log.finer("UdmPseudoObject swigParent = new UdmPseudoObject();");
        edu.vanderbilt.isis.udm.swig.UdmPseudoObject swigParent = new edu.vanderbilt.isis.udm.swig.UdmPseudoObject();
        log.finer("UdmPseudoObject.getParent(\"" + parentRole + "\",swigParent);");
        boolean success = swigUPO.getParent(parentRole, swigParent);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::getParent() failed");
        return new UdmPseudoObject(swigParent);
    }

    public void setParent(UdmPseudoObject parent, String parentRole) throws UdmException {
        log.finer("UdmPseudoObject.setParent(parent,\"" + parentRole + "\");");
        boolean success = swigUPO.setParent(parent.getInternal(), parentRole);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::setParent() failed");
    }

    public void detach() throws UdmException {
        log.finer("UdmPseudoObject.detach();");
        boolean success = swigUPO.detach();
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::detach() failed");
    }

    public long getChildrenCount(String childRole, String kind, String kind_ns_path) throws UdmException {
        log.finer("UdmLong swigVal = new UdmLong(1);");
        UdmLong swigVal = new UdmLong(1);
        log.finer("UdmPseudoObject.getChildrenCount(\"" + childRole + "\",\"" + kind
                + "\",\"" + kind_ns_path
                + "\",swigVal);");
        boolean success = swigUPO.getChildrenCount(childRole, kind, kind_ns_path, swigVal);
        UdmHelper.checkError(success,
                "Call to UDM method UdmPseudoObject::getChildrenCount() failed");
        log.finer("swigVal.getLongVal();");
        return swigVal.getLongVal();
    }

    public UdmPseudoObjectContainer getChildren(String childRole, String kind, String kind_ns_path) throws UdmException {
        long childCount = getChildrenCount(childRole, kind, kind_ns_path);
        log.finer("UdmPseudoObjectS swigContainer = new UdmPseudoObjectS(" + childCount + ");");
        UdmPseudoObjectS swigContainer = new UdmPseudoObjectS((int) childCount);
        log.finer("UdmPseudoObject.getChildren(\"" + childRole + "\",\"" + kind
                + "\",\"" + kind_ns_path
                + "\",swigContainer);");
        boolean success = swigUPO.getChildren(childRole, kind, kind_ns_path, swigContainer);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::getChildren() failed");
        return new UdmPseudoObjectContainer(swigContainer);
    }

    // Gergely: Example for how the type casting can be avoided on the client side by
    // assembling the array of the proper type here.
    //    public UdmPseudoObject[] getChildObjects(String metaName, Class childClass) throws
    // UdmException {
    //        if (!UdmPseudoObject.class.isAssignableFrom(childClass)) {
    //            throw new UdmException("" + childClass + " is not a subclass of "
    //                    + UdmPseudoObject.class.getName(), true);
    //        }
    //        
    //        UdmPseudoObjectContainer children = getChildObjects(metaName);
    //        
    //        
    //        int childCount = getChildrenCount(childRole, kind);
    //        UdmPseudoObjectS swigContainer = new UdmPseudoObjectS(childCount);
    //        boolean success = swigUPO.getChildren(childRole, kind, swigContainer);
    //        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::getChildren() failed");
    //        return new UdmPseudoObjectContainer(swigContainer);
    //    }

    public void setChildren(String childRole, UdmPseudoObjectContainer container)
            throws UdmException {
        log.finer("UdmPseudoObject.setChildren(\"" + childRole + "\",container);");
        boolean success = swigUPO.setChildren(childRole, container.getInternal());
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::setChildren() failed");
    }

    public long getAssociationCount(String associationRole) throws UdmException {
        log.finer("UdmLong swigVal = new UdmLong(1);");
        UdmLong swigVal = new UdmLong(1);
        log.finer("UdmPseudoObject..getAssociationCount(\"" + associationRole + "\",swigVal);");
        boolean success = swigUPO.getAssociationCount(associationRole, swigVal);
        UdmHelper.checkError(success,
                "Call to UDM method UdmPseudoObject::getAssociationCount() failed");
        log.finer("swigVal.getLongVal();");
        return swigVal.getLongVal();
    }

    public UdmPseudoObjectContainer getAssociation(String associationRole) throws UdmException {
        long associationCount = getAssociationCount(associationRole);
        log.finer("UdmPseudoObjectS swigContainer = new UdmPseudoObjectS(" + associationCount
                + ");");
        UdmPseudoObjectS swigContainer = new UdmPseudoObjectS((int) associationCount);
        log.finer("UdmPseudoObject.getAssociation(\"" + associationRole + "\",swigContainer);");
        boolean success = swigUPO.getAssociation(associationRole, swigContainer);
        UdmHelper
                .checkError(success, "Call to UDM method UdmPseudoObject::getAssociation() failed");
        return new UdmPseudoObjectContainer(swigContainer);
    }

    public long getAssociationCount(String associationRole, int mode) throws UdmException {
        log.finer("UdmLong swigVal = new UdmLong(1);");
        UdmLong swigVal = new UdmLong(1);
        log.finer("UdmPseudoObject.getAssociationCount(\"" + associationRole + "\",swigVal," + mode
                + ");");
        boolean success = swigUPO.getAssociationCount(associationRole, swigVal, mode);
        UdmHelper.checkError(success,
                "Call to UDM method UdmPseudoObject::getAssociationCount() failed");
        log.finer("swigVal.getLongVal();");
        return swigVal.getLongVal();
    }

    public UdmPseudoObjectContainer getAssociation(String associationRole, int mode)
            throws UdmException {
        long associationCount = getAssociationCount(associationRole, mode);
        log.finer("UdmPseudoObjectS swigContainer = new UdmPseudoObjectS(" + associationCount
                + ");");
        UdmPseudoObjectS swigContainer = new UdmPseudoObjectS((int) associationCount);
        log.finer("UdmPseudoObject.getAssociation(\"" + associationRole + "\",swigContainer,"
                + mode + ");");
        boolean success = swigUPO.getAssociation(associationRole, swigContainer, mode);
        UdmHelper
                .checkError(success, "Call to UDM method UdmPseudoObject::getAssociation() failed");
        return new UdmPseudoObjectContainer(swigContainer);
    }

    public void setAssociation(String associationRole, UdmPseudoObject otherObject, int mode) throws UdmException {
        UdmPseudoObjectContainer container;
        if (otherObject != null) {
            container = new UdmPseudoObjectContainer(1);
            container.setAt(0, otherObject);
        } else {
            container = new UdmPseudoObjectContainer(0);
        }
        if (mode == UNSPECIFIED_MODE) {
        setAssociation(associationRole, container);
        } else {
            setAssociation(associationRole, container, mode);
    }
    }

    /**
     * Convenience method for setting an association where there is only one object at the other
     * end.
     */
    public void setAssociation(String associationRole, UdmPseudoObject otherObject)
            throws UdmException {
        setAssociation(associationRole, otherObject, UNSPECIFIED_MODE);
    }

    public void setAssociation(String associationRole, Object otherObject)
            throws UdmException {
        setAssociation(associationRole, (UdmPseudoObject) otherObject);
    }

    public void setAssociation(String associationRole, Object otherObject, int mode) throws UdmException {
    	setAssociation(associationRole, (UdmPseudoObject)otherObject, mode);
    }
    
    public UdmPseudoObject getSingleAssociatedObject(String associationRole, int mode) throws UdmException {
        UdmPseudoObjectContainer result;
        if (mode == UNSPECIFIED_MODE) {
            result = getAssociation(associationRole);
        } else {
            result = getAssociation(associationRole, mode);
        }
        if (result.getLength() > 1) {
            throw new UdmException(
                    "Don't call this method for associations where > 1 objects can be returned",
                    true);
        }
        if (result.getLength() == 0) {
            return null;
        } else {
            return result.getAt(0);
        }
    }

    /**
     * Helper method for 0..1 or 1..1 associations. Returns the object with the specified
     * association role. Throws an exception if there is more than one such object.
     * 
     * @throws UdmException
     */
    public UdmPseudoObject getSingleAssociatedObject(String associationRole) throws UdmException {
        return getSingleAssociatedObject(associationRole, UNSPECIFIED_MODE);
    }

    public void setAssociation(String associationRole, UdmPseudoObjectContainer container)
            throws UdmException {
        log.finer("UdmPseudoObject.setAssociation(\"" + associationRole + "\",container);");
        boolean success = swigUPO.setAssociation(associationRole, container.getInternal());
        UdmHelper
                .checkError(success, "Call to UDM method UdmPseudoObject::setAssociation() failed");
    }

    public void setAssociation(String associationRole, UdmPseudoObjectContainer container, int mode)
            throws UdmException {
        log.finer("UdmPseudoObject.setAssociation(\"" + associationRole + "\",container," + mode
                + ");");
        boolean success = swigUPO.setAssociation(associationRole, container.getInternal(), mode);
        UdmHelper
                .checkError(success, "Call to UDM method UdmPseudoObject::setAssociation() failed");
    }

    public void setIntVal(String name, long value) throws UdmException {
        log.finer("UdmPseudoObject.SetIntVal(\"" + name + "\"," + value + ");");
        boolean success = swigUPO.SetIntVal(name, value);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::SetIntVal() failed");
    }

    public void setRealVal(String name, double value) throws UdmException {
        log.finer("UdmPseudoObject.SetRealVal(\"" + name + "\"," + value + ");");
        boolean success = swigUPO.SetRealVal(name, value);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::SetRealVal() failed");
    }

    public void setStringVal(String name, String value) throws UdmException {
        log.finer("UdmPseudoObject.SetStrVal(\"" + name + "\"," + value + ");");
        boolean success = swigUPO.SetStrVal(name, value);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::SetStringVal() failed");
    }

    public void setBooleanVal(String name, boolean value) throws UdmException {
        log.finer("UdmPseudoObject.SetBoolVal(\"" + name + "\"," + value + ");");
        boolean success = swigUPO.SetBoolVal(name, value);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::SetBoolVal() failed");
    }

    public long getIntVal(String name) throws UdmException {
        log.finer("UdmLong swigVal = new UdmLong(0);");
        UdmLong swigVal = new UdmLong(0);
        log.finer("UdmPseudoObject.GetIntVal(\"" + name + "\",swigVal);");
        boolean success = swigUPO.GetIntVal(name, swigVal);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::GetIntVal() failed");
        log.finer("swigVal.getLongVal();");
        return swigVal.getLongVal();
    }

    public long[] getIntValues(String name) throws UdmException {
        VectorOfLongs values = new VectorOfLongs();
        boolean success = swigUPO.GetIntValues(name, values);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::GetIntValues() failed");
        log.finer("SWIG: No. of int values = " + values.size());
        long[] longVals = new long[(int) values.size()];
        for(int i=0; i<values.size(); i++) {
            longVals[i] = ((UdmLong) values.get(i)).getLongVal();
            log.finer("SWIG: Int Value " + (i+1) + " = " + values.get(i));
        }
        return longVals;
    }

    public double[] getRealValues(String name) throws UdmException {
        VectorOfDoubles values = new VectorOfDoubles();
        boolean success = swigUPO.GetRealValues(name, values);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::GetRealValues() failed");
        log.finer("SWIG: No. of real values = " + values.size());
        double[] realVals = new double[(int) values.size()];
        for(int i=0; i<values.size(); i++) {
            realVals[i] = values.get(i);
            log.finer("SWIG: Real Value " + (i+1) + " = " + values.get(i));
        }
        return realVals;
    }

    public String[] getStrValues(String name) throws UdmException {
        VectorOfStrings values = new VectorOfStrings();
        boolean success = swigUPO.GetStrValues(name, values);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::GetStrValues() failed");
        log.finer("SWIG: No. of string values = " + values.size());
        String[] strVals = new String[(int) values.size()];
        for(int i=0; i<values.size(); i++) {
            strVals[i] = values.get(i).buffer();
            log.finer("SWIG: String Value " + (i+1) + " = " + values.get(i));
        }
        return strVals;
    }

    public boolean[] getBoolValues(String name) throws UdmException {
        VectorOfBools values = new VectorOfBools();
        boolean success = swigUPO.GetBoolValues(name, values);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::GetBoolValues() failed");
        log.finer("SWIG: No. of boolean values = " + values.size());
        boolean[] boolVals = new boolean[(int) values.size()];
        for(int i=0; i<values.size(); i++) {
            boolVals[i] = values.get(i);
            log.finer("SWIG: Boolean Value " + (i+1) + " = " + values.get(i));
        }
        return boolVals;
    }

    public void setIntValues(String name, long[] longVals) throws UdmException {
        VectorOfLongs values = new VectorOfLongs();
        for(int i=0; i<longVals.length; i++) {
            UdmLong val = new UdmLong(longVals[i]);
            values.add(val);
        }
        boolean success = swigUPO.SetIntValues(name, values);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::SetIntValues() failed");
    }

    public void setRealValues(String name, double[] realVals) throws UdmException {
        VectorOfDoubles values = new VectorOfDoubles();
        for(int i=0; i<realVals.length; i++) {
            values.add(realVals[i]);
        }
        boolean success = swigUPO.SetRealValues(name, values);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::SetRealValues() failed");
    }

    public void setStrValues(String name, String[] strVals) throws UdmException {
        VectorOfStrings values = new VectorOfStrings();
        for(int i=0; i<strVals.length; i++) {
            values.add(new cint_string(strVals[i]));
        }
        boolean success = swigUPO.SetStrValues(name, values);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::SetStrValues() failed");
    }

    public void setBoolValues(String name, boolean[] booleanVals) throws UdmException {
        VectorOfBools values = new VectorOfBools();
        for(int i=0; i<booleanVals.length; i++) {
            values.add(booleanVals[i]);
        }
        boolean success = swigUPO.SetBoolValues(name, values);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::SetBoolValues() failed");
    }

    public double getRealVal(String name) throws UdmException {
        log.finer("UdmDouble swigVal = new UdmDouble(0.0);");
        UdmDouble swigVal = new UdmDouble(0.0);
        log.finer("UdmPseudoObject.GetRealVal(\"" + name + "\",swigVal);");
        boolean success = swigUPO.GetRealVal(name, swigVal);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::GetRealVal() failed");
        log.finer("swigVal.getDoubleVal();");
        return swigVal.getDoubleVal();
    }

    public String getStringVal(String name) throws UdmException {
        log.finer("cint_string swigVal = new cint_string(\"\");");
        cint_string swigVal = new cint_string("");
        log.finer("UdmPseudoObject.GetStrVal(\"" + name + "\",swigVal);");
        boolean success = swigUPO.GetStrVal(name, swigVal);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::GetStrVal() failed");
        log.finer("swigVal.buffer();");
        return swigVal.buffer();
    }

    public boolean getBooleanVal(String name) throws UdmException {
        log.finer("UdmBool swigVal = new UdmBool(false);");
        UdmBool swigVal = new UdmBool(false);
        log.finer("UdmPseudoObject.GetBoolVal(\"" + name + "\",swigVal);");
        boolean success = swigUPO.GetBoolVal(name, swigVal);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::GetBoolVal() failed");
        log.finer("swigVal.getBoolVal();");
        return swigVal.getBoolVal();
    }

    public long getAdjacentObjectsCount() throws UdmException {
        log.finer("UdmLong swigVal = new UdmLong(1);");
        UdmLong swigVal = new UdmLong(1);
        log.finer("UdmPseudoObject.GetAdjacentObjectsCount(swigVal);");
        boolean success = swigUPO.GetAdjacentObjectsCount(swigVal);
        UdmHelper.checkError(success,
                "Call to UDM method UdmPseudoObject::GetAdjacentObjectsCount() failed");
        log.finer("swigVal.getLongVal();");
        return swigVal.getLongVal();
    }

    public UdmPseudoObjectContainer getAdjacentObjects() throws UdmException {
        long adjacentObjectsCount = getAdjacentObjectsCount();
        log.finer("UdmPseudoObjectS swigContainer = new UdmPseudoObjectS(" + adjacentObjectsCount
                + ");");
        UdmPseudoObjectS swigContainer = new UdmPseudoObjectS((int) adjacentObjectsCount);
        log.finer("UdmPseudoObject.GetAdjacentObjects(swigContainer);");
        boolean success = swigUPO.GetAdjacentObjects(swigContainer);
        UdmHelper.checkError(success,
                "Call to UDM method UdmPseudoObject::GetAdjacentObjects() failed");
        return new UdmPseudoObjectContainer(swigContainer);
    }

    public long getAdjacentObjectsCount(String dstType, String dst_type_ns_path) throws UdmException {
        log.finer("UdmLong swigVal = new UdmLong(1);");
        UdmLong swigVal = new UdmLong(1);
        log.finer("UdmPseudoObject.GetAdjacentObjectsCount(\"" + dstType
                + "\",\"" + dst_type_ns_path
                + "\",swigVal);");
        boolean success = swigUPO.GetAdjacentObjectsCount(dstType, dst_type_ns_path, swigVal);
        UdmHelper.checkError(success,
                "Call to UDM method UdmPseudoObject::GetAdjacentObjectsCount() failed");
        log.finer("swigVal.getLongVal();");
        return swigVal.getLongVal();
    }

    public UdmPseudoObjectContainer getAdjacentObjects(String dstType, String dst_type_ns_path) throws UdmException {
        long adjacentObjectsCount = getAdjacentObjectsCount(dstType, dst_type_ns_path);
        log.finer("UdmPseudoObjectS swigContainer = new UdmPseudoObjectS(" + adjacentObjectsCount
                + ");");
        UdmPseudoObjectS swigContainer = new UdmPseudoObjectS((int) adjacentObjectsCount);
        log.finer("UdmPseudoObject.GetAdjacentObjects(\"" + dstType
                + "\",\"" + dst_type_ns_path
                + "\",swigContainer);");
        boolean success = swigUPO.GetAdjacentObjects(dstType, dst_type_ns_path, swigContainer);
        UdmHelper.checkError(success,
                "Call to UDM method UdmPseudoObject::GetAdjacentObjects() failed");
        return new UdmPseudoObjectContainer(swigContainer);
    }

    public long getAdjacentObjectsCount(String dstType, String dst_type_ns_path, AssociationInfo associationInfo)
            throws UdmException {
        log.finer("UdmLong swigVal = new UdmLong(1);");
        UdmLong swigVal = new UdmLong(1);
        log.finer("UdmPseudoObject.GetAdjacentObjectsCount(\"" + dstType
                + "\",\"" + dst_type_ns_path
                + "\",associationInfo,swigVal);");
        boolean success = swigUPO.GetAdjacentObjectsCount(dstType, dst_type_ns_path, associationInfo.getInternal(),
                swigVal);
        UdmHelper.checkError(success,
                "Call to UDM method UdmPseudoObject::GetAdjacentObjectsCount() failed");
        log.finer("swigVal.getLongVal();");
        return swigVal.getLongVal();
    }

    public UdmPseudoObjectContainer getAdjacentObjects(String dstType, String dst_type_ns_path,
            AssociationInfo associationInfo) throws UdmException {
        long adjacentObjectsCount = getAdjacentObjectsCount(dstType, dst_type_ns_path, associationInfo);
        log.finer("UdmPseudoObjectS swigContainer = new UdmPseudoObjectS(" + adjacentObjectsCount
                + ");");
        UdmPseudoObjectS swigContainer = new UdmPseudoObjectS((int) adjacentObjectsCount);
        edu.vanderbilt.isis.udm.swig.AssociationInfo swigAI = associationInfo.getInternal();
        log.finer("UdmPseudoObject.GetAdjacentObjects(\"" + dstType
                + "\",\"" + dst_type_ns_path
                + "\",AssociationInfo,swigContainer);");
        boolean success = swigUPO.GetAdjacentObjects(dstType, dst_type_ns_path, swigAI, swigContainer);
        UdmHelper.checkError(success,
                "Call to UDM method UdmPseudoObject::GetAdjacentObjects() failed");
        return new UdmPseudoObjectContainer(swigContainer);
    }

    public UdmPseudoObject getParent() throws UdmException {
        log.finer("UdmPseudoObject parent = new UdmPseudoObject();");
        edu.vanderbilt.isis.udm.swig.UdmPseudoObject parent = new edu.vanderbilt.isis.udm.swig.UdmPseudoObject();
        log.finer("UdmPseudoObject.GetParent(parent);");
        boolean success = swigUPO.GetParent(parent);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::GetParent() failed");
        return new UdmPseudoObject(parent);
    }

    public long getChildObjectsCount(String type, String type_ns_path) throws UdmException {
        log.finer("UdmLong swigVal = new UdmLong(1);");
        UdmLong swigVal = new UdmLong(1);
        log.finer("UdmPseudoObject.GetChildObjectsCount(\"" + type
                + "\",\"" + type_ns_path
                + "\",swigVal);");
        boolean success = swigUPO.GetChildObjectsCount(type, type_ns_path, swigVal);
        UdmHelper.checkError(success,
                "Call to UDM method UdmPseudoObject::GetChildObjectsCount() failed");
        log.finer("swigVal.getLongVal();");
        return swigVal.getLongVal();
    }

    public UdmPseudoObjectContainer getChildObjects(String type, String type_ns_path) throws UdmException {
        long childObjectsCount = getChildObjectsCount(type, type_ns_path);
        log.finer("UdmPseudoObjectS swigContainer = new UdmPseudoObjectS(" + childObjectsCount
                + ");");
        UdmPseudoObjectS swigContainer = new UdmPseudoObjectS((int) childObjectsCount);
        log.finer("UdmPseudoObject.GetChildObjects(\"" + type
                + "\",\"" + type_ns_path
                + "\",swigContainer);");
        boolean success = swigUPO.GetChildObjects(type, type_ns_path, swigContainer);
        UdmHelper.checkError(success,
                "Call to UDM method UdmPseudoObject::GetChildObjects() failed");
        return new UdmPseudoObjectContainer(swigContainer);
    }

    public long getChildObjectsCount(CompositionInfo compositionInfo, String type, String type_ns_path)
            throws UdmException {
        log.finer("UdmLong swigVal = new UdmLong(1);");
        UdmLong swigVal = new UdmLong(1);
        log.finer("cint_string cintStrType = new cint_string(\"" + type + "\");");
        cint_string cintStrType = new cint_string(type);
        log.finer("cint_string cintStrTypeNSPath = new cint_string(\"" + type_ns_path + "\");");
        cint_string cintStrTypeNSPath = new cint_string(type_ns_path);
        log.finer("UdmPseudoObject.GetChildObjectsCount(compositionInfo,cintStrType,cintStrTypeNSPath,swigVal);");
        boolean success = swigUPO.GetChildObjectsCount(compositionInfo.getInternal(), cintStrType,
                cintStrTypeNSPath, swigVal);
        UdmHelper.checkError(success,
                "Call to UDM method UdmPseudoObject::GetChildObjectsCount() failed");
        log.finer("swigVal.getLongVal();");
        return swigVal.getLongVal();
    }

    public UdmPseudoObjectContainer getChildObjects(CompositionInfo compositionInfo, String type, String type_ns_path)
            throws UdmException {
        long childObjectsCount = getChildObjectsCount(compositionInfo, type, type_ns_path);
        log.finer("UdmPseudoObjectS swigContainer = new UdmPseudoObjectS(" + childObjectsCount
                + ");");
        UdmPseudoObjectS swigContainer = new UdmPseudoObjectS((int) childObjectsCount);
        log.finer("cint_string swigType = new cint_string(\"" + type + "\");");
        cint_string swigType = new cint_string(type);
        log.finer("cint_string swigTypeNSPath = new cint_string(\"" + type_ns_path + "\");");
        cint_string swigTypeNSPath = new cint_string(type_ns_path);
        edu.vanderbilt.isis.udm.swig.CompositionInfo swigCI = compositionInfo.getInternal();
        log.finer("UdmPseudoObject.GetChildObjects(compositionInfo,type_ns_path,swigType,swigContainer);");
        boolean success = swigUPO.GetChildObjects(swigCI, swigType, swigTypeNSPath, swigContainer);
        UdmHelper.checkError(success,
                "Call to UDM method UdmPseudoObject::GetChildObjects() failed");
        return new UdmPseudoObjectContainer(swigContainer);
    }

    public long getAssociationClassObjectsCount(UdmPseudoObject dstObject, AssociationInfo ai)
            throws UdmException {
        log.finer("UdmLong swigVal = new UdmLong(1);");
        UdmLong swigVal = new UdmLong(1);
        log
                .finer("UdmPseudoObject.GetAssociationClassObjectsCount(dstObject,assocationInfo,swigVal);");
        boolean success = swigUPO.GetAssociationClassObjectsCount(dstObject.getInternal(), ai
                .getInternal(), swigVal);
        UdmHelper.checkError(success,
                "Call to UDM method UdmPseudoObject::GetAssociationClassObjectsCount() failed");
        log.finer("swigVal.getLongVal();");
        return swigVal.getLongVal();
    }

    public UdmPseudoObjectContainer getAssociationClassObjects(UdmPseudoObject dstObject,
            AssociationInfo ai) throws UdmException {
        long associationClassObjectsCount = getAssociationClassObjectsCount(dstObject, ai);
        log.finer("UdmPseudoObjectS swigContainer = new UdmPseudoObjectS("
                + associationClassObjectsCount + ");");
        UdmPseudoObjectS swigContainer = new UdmPseudoObjectS((int) associationClassObjectsCount);
        edu.vanderbilt.isis.udm.swig.UdmPseudoObject swigDstUPO = dstObject.getInternal();
        edu.vanderbilt.isis.udm.swig.AssociationInfo swigAI = ai.getInternal();
        log
                .finer("UdmPseudoObject.GetAssociationClassObjects(dstObject, associationInfo, swigContainer);");
        boolean success = swigUPO.GetAssociationClassObjects(swigDstUPO, swigAI, swigContainer);
        UdmHelper.checkError(success,
                "Call to UDM method UdmPseudoObject::GetAssociationClassObjects() failed");
        return new UdmPseudoObjectContainer(swigContainer);
    }

    public long getPeersFromAssociationClassObjectCount() throws UdmException {
        log.finer("UdmLong swigVal = new UdmLong(1);");
        UdmLong swigVal = new UdmLong(1);
        log.finer("UdmPseudoObject.GetPeersFromAssociationClassObjectCount(swigVal);");
        boolean success = swigUPO.GetPeersFromAssociationClassObjectCount(swigVal);
        UdmHelper
                .checkError(success,
                        "Call to UDM method UdmPseudoObject::GetPeersFromAssociationClassObjectCount() failed");
        log.finer("swigVal.getLongVal();");
        return swigVal.getLongVal();
    }

    public UdmPseudoObjectContainer getPeersFromAssociationClassObject() throws UdmException {
        long peersFromAssociationClassObjectCount = getPeersFromAssociationClassObjectCount();
        log.finer("UdmPseudoObjectS swigContainer = new UdmPseudoObjectS("
                + peersFromAssociationClassObjectCount + ");");
        UdmPseudoObjectS swigContainer = new UdmPseudoObjectS((int) peersFromAssociationClassObjectCount);
        log.finer("UdmPseudoObject.GetPeersFromAssociationClassObject(swigContainer);");
        boolean success = swigUPO.GetPeersFromAssociationClassObject(swigContainer);
        UdmHelper.checkError(success,
                "Call to UDM method UdmPseudoObject::GetPeersFromAssociationClassObject() failed");
        return new UdmPseudoObjectContainer(swigContainer);
    }

    public UdmPseudoObject createObject(String type, String type_ns_path) throws UdmException {
        log.finer("UdmPseudoObject swigNewUPO = new UdmPseudoObject();");
        edu.vanderbilt.isis.udm.swig.UdmPseudoObject swigNewUPO = new edu.vanderbilt.isis.udm.swig.UdmPseudoObject();
        log.finer("UdmPseudoObject.CreateObject(\"" + type + "," + type_ns_path + "\",swigNewUPO);");
        boolean success = swigUPO.CreateObject(type, type_ns_path, swigNewUPO);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::CreateObject() failed");
        return new UdmPseudoObject(swigNewUPO);
    }

    public UdmPseudoObject createObject(String type, String type_ns_path, String childRole) throws UdmException {
        log.finer("UdmPseudoObject swigNewUPO = new UdmPseudoObject();");
        edu.vanderbilt.isis.udm.swig.UdmPseudoObject swigNewUPO = new edu.vanderbilt.isis.udm.swig.UdmPseudoObject();
        log.finer("UdmPseudoObject.CreateObject(\"" + type + "," + type_ns_path  + "\",swigNewUPO,\"" + childRole
                + "\");");
        boolean success = swigUPO.CreateObject(type, type_ns_path, swigNewUPO, childRole);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::CreateObject() failed");
        return new UdmPseudoObject(swigNewUPO);
    }

    public void createLink(UdmPseudoObject dst, AssociationInfo associationType)
            throws UdmException {
        edu.vanderbilt.isis.udm.swig.UdmPseudoObject swigDstUPO = dst.getInternal();
        edu.vanderbilt.isis.udm.swig.AssociationInfo swigAI = associationType.getInternal();
        log.finer("UdmPseudoObject.CreateLink(dst,associationType);");
        boolean success = swigUPO.CreateLink(swigDstUPO, swigAI);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::CreateLink() failed");
    }

    public void deleteObject() throws UdmException {
        log.finer("UdmPseudoObject.DeleteObject();");
        boolean success = swigUPO.DeleteObject();
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoObject::DeleteObject() failed");
    }
}
