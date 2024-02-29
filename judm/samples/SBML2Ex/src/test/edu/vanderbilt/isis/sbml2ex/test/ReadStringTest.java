/*
 * Created on Apr 8, 2005
 *
 * Java version 1.4.2_04
 */
package edu.vanderbilt.isis.sbml2ex.test;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.RandomAccessFile;
import junit.framework.TestCase;
import edu.vanderbilt.isis.sbml2ex.FactoryRepository;
import edu.vanderbilt.isis.sbml2ex.math.apply;
import edu.vanderbilt.isis.sbml2ex.math.ci;
import edu.vanderbilt.isis.sbml2ex.math.cn;
import edu.vanderbilt.isis.sbml2ex.math.lambda;
import edu.vanderbilt.isis.sbml2ex.math.math;
import edu.vanderbilt.isis.sbml2ex.sbml.annotation;
import edu.vanderbilt.isis.sbml2ex.sbml.compartment;
import edu.vanderbilt.isis.sbml2ex.sbml.delay;
import edu.vanderbilt.isis.sbml2ex.sbml.dialect;
import edu.vanderbilt.isis.sbml2ex.sbml.event;
import edu.vanderbilt.isis.sbml2ex.sbml.functionDefinition;
import edu.vanderbilt.isis.sbml2ex.sbml.listOfCompartments;
import edu.vanderbilt.isis.sbml2ex.sbml.listOfEvents;
import edu.vanderbilt.isis.sbml2ex.sbml.listOfFunctionDefinitions;
import edu.vanderbilt.isis.sbml2ex.sbml.listOfSpecies;
import edu.vanderbilt.isis.sbml2ex.sbml.model;
import edu.vanderbilt.isis.sbml2ex.sbml.notes;
import edu.vanderbilt.isis.sbml2ex.sbml.sbml;
import edu.vanderbilt.isis.sbml2ex.sbml.sbmlStringFactory;
import edu.vanderbilt.isis.sbml2ex.sbml.species;
import edu.vanderbilt.isis.sbml2ex.unigene.acc;
import edu.vanderbilt.isis.sbml2ex.unigene.cluster_id;
import edu.vanderbilt.isis.sbml2ex.xhtml.head;
import edu.vanderbilt.isis.udm.UdmException;


/**
 * Test for SBML2Ex simple instance creation from string.
 *
 * @author nsomogyi
 *
 */
public class ReadStringTest extends TestCase {
    private static final String TEST_FILE = "samples/string_test.log";
    
    private String tesLog = new String();

    /**
     * Fills the root object with some dummy data.
     * @param root
     * @throws UdmException
     */
    protected void fillSbml(sbml root) throws UdmException {
        // set the dialect
        annotation an = annotation.create(root);
        dialect di = dialect.create(an);
        di.setname("unigene");

        // create a model
        model mo = model.create(root);

        // create compartment
        listOfCompartments li = listOfCompartments.create(mo);
        compartment co = compartment.create(li);
        co.setid("compartment");

        // create some species with unigene annotation
        listOfSpecies lis = listOfSpecies.create(mo);
        species sp = species.create(lis);
        sp.setid("AA017353");
        sp.setcompartment("compartment");

        annotation ann = annotation.create(sp);
        acc _acc = acc.create(ann);
        _acc.setvalue("AA017353");

        edu.vanderbilt.isis.sbml2ex.unigene.title t =
            edu.vanderbilt.isis.sbml2ex.unigene.title.create(ann);
        t.setvalue("7-dehydrocholesterol reductase");

        cluster_id cid = cluster_id.create(ann);
        cid.setvalue("Hs.11806");

        edu.vanderbilt.isis.sbml2ex.unigene.gene g =
            edu.vanderbilt.isis.sbml2ex.unigene.gene.create(ann);
        g.setvalue("DHCR7");

        // create funtions with some math 
        listOfFunctionDefinitions lifd = listOfFunctionDefinitions.create(mo);
        functionDefinition fd = functionDefinition.create(lifd);
        math ma = math.create(fd);
        apply appl = apply.create(ma);
        cn _cn = cn.create(appl);
        _cn.settype("integer");
        _cn.setvalue("1");
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
        long time1 = System.currentTimeMillis();
        sbmlStringFactory gtf = FactoryRepository.getsbmlsbmlStringFactory();
        sbml root = gtf.create();

        System.out.println("\n------------\n");
        System.out.println("Create blank instance in string");

        fillSbml(root);
        printSBML(root);

        String result = gtf.checkConstraints();
        if (result.trim().compareTo("") == 0) {
            System.out.println("No constraint violation found.");
        } else {
            System.out.println("Constraint violation found:\n" + result);
        }

        String res = gtf.save();
        System.out.println(res);
        long time2 = (System.currentTimeMillis() - time1) / 1000;
        tesLog += "Create blank instance in string: " + time2 + "s\n";
    }

