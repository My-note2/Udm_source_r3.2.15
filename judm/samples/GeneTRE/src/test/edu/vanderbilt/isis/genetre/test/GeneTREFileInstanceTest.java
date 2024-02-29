/*
 * Created on Apr 8, 2005
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.genetre.test;

import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.Vector;
import junit.framework.TestCase;
import edu.vanderbilt.isis.genetre.FactoryRepository;
import edu.vanderbilt.isis.genetre.genetre.Container;
import edu.vanderbilt.isis.genetre.genetre.Gene;
import edu.vanderbilt.isis.genetre.genetre.ContainerFileFactory;
import edu.vanderbilt.isis.genetre.genetre.PWM;
import edu.vanderbilt.isis.genetre.genetre.TF;
import edu.vanderbilt.isis.genetre.genetre.TRE;
import edu.vanderbilt.isis.udm.UdmException;


/**
 * Test for genetre simple instance creation from file.
 *
 * @author nsomogyi
 *
 */
public class GeneTREFileInstanceTest extends TestCase {
    private static final String NEW_INSTANCE_FILE = "samples/NewEmptyInstanceFile.xml";
    private static final String BM_FILE_IN = "samples/test2.xml";
    private static final String BM_FILE_OUT = "samples/test2_out.txt";
    static int numGenes;
    static int numTFs;
    static int numUnknown;
    static Gene[] genes;
    static Vector tfs;
    static Vector unknowns;
    static PWM[] pwms;
    static TRE[] tres;
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
            Gene gene = genes[i];

            System.out.print("Gene: " + gene.getname() + "\n");

            TRE[] tres = gene.getsite();

            for (int ittre = 0; ittre < tres.length; ++ittre) {
                TRE tre = tres[ittre];

                System.out.print("\tTRE coreSocre: " + tre.getcoreScore() + "\n");

                PWM pwm = tre.getsite();
                String pwmn = pwm.getname();
                System.out.print("\tPWM: " + pwmn + "\n");

                TF[] tfs = pwm.gettf();

                for (int ittf = 0; ittf < tfs.length; ++ittf) {
                    TF tf = tfs[ittf];
                    String tfn = tf.getname();
                    System.out.print("\t\tTF : " + tfn + "\n");
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
        /*long tn = 5;
        long gnc = 0;
        for (int i = 0; i < tn; ++i) {
            gnc++;

            String gn = new String("gene" + gnc);
            String tfn = new String("tf" + gnc);

            Gene gene = Gene.create(con);
            gene.setname(gn);

            TF tf = TF.create(con);
            tf.setname(tfn);

            PWM pwm = PWM.create(con);

            TRE tre = TRE.create(con);
            tre.setsite(pwm);
            tre.setgene(gene);

            PWM[] pwma = new PWM[1];
            pwma[0] = pwm;
            pwm.setname("pwm" + gnc);
            tf.setbindsTo(pwma);
        }

        PWM pwm = PWM.create(con);
        pwm.setname("pwm");

        PWM[] pwma = new PWM[1];
        pwma[0] = pwm;

        for (int j = 0; j < tn; ++j) {
            gnc++;

            String gn = new String("gene" + gnc);
            String tfn = new String("tf" + gnc);

            Gene gene = Gene.create(con);
            gene.setname(gn);

            TF tf = TF.create(con);
            tf.setname(tfn);

            TRE tre = TRE.create(con);
            tre.setsite(pwm);
            tre.setgene(gene);

            tf.setbindsTo(pwma);
        }*/

        Gene gene = Gene.create(con);
        gene.setname("jojo");

        TF tf = TF.create(con);
        tf.setname("toto");

        PWM p = PWM.create(con);

        TRE tre = TRE.create(con);
        tre.setsite(p);
        tre.setgene(gene);
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
     * Creates a new data network in a file.
     * @throws UdmException
     */
    public void testCreateBlankInstance() throws UdmException {
        ContainerFileFactory gtf = FactoryRepository.getgenetreContainerFileFactory();
        Container con = gtf.create(NEW_INSTANCE_FILE);

        fillContainer(con);

        System.out.println("\ntestCreateBlankInstance():");
        printContainer(con);

        gtf.save();
    }

    /**
     * Opens an existing data network from a file.
     * @throws UdmException
     */
    public void testOpenExistingFromFile() throws UdmException {
    	testCreateBlankInstance();
        ContainerFileFactory gtf = FactoryRepository.getgenetreContainerFileFactory();
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
        ContainerFileFactory gtf = FactoryRepository.getgenetreContainerFileFactory();
        Container con = gtf.open(NEW_INSTANCE_FILE);

        InputStream a = gtf.saveAsStream();
        System.out.println("\ntestOpenExistingFromFileSaveToStream():\n" + a);
        printInputStream(a);
    }

    /**
     * Opens an existing data network from a stream and saves in a file.
     * @throws UdmException
     * @throws FileNotFoundException 
     */
    public void testOpenExistingFromStreamSaveToFile()
        throws UdmException, FileNotFoundException {
        InputStream xml = null;
            xml = new FileInputStream(BM_FILE_IN);

        if (xml == null) {
            System.out.println(
                "testOpenExistingFromStreamSaveToFile faild: the inputstream is null");
            return;
        }

        ContainerFileFactory gtf = FactoryRepository.getgenetreContainerFileFactory();
        Container con = gtf.open(xml);

        gtf.saveAs(BM_FILE_OUT);
        System.out.println("\ntestOpenExistingFromStreamSaveToFile():\n\t" + BM_FILE_OUT);
    }
}
