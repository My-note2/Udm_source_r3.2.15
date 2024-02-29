/**
 * Created on Jan 6, 2005
 */
package edu.vanderbilt.isis.udm;

import java.util.logging.Logger;

import edu.vanderbilt.isis.udm.swig.UdmBool;
import edu.vanderbilt.isis.udm.swig.UdmLong;
import edu.vanderbilt.isis.udm.swig.cint_string;

/**
 * Main UDM Data Network implementation class
 * 
 * @author Himanshu Neema
 */
public class UdmPseudoDataNetwork {
    private static final Logger log = Logger.getLogger(UdmPseudoDataNetwork.class.getName());
    private edu.vanderbilt.isis.udm.swig.UdmPseudoDataNetwork swigDN;
    
    public UdmPseudoDataNetwork(String metaName) throws UdmException {
        log.finer("UdmBool success = new UdmBool(false);");
        UdmBool success = new UdmBool(false);
        log.finer("new UdmPseudoDataNetwork(\"" + metaName + "\",success)");
        swigDN = new edu.vanderbilt.isis.udm.swig.UdmPseudoDataNetwork(metaName, success);
        log.finer("success.getBoolVal();");
        if(!success.getBoolVal()) {
            throw new UdmException("Call to UDM method UdmPseudoDataNetwork() failed");
        }
    }

    public void openExisting(String sysName, String metaLocator) throws UdmException {
        log.finer("UdmPseudoDataNetwork.OpenExisting(\"" + sysName + "\",\"" + metaLocator + "\");");
        boolean success = swigDN.OpenExisting(sysName, metaLocator);
        UdmHelper.checkError(success, "Couldn't open existing data network, with file: '" + sysName + "' and XSD: '" + metaLocator + "'");
    }

    public void openExisting(String sysName, String metaLocator, int backendSemantics) throws UdmException {
        log.finer("UdmPseudoDataNetwork.OpenExisting(\"" + sysName + "\",\"" + metaLocator + "\"," +
                  backendSemantics + ");");
        boolean success = swigDN.OpenExisting(sysName, metaLocator, backendSemantics);
        UdmHelper.checkError(success, "Couldn't open existing data network, with file: '" + sysName + "' and XSD: '" + metaLocator + "'");
    }
    public void openExistingFromString(String sysName, String metaLocator) throws UdmException {
        log.finer("UdmPseudoDataNetwork.openExistingFromString(\""+ metaLocator + ");");
        boolean success = swigDN.OpenExistingFromString(sysName, metaLocator);
        UdmHelper.checkError(success, "Couldn't open existing data network from string,  XSD: " + metaLocator + "'");
    }

    public void createNew(String sysName, String metaLocator, UdmPseudoObject rootClass) throws UdmException {
        edu.vanderbilt.isis.udm.swig.UdmPseudoObject swigUPO = rootClass.getInternal();
        log.finer("UdmPseudoDataNetwork.CreateNew(\"" + sysName + "\",\"" + metaLocator +
                  "\",rootClass);");
        boolean success = swigDN.CreateNew(sysName, metaLocator, swigUPO);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoDataNetwork::CreateNew() failed");
    
    }

    public void createNewToString(String metaLocator, UdmPseudoObject rootClass) throws UdmException {
        edu.vanderbilt.isis.udm.swig.UdmPseudoObject swigUPO = rootClass.getInternal();
        log.finer("UdmPseudoDataNetwork.CreateNewToString(\"" + metaLocator +
                  "\",rootClass);");
        boolean success = swigDN.CreateNewToString(metaLocator, swigUPO);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoDataNetwork::CreateNew() failed");
    
    }

    public void createNew(String sysName, String metaLocator, UdmPseudoObject rootClass, int backendSemantics) throws UdmException {
        edu.vanderbilt.isis.udm.swig.UdmPseudoObject swigUPO = rootClass.getInternal();
        log.finer("UdmPseudoDataNetwork.CreateNew(\"" + sysName + "\",\"" + metaLocator +
        "\",rootClass," + backendSemantics + ");");
        boolean success = swigDN.CreateNew(sysName, metaLocator, swigUPO, backendSemantics);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoDataNetwork::CreateNew() failed");
    }

