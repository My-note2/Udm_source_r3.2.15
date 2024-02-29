/*
 * Created on Apr 8, 2005
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.udm;

import java.io.InputStream;


/**
 * @author nsomogyi
 *
 */
public abstract class UdmFileFactory extends ExtendedUdmFactory {
    /**
     *
     * @param xmlMetaFile
     * @param xsdMetaFile
     * @param metaName
     * @param packagePath
     * @throws UdmException
     */
    public UdmFileFactory(
        String xmlMetaFile, String xsdMetaFile, String metaName, String packagePath)
        throws UdmException {
        super(xmlMetaFile, xsdMetaFile, metaName, packagePath);
    }

    /**
     * Creates a data network in a file with the specified root type.
     * @param instanceFileName
     * @param metaType
     * @return The instance of the root object in the created data network.
     * @throws UdmException
     */
    public UdmPseudoObject createNewDataNetwork(String instanceFileName, String metaType, String ns)
        throws UdmException {
        return new UdmPseudoObject(
            createNewDNToFile(instanceFileName, metaDiagram.getMetaClass(metaType, ns)), metaDiagram);
    }

    /**
     * Creates and opens a data network from a file specified by the parameter.
     * @param instanceFileName The file containing the xml.
     * @return The root object of the opened data network
     * @throws UdmException
     */
    public UdmPseudoObject openExistingDataNetwork(String instanceFileName)
        throws UdmException {
        return openExistingDNFromFile(instanceFileName);
    }

    /**
     * Creates and opens a data network from a string specified by the parameter.
     * @param xmlStream The string containing the xml.
     * @return The root object of the opened data network
     * @throws UdmException
     */
    public UdmPseudoObject openExistingDataNetworkFromStream(InputStream xmlStream)
        throws UdmException {
        return openExistingDNFromStream(xmlStream);
    }

    /**
     * Saves the data network in a file specified by the parameter.
     * @param instanceFileName 
     */
    public void saveAs(String instanceFileName) throws UdmException {
        saveAsFile(instanceFileName);
    }

    /**
     * Returns the data network in a stream.
     * @return The data network.
     */
    public InputStream saveAsStream() throws UdmException {
        return super.saveAsStream();
    }

    /**
     * Overrides #java.lang.Object.toString().
     * Returns the data network in a string.
     * @return The data network.
     * @deprecated
     */
    public String toString() {
        try {
            return saveAsString();
        } catch (UdmException udmEx) {
        }
        return null;
    }
}