    /**
     * Opens an existing data network from a file.
     * @throws UdmException
     * @throws IOException 
     */
    public void testOpenExistingFromString() throws UdmException, IOException {
        System.out.println("\n-----------------------------------");
        System.out.println("\tOpenExistingFromString");
        System.out.println("-----------------------------------");

        /*readStringTest("samples/compOutRef.xml");
        readStringTest("samples/compUnitRef.xml");
        readStringTest("samples/delayLabelRef.xml");
        readStringTest("samples/eventUnitRef.xml");
        readStringTest("samples/eventVariableRef.xml");
        readStringTest("samples/functionLabelRef.xml");
        readStringTest("samples/invGlobalId.xml");
        readStringTest("samples/invParameterUnitRef.xml");
        readStringTest("samples/invReactParam.xml");
        readStringTest("samples/invReactRef.xml");
        readStringTest("samples/invSpCompartment.xml");
        readStringTest("samples/invSpeciesIC.xml");
        readStringTest("samples/invSpeciesUnitRef.xml");
        readStringTest("samples/invUnitDef.xml");
        readStringTest("samples/klawUnitRef.xml");
        readStringTest("samples/levelCheck.xml");
        readStringTest("samples/ruleLabelRef.xml");
        readStringTest("samples/ruleVariableRef.xml");
        readStringTest("samples/triggerLabelRef.xml");
        readStringTest("samples/unitDefs.xml");
        readStringTest("samples/uniqueRuleRef.xml");
        readStringTest("samples/versionCheck.xml");
        readStringTest("samples/l2v1-2D-compartments.xml");
        readStringTest("samples/l2v1-algebraic.xml");
        readStringTest("samples/l2v1-assignment.xml");
        readStringTest("samples/l2v1-boundary.xml");
        readStringTest("samples/l2v1-branch.xml");
        readStringTest("samples/l2v1-delay.xml");
        readStringTest("samples/l2v1-events.xml");
        readStringTest("samples/l2v1-functions.xml");
        readStringTest("samples/l2v1-mc-ode.xml");
        readStringTest("samples/l2v1-units.xml");*/

        //jigcell
        //readStringTest("samples/yeastnew2_core.xml");
        //readStringTest("samples/frogegg.xml");

        //bc
        //readStringTest("samples/Lac_v1.2.2.xml");

        // flux
        //readStringTest("samples/JR904.xml");

        //dbi, unigene 
        readStringTest("samples/demo_30_paintfb_out.xml");
 
        /*try {
            RandomAccessFile raf = new RandomAccessFile(TEST_FILE, "rw");
            raf.writeBytes(tesLog);
            raf.close();
        } catch (Exception ex) {
        }*/
    }

    /**
      * Opens an existing data network from a file.
      * @throws UdmException
     * @throws IOException 
      */
    public void readStringTest(String fileName) throws UdmException, IOException {
        long time1 = System.currentTimeMillis();
        sbmlStringFactory gtf = FactoryRepository.getsbmlsbmlStringFactory();

        StringBuffer xmlString = new StringBuffer();
            RandomAccessFile ra = new RandomAccessFile(fileName, "r");
            String line = "";
            while ((line = ra.readLine()) != null) {
                xmlString.append(line);
                xmlString.append("\n");
            }

        sbml root = gtf.open(xmlString.toString());

        System.out.println("\n------------\n");
        System.out.println(fileName);

        printSBML(root);

        String result = gtf.checkConstraints();
        if (result.trim().compareTo("") == 0) {
            System.out.println("No constraint violation found.");
        } else {
            System.out.println("Constraint violation found:\n" + result);
        }

        gtf.close();
        long time2 = (System.currentTimeMillis() - time1) / 1000;
        tesLog += "Read from string" + fileName + ": " + time2 + "s\n";
    }

