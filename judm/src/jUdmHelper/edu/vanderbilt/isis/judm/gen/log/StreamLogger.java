/*
 * Created on Jun 30, 2005
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.judm.gen.log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;


/**
 * @author nsomogyi
 *
 */
public class StreamLogger extends Thread {
    InputStream is;
    String type;
    GenLogger logger;

    public StreamLogger(InputStream is, String type, GenLogger logger) {
        this.is = is;
        this.type = type;
        this.logger = logger;
    }

    public void run() {
        try {
            InputStreamReader isr = new InputStreamReader(is);
            BufferedReader br = new BufferedReader(isr);
            String line = null;

            while ((line = br.readLine()) != null)
                logger.log(type + "> " + line + "\n");
        } catch (IOException ioe) {
            ioe.printStackTrace();
        }
    }
}
