/*
 * Created on Apr 7, 2005
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.timeseries.test;

import edu.vanderbilt.isis.timeseries.FactoryRepository;
import edu.vanderbilt.isis.timeseries.Container;
import edu.vanderbilt.isis.timeseries.ContainerStringFactory;
import edu.vanderbilt.isis.timeseries.data_label;
import edu.vanderbilt.isis.timeseries.data_value;
import edu.vanderbilt.isis.timeseries.real_data;
import edu.vanderbilt.isis.timeseries.time_point;
import edu.vanderbilt.isis.udm.UdmException;
import edu.vanderbilt.isis.udm.UdmPseudoObject;

import junit.framework.TestCase;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.RandomAccessFile;


/**
 * Test for TimeSeries simple instance creation from string.
 *
 * @author nsomogyi
 *
 */
public class TimeSeriesStringInstanceTest extends TestCase {
    private static final String NEW_INSTANCE_FILE = "samples/NewEmptyInstanceFile.xml";
    private static final String BM_FILE_IN = "samples/test.xml";
    private static final String BM_FILE_OUT = "samples/test_out.txt";

    /**
     * Prints the content of the specified container to the output.
     * @param con
     * @throws UdmException
     */
    protected void printContainer(Container con) throws UdmException {
        data_label[] dl_array = con.getdata_labelChildren();

        for (int i = 0; i < dl_array.length; ++i) {
            System.out.println(dl_array[i].getname());

            data_value[] rd_array = dl_array[i].getsrc();

            for (int j = 0; j < rd_array.length; ++j) {
                time_point[] tp_array = rd_array[j].getdst_dv();

                for (int k = 0; k < tp_array.length; ++k) {
                    System.out.println("\t(" + tp_array[k].getat() + ", " +
                        ((real_data) rd_array[j]).getvalue() + ")");
                }
            }
        }
    }

    /**
     * Fills the container with some dummy data.
     * @param con
     * @throws UdmException
     */
    protected void fillContainer(Container con) throws UdmException {
        data_label dl = data_label.create(con);
        dl.setname("label_1");

        time_point tp1 = time_point.create(con);
        tp1.setat(1.25 + 1 + 0.001);

        time_point tp2 = time_point.create(con);
        tp2.setat(1.26 + 2 + 0.001);

        time_point tp3 = time_point.create(con);
        tp3.setat(1.27 + 3 + 0.001);

        real_data dv1 = real_data.create(con);
        dv1.setvalue(0.8);
        dv1.setdst_dv(new time_point[] { tp1 });

        real_data dv2 = real_data.create(con);
        dv2.setvalue(0.8);
        dv2.setdst_dv(new time_point[] { tp2 });

        real_data dv3 = real_data.create(con);
        dv3.setvalue(0.8);
        dv3.setdst_dv(new time_point[] { tp3 });

        real_data[] rd_array = new real_data[] { dv1, dv2, dv3 };

        dl.setsrc(rd_array);
    }

    /**
     * Prints the content of the specified input stream to the output.
     * @param is
     * @throws UdmException
     * @throws IOException 
     */
    protected void printInputStream(InputStream is) throws UdmException, IOException {
        java.io.DataInputStream din = new java.io.DataInputStream(is);
        String xml = new String("");

        try {
            String line = null;

            while ((line = din.readLine()) != null) {
                xml += (line + "\n");
            }
        } finally {
            //  close stream
            try {
                din.close();
            } catch (Exception ex) {
            }
        }
    }

    /**
    * Creates a new data network in a string.
    * @throws UdmException
    */
    public void testCreateBlankInstance() throws UdmException {
        ContainerStringFactory gtf = FactoryRepository.getTimeSeriesContainerStringFactory();
        Container con = gtf.create();

        fillContainer(con);

        String a = gtf.save();
        System.out.println("\ntestCreateBlankInstance():\n" + a);
    }

    /**
     * Opens an existing data network from a string.
     * @throws UdmException
     */
    public void testOpenExistingFromString() throws UdmException, IOException {
        String xml = new String("");

            RandomAccessFile raf = new RandomAccessFile(BM_FILE_IN, "r");
            String line = null;

            while ((line = raf.readLine()) != null) {
                xml += (line + "\n");
            }

            raf.close();

        ContainerStringFactory gtf = FactoryRepository.getTimeSeriesContainerStringFactory();
        UdmPseudoObject rootUPO = gtf.open(xml);
        Container con = null;

        if (rootUPO instanceof Container) {
            con = (Container) rootUPO;
        } else {
            fail("Unexpected root type: " + rootUPO.getType());
        }

        System.out.println("\ntestOpenExistingFromString():\n");
        printContainer(con);

        gtf.close();
    }

    /**
     * Opens an existing data network from a string and saves as a stream.
     * @throws UdmException
     * @throws IOException 
     */
    public void testOpenExistingFromStringSaveToStream()
        throws UdmException, IOException {
        String xml = new String("");

            RandomAccessFile raf = new RandomAccessFile(BM_FILE_IN, "r");
            String line = null;

            while ((line = raf.readLine()) != null) {
                xml += (line + "\n");
            }

            raf.close();

        ContainerStringFactory gtf = FactoryRepository.getTimeSeriesContainerStringFactory();
        UdmPseudoObject rootUPO = gtf.open(xml);
        Container con = null;

        if (rootUPO instanceof Container) {
            con = (Container) rootUPO;
        } else {
            fail("Unexpected root type: " + rootUPO.getType());
        }

        InputStream a = gtf.saveAsStream();
        System.out.println("\ntestOpenExistingFromStringSaveToStream():\n" + a);
        printInputStream(a);
    }

    /**
     * Creates a new data network in a stream.
     * @throws UdmException
     */
    public void testCreateToStreamBlankInstance() throws UdmException {
        ContainerStringFactory gtf = FactoryRepository.getTimeSeriesContainerStringFactory();
        Container con = gtf.create();

        fillContainer(con);

        InputStream a = gtf.saveAsStream();
        System.out.println("\ntestCreateToStreamBlankInstance():\n" + a);
    }

    /**
     * Opens an existing data network from a stream.
     * @throws UdmException
     * @throws FileNotFoundException 
     */
    public void testOpenExistingFromStream() throws UdmException, FileNotFoundException {
        InputStream xml = null;

            xml = new FileInputStream(BM_FILE_IN);

        ContainerStringFactory gtf = FactoryRepository.getTimeSeriesContainerStringFactory();
        UdmPseudoObject rootUPO = gtf.open(xml);
        Container con = null;

        if (rootUPO instanceof Container) {
            con = (Container) rootUPO;
        } else {
            fail("Unexpected root type: " + rootUPO.getType());
        }

        System.out.println("\ntestOpenExistingFromStream():\n");
        printContainer(con);

        gtf.close();
    }

    /**
     * Opens an existing data network from a stream and saves in a string.
     * @throws UdmException
     * @throws FileNotFoundException 
     */
    public void testOpenExistingFromStreamSaveToString()
        throws UdmException, FileNotFoundException {
        InputStream xml = null;

            xml = new FileInputStream(BM_FILE_IN);

        ContainerStringFactory gtf = FactoryRepository.getTimeSeriesContainerStringFactory();
        UdmPseudoObject rootUPO = gtf.open(xml);
        Container con = null;

        if (rootUPO instanceof Container) {
            con = (Container) rootUPO;
        } else {
            fail("Unexpected root type: " + rootUPO.getType());
        }

        String a = gtf.save();
        System.out.println("\ntestOpenExistingFromStreamSaveToString():\n" + a);
    }
}
