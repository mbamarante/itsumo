/*
 * Created on Jan 26, 2006 by apetcu
 * Project Public
 * Package frodo.OCOP.graph
 */
package frodo.OCOP.graph;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Vector;

import frodo.CSP.BinaryConstraint;
import frodo.OCOP.DFSNode;
import frodo.OCOP.OCOPAbstractNode;
import frodo.OCOP.OCOPAbstractProblem;
import frodo.OCOP.DPOP.DPOPNode;

public class GeorgesHeuristics {
	/*	
	 public void getAllAncestors(HashMap map, DPOPNode node) {
	 if (node.getParent() != null) {
	 DPOPNode parent = (DPOPNode) node.getParent();
	 map.put(new Integer(parent.getId()), new Integer(0));
	 getAllAncestors(map, parent);
	 }
	 }

	 public void getContribution(HashMap map, DPOPNode node) {
	 NeighborGroup pps = node.getPseudoParents();
	 if (pps != null) {
	 //count all the pseudoparents contribution
	 for (int i = 0; i < pps.getSize(); i++) {
	 DPOPNode ppNode = (DPOPNode) pps.getNeighborByIndex(i);
	 Integer ppIdInt = new Integer(ppNode.getId());
	 //pseudoparent has to be one of the ancestors
	 Integer contribution = (Integer) map.get(ppIdInt);
	 if (contribution != null) {
	 if (contribution.intValue() == 0) {
	 contribution = new Integer(1);
	 map.put(ppIdInt, contribution);
	 }
	 }
	 }
	 }
	 NeighborGroup children = node.getChildren();
	 if (children != null) {
	 //count the contribution of each child
	 for (int i = 0; i < children.getSize(); i++) {
	 DPOPNode child = (DPOPNode) children.getNeighborByIndex(i);
	 getContribution(map, child);
	 }
	 }
	 }

	 //-------------------------------------------------------------------------------------------------------------//
	 public static long getTotalHCSize(OCOPAbstractProblem problem) {
	 long answer = 0;
	 //		Vector nodes = problem.getNodes();
	 for (int i = 0; i < problem.getNumberOfNodes(); i++) {
	 OCOPAbstractNode node = problem.getNodeByIndex(i);
	 answer += getHypercubeSize(problem, node);
	 }
	 return answer;
	 }

	 //-------------------------------------------------------------------------------------------------------------//
	 public static long getHypercubeSize(OCOPAbstractProblem problem, OCOPAbstractNode node) {
	 long answer = 0;
	 //		int domSize = node.getVariable().getDomainSize();
	 if (node.getParent() != null) {
	 DPOPNode parent = (DPOPNode) node.getParent();
	 int parentDomSize = parent.getVariable().getDomainSize();
	 answer = parentDomSize;
	 HashMap ancestorsMap = new HashMap();
	 getAllAncestors(ancestorsMap, parent);
	 getContribution(ancestorsMap, node);
	 Iterator iter = ancestorsMap.keySet().iterator();
	 while (iter.hasNext()) {
	 Integer ancestorIdInt = (Integer) iter.next();
	 int contribution = ((Integer) ancestorsMap.get(ancestorIdInt)).intValue();
	 if (contribution == 1) {
	 //found an ancestor which is a back-edge handler for an back-edge
	 //starting with some descendant of this node.
	 DPOPNode ancestor = (DPOPNode) getNodeById(ancestorIdInt.intValue());
	 int ancestorDomSize = ancestor.getVariable().getDomainSize();
	 answer *= ancestorDomSize;
	 }
	 }
	 }
	 return answer;
	 }
	 */
	//--------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	// Added by gushakov.
	/**
	 * Depth First Search with Constraint Type, Most Connected Node first heuristic.
	 * This allows for construction of pseudo-trees with the interconnected nodes
	 * of certain constraint on the top.
	 */
	private static void getGraphWidthByConstraintTypeDFSMCN(OCOPAbstractProblem problem, int rootId, char type) {
		System.out.println("======================================================");
		System.out.println("Getting graph width for recursive DFS CLIQUE(" + type + ") MCN from root=" + rootId);
		System.out.println("======================================================");
		DFSMaster.dfsCleanup(problem);
		System.out.println("Building DFS CLIQUE MCN recursively from root=" + rootId);
		//		buildDFSMCN((DPOPNode) getNodeById(rootId), null);
		for (int i = 0; i < problem.getNumberOfNodes(); i++)
			sortNeighborsByConstraintTypeMCN((problem.getNodeByIndex(i)), type);
		DFSMaster.buildDFS(problem, rootId);
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	private static void getGraphWidthDFSMCNDomainSizeTieBreak(OCOPAbstractProblem problem, int rootId) {
		System.out.println("======================================================");
		System.out.println("Getting graph width for recursive DFS MCN DSTB from root=" + rootId);
		System.out.println("======================================================");
		DFSMaster.dfsCleanup(problem);
		System.out.println("Building DFS MCN recursively from root=" + rootId);
		//		buildDFSMCN((DPOPNode) getNodeById(rootId), null);
		MCNHeuristic.sortNeighborsByConnectivityDomainSizeTieBreak(problem);
		DFSMaster.buildDFS(problem, rootId);
	}

	//------------------------------------------------------------------------------------------------------------------------------------//

	public static void getMinimumGraphWidthByConstraintType(OCOPAbstractProblem problem, char type) {
		int maxcon = 0;
		int idmaxcon = -1;
		for (int i = 0; i < problem.getNumberOfNodes(); i++) {
			OCOPAbstractNode nd = problem.getNodeByIndex(i);
			int wi = nd.getNumberOfNeighboursByConstraintType(type);
			if (maxcon < wi) {
				maxcon = wi;
				idmaxcon = nd.getId();
			}
		}
		if (idmaxcon != -1) {
			System.out.println("Maximal connections: node X" + idmaxcon + " with " + maxcon + " neighbors.");
		} else {
			idmaxcon = 0;
			System.out.println("No connections of type specified: node X" + idmaxcon + " (default) with " + maxcon + " neighbors.");
		}
		getGraphWidthByConstraintTypeDFSMCN(problem, idmaxcon, type);
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	//gushakov
	public static void getMinimumGraphWidthDomainSizeTieBreak(OCOPAbstractProblem problem) {
		int maxcon = 0;
		int idmaxcon = -1;
		int minDomainSize = Integer.MAX_VALUE;
		for (int i = 0; i < problem.getNumberOfNodes(); i++) {
			OCOPAbstractNode nd = problem.getNodeByIndex(i);
			int wi = nd.getNumberOfNeighbours();
			if (maxcon < wi) {
				maxcon = wi;
				idmaxcon = nd.getId();
			} else if (maxcon == wi) {
				int domainSize = nd.getVariable().getDomainSize();
				if (domainSize < minDomainSize) {
					maxcon = wi;
					idmaxcon = nd.getId();
				}
			}
		}
		System.out.println("Maximal connections: node X" + idmaxcon + " with " + maxcon + " neighbors.");
		getGraphWidthDFSMCNDomainSizeTieBreak(problem, idmaxcon);
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	//-----BACK EDGE COUNTING HEURISTICS--------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	//gushakov
	public static void getMinimumGraphWidthBackEdgeConnectivity(OCOPAbstractProblem problem) {
		DFSNode root = (DFSNode) MCNHeuristic.getMostConnectedNode(problem);
		getGraphWidthDFSMCNBEC(problem, root);
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	public static void getGraphWidthDFSMCNBEC(OCOPAbstractProblem problem, DFSNode root) {
		System.out.println("======================================================");
		System.out.println("Getting graph width for recursive DFS MCN BEC from root=" + root.getId());
		System.out.println("======================================================");
		DFSMaster.dfsCleanup(problem);
		HashMap ancestors = new HashMap();
		ancestors.put(new Integer(root.getId()), root);
		buildDFSMCNBEC(root, ancestors);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------//
	// gushakov
	private static void buildDFSMCNBEC(DFSNode cur, HashMap ancestors) {
		sortNeighborsByBackEdgeConnectivity(cur, ancestors);
		cur.setLabel(DFSMaster.NODE_VISITED);
		//		System.out.println("Visiting " + cur.getId());
		//consider all its neighbors
		//in the following we'll determine the best node to proceed with
		//that is the node which has the most connections with unvisited nodes
		for (int k = 0; k < cur.getNumberOfNeighbours(); k++) {
			DFSNode neighbor = (DFSNode) cur.getNeighborByIndex(k);
			if (neighbor.getLabel() != DFSMaster.NODE_VISITED) {
				//ok, so this was not yet visited, so I am its parent, and he's my child
				neighbor.getDFSData().setParent(cur);//set myself as its parent
				cur.getDFSData().getChildren().addNeighbor(neighbor);//add him to my children list
				ancestors.put(new Integer(neighbor.getId()), neighbor);
				buildDFSMCNBEC(neighbor, ancestors);
			} else {
				//this neighbor was already visited, so it must be either my parent, or a PP
				if (cur.getDFSData().getParentID() != neighbor.getId()) {//must not be my parent
					boolean amIAlreadyHisPC = cur.getDFSData().getPseudoparents().containsNeighbor(neighbor.getId());
					boolean amIAlreadyHisPP = cur.getDFSData().getPseudochildren().containsNeighbor(neighbor.getId());
					if (!amIAlreadyHisPC && !amIAlreadyHisPP) {
						//ok, so this means that I am his PC
						//						System.out.println("BE: " + cur.getId() + " to " + neighbor.getId());
						neighbor.getDFSData().getPseudochildren().addNeighbor(cur);//I add myself to his PCs
						cur.getDFSData().getPseudoparents().addNeighbor(neighbor);//I add him as a PP
					}
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------------------//

	public static void sortNeighborsByBackEdgeConnectivity(OCOPAbstractNode node, HashMap ancestors) {
		DFSNode[] neighborsSorted = new DFSNode[node.getNumberOfNeighbours()];
		//		 System.out.print("Neighbors: ");
		for (int k = 0; k < node.getNumberOfNeighbours(); k++) {
			neighborsSorted[k] = (DFSNode) node.getNeighborByIndex(k);
			//			System.out.print("X" + neighborsSorted[k].getId() + " ");
		}
		//		System.out.println();
		for (int k = 0; k < node.getNumberOfNeighbours() - 1; k++) {
			for (int l = k + 1; l < node.getNumberOfNeighbours(); l++) {
				int howmanyBEsK = getNumberOfBackEdges(neighborsSorted[k], ancestors);
				int howmanyBEsL = getNumberOfBackEdges(neighborsSorted[l], ancestors);
				if (howmanyBEsK < howmanyBEsL) {
					DFSNode tmpnode = neighborsSorted[k];
					neighborsSorted[k] = neighborsSorted[l];
					neighborsSorted[l] = tmpnode;
				} else if (howmanyBEsK == howmanyBEsL) {
					if (neighborsSorted[k].getNumberOfNeighbours() < neighborsSorted[l].getNumberOfNeighbours()) {
						DFSNode tmpnode = neighborsSorted[k];
						neighborsSorted[k] = neighborsSorted[l];
						neighborsSorted[l] = tmpnode;
					}
				}
			}
		}
		node.setNeighbours(new Vector(Arrays.asList(neighborsSorted)));
	}

	/**
	 * @param node - the current node
	 * @param ancestors - a hashset of its ancestors (this should be the root path, but in its current implementation, george left it as the full set of previously explored nodes (including siblings))
	 * @return - the number of back edges this node has
	 */
	public static int getNumberOfBackEdges(DFSNode node, HashMap ancestors) {
		int result = 0;
		for (int i = 0; i < node.getNumberOfNeighbours(); i++) {
			DFSNode neighbor = (DFSNode) node.getNeighborByIndex(i);
			int neighborId = neighbor.getId();
			//			if (neighborId != neighbor.getDFSData().getParentID()) {//this was wrong: I need to check whether neighbor is *my* parent, not its own!!
			if (neighborId != node.getDFSData().getParentID()) {
				if (ancestors.containsKey(new Integer(neighborId))) {
					result++;
				}
			}
		}
		return result;
	}

	//--------------------------------------------------------------------------------------------------//
	//gushakov
	/**
	 * Another heuristic for the traversal of nodes in DFS fashion.
	 * Visit first of all the neighbours with whom there is 
	 * a constraint of a certain type (in the order of connectivity).
	 * The remaining neighbours can be traversed in simply
	 * most connected fashion.
	 */
	public static void sortNeighborsByConstraintTypeMCN(OCOPAbstractNode node, char type) {
		OCOPAbstractNode[] neighborsSorted = new DPOPNode[node.getNumberOfNeighbours()];
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

		//Move the inequality neighbors to the front.
		Vector tempVector = new Vector();
		int numInserted = 0;
		for (int i = 0; i < neighborsSorted.length; i++) {
			//Figure out what sort of constraint this is.
			//Consider only first binary constraint between neighbours.
			int neighbourId = node.getNeighborByIndex(i).getId();
			Vector v = node.getConstraintsWithNeighbor(neighbourId);
			BinaryConstraint bc = (v.size() > 0) ? (BinaryConstraint) v.elementAt(0) : null;
			if (bc != null && bc.getType() == type) {
				tempVector.insertElementAt(neighborsSorted[i], numInserted);
				numInserted++;
			} else {
				tempVector.add(neighborsSorted[i]);
			}
		}

		node.setNeighbours(tempVector);
	}

}
