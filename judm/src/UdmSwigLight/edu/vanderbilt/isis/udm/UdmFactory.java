/**
* Created on Jan 24, 2005
*/
package edu.vanderbilt.isis.udm;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.StringTokenizer;


/**
* A base class for the domain specific factory classes. Includes methods for creation of
* metaDiagram and dataNetwork.
*
* @author Himanshu Neema
*/
public abstract class UdmFactory {
    private static String SWIG_LIB_PATH = null;
    //private static final String udmPackagePath = "/edu/vanderbilt/isis/udm/";
    //private static final String umlXsdFile = "Uml.xsd";
    private String xmlMetaFile;
    private String xsdMetaFile;
    private String metaName;
    private String packagePath;
    protected Diagram metaDiagram;
    protected UdmPseudoDataNetwork dataNetwork;
	
    private void findSwigDll() {
        String judm_path = null;
        String swig_lib = System.mapLibraryName("UdmSwig");
        String os_name = System.getProperty("os.name");
        try {
            String exec = "cmd /c set";
            if (os_name.indexOf("Windows") == -1) {
                // linux specific
                exec = "/bin/bash -c set";
            }

            Process p = Runtime.getRuntime().exec(exec);
            InputStream is = p.getInputStream();
            BufferedReader br = new BufferedReader(new InputStreamReader(is));
            String line;
            while ((line = br.readLine()) != null) {
                StringTokenizer st = new StringTokenizer(line, "=");
                while (st.hasMoreTokens()) {
                    String key = st.nextToken();
                    if (key.compareTo("UDM_PATH") == 0) {
                        if (st.hasMoreTokens()) {
                            judm_path = st.nextToken();
                        }
                    }
                }
            }
        } catch (IOException ioEx) {
            System.out.println("exception: " + ioEx.getMessage());
        }

        if (judm_path != null) 
        {
//            throw new UnsatisfiedLinkError("No UDM_PATH environment variable defined");
        
        	if (os_name.indexOf("Windows") == -1) {
        		SWIG_LIB_PATH = judm_path + "/lib/" + swig_lib;
        	} else {
        		SWIG_LIB_PATH = judm_path + "/bin/" + swig_lib;
        	}
        }
    }

    protected UdmFactory(
        String xmlMetaFile, String xsdMetaFile, String metaName, String packagePath)
        throws UdmException {
    	
    	String err = new String("");
    	boolean success = true;
        try
		{
            System.loadLibrary("UdmSwig");
            //System.out.println("library path: UdmSwig");
        } 
        catch (UnsatisfiedLinkError linkEx)
		{
        	err = "Could not load UdmSwig " + linkEx.getMessage() + "\n Searched in folders:\n";
        	err += "java.library.path: " +System.getProperties().get("java.library.path")+"\n";
        	success = false;
		}
        
        // try in folder /usr/lib on linux 
        if (!success)
        {
        	if (System.getProperty("os.name").indexOf("Windows") == -1) 
        	{
        		success = true;
        		try
				{
        			System.load("/usr/lib/" + System.mapLibraryName("UdmSwig"));
				}
        		catch (UnsatisfiedLinkError linkEx1)
				{
        			err += "/usr/lib/\n";
        			success = false;
				}
        	}
        }
        // try in UDM_PATH
        if (!success)
        {
        	findSwigDll();
            if (SWIG_LIB_PATH == null)
            {
            	err += "Can't search UDM_PATH: no UDM_PATH environment variable defined";
            }
            else
            {
            	err += "Tried $UDM_PATH='" + SWIG_LIB_PATH + "'";
            	success = true;
                try
				{
	           		System.load(SWIG_LIB_PATH);
				}
	            catch (UnsatisfiedLinkError linkEx1)
				{
					err += "\n\t" + linkEx1.getMessage();
	            	success = false;
				}
            }
        }
        
        if (!success)
          throw new UnsatisfiedLinkError(err);
        // store the Uml.xsd file
        //UdmHelper.StoreXsd(umlXsdFile, Uml_xsd.getString());
        this.xmlMetaFile = xmlMetaFile;
        this.xsdMetaFile = xsdMetaFile;
        this.metaName = metaName;
        this.packagePath = packagePath;
        init();
    }

    public Diagram getDiagram() {
        return metaDiagram;
    }

    public UdmPseudoDataNetwork getDataNetwork() {
        return dataNetwork;
    }

    public String getDiagramMetaXmlFileLocation() {
        return xmlMetaFile;
    }

    public String getDiagramMetaXsdFileLocation() {
        return xsdMetaFile;
    }

    private void init() throws UdmException {
        loadDiagram();
    }

    protected abstract void loadDiagram() throws UdmException;
/*  {
         metaDiagram = new Diagram(xmlMetaFile, "cica");
    }*/

    protected void createDataNetwork() throws UdmException {
        dataNetwork = new UdmPseudoDataNetwork(metaName);
    }


    protected UdmPseudoObject openExistingDN(String instanceFileName)
        throws UdmException {
        return openExistingDNFromFile(instanceFileName);
    }

    protected UdmPseudoObject openExistingDNFromFile(String instanceFileName)
        throws UdmException {
        createDataNetwork();

        dataNetwork.openExisting(
            instanceFileName, xsdMetaFile, UdmHelper.UPDN_CHANGES_LOST_DEFAULT);
        return dataNetwork.getRootObject();
    }

    protected UdmPseudoObject openExistingDNFromString(String xml)
        throws UdmException {
        createDataNetwork();

        dataNetwork.openExistingFromString(xml, xsdMetaFile);
        return dataNetwork.getRootObject();
    }

    protected UdmPseudoObject openExistingDNFromStream(InputStream in)
        throws UdmException {
        java.io.DataInputStream din = new java.io.DataInputStream(in);
        String xml = new String("");
        try {
            String line = null;
            while ((line = din.readLine()) != null) {
                xml += (line + "\n");
            }
        } catch (IOException ex) {
            System.out.println("Error reading from stream: " + ex.getMessage());
        } finally {
            //  close stream
            try {
                din.close();
            } catch (Exception ex) {
            }
        }

        return openExistingDNFromString(xml);
    }

    protected UdmPseudoObject createNewDN(String instanceFileName, UdmPseudoObject rootMetaClass)
        throws UdmException {
        return createNewDNToFile(instanceFileName, rootMetaClass);
    }

    protected UdmPseudoObject createNewDNToFile(
        String instanceFileName, UdmPseudoObject rootMetaClass)
        throws UdmException {
        createDataNetwork();

        dataNetwork.createNew(
            instanceFileName, xsdMetaFile, rootMetaClass, UdmHelper.UPDN_CHANGES_LOST_DEFAULT);
        return dataNetwork.getRootObject();
    }

    protected UdmPseudoObject createNewDNToString(UdmPseudoObject rootMetaClass)
        throws UdmException {
        createDataNetwork();

        dataNetwork.createNewToString(xsdMetaFile, rootMetaClass);
        return dataNetwork.getRootObject();
    }
}
