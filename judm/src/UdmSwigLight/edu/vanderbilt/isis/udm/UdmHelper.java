/**
 * Created on Jan 6, 2005
 */
package edu.vanderbilt.isis.udm;

import edu.vanderbilt.isis.udm.swig.UDMSwig;
import edu.vanderbilt.isis.udm.swig.cint_string;

import java.util.logging.Logger;


/**
 * UDMPsuedoObject flat cint interface
 *
 * @author Himanshu Neema
 */
public class UdmHelper {
    private static final Logger log = Logger.getLogger(UdmHelper.class.getName());
    public static int TARGET_FROM_PEER = UDMSwig.getTARGETFROMPEER();
    public static int TARGET_FROM_CLASS = UDMSwig.getTARGETFROMCLASS();
    public static int CLASS_FROM_TARGET = UDMSwig.getCLASSFROMTARGET();
    public static int UPDN_CHANGES_PERSIST_ALWAYS = UDMSwig.getUPDN_CHANGES_PERSIST_ALWAYS();
    public static int UPDN_CHANGES_PERSIST_DEFAULT = UDMSwig.getUPDN_CHANGES_PERSIST_DEFAULT();
    public static int UPDN_CHANGES_LOST_DEFAULT = UDMSwig.getUPDN_CHANGES_LOST_DEFAULT();

    public static UdmPseudoObject loadDiagram(String xmlMetaFile)
        throws UdmException {
        log.finer("UdmPseudoObject swigUpo = new UdmPseudoObject();");

        edu.vanderbilt.isis.udm.swig.UdmPseudoObject swigUpo = new edu.vanderbilt.isis.udm.swig.UdmPseudoObject();
        log.finer("UDMSwig.UPO_LoadDiagram(\"" + xmlMetaFile + "\",swigUpo);");

        boolean success = UDMSwig.UPO_LoadDiagram(xmlMetaFile, swigUpo);
        checkError(success,
            "Error while loading metaDiagram file: " + xmlMetaFile);

        return new UdmPseudoObject(swigUpo);
    }

    public static UdmPseudoObject loadDiagramFromString(String xmlMetaFile,
        String xmlString) throws UdmException {
        log.finer("UdmPseudoObject swigUpo = new UdmPseudoObject();");

        edu.vanderbilt.isis.udm.swig.UdmPseudoObject swigUpo = new edu.vanderbilt.isis.udm.swig.UdmPseudoObject();
        log.finer("UDMSwig.UPO_LoadDiagramFromString(\"" + xmlMetaFile + "\"," +
            xmlString + "\",swigUpo);");

        boolean success = UDMSwig.UPO_LoadDiagramFromString(xmlMetaFile,
                xmlString, swigUpo);
        checkError(success,
            "Error while loading metaDiagram file from string: " + xmlMetaFile);

        return new UdmPseudoObject(swigUpo);
    }

    public static void unloadDiagram(String xmlMetaFile)
        throws UdmException {
        log.finer("UDMSwig.UPO_UnLoadDiagram(\"" + xmlMetaFile + "\");");

        boolean success = UDMSwig.UPO_UnLoadDiagram(xmlMetaFile);
        checkError(success,
            "Error while unloading metaDiagram file: " + xmlMetaFile);
    }

    public static void UPO_SetDiagram(UdmPseudoObject diagramWhat, String name)
        throws UdmException {
        log.finer("UDMSwig.UPO_SetDiagram(diagram,\"" + name + "\");");

        boolean success = UDMSwig.UPO_SetDiagram(diagramWhat.getInternal(), name);
        checkError(success, "Error in UDM method UPO_SetDiagram()");
    }

    public static void UPO_SetClass(UdmPseudoObject classWhat,
        UdmPseudoObject parentWhat, String targetName)
        throws UdmException {
        log.finer("UDMSwig.UPO_SetClass(class,parent,\"" + targetName +
            "\");");

        boolean success = UDMSwig.UPO_SetClass(classWhat.getInternal(),
                parentWhat.getInternal(), targetName);
        checkError(success, "Error in UDM method UPO_SetClass");
    }

