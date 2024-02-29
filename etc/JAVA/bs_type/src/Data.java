/*
 * Created on @DATE@
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.@DSPACKAGELOWER@analyzer.datatypes;

import org.biospice.datatypes.impl.BiodataFileImpl;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;


/**
 * @author nsomogyi
 *
 */
public class @DSPACKAGE@Data extends BiodataFileImpl {
    /**
     * @param arg0
     * @throws java.io.IOException
     */
    public @DSPACKAGE@Data(File arg0) throws IOException {
        super(arg0);
    }

    /**
     * @param arg0
     * @param arg1
     * @param arg2
     * @throws java.io.IOException
     */
    public @DSPACKAGE@Data(File arg0, int arg1, int arg2) throws IOException {
        super(arg0, arg1, arg2);
    }

    /**
     * @param arg0
     * @throws java.io.IOException
     */
    public @DSPACKAGE@Data(InputStream arg0) throws IOException {
        super(arg0);
    }

    /**
     * @param arg0
     * @param arg1
     * @throws java.io.IOException
     */
    public @DSPACKAGE@Data(InputStream arg0, boolean arg1) throws IOException {
        super(arg0, arg1);
    }

    /**
     * @param arg0
     * @param arg1
     * @param arg2
     * @param arg3
     * @throws java.io.IOException
     */
    public @DSPACKAGE@Data(InputStream arg0, boolean arg1, int arg2, int arg3)
        throws IOException {
        super(arg0, arg1, arg2, arg3);
    }

    /**
     * @param arg0
     * @param arg1
     * @throws java.io.IOException
     */
    public @DSPACKAGE@Data(InputStream arg0, File arg1) throws IOException {
        super(arg0, arg1);
    }

    /**
     * @param arg0
     * @param arg1
     * @param arg2
     * @throws java.io.IOException
     */
    public @DSPACKAGE@Data(InputStream arg0, File arg1, boolean arg2)
        throws IOException {
        super(arg0, arg1, arg2);
    }

    /**
     * @param arg0
     * @param arg1
     * @param arg2
     * @param arg3
     * @param arg4
     * @throws java.io.IOException
     */
    public @DSPACKAGE@Data(InputStream arg0, File arg1, boolean arg2, int arg3,
        int arg4) throws IOException {
        super(arg0, arg1, arg2, arg3, arg4);
    }

    /**
     * @param arg0
     * @throws java.io.IOException
     */
    public @DSPACKAGE@Data(String arg0) throws IOException {
        super(arg0);
    }

    /**
     * @param arg0
     * @param arg1
     * @param arg2
     * @throws java.io.IOException
     */
    public @DSPACKAGE@Data(String arg0, int arg1, int arg2)
        throws IOException {
        super(arg0, arg1, arg2);
    }
}