    public UdmPseudoObject getRootObject() throws UdmException {
        log.finer("UdmPseudoObject swigNewUPO = new UdmPseudoObject();");
        edu.vanderbilt.isis.udm.swig.UdmPseudoObject swigNewUPO = new edu.vanderbilt.isis.udm.swig.UdmPseudoObject();
        log.finer("UdmPseudoDataNetwork.getRootObject(swigNewUPO);");
        boolean success = swigDN.GetRootObject(swigNewUPO);
        if(!success) {
            throw new UdmException("Call to UDM method UdmPseudoDataNetwork::GetRootObject() failed");
        }
        return new UdmPseudoObject(swigNewUPO);
    }

    public UdmPseudoObject getRootMeta() throws UdmException {
        log.finer("UdmPseudoObject swigNewUPO = new UdmPseudoObject();");
        edu.vanderbilt.isis.udm.swig.UdmPseudoObject swigNewUPO = new edu.vanderbilt.isis.udm.swig.UdmPseudoObject();
        log.finer("UdmPseudoDataNetwork.getRootMeta(swigNewUPO);");
        boolean success = swigDN.GetRootMeta(swigNewUPO);
        if(!success) {
            throw new UdmException("Call to UDM method UdmPseudoDataNetwork::GetRootMeta() failed");
        }
        return new UdmPseudoObject(swigNewUPO);
    }

    public void closeWithUpdate() throws UdmException {
        log.finer("UdmPseudoDataNetwork.CloseWithUpdate();");
        boolean success = swigDN.CloseWithUpdate();
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoDataNetwork::CloseWithUpdate() failed");
    }

    public void closeNoUpdate() throws UdmException {
        log.finer("UdmPseudoDataNetwork.CloseNoUpdate();");
        boolean success = swigDN.CloseNoUpdate();
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoDataNetwork::CloseNoUpdate() failed");
    }

    public void saveAs(String systemName) throws UdmException {
        log.finer("UdmPseudoDataNetwork.SaveAs(\"" + systemName + "\");");
        boolean success = swigDN.SaveAs(systemName);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoDataNetwork::SaveAs() failed");
    }

    public String saveAsString() throws UdmException {
        log.finer("UdmPseudoDataNetwork.saveAsString();");
        cint_string res = new cint_string("");
        boolean success = swigDN.SaveAsString(res);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoDataNetwork::CloseAs() failed");
        return res.buffer();
    }

    public void closeAs(String systemName) throws UdmException {
        log.finer("UdmPseudoDataNetwork.CloseAs(\"" + systemName + "\");");
        boolean success = swigDN.CloseAs(systemName);
        UdmHelper.checkError(success, "Call to UDM method UdmPseudoDataNetwork::CloseAs() failed");
    }

    public boolean isOpen() throws UdmException {
        log.finer("UdmBool bIsOpen = new UdmBool(false);");
        UdmBool bIsOpen = new UdmBool(false);
        log.finer("UdmPseudoDataNetwork.isOpen(bIsOpen);");
        boolean success = swigDN.isOpen(bIsOpen);
        if(!success) {
            throw new UdmException("Call to UDM method UdmPseudoDataNetwork::isOpen() failed");
        }
        log.finer("bIsOpen.getBoolVal();");
        return bIsOpen.getBoolVal();
    }

    public long getUniqueId() throws UdmException {
        log.finer("UdmLong swigId = new UdmLong(0);");
        UdmLong swigId = new UdmLong(0);
        log.finer("UdmPseudoDataNetwork.uniqueId(swigId);");
        boolean success = swigDN.uniqueId(swigId);
        if(!success) {
            throw new UdmException("Call to UDM method UdmPseudoDataNetwork::uniqueId() failed");
        }
        log.finer("swigId.getLongVal();");
        return swigId.getLongVal();
    }
    
    /**
     * Calls the constraint checker in this data network and returns the result in
     * a string.
     * @return The result of the constraint evaluation.
     * @throws UdmException
     */
    public String checkConstraints() throws UdmException {
        log.finer("UdmPseudoDataNetwork.checkConstraints();");
        cint_string res = new cint_string("");
        boolean success = swigDN.OCL_Evaluate(res);
        if(!success) {
            throw new UdmException("Call to UDM method UdmPseudoDataNetwork::checkConstraints() failed");
        }
        return res.buffer();
    }
    
 }
