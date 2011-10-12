/*
 * Created on Jul 12, 2005
 */
package frodo.interaction.meeting.graph;

import frodo.OCOP.OCOPAbstractProblem;
import frodo.OCOP.graph.DFSMaster;
import frodo.OCOP.graph.DFSStatistics;
import frodo.OCOP.graph.GeorgesHeuristics;

/**
 * This is a convenience class holding all of the heuristic options currently used to solve the problem.
 * 
 * @author ushakov
 *
 */
public class Heuristics {

	public static final int DFS = 0;

	public static final int DFS_MCN = 1;

	public static final int DFS_CLIQUE_MCN = 2;

	public static final int DFS_MCN_DSTB = 3;

	public static final int DFS_MCN_BEC = 4;

	public static final int CONNECT_TO_FIRST = 10;

	public static final int CHAIN = 11;

	public static int CURRENT_TREE_BUILD_METHOD = DFS_MCN;

	public static int CURRENT_EQUALITY_CONNECTION = CONNECT_TO_FIRST;

	public static int CURRENT_DFS_ROOT_ID = 0;

	/**
	 * @param ocop - the problem to deal with
	 * @return a statistics package corresponding to the DFS generated
	 */
	public static DFSStatistics buildPseudoTree(OCOPAbstractProblem ocop){
		//		int[] info = { -1, -1, -1 };
		//		DFSStatistics info = new DFSStatistics();
		// Do not build the tree if there are 0 or 1 nodes.
		if (ocop.getNumberOfNodes() > 1) {
			DFSMaster.dfsCleanup(ocop);
			if (CURRENT_TREE_BUILD_METHOD == DFS) {
				DFSMaster.buildDFS(ocop, CURRENT_DFS_ROOT_ID);
				//				info[1] = CURRENT_DFS_ROOT_ID;
				//				info[2] = ((DFSNode) ocop.getNodeById(Heuristics.CURRENT_DFS_ROOT_ID)).getNumberOfNeighbours();
			} else if (CURRENT_TREE_BUILD_METHOD == DFS_MCN) {
				//				info = DFSMaster.getMinimalWidthDFS_MCN(ocop);
				DFSMaster.getMinimalWidthDFS_MCN(ocop);
			} else if (CURRENT_TREE_BUILD_METHOD == DFS_CLIQUE_MCN) {
				//				info = GeorgesHeuristics.getMinimumGraphWidthByConstraintType('#');
				GeorgesHeuristics.getMinimumGraphWidthByConstraintType(ocop, '#');
			} else if (CURRENT_TREE_BUILD_METHOD == DFS_MCN_DSTB) {
				//				info = GeorgesHeuristics.getMinimumGraphWidthDomainSizeTieBreak(ocop);
				GeorgesHeuristics.getMinimumGraphWidthDomainSizeTieBreak(ocop);
			} else if (CURRENT_TREE_BUILD_METHOD == DFS_MCN_BEC) {
				//				info = GeorgesHeuristics.getMinimumGraphWidthBackEdgeConnectivity(ocop);
				GeorgesHeuristics.getMinimumGraphWidthBackEdgeConnectivity(ocop);
			}
		}
		return DFSMaster.getStatisticsOfDFS(ocop);
	}

	public static String getCurrentHeuristic(){
		String heuristic;
		if (CURRENT_TREE_BUILD_METHOD == DFS) {
			heuristic = "DFS";
		} else if (CURRENT_TREE_BUILD_METHOD == DFS_MCN) {
			heuristic = "DFS MCN";
		} else if (CURRENT_TREE_BUILD_METHOD == DFS_CLIQUE_MCN) {
			heuristic = "DFS CLIQUE MCN";
		} else if (CURRENT_TREE_BUILD_METHOD == DFS_MCN_DSTB) {
			heuristic = "DFS MCN DSTB";
		} else if (CURRENT_TREE_BUILD_METHOD == DFS_MCN_BEC) {
			heuristic = "DFS MCN BEC";
		} else {
			heuristic = "not defined";
		}
		return heuristic;
	}

}
