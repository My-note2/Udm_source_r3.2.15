/*
 * Created on Apr 7, 2005
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.genetf.test;

import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.RandomAccessFile;
import java.util.Vector;

import junit.framework.TestCase;
import edu.vanderbilt.isis.genetf.FactoryRepository;
import edu.vanderbilt.isis.genetf.genetf.Container;
import edu.vanderbilt.isis.genetf.genetf.ContainerStringFactory;
import edu.vanderbilt.isis.genetf.genetf.Gene;
import edu.vanderbilt.isis.genetf.genetf.Regulation;
import edu.vanderbilt.isis.genetf.genetf.TF;
import edu.vanderbilt.isis.udm.UdmException;
import edu.vanderbilt.isis.udm.UdmPseudoObject;


/**
 * Test for GeneTF simple instance creation from string.
 *
 * @author nsomogyi
 *
 */
public class GeneTFStringInstanceTest extends TestCase {
    private static final String NEW_INSTANCE_FILE = "samples/NewEmptyInstanceFile.xml";
    private static final String BM_FILE_IN = "samples/test2.xml";
    private static final String BM_FILE_OUT = "samples/test2_out.txt";
    static int numGenes;
    static int numTFs;
    static int numUnknown;
    static Gene[] genes;
    static Vector tfs;
    static Vector unknowns;
    static Regulation[] regs;
    static String outFileName;
    static BufferedWriter bw;

    /**
     * Prints the content of the specified container to the output.
     * @param con
     * @throws UdmException
     */
    protected void printContainer(Container con) throws UdmException {
        Gene[] genes = con.getGeneChildren();
        for (int i = 0; i < genes.length; i++) {
            Gene g = genes[i];
            String n = g.getname();
            System.out.print("Gene " + n + " regulates: ");

            Regulation[] regs = g.gettf();
            for (int j = 0; j < regs.length; j++) {
                try {
                    if (j > 0) {
                        System.out.print(", ");
                    }

                    Regulation r = regs[j];
                    TF t = r.gettf();
                    String tn = t.getname();
                    System.out.print(tn);
                } catch (UdmException e) {
                    System.out.print("Gene " + n + "fails ");
                    throw e;
                }
            }

            System.out.println();
        }
    }

    /**
     * Fills the container with some dummy data.
     * @param con
     * @throws UdmException
     */
    protected void fillContainer(Container con) throws UdmException {
        Gene g1 = Gene.create(con);
        g1.setname("jojo");

        TF t1 = TF.create(con);
        t1.setname("toto");

        Regulation r1 = Regulation.create(con);
        r1.setgene(g1);
        r1.settf(t1);
        r1.settype(2);
    }

    /**
     * Prints the content of the specified input stream to the output.
     * @param is
     * @throws UdmException
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
        ContainerStringFactory gtf = FactoryRepository.getgenetfContainerStringFactory();
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

        ContainerStringFactory gtf = FactoryRepository.getgenetfContainerStringFactory();
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
     */
    public void testOpenExistingFromStringSaveToStream() throws IOException
        , UdmException {
        String xml = new String("");
            RandomAccessFile raf = new RandomAccessFile(BM_FILE_IN, "r");
            String line = null;

            while ((line = raf.readLine()) != null) {
                xml += (line + "\n");
            }
            raf.close();

        ContainerStringFactory gtf = FactoryRepository.getgenetfContainerStringFactory();
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
        ContainerStringFactory gtf = FactoryRepository.getgenetfContainerStringFactory();
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

        ContainerStringFactory gtf = FactoryRepository.getgenetfContainerStringFactory();
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

        ContainerStringFactory gtf = FactoryRepository.getgenetfContainerStringFactory();
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
