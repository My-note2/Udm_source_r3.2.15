/*
 * Created on @DATE@
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.@DSPACKAGELOWER@analyzer.datatypes;

import org.biospice.datatypes.Biodata;
import org.biospice.datatypes.BiodataFactory;

import java.io.IOException;
import java.io.InputStream;


/**
 * @author nsomogyi
 *
 */
public class @DSPACKAGE@DataFactory extends BiodataFactory {
    /**
     *
     */
    public @DSPACKAGE@DataFactory() {
        super();
    }

    /* (non-Javadoc)
     * @see org.biospice.datatypes.BiodataFactory#fromStream(java.io.InputStream)
     */
    public Biodata fromStream(InputStream arg0)
        throws IOException, IllegalArgumentException {
        return new @DSPACKAGE@Data(arg0, false);
    }

    /* (non-Javadoc)
     * @see org.biospice.datatypes.BiodataFactory#fromString(java.lang.String)
     */
    public Biodata fromString(String arg0) throws IllegalArgumentException {
        try {
            return new @DSPACKAGE@Data(@DSPACKAGE@Data.stringToStream(arg0), false);
        } catch (IOException ioEx) {
            throw new IllegalArgumentException(ioEx.getMessage());
        }
    }
}