    private void printSBML(sbml root) throws UdmException {
        // model child
        model m = root.getmodelChild();

        // note child
        notes n = m.getnotesChild();
        if (n != null) {
            head h = n.getheadChild();
            if (h != null) {
                edu.vanderbilt.isis.sbml2ex.xhtml.title t = h.gettitleChild();

                if (t != null) {
                    System.out.println("title " + t.getvalue());
                }
            }
        }

        // species
        listOfSpecies ls = m.getlistOfSpeciesChild();
        if (ls != null) {
            species[] sa = ls.getspeciesChildren();
            for (int i = 0; i < sa.length; i++) {
                System.out.print(sa[i].getid() + " ");
            }
            System.out.println();
        }

        // function defs
        listOfFunctionDefinitions fDefs = m.getlistOfFunctionDefinitionsChild();
        if (fDefs != null) {
            functionDefinition[] fDefA = fDefs.getfunctionDefinitionChildren();
            for (int i = 0; i < fDefA.length; i++) {
                functionDefinition fd = fDefA[i];
                System.out.println("functionDefinition " + fd.getid());

                math m_v = fd.getmathChild();

                apply[] appA = m_v.getapplyChildren();
                for (int k = 0; k < appA.length; k++) {
                    apply app = appA[k];

                    cn[] cnA = app.getcnChildren();
                    for (int l = 0; l < cnA.length; l++) {
                        cn cn_v = cnA[l];
                        System.out.println("cntype " + cn_v.gettype());
                        System.out.println("cnval " + cn_v.getvalue());
                    }

                    ci[] ciA = app.getciChildren();
                    for (int l = 0; l < ciA.length; l++) {
                        ci ci_v = ciA[l];
                        System.out.println("cival " + ci_v.getvalue());
                    }
                }

                lambda[] lmA = m_v.getlambdaChildren();
                for (int k = 0; k < lmA.length; k++) {
                    lambda lm_v = lmA[k];

                    apply[] appA2 = lm_v.getapplyChildren();
                    for (int l = 0; l < appA2.length; l++) {
                        apply app = appA2[l];

                        cn[] cnA = app.getcnChildren();
                        for (int x = 0; x < cnA.length; x++) {
                            cn cn_v = cnA[x];
                            System.out.println("cntype " + cn_v.gettype());
                            System.out.println("cnval " + cn_v.getvalue());
                        }

                        ci[] ciA = app.getciChildren();
                        for (int x = 0; x < ciA.length; x++) {
                            ci ci_v = ciA[x];
                            System.out.println("cival " + ci_v.getvalue());
                        }
                    }
                }
            }
        }

        // listOfEvents
        listOfEvents lEv = m.getlistOfEventsChild();
        if (lEv != null) {
            event[] evA = lEv.geteventChildren();
            for (int i = 0; i < evA.length; i++) {
                event e_v = evA[i];

                delay d_v = e_v.getdelayChild();
                if (d_v != null) {
                    math m_v = d_v.getmathChild();
                    if (m_v != null) {
                        apply[] appA = m_v.getapplyChildren();
                        for (int k = 0; k < appA.length; k++) {
                            apply app = appA[k];

                            cn[] cnA = app.getcnChildren();
                            for (int l = 0; l < cnA.length; l++) {
                                cn cn_v = cnA[l];
                                System.out.println("cntype " + cn_v.gettype());
                                System.out.println("cnval " + cn_v.getvalue());
                            }

                            ci[] ciA = app.getciChildren();
                            for (int l = 0; l < ciA.length; l++) {
                                ci ci_v = ciA[l];
                                System.out.println("cival " + ci_v.getvalue());
                            }
                        }

                        lambda[] lmA = m_v.getlambdaChildren();
                        for (int k = 0; k < lmA.length; k++) {
                            lambda lm_v = lmA[k];

                            apply[] appA2 = lm_v.getapplyChildren();
                            for (int l = 0; l < appA2.length; l++) {
                                apply app = appA2[l];

                                cn[] cnA = app.getcnChildren();
                                for (int x = 0; x < cnA.length; x++) {
                                    cn cn_v = cnA[x];
                                    System.out.println("cntype " + cn_v.gettype());
                                    System.out.println("cnval " + cn_v.getvalue());
                                }

                                ci[] ciA = app.getciChildren();
                                for (int x = 0; x < ciA.length; x++) {
                                    ci ci_v = ciA[x];
                                    System.out.println("cival " + ci_v.getvalue());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
