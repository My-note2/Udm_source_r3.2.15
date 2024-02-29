/**
 * Created on Jan 12, 2005
 */
package edu.vanderbilt.isis.udm;

import java.util.Map;
import java.util.TreeMap;


/**
 * Generic implementation for the Diagram class
 * 
 * @author Himanshu Neema
 */
public class Diagram {
    private UdmPseudoObject diagram;

    private Map metaClassesByPath = new TreeMap();
    
    /*public Diagram(String xmlMetaFile) throws UdmException {
        diagram = UdmHelper.loadDiagram(xmlMetaFile);
    }*/

    public Diagram(String xmlMetaFile, String xmlString) throws UdmException {
        diagram = UdmHelper.loadDiagramFromString(xmlMetaFile, xmlString);
    }

    public UdmPseudoObject getDiagram() {
        return diagram;
    }
    
    public UdmPseudoObject getMetaClass(String metaName, String nsPath) throws UdmException {
        UdmPseudoObject metaClass = (UdmPseudoObject) metaClassesByPath.get(nsPath + "::" + metaName);
        if (metaClass == null) {
            metaClass = UdmHelper.initClassByName(this, metaName, nsPath);
            UdmHelper.checkNotNull(metaClass, "Couldn't init meta-class for '" + nsPath + "::" + metaName
                    + "' by UPO_SetClass()");
            metaClassesByPath.put(nsPath + "::" + metaName, metaClass);
        }
        return metaClass;
    }
}
