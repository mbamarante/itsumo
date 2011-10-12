package frodo.OCOP.graph;

import java.util.Vector;

import frodo.COP.OCOPProblemFactory;
import frodo.OCOP.DFSNode;
import frodo.OCOP.OCOPAbstractNode;
import frodo.OCOP.OCOPBasicProblem;
import frodo.utils.FileUtilities;

public class TesterHeuristics {
	private static final int TESTS = 100;

	private static final int ALGOS = 6;

	//	public final static String algo_name = "heu";

	public TesterHeuristics() {
	}

	public static void main(String[] args) {
		if (args.length < 1) {
			System.out.println("Usage: TesterHeuristics problem_file [result_file]");
			System.out.println("If not specified, result_file defaults to problem_file.rez");
			System.exit(0);
		}
		String problem_file = args[0];
		//		String result_file = null;
		//		if (args.length > 1)
		//			result_file = args[1];
		//		else
		//			result_file = problem_file + "." + algo_name;

		//build an OCOPProblem object from the input text file
		DFSNode dummyNode = new DFSNode(null, 0, null);
		OCOPBasicProblem myProblem = (OCOPBasicProblem) OCOPProblemFactory.loadFromTextFile(problem_file, dummyNode);
		int howMany = myProblem.getNumberOfNodes();
		int constrNo = myProblem.getNumberOfConstraints();
		System.out.println("ODPOP starting on a problem with " + howMany + " variables and " + constrNo + " constraints;");

		DFSStatistics[][] statistix = new DFSStatistics[ALGOS][TESTS];
		String names[] = new String[] { "stats_plain", "stats_random", "stats_rndroot_mcn", "stats_mcn", "stats_mcn_bec", "minfill" };
		double[] averages = new double[ALGOS];
		double[] mins = new double[ALGOS];
		double[] maxs = new double[ALGOS];

		//		PseudoTreeViewer ptv = new PseudoTreeViewer(myProblem);
		//		DFSStatistics[] stats_plain = new DFSStatistics[TESTS];
		//		DFSStatistics[] stats_random = new DFSStatistics[TESTS];
		//		DFSStatistics[] stats_rndroot_mcn = new DFSStatistics[TESTS];
		//		DFSStatistics[] stats_mcn = new DFSStatistics[TESTS];
		//		DFSStatistics[] stats_mcn_bec = new DFSStatistics[TESTS];

		//		double avg_plain = 0;
		//		double avg_random = 0;
		//		double avg_rndroot_mcn = 0;
		//		double avg_mcn = 0;
		//		double avg_mcn_bec = 0;

		//this is just to clear any previous content in those files
		for (int j = 0; j < ALGOS; j++) {
			FileUtilities.writeStringToFile("", problem_file + "." + names[j]);
			mins[j] = Integer.MAX_VALUE;
			maxs[j] = Integer.MIN_VALUE;
		}

		//starting tests
		for (int i = 0; i < TESTS; i++) {
			//look at the problem: build a DFS, and determine width/depth
			OCOPAbstractNode randomRoot = DFSMaster.pickRandomNode(myProblem);

			//random root, problem ordering
			DFSMaster.buildDFS(myProblem, randomRoot.getId());
			statistix[0][i] = DFSMaster.getStatisticsOfDFS(myProblem);
			//			System.out.println("PROBLEM ORDERING:\n" + stats_plain);

			//random root, random node ordering
			DFSMaster.randomizeNodeOrder(myProblem);
			DFSMaster.buildDFS(myProblem, randomRoot.getId());
			statistix[1][i] = DFSMaster.getStatisticsOfDFS(myProblem);
			//			System.out.println("RANDOM ORDERING :\n" + stats_random);

			//random root, MCN node ordering
			DFSMaster.buildDFS_MCN(myProblem, randomRoot.getId());
			statistix[2][i] = DFSMaster.getStatisticsOfDFS(myProblem);
			//			System.out.println("MCN ORDERING    :\n" + stats_rndroot_mcn);

			//MCN root, MCN node ordering
			DFSMaster.getMinimalWidthDFS_MCN(myProblem);
			statistix[3][i] = DFSMaster.getStatisticsOfDFS(myProblem);

			//MCN root, BEC_MCN node ordering
			GeorgesHeuristics.getMinimumGraphWidthBackEdgeConnectivity(myProblem);
			statistix[4][i] = DFSMaster.getStatisticsOfDFS(myProblem);

			Vector minFillOrdering = MinfillHeuristic.bestOrdering(myProblem);
			//this is a dirty hack, since the minfill does not do a DFS
			statistix[5][i] = new DFSStatistics(0, -1, 0, 0, 0, 0, 0);

			for (int j = 0; j < ALGOS; j++) {
				averages[j] += statistix[j][i].getWidth();//add to averages
				FileUtilities.appendStringToFile(i + "\t" + statistix[j][i].toStringPlain() + "\n", problem_file + "." + names[j]);//write results of this test to the corresponding file
				if (mins[j] > statistix[j][i].getWidth())
					mins[j] = statistix[j][i].getWidth();
				if (maxs[j] < statistix[j][i].getWidth())
					maxs[j] = statistix[j][i].getWidth();
			}
		}
		for (int j = 0; j < ALGOS; j++) {
			averages[j] = averages[j] / TESTS;//compute averages
			System.out.println("" + averages[j] + "\t" + mins[j] + "\t" + maxs[j]);
		}
	}
}