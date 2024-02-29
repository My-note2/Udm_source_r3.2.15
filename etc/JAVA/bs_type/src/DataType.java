/*
 * Created on @DATE@
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.@DSPACKAGELOWER@analyzer.datatypes;

import org.biospice.datatypes.impl.BiodataFileType;


/**
 * @author nsomogyi
 *
 */
public class @DSPACKAGE@DataType extends BiodataFileType {
    private static String @DSPACKAGE@_MIME_TYPE = "application/x-@DSPACKAGELOWER@+xml";

    /**
     * Constructor.
     */
    public @DSPACKAGE@DataType() {
        super();
    }

    /* (non-Javadoc)
     * @see org.biospice.datatypes.TypeEx#getTypeName()
     */
    public String getTypeName() {
        return "@DSPACKAGE@ XML Text File";
    }

    /* (non-Javadoc)
     * @see org.biospice.datatypes.TypeEx#getShortDescription()
     */
    public String getShortDescription() {
        return "@DSPACKAGE@ XML Text File";
    }

    /* (non-Javadoc)
     * @see org.biospice.datatypes.TypeEx#getLongDescription()
     */
    public String getLongDescription() {
        return "@DSPACKAGE@ Data File. Contains data in XML format.";
    }

    /* (non-Javadoc)
     * @see org.biospice.datatypes.TypeEx#getReferecnes()
     */
    public String getReferecnes() {
        return "";
    }

    /* (non-Javadoc)
     * @see org.biospice.datatypes.Type#getMIMEType()
     */
    public String getMIMEType() {
        return @DSPACKAGE@_MIME_TYPE;
    }

    /* (non-Javadoc)
     * @see org.biospice.datatypes.Type#getJavaType()
     */
    public Class getJavaType() {
        return edu.vanderbilt.isis.@DSPACKAGELOWER@analyzer.datatypes.@DSPACKAGE@Data.class;
    }

    /* (non-Javadoc)
     * @see org.biospice.datatypes.Type#getBiodataFactory()
     */
    public Class getBiodataFactory() {
        // TODO Auto-generated method stub
        return edu.vanderbilt.isis.@DSPACKAGELOWER@analyzer.datatypes.@DSPACKAGE@DataFactory.class;
    }

    /* (non-Javadoc)
     * @see org.biospice.datatypes.Type#getFileExtension()
     */
    public String getFileExtension() {
        return "xml";
    }
}
