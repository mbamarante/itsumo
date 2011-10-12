/*
 * TestClass.java
 *
 * Created on October 17, 2002, 3:51 PM
 */

package frodo;

import frodo.COP.OCOPProblemFactory;
import frodo.COP.ValuedConstraint;
import frodo.COP.ValuedNogood;
import frodo.CSP.VariableId;
import frodo.OCOP.DFSNode;
import frodo.OCOP.OCOPBasicProblem;
import frodo.OCOP.DPOP.HyperCube;
import frodo.OCOP.ODPOP.GoodSource;
import frodo.OCOP.ODPOP.ValuedNogoodStorage;
import frodo.OCOP.graph.DFSMaster;
import frodo.interaction.meeting.graph.PseudoTreeViewer;

/**
 *
 * @author  apetcu
 */
public class TestClass {

	/** Creates a new instance of TestClass */
	public TestClass() {
	}

	/**
	 * @param args the command line arguments
	 */

	public static void main(String[] args) {

		ValuedConstraint valc = ValuedConstraint
				.fromString("0	0.0@$@1.0	0,0:-10000	0,1:0	0,2:1	0,3:2	0,4:3	0,5:4	0,6:5	0,7:6	1,0:7	1,1:-10000	1,2:0	1,3:0	1,4:0	1,5:0	1,6:0	1,7:0	2,0:0	2,1:0	2,2:-10000	2,3:0	2,4:0	2,5:0	2,6:0	2,7:0	3,0:0	3,1:0	3,2:0	3,3:-10000	3,4:0	3,5:0	3,6:0	3,7:0	4,0:0	4,1:0	4,2:0	4,3:0	4,4:-10000	4,5:4	4,6:0	4,7:0	5,0:0	5,1:0	5,2:0	5,3:0	5,4:0	5,5:-10000	5,6:3	5,7:0	6,0:0	6,1:0	6,2:0	6,3:0	6,4:0	6,5:0	6,6:-10000	6,7:0	7,0:0	7,1:0	7,2:0	7,3:0	7,4:0	7,5:0	7,6:0	7,7:-10000");
		HyperCube hc = new HyperCube(valc);
		System.out.println("HC=\n" + hc.toStringVectors());

		System.out.println("From HC: \n\n");
		GoodSource vngs = ValuedNogoodStorage.fromHyperCube(hc);
		System.out.println(vngs.toString());

		System.out.println("From relation: \n\n");
		VariableId varid = new VariableId(0, 0);
		GoodSource vngs1 = ValuedNogoodStorage.fromRelation(valc, varid);
		System.out.println(vngs1.toString());
		boolean done = false;
		while (!done) {
			ValuedNogood nog = vngs1.more();
			System.out.println("... " + nog);
			done = nog == null;
		}
		/*

		 //		ValuedConstraint valc = ValuedConstraint.fromString("0	1<->0	0,0:-10000	0,1:0	0,2:1	0,3:2	0,4:3	0,5:4	0,6:5	0,7:6	1,0:7	1,1:-10000	1,2:0	1,3:0	1,4:0	1,5:0	1,6:0	1,7:0	2,0:0	2,1:0	2,2:-10000	2,3:0	2,4:0	2,5:0	2,6:0	2,7:0	3,0:0	3,1:0	3,2:0	3,3:-10000	3,4:0	3,5:0	3,6:0	3,7:0	4,0:0	4,1:0	4,2:0	4,3:0	4,4:-10000	4,5:0	4,6:0	4,7:0	5,0:0	5,1:0	5,2:0	5,3:0	5,4:0	5,5:-10000	5,6:0	5,7:0	6,0:0	6,1:0	6,2:0	6,3:0	6,4:0	6,5:0	6,6:-10000	6,7:0	7,0:0	7,1:0	7,2:0	7,3:0	7,4:0	7,5:0	7,6:0	7,7:-10000");
		 //		System.out.println("VC=" + valc.toString());
		 //		valc = ValuedConstraint.fromString("0	2.0<->1.0	0:0:0	0:1:-10000	0:2:-10000	0:3:-10000	0:4:-10000	0:5:-10000	0:6:-10000	0:7:-10000	1:0:-10000	1:1:0	1:2:-10000	1:3:-10000	1:4:-10000	1:5:-10000	1:6:-10000	1:7:-10000	2:0:-10000	2:1:-10000	2:2:0	2:3:-10000	2:4:-10000	2:5:-10000	2:6:-10000	2:7:-10000	3:0:-10000	3:1:-10000	3:2:-10000	3:3:0	3:4:-10000	3:5:-10000	3:6:-10000	3:7:-10000	4:0:-10000	4:1:-10000	4:2:-10000	4:3:-10000	4:4:0	4:5:-10000	4:6:-10000	4:7:-10000	5:0:-10000	5:1:-10000	5:2:-10000	5:3:-10000	5:4:-10000	5:5:0	5:6:-10000	5:7:-10000	6:0:-10000	6:1:-10000	6:2:-10000	6:3:-10000	6:4:-10000	6:5:-10000	6:6:0	6:7:-10000	7:0:-10000	7:1:-10000	7:2:-10000	7:3:-10000	7:4:-10000	7:5:-10000	7:6:-10000	7:7:0");
		 //		System.out.println("VC=" + valc.toString());
		 HyperCube cube = HyperCube
		 .fromString("8:7:8	5:7:8	7:7:8|-10 -10 -10 -10 -10 -10 -10 -10 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 -10 -10 -10 -10 -10 -10 -10 -10 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 -10 -10 -10 -10 -10 -10 -10 -10 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 -10 -10 -10 -10 -10 -10 -10 -10 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 -10 -10 -10 -10 -10 -10 -10 -10 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 -10 -10 -10 -10 -10 -10 -10 -10 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 -10 -10 -10 -10 -10 -10 -10 -10 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 -10 -10 -10 -10 -10 -10 -10 -10 ");
		 System.out.println("CUBE=\n" + cube.toStringVectors());
		 HyperCube cube2 = HyperCube
		 .fromString("8:0:8	7:0:8|0 -10 -10 -10 -10 -10 -10 -10 -10 0 -10 -10 -10 -10 -10 -10 -10 -10 0 -10 -10 -10 -10 -10 -10 -10 -10 0 -10 -10 -10 -10 -10 -10 -10 -10 0 -10 -10 -10 -10 -10 -10 -10 -10 0 -10 -10 -10 -10 -10 -10 -10 -10 0 -10 -10 -10 -10 -10 -10 -10 -10 0");
		 System.out.println("CUBE=\n" + cube2.toStringVectors());

		 HyperCube join1 = cube2.join(cube);
		 //HyperCube join2 = cube.join(cube2);
		 System.out.println("JOIN1=\n" + join1.toStringVectors());
		 //System.out.println("JOIN2=\n" + join2.toStringVectors());

		 //        TestClass tcl = new TestClass();
		 Dimension d1 = new Dimension(1, 8);
		 Dimension d2 = new Dimension(0, 8);
		 Dimension d3 = Dimension.fromString("1:8");

		 //        Dimension[] dims = { d1, d2, d3 };
		 Dimension[] dims1 = { d2};
		 d2.setCurrentIndex(7);
		 Dimension[] dims2 = { d3 };
		 HyperCube tstC = cube.section(dims1);
		 System.out.println("CUBE=" + tstC.toString());

		 System.out.println(HelperUtilities.arrayToString(Dimension.setMinusDimensions(dims1, dims2)));
		 /*
		 HyperCube hc1 = new HyperCube(dims1);
		 for (int i = 0; i < 3; i++) {
		 d1.setCurrentIndex(i);
		 d2.setCurrentIndex(i);
		 System.out.print(Dimension.translateHyperToLinear(dims1) + "\t");
		 hc1.setValue(dims1, i + 1);
		 }
		 System.out.println();
		 System.out.println(hc1.toString());

		 HyperCube hc2 = new HyperCube(dims2);
		 for (int i = 0; i < 3; i++) {
		 d2.setCurrentIndex(i);
		 d3.setCurrentIndex(i);
		 System.out.print(Dimension.translateHyperToLinear(dims2) + "\t");
		 hc2.setValue(dims2, i + 2);
		 }
		 System.out.println();
		 System.out.println(hc2.toString());

		 System.out.println("JOIN:");
		 HyperCube hc3 = hc1.join(hc2);
		 System.out.println(hc3.toString());
		 System.out.println("PROJ 2 and then 1 out:");
		 Dimension[] dims3 = { d2 };
		 HyperCube hc4 = hc3.project(dims3);
		 System.out.println(hc4.toString());
		 dims3[0] = d1;
		 hc4 = hc4.project(dims3);
		 System.out.println(hc4.toString());
		 

		 System.out.println("PROJ 1 and then 2 out:");
		 dims3[0] = d1;
		 hc4 = hc3.project(dims3);
		 System.out.println(hc4.toString());
		 dims3[0] = d2;
		 hc4 = hc4.project(dims3);
		 System.out.println(hc4.toString());
		 
		 
		 System.out.println("PROJ 2 and then 0 with bounds out:");
		 dims3[0] = d2;
		 hc4 = hc3.project(dims3);
		 System.out.println(hc4.toString());
		 dims3[0] = d1;
		 hc4 = hc4.project_bounds(dims3);
		 System.out.println(hc4.toString());
		 
		 
		 
		 
		 
		 System.out.println("PROJ 0 out with bounds:");
		 dims3[0] = d1;
		 //		HyperCube hc5 = hc4.project_bounds(dims3);
		 //		System.out.println(hc5.toString());
		 */
		DFSNode dummyNode = new DFSNode(null, 0, null);

		OCOPBasicProblem myProblem = (OCOPBasicProblem) OCOPProblemFactory.loadFromTextFile(args[0], dummyNode);
		int howMany = myProblem.getNumberOfNodes();
		int constrNo = myProblem.getNumberOfConstraints();
		System.out.println("ODPOP starting on a problem with " + howMany + " variables and " + constrNo + " constraints;");
DFSMaster.getMinimalWidthDFS_MCN(myProblem);
		PseudoTreeViewer ptv = new PseudoTreeViewer(myProblem);

	}
}