/*
 * Created on Apr 5, 2005
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.udm;

import java.io.InputStream;

/**
 * @author nsomogyi
 *
 */
public abstract class UdmStringFactory extends ExtendedUdmFactory {
    /**
     *
     * @param xmlMetaFile
     * @param xsdMetaFile
     * @param metaName
     * @param packagePath
     * @throws UdmException
     */
    public UdmStringFactory(
        String xmlMetaFile, String xsdMetaFile, String metaName, String packagePath)
        throws UdmException {
        super(xmlMetaFile, xsdMetaFile, metaName, packagePath);
    }

    /**
     * Creates a data network in string with the specified root type.
     * @param metaType
     * @param ns
     * @return The root object of the created data network
     * @throws UdmException
     */
    public UdmPseudoObject createNewDataNetwork(String metaType, String ns) throws UdmException {
        return new UdmPseudoObject(
            createNewDNToString(metaDiagram.getMetaClass(metaType, ns)), metaDiagram);
    }

    /**
     * Creates and opens a data network from a string specified by the parameter.
     * @param xmlString The string containing the xml.
     * @return The root object of the opened data network
     * @throws UdmException
     */
    public UdmPseudoObject openExistingDataNetwork(String xmlString) throws UdmException {
        return openExistingDNFromString(xmlString);
    }

    /**
     * Creates and opens a data network from a stream specified by the parameter.
     * @param xmlStream The stream containing the xml.
     * @return The root object of the opened data network
     * @throws UdmException
     */
    public UdmPseudoObject openExistingDataNetworkFromStream(InputStream xmlStream) throws UdmException {
        return openExistingDNFromStream(xmlStream);
    }

    /**
     * Returns the data network in a string.
     * @return The data network.
     */
    public String saveAs() throws UdmException {
        return saveAsString();
    }

    /**
     * Returns the data network in a string.
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