    public static void UPO_SetNamespace(UdmPseudoObject namespaceWhat,
        UdmPseudoObject parentWhat, String targetName)
        throws UdmException {
        log.finer("UDMSwig.UPO_SetNamespace(namespace,parent,\"" + targetName +
            "\");");

        boolean success = UDMSwig.UPO_SetNamespace(namespaceWhat.getInternal(),
                parentWhat.getInternal(), targetName);
        checkError(success, "Error in UDM method UPO_SetNamespace");
    }

    public static void UPO_SetNamespaceByPath(UdmPseudoObject namespaceWhat,
        UdmPseudoObject diagram, String path)
        throws UdmException {
        log.finer("UDMSwig.UPO_SetNamespaceByPath(namespace,diagram,\"" + path +
            "\");");

        boolean success = UDMSwig.UPO_SetNamespaceByPath(namespaceWhat.getInternal(),
                diagram.getInternal(), path);
        checkError(success, "Error in UDM method UPO_SetNamespaceByPath");
    }

    public static void UPO_SetAttribute(UdmPseudoObject attrWhat,
        UdmPseudoObject classWhatClass, String targetName)
        throws UdmException {
        log.finer("UDMSwig.UPO_SetAttribute(attr,class,\"" + targetName +
            "\");");

        boolean success = UDMSwig.UPO_SetAttribute(attrWhat.getInternal(),
                classWhatClass.getInternal(), targetName);
        checkError(success, "Error in UDM method UPO_SetAttribute");
    }

    public static void UPO_SetChildRole(UdmPseudoObject ccrWhat,
        UdmPseudoObject whatTargetClass, UdmPseudoObject whatTheoTargetClass,
        String roleName, String oRoleName) throws UdmException {
        log.finer("UDMSwig.UPO_SetChildRole(ccr,targetClass,theoTargetClass,\"" +
            roleName + "\",\"" + oRoleName + "\");");

        boolean success = UDMSwig.UPO_SetChildRole(ccrWhat.getInternal(),
                whatTargetClass.getInternal(),
                whatTheoTargetClass.getInternal(), roleName, oRoleName);
        checkError(success, "Error in UDM method UPO_SetChildRole");
    }

    public static void UPO_SetParentRole(UdmPseudoObject cprWhat,
        UdmPseudoObject whatTargetClass, UdmPseudoObject whatTheoTargetClass,
        String roleName, String oRoleName) throws UdmException {
        log.finer(
            "UDMSwig.UPO_SetParentRole(cpr,targetClass,theoTargetClass,\"" +
            roleName + "\",\"" + oRoleName + "\");");

        boolean success = UDMSwig.UPO_SetParentRole(cprWhat.getInternal(),
                whatTargetClass.getInternal(),
                whatTheoTargetClass.getInternal(), roleName, oRoleName);
        checkError(success, "Error in UDM method UPO_SetParentRole");
    }

    public static void UPO_SetAssocRole(UdmPseudoObject arWhat,
        UdmPseudoObject whatTargetClass, UdmPseudoObject whatTheoTargetClass,
        String targetName) throws UdmException {
        log.finer("UDMSwig.UPO_SetAssocRole(ar,targetClass,theoTargetClass,\"" +
            targetName + "\");");

        boolean success = UDMSwig.UPO_SetAssocRole(arWhat.getInternal(),
                whatTargetClass.getInternal(),
                whatTheoTargetClass.getInternal(), targetName);
        checkError(success, "Error in UDM method UPO_SetAssocRole");
    }

    public static void StoreXsd(String key, String xsd_str)
        throws UdmException {
        cint_string key_c = new cint_string(key);
        cint_string xsd_str_c = new cint_string(xsd_str);
        boolean success = UDMSwig.StoreXsd(key_c, xsd_str_c);
        checkError(success, "Error in UDM method StoreXsd");
    }

