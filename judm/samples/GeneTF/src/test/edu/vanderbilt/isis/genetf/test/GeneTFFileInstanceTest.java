/*
 * Created on Apr 8, 2005
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.genetf.test;

import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Vector;

import junit.framework.TestCase;
import edu.vanderbilt.isis.genetf.FactoryRepository;
import edu.vanderbilt.isis.genetf.genetf.Container;
import edu.vanderbilt.isis.genetf.genetf.ContainerFileFactory;
import edu.vanderbilt.isis.genetf.genetf.Gene;
import edu.vanderbilt.isis.genetf.genetf.Regulation;
import edu.vanderbilt.isis.genetf.genetf.TF;
import edu.vanderbilt.isis.udm.UdmException;


/**
 * Test for GeneTF simple instance creation from file.
 *
 * @author nsomogyi
 *
 */
public class GeneTFFileInstanceTest extends TestCase {
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
        /*for (int i = 1; i < 10; ++i) {
            Gene g1 = Gene.create(con);
            g1.setname("gene_" + i);

            TF t1 = TF.create(con);
            t1.setname("tf_" + i);

            Regulation r1 = Regulation.create(con);
            r1.setgene(g1);
            r1.settf(t1);
            r1.settype(2);
        }*/
        Gene g1 = Gene.create(con);
        g1.setname("jojo");

        TF t1 = TF.create(con);
        t1.setname("toto");
        
        Regulation r1 = Regulation.create(con);
        r1.setgene(g1);
        r1.settf(t1);
        r1.settype(2);
        
        /*g1.setAssociation("tf_end", r1);
        t1.setAssociation("gene_end", r1);*/
        
        /*Regulation[] r_array = new Regulation[] {r1};
        
        g1.settf(r_array);
        t1.setgene(r_array);*/
        
        /*Regulation r1 = Regulation.create(con);
        r1.setgene(t1);
        r1.settf(t1);
        r1.settype(2);*/
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
     * Creates a new data network in a file.
     * @throws UdmException
     */
    public void testCreateBlankInstance() throws UdmException {
        ContainerFileFactory gtf = FactoryRepository.getgenetfContainerFileFactory();
        Container con = gtf.create(NEW_INSTANCE_FILE);

        fillContainer(con);

        System.out.println("\ntestCreateBlankInstance():");
        //printContainer(con);
        
        // call constraint checker
        //String res = gtf.checkConstraints();
        //System.out.println("\nResult of constraint evaluation: " + res + "\n");

        gtf.save();
    }

    /**
     * Opens an existing data network from a file.
     * @throws UdmException
     */
    public void testOpenExistingFromFile() throws UdmException {
    	testCreateBlankInstance();
        ContainerFileFactory gtf = FactoryRepository.getgenetfContainerFileFactory();
        Container con = gtf.open(NEW_INSTANCE_FILE);

        System.out.println("\ntestOpenExistingFromFile():\n");
        printContainer(con);

        gtf.close();
    }

    /**
     * Opens an existing data network from a file and saves in a stream.
     * @throws UdmException
     * @throws IOException 
     */
    public void testOpenExistingFromFileSaveToStream()
        throws UdmException, IOException {
    	testCreateBlankInstance();
        ContainerFileFactory gtf = FactoryRepository.getgenetfContainerFileFactory();
        Container con = gtf.open(NEW_INSTANCE_FILE);

        InputStream a = gtf.saveAsStream();
        System.out.println("\ntestOpenExistingFromFileSaveToStream():\n" + a);
        printInputStream(a);
    }

    /**
     * Opens an existing data network from a stream and saves in a file.
     * @throws UdmException
     */
    public void testOpenExistingFromStreamSaveToFile() throws IOException
        , UdmException {
        InputStream xml = null;
        xml = new FileInputStream(BM_FILE_IN);

        if (xml == null) {
            System.out.println(
                "testOpenExistingFromStreamSaveToFile faild: the inputstream is null");
            return;
        }

        ContainerFileFactory gtf = FactoryRepository.getgenetfContainerFileFactory();
        Container con = gtf.open(xml);

        gtf.saveAs(BM_FILE_OUT);
        System.out.println("\ntestOpenExistingFromStreamSaveToFile():\n\t" + BM_FILE_OUT);
    }
}
