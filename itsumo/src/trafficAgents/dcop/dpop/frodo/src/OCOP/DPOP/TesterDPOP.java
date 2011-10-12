package frodo.OCOP.DPOP;

import frodo.COP.OCOPProblemFactory;
import frodo.OCOP.OCOPAbstractProblem;
import frodo.OCOP.DPOP.graphics.OCOPViewer;
import frodo.OCOP.communication.Environment;
import frodo.OCOP.graph.DFSMaster;
import frodo.OCOP.graph.DFSStatistics;

public class TesterDPOP {
	public final static String algo_name = "DPOP";
	public final static boolean STORES_MESSAGES = true;

	public TesterDPOP() {
	}

	public static void main(String[] args) {
		if (args.length < 1) {
			System.out.println("Usage: TesterDPOP problem_file [result_file] [on_screen]");
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
			result_file = problem_file + "." + algo_name;

		int onscreen = 0;
		if (args.length > 2)
			onscreen = Integer.parseInt(args[2]);

		Environment env = new Environment(problem_file, 0, 0);
		env.setResultFile(result_file);

		//build a OCOPProblem object from the input text file
		DPOPNode dummyNode = new DPOPNode(null, 0, null);
		OCOPAbstractProblem myProblem = OCOPProblemFactory.loadFromTextFile(problem_file, dummyNode);
		env.setProblem(myProblem);
		env.setStoreMessages(STORES_MESSAGES);

		int howMany = env.getProblem().getNumberOfNodes();
		int constrNo = env.getProblem().getNumberOfConstraints();
		System.out.println("DPOP starting with " + howMany + " variables and " + constrNo + " constraints.");

		//look at the problem: build a DFS, and determine width/depth 
		DFSMaster.getMinimalWidthDFS_MCN(myProblem);
		DFSStatistics statistix  = DFSMaster.getStatisticsOfDFS(myProblem);
		System.out.println("DFS stats:\n" + statistix);
//		System.out.println("Problem has width " + wi[0]);
//		System.out.println("Problem has height " + DFSMaster.getCurrentDFSHeight(myProblem));

		env.cycles = ((int)statistix.getDepth()) * 2;

		if (onscreen > 0) {
			System.out.println("ON_SCREEN=" + onscreen + " constructing video.");
			new OCOPViewer((OCOPAbstractProblem) env.getProblem());
		}

		//time to construct a decentralized problem object
		//		OCOPBasicProblem decentralizedProblem = OCOPProblemFactory.decentralize(myProblem, null);
		//		env.setProblem(decentralizedProblem);

		//constructing an agent for each node of the problem
		for (int i = 0; i < howMany; i++) {
			DPOPAgent agent = new DPOPAgent(env, i);
			env.addAgent(agent);
		}

		//this sets as the root of the DFS tree the node with the maximum  no of connections
		env.setRootAgent(statistix.getRoot());
		env.getLogger().println("Starting with root=" + statistix.getRoot());

		//we destroy the DFS structure created previously (in a centralized fashion)
		DFSMaster.dfsCleanup(env.getProblem());
		//		new OCOPViewer(myProblem);
		//we start the solving process. As a first step, a new DFS is created, in a real distributed fashion (message passing)
		env.start();
		//main thread exits, and only the real agents are still working.
		System.out.println("MAIN FINISHED");
		//		System.out.println("DFS stats:\n" + DFSMaster.getStatisticsOfDFS(myProblem));
		//		System.out.println("Max UTIL: " + DFSMaster.maxSize);
		//		new OCOPViewer(myProblem);
	}
}