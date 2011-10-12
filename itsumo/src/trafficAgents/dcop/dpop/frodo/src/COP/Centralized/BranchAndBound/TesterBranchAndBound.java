/*
 * Created on Feb 13, 2006 by apetcu
 * Project Public
 * Package frodo.COP.Centralized.BranchAndBound
 */
package frodo.COP.Centralized.BranchAndBound;

import frodo.COP.OCOPProblemFactory;
import frodo.COP.Solution;
import frodo.OCOP.OCOPAbstractProblem;
import frodo.OCOP.DPOP.DPOPNode;
import frodo.OCOP.DPOP.graphics.OCOPViewer;
import frodo.OCOP.graph.DFSMaster;

/**
 * @author apetcu
 *This is just a placeholder; no implementation yet
 */
public class TesterBranchAndBound {

	public TesterBranchAndBound() {
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		if (args.length < 1) {
			System.out.println("Usage: TesterBB problem_file [result_file] [on_screen]");
			System.out.println("\tproblem_file = an input text file with the description of the problem, in DPOP format.");
			System.out.println("\tresult_file = an output text file with the solution in DPOP format, plus valuation and time to solution.");
			System.out.println("\ton_screen = 0(no visual) / 1 (visual)");
			System.exit(0);
		}
		String problem_file = args[0];
		String result_file = null;
		if (args.length > 1)
			result_file = args[1];
		else
			result_file = problem_file + "." + BranchAndBound.algo_name;

		int onscreen = 0;
		if (args.length > 2)
			onscreen = Integer.parseInt(args[2]);

		//build a OCOPProblem object from the input text file
		DPOPNode dummyNode = new DPOPNode(null, 0, null);
		OCOPAbstractProblem myProblem = OCOPProblemFactory.loadFromTextFile(problem_file, dummyNode);

		int howMany = myProblem.getNumberOfNodes();
		int constrNo = myProblem.getNumberOfConstraints();
		System.out.println("B&B starting with " + howMany + " variables and " + constrNo + " constraints.");

		//look at the problem: build a DFS, and determine width/depth 
		DFSMaster.getMinimalWidthDFS_MCN(myProblem);
		System.out.println("DFS stats:\n" + DFSMaster.getStatisticsOfDFS(myProblem));

		if (onscreen > 0) {
			System.out.println("ON_SCREEN=" + onscreen + " constructing video.");
			new OCOPViewer(myProblem);
		}
		Solution solution = BranchAndBound.solve(myProblem);
		System.out.println(solution.toString());
		Solution.writeResultToFile(solution, result_file);
	}
}
