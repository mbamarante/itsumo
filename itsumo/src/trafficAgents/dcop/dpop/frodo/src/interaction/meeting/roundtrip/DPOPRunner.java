/*
 * Created on 5 juil. 2005
 *
 */
package frodo.interaction.meeting.roundtrip;

import frodo.OCOP.OCOPAbstractProblem;
import frodo.OCOP.DPOP.DPOPAgent;
import frodo.OCOP.communication.Environment;
import frodo.OCOP.graph.DFSMaster;
import frodo.OCOP.graph.DFSStatistics;
import frodo.interaction.meeting.graph.Heuristics;
import frodo.interaction.meeting.graph.PseudoTreeEditor;
import frodo.monitoring.Monitor;
import frodo.monitoring.interfaces.MonitorListener;

/**
 * @author George
 *
 */
public class DPOPRunner {

	private static Monitor monitor = null;

	private static DFSStatistics info = null;

	public static void runDPOP(OCOPAbstractProblem ocop, MonitorListener viewer) {
		if (ocop != null) {
			String result_file = "output/out.txt";
			Environment env = new Environment(null, 0, 0);
			env.setResultFile(result_file);
			//			env.setCentralizedProblem(ocop);
			env.setProblem(ocop);
			int howMany = env.getProblem().getNumberOfNodes();
			int constrNo = env.getProblem().getNumberOfConstraints();
			System.out.println("DPOP starting with " + howMany + " agents and " + constrNo + " constraints; mode=" + Heuristics.getCurrentHeuristic());

			if (info == null) {
				// Buid the pseudo-tree based on the current heuristic used.
				System.out.println("**************************************");
				System.out.println("************Rebuilding****************");
				System.out.println("**************************************");
				info = Heuristics.buildPseudoTree(ocop);
			}

			DFSStatistics stats = DFSMaster.getStatisticsOfDFS(ocop);
			System.out.println("Problem statistics:\n" + info.toString());
			long problemHeight = stats.getDepth();
			System.out.println("Problem has height " + problemHeight);
			System.out.println(DFSMaster.getCurrentDFSTraversalString(ocop));

			DPOPAgent agent = null;

			//			OCOPProblem decentralizedProblem = OCOPProblem.decentralize(ocop);
			//			env.setProblem(decentralizedProblem);

			for (int i = 0; i < howMany; i++) {
				agent = new DPOPAgent(env, i);
				env.addAgent(agent);
			}
			env.setRootAgent(info.getRoot());//this sets as the root of the DFS tree the
			// node with the maximum no of connections
			env.getLogger().println("Starting with root=" + info.getRoot());

			DFSMaster.dfsCleanup(env.getProblem());

			//New monitor instance for every problem run.

			monitor = new Monitor();

			monitor.setProblemWidth(info.getWidth());
			monitor.setProblemHeight((int) problemHeight);

			if (viewer != null) {
				if (viewer instanceof PseudoTreeEditor) {
					// Flag the communication between two agents.
					monitor.flagCommunicationLink(((PseudoTreeEditor) viewer).getFlaggedLink());
					monitor.setCSPViewer(viewer);
				}
			}

			env.addMessageListener(monitor);
			monitor.start();

			env.start();
			System.out.println("MAIN FINISHED");
		}

	}

	public static DFSStatistics getInfo() {
		return info;
	}

	public static void setInfo(DFSStatistics stats) {
		DPOPRunner.info = stats;
	}

	/**
	 * Displays the statistics of the current problem cycle.
	 */
	public static void showMonitorWindow() {
		if (monitor != null) {
			monitor.getWindow().setUp();
		}
	}

	public static Monitor getMonitor() {
		return monitor;
	}
}