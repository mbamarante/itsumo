/*
 * Created on Jan 26, 2006 by apetcu
 * Project Public
 * Package frodo.OCOP.graph
 */
package frodo.OCOP.graph;

import java.util.Arrays;
import java.util.Vector;

import frodo.OCOP.DFSNode;
import frodo.OCOP.OCOPAbstractNode;
import frodo.OCOP.OCOPAbstractProblem;

public class MCNHeuristic {

	//--------------------------------------------------------------------------------------------------//
	public static void sortNeighborsByConnectivity(OCOPAbstractProblem problem) {
		for (int i = 0; i < problem.getNumberOfNodes(); i++)
			sortNeighborsByConnectivity(problem.getNodeByIndex(i));
	}

	private static void sortNeighborsByConnectivity(OCOPAbstractNode node) {
		OCOPAbstractNode[] neighborsSorted = new OCOPAbstractNode[node.getNumberOfNeighbours()];
		//		 System.out.print("Neighbors: ");
		for (int k = 0; k < node.getNumberOfNeighbours(); k++) {
			neighborsSorted[k] = node.getNeighborByIndex(k);
			//			System.out.print("X" + neighborsSorted[k].getId() + " ");
		}
		//		System.out.println();
		for (int k = 0; k < node.getNumberOfNeighbours() - 1; k++) {
			for (int l = k + 1; l < node.getNumberOfNeighbours(); l++) {
				if (neighborsSorted[k].getNumberOfNeighbours() < neighborsSorted[l].getNumberOfNeighbours()) {
					OCOPAbstractNode tmpnode = neighborsSorted[k];
					neighborsSorted[k] = neighborsSorted[l];
					neighborsSorted[l] = tmpnode;
				}
			}
		}
		node.setNeighbours(new Vector(Arrays.asList(neighborsSorted)));
	}

	//--------------------------------------------------------------------------------------------------//
	// gushakov
	public static void sortNeighborsByConnectivityDomainSizeTieBreak(OCOPAbstractProblem problem) {
		for (int i = 0; i < problem.getNumberOfNodes(); i++)
			sortNeighborsByConnectivityDomainSizeTieBreak(problem.getNodeByIndex(i));
	}

	private static void sortNeighborsByConnectivityDomainSizeTieBreak(OCOPAbstractNode node) {
		OCOPAbstractNode[] neighborsSorted = new OCOPAbstractNode[node.getNumberOfNeighbours()];
		//		 System.out.print("Neighbors: ");
		for (int k = 0; k < node.getNumberOfNeighbours(); k++) {
			neighborsSorted[k] = node.getNeighborByIndex(k);
			//			System.out.print("X" + neighborsSorted[k].getId() + " ");
		}
		//		System.out.println();
		for (int k = 0; k < node.getNumberOfNeighbours() - 1; k++) {
			for (int l = k + 1; l < node.getNumberOfNeighbours(); l++) {
				if (neighborsSorted[k].getNumberOfNeighbours() < neighborsSorted[l].getNumberOfNeighbours()) {
					OCOPAbstractNode tmpnode = neighborsSorted[k];
					neighborsSorted[k] = neighborsSorted[l];
					neighborsSorted[l] = tmpnode;
				}
				// Add a tie break with the preference for nodes with variables of smaller domain
				else if (neighborsSorted[k].getNumberOfNeighbours() == neighborsSorted[l].getNumberOfNeighbours()) {
					if (neighborsSorted[k].getVariable().getDomainSize() > neighborsSorted[l].getVariable().getDomainSize()) {
						OCOPAbstractNode tmpnode = neighborsSorted[k];
						neighborsSorted[k] = neighborsSorted[l];
						neighborsSorted[l] = tmpnode;
					}
				}
			}
		}
		node.setNeighbours(new Vector(Arrays.asList(neighborsSorted)));
	}

	//--------------------------------------------------------------------------------------------------//
	public static OCOPAbstractNode getMostConnectedNode(OCOPAbstractProblem problem) {
		int maxcon = 0;
		OCOPAbstractNode idmaxcon = null;
		for (int i = 0; i < problem.getNumberOfNodes(); i++) {
			OCOPAbstractNode nd = problem.getNodeByIndex(i);
			int wi = nd.getNumberOfNeighbours();
			if (maxcon < wi) {
				maxcon = wi;
				idmaxcon = nd;
			}
		}
		System.out.println("Maximal connections: node X" + idmaxcon + " with " + maxcon + " neighbors.");
		return idmaxcon;
	}
	//--------------------------------------------------------------------------------------------------//
	/**
	 * This implements a node ordering heuristic while traversing the graph
	 * it (should) select the unvisited neighbor with the highest number of neighbors 
	 * @param current the current DFS node, in its present state 
	 * @return the node that should be visited next, or null if all are visited
	 */
	public static DFSNode getNextTarget(DFSNode current) {
		DFSNode rez = null;
		return rez;
	}
	//--------------------------------------------------------------------------------------------------//
}