    /**
     * Removes the stored XSDs.
     * @throws UdmException
     */
    public static void ClearXsdStorage() throws UdmException {
        boolean success = UDMSwig.ClearXsdStorage();
        checkError(success, "Error in UDM method ClearXsdStorage");
    }

    public static void RemoveXsd(String key) throws UdmException {
        cint_string key_c = new cint_string(key);
        boolean success = UDMSwig.RemoveXsd(key_c);
        checkError(success, "Error in UDM method RemoveXsd");
    }

    /**
     * Maps the given namespace to the given xsd uri target namespace.
     * @param namespaceURI The namespace.
     * @param namespaceUML The target namespace.
     * @return True if the mapping sucesful; otherwise, false.
     */
    public static void AddURIToUMLNamespaceMapping(String namespaceURI,
        String namespaceUML, 
		String xsdName ) throws UdmException {
        cint_string ns_uri = new cint_string(namespaceURI);
        cint_string ns_uml = new cint_string(namespaceUML);
		cint_string xn = new cint_string(xsdName);
        boolean success = UDMSwig.AddURIToUMLNamespaceMapping(ns_uri, ns_uml, xn);
        checkError(success, "Error in UDM method AddURIToUMLNamespaceMapping");
    }

    /**
     * @param diagram 
     * @param className
     * @param nsPath
     * @return The instance of an obejct of type <code>className</code> in namespace <code>nsPath</code>.
     * @throws UdmException
     */
    public static UdmPseudoObject initClassByName(UdmPseudoObject diagram,
        String className, String nsPath) throws UdmException {
        log.finer("UdmPseudoObject swigNewUPO = new UdmPseudoObject();");

        edu.vanderbilt.isis.udm.swig.UdmPseudoObject swigNewUPO = new edu.vanderbilt.isis.udm.swig.UdmPseudoObject();
        log.finer("UDMSwig.UPO_SetClass(swigNewUPO,parent,\"" + className +
            "\");");

        if (nsPath != null && nsPath.length() > 0) {
            edu.vanderbilt.isis.udm.swig.UdmPseudoObject nsUPO = new edu.vanderbilt.isis.udm.swig.UdmPseudoObject();

            boolean success = UDMSwig.UPO_SetNamespaceByPath(nsUPO,
                    diagram.getInternal(), nsPath);
            UdmHelper.checkError(success, "UDM method UPO_SetNamespaceByPath() failed");

             success = UDMSwig.UPO_SetClass(swigNewUPO, nsUPO, className);
             UdmHelper.checkError(success, "UDM method UPO_SetClass() failed");
        } else {
            boolean success = UDMSwig.UPO_SetClass(swigNewUPO, diagram.getInternal(), className);
             UdmHelper.checkError(success, "UDM method UPO_SetClass() failed");
        }

        return new UdmPseudoObject(swigNewUPO);
    }

    public static UdmPseudoObject initClassByName(Diagram diagram,
        String className, String nsPath) throws UdmException {
        return initClassByName(diagram.getDiagram(), className, nsPath);
    }

    public static String GetLastError() throws Exception {
        log.finest("cint_string swigLastError = new cint_string(\"\");");

        cint_string swigLastError = new cint_string("");
        log.finest("UdmPseudoObject.GetLastError(swigLastError);");

        boolean success = edu.vanderbilt.isis.udm.swig.UdmPseudoObject.GetLastError(swigLastError);

        if (!success) {
            throw new RuntimeException("UDM::GetLastError() failed");
        }

        log.finest("swigLastError.buffer();");

        return swigLastError.buffer();
    }

    public static void checkError(boolean success, String message)
        throws UdmException {
        if (!success) {
            throw new UdmException(message);
        }
    }

    public static void initialize() throws Exception {
        try {
            System.loadLibrary("UdmSwig");
        } catch (Exception e) {
            throw new RuntimeException("Error while initializing UDM DLLs through JNI",
                e);
        }
    }

    public static void checkNotNull(Object object, String message)
        throws UdmException {
        if (object == null) {
            throw new UdmException(message);
        }
    }
}
