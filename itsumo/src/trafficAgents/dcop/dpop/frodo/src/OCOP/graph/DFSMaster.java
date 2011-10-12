/*
 * Created on Jan 26, 2006 by apetcu
 * Project Public
 * Package frodo.OCOP.graph
 */
package frodo.OCOP.graph;

import java.util.Random;
import java.util.Vector;

import frodo.CSP.CSPVariable;
import frodo.CSP.ContextElement;
import frodo.CSP.VariableId;
import frodo.OCOP.DFSNode;
import frodo.OCOP.MessageContext;
import frodo.OCOP.OCOPAbstractNode;
import frodo.OCOP.OCOPAbstractProblem;
import frodo.utils.HelperUtilities;

public class DFSMaster {
	public static final char NODE_VISITED = 'V';

	public static final char NODE_NOT_VISITED = 'N';

	//this is used to determine where is the maximal width
	private MessageContext largestContext = new MessageContext();

	private long maxSize = 0;

	private long totalSize = 0;

	private int width = 0;

	private int depth = 0;

	private int nodeWithMaxSize;

	private static Random myRandomGenerator = new Random();

	public DFSMaster() {
		largestContext = new MessageContext();
		//		randomGenerator = new Random();
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	/**
	 * returns a heuristic estimate (upper bound) of the lowest width of the problem
	 * this is obviously not the absolute best, that one is NP-hard to compute 
	 * @return an array of: [0]=the width, [1]=the root for that width, [2]=the maximum connectivity (number of neighbors of the root)
	 */
	public static void getMinimalWidthDFS_MCN(OCOPAbstractProblem problem) {
		OCOPAbstractNode root = MCNHeuristic.getMostConnectedNode(problem);
		buildDFS_MCN(problem, root.getId());
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	/**
	 * @param problem - the problem object
	 * @param rootId - the ID of the root
	 */
	public static void buildDFS(OCOPAbstractProblem problem, int rootId) {
		System.out.println("================================================");
		System.out.println("Getting graph width for recursive DFS from root=" + rootId);
		System.out.println("================================================");
		System.out.println("Building DFS recursively from root=" + rootId);
		dfsCleanup(problem);
		buildDFS((DFSNode) problem.getNodeById(rootId));
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	/**
	 * this does a DFS traversal based on the MCN heuristic
	 * each node knows afterwards its parent, pseudoparents, children, pseudochildren
	 * This works by sorting the neighbors of each node by connectivity before doing the DFS traversal
	 * @param problem
	 * @param rootId
	 * @return the width of the DFS
	 */
	public static void buildDFS_MCN(OCOPAbstractProblem problem, int rootId) {
		//		System.out.println("Problem unsorted: " + problem.toStringSerialize());
		MCNHeuristic.sortNeighborsByConnectivity(problem);
		//		System.out.println("Problem sorted: " + problem.toStringSerialize());
		buildDFS(problem, rootId);
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	/**
	 * @param problem
	 * This goes through each node and resets its DFSData structure (all relationships with its neighbors)
	 */
	public static void dfsCleanup(OCOPAbstractProblem problem) {
		System.out.println("Cleanup DFS relationships..");
		for (int i = 0; i < problem.getNumberOfNodes(); i++) {
			DFSNode nd = (DFSNode) problem.getNodeByIndex(i);
			nd.getDFSData().getPseudochildren().getNeighbors().clear();
			nd.getDFSData().getPseudoparents().getNeighbors().clear();
			nd.getDFSData().getChildren().getNeighbors().clear();
			nd.getDFSData().setParent(null);
			nd.setLabel(DFSMaster.NODE_NOT_VISITED);
		}
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	/**
	 * This builds recursively the pseudotree structure after a DFS traversal of the graph 
	 * (the parent-child and pseudo-parent - pseudo-child relationships are established)
	 * @param currentNode = the current node
	 * @param parent = its parent
	 */

	private static void buildDFS(DFSNode currentNode) {
		//each node knows afterwards its parent, pseudoparents, children, pseudochildren

		currentNode.setLabel(DFSMaster.NODE_VISITED);
		//		System.out.println("Visiting " + currentNode.getId() + "\n" + currentNode.getDFSData().toString());
		//consider all its neighbors
		//in the following we'll determine the best node to proceed with
		//that is the node which has the most connections with unvisited nodes
		for (int k = 0; k < currentNode.getNumberOfNeighbours(); k++) {
			DFSNode neighbor = (DFSNode) currentNode.getNeighborByIndex(k);
			if (neighbor.getLabel() != DFSMaster.NODE_VISITED) {//ok, this was not visited, so it is my child
				neighbor.getDFSData().setParent(currentNode);//make myself its parent
				currentNode.getDFSData().getChildren().addNeighbor(neighbor);//add it to my children
				//				neighbor.getDFSData().setPathFromRoot();
				//				System.out.println("PARENT=" + currentNode.getId() + " CHILD=" + neighbor.getId());
				buildDFS(neighbor);
			} else {
				//this neighbor was already visited, so it must be either my parent, a PP or a PC
				if (currentNode.getDFSData().getParentID() != neighbor.getId()) {//it is not my parent
					//					System.out.println("BEEP=" + currentNode.getId() + " BIBBIP=" + neighbor.getId());
					if (!currentNode.getDFSData().getPseudoparents().containsNeighbor(neighbor.getId())) {//it is not my pseudoparent
						//						System.out.println("BEEP=" + currentNode.getId() + " BIBBIP=" + neighbor.getId());
						if (!currentNode.getDFSData().getPseudochildren().containsNeighbor(neighbor.getId())) {//it is not my pseudochild
							neighbor.getDFSData().getPseudochildren().addNeighbor(currentNode);//since it was already visited, I am its PC
							currentNode.getDFSData().getPseudoparents().addNeighbor(neighbor);//since it was already visited, then it is my PP
							//							System.out.println("PP=" + neighbor.getId() + " PC=" + currentNode.getId());
						}
					} else {
						//						System.out.println("CURNODE PP=" + currentNode.getDFSData().getPseudoparents() + " contains " + neighbor.getId());
					}
				}
			}
		}
	}
	/**
	 * This builds recursively the pseudotree structure after a DFS traversal of the graph 
	 * (the parent-child and pseudo-parent - pseudo-child relationships are established)
	 * @param currentNode = the current node
	 * @param parent = its parent
	 */
/*
	private static void buildDFS_BEC(DFSNode currentNode) {
		//each node knows afterwards its parent, pseudoparents, children, pseudochildren

		currentNode.setLabel(DFSMaster.NODE_VISITED);
		//		System.out.println("Visiting " + currentNode.getId() + "\n" + currentNode.getDFSData().toString());
		//consider all its neighbors
		//in the following we'll determine the best node to proceed with
		//that is the node which has the most connections with unvisited nodes
		for (int k = 0; k < currentNode.getNumberOfNeighbours(); k++) {
			DFSNode neighbor = (DFSNode) currentNode.getNeighborByIndex(k);
			if (neighbor.getLabel() != DFSMaster.NODE_VISITED) {//ok, this was not visited, so it is my child
				neighbor.getDFSData().setParent(currentNode);//make myself its parent
				currentNode.getDFSData().getChildren().addNeighbor(neighbor);//add it to my children
				//				neighbor.getDFSData().setPathFromRoot();
				//				System.out.println("PARENT=" + currentNode.getId() + " CHILD=" + neighbor.getId());
				buildDFS(neighbor);
			} else {
				//this neighbor was already visited, so it must be either my parent, a PP or a PC
				if (currentNode.getDFSData().getParentID() != neighbor.getId()) {//it is not my parent
					//					System.out.println("BEEP=" + currentNode.getId() + " BIBBIP=" + neighbor.getId());
					if (!currentNode.getDFSData().getPseudoparents().containsNeighbor(neighbor.getId())) {//it is not my pseudoparent
						//						System.out.println("BEEP=" + currentNode.getId() + " BIBBIP=" + neighbor.getId());
						if (!currentNode.getDFSData().getPseudochildren().containsNeighbor(neighbor.getId())) {//it is not my pseudochild
							neighbor.getDFSData().getPseudochildren().addNeighbor(currentNode);//since it was already visited, I am its PC
							currentNode.getDFSData().getPseudoparents().addNeighbor(neighbor);//since it was already visited, then it is my PP
							//							System.out.println("PP=" + neighbor.getId() + " PC=" + currentNode.getId());
						}
					} else {
						//						System.out.println("CURNODE PP=" + currentNode.getDFSData().getPseudoparents() + " contains " + neighbor.getId());
					}
				}
			}
		}
	}
*/
	//------------------------------------------------------------------------------------------------------------------------------------//
	/*
	 public static MessageContext dfsTraversalRecursive(DFSNode cur) {
	 //		System.out.println("Node " + cur.getId() + " starts with " + cur.getDFSData().getNumberOfChildren() + " children.");
	 //		System.out.println("Children: " + cur.getDFSData().getChildren());
	 MessageContext toParent = new MessageContext();

	 //first I have to explore all of this guy's children (if any)
	 for (int i = 0; i < cur.getDFSData().getNumberOfChildren(); i++) {
	 //going through the children, one by one and adding their contexts to my own
	 DFSNode kid = (DFSNode) cur.getDFSData().getChildren().getNeighborByIndex(i);
	 MessageContext fromChild = dfsTraversalRecursive(kid);
	 toParent.combine(fromChild);
	 }
	 //We add to the context any pseudoparents this guy may have
	 for (int i = 0; i < cur.getDFSData().getNumberOfPseudoParents(); i++) {
	 CSPVariable tmpVar = cur.getDFSData().getPseudoparents().getNeighborByIndex(i).getVariable();
	 VariableId vid = tmpVar.getId();
	 ContextElement tmpcontext = new ContextElement(vid, null, tmpVar.getDomainSize());
	 toParent.addElementIfNotPresent(tmpcontext);
	 }

	 if (cur.getDFSData().getParent() != null) {
	 //We add the parent
	 CSPVariable tmpVar = cur.getDFSData().getParent().getVariable();
	 VariableId vid = tmpVar.getId();
	 ContextElement tmpcontext = new ContextElement(vid, null, tmpVar.getDomainSize());
	 toParent.addElementIfNotPresent(tmpcontext);
	 }

	 //removing myself from the context before sending it to my parent
	 ContextElement cel = new ContextElement(cur.getVariable().getId(), null, 2);
	 if (toParent.containsRegardlessValue(cel))
	 toParent.removeContextElementRegardlessValue(cel); //this should always be the case

	 //updating the largest context encountered so far
	 if (largestContext.size() < toParent.size()) {
	 largestContext.getElements().clear();
	 for (int i = 0; i < toParent.size(); i++) {
	 ContextElement niu = new ContextElement(toParent.getElement(i).getId(), null, 2);
	 largestContext.addElement(niu);
	 }
	 }
	 long thissize = toParent.possibleCombinations();
	 System.out.println("Node " + cur.getId() + " returns CTX: " + toParent.toString() + " -> " + thissize + " valuations.");
	 if (maxSize < thissize)
	 maxSize = thissize;
	 totalSize += thissize;

	 return toParent;
	 }
	 */
	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	/**
	 * This method traverses a problem along a DFS which is previously created.
	 * The point is to get some statistics about this DFS (width, height, total/max UTIL message size, etc)
	 * @param problem - this is the problem object. This has to be DFS-traversed beforehand.
	 * @param cur - this is the root node
	 * @return the context of the message going back from the current node to its ancestor (the separator)
	 */
	private MessageContext traverseDFSRecursive(OCOPAbstractProblem problem, DFSNode cur, int level) {
		//		System.out.println("Node " + cur.getId() + " starts with " + cur.getDFSData().getNumberOfChildren() + " children.");
		//		System.out.println("Children: " + cur.getDFSData().getChildren());
		MessageContext toParent = new MessageContext();

		//first I have to explore all of this guy's children (if any)
		for (int i = 0; i < cur.getDFSData().getNumberOfChildren(); i++) {
			//going through the children, one by one and adding their contexts to my own
			DFSNode tmpkid = (DFSNode) cur.getDFSData().getChildren().getNeighborByIndex(i);
			DFSNode kid = (DFSNode) problem.getNodeById(tmpkid.getId());
			MessageContext fromChild = traverseDFSRecursive(problem, kid, level + 1);
			toParent.combine(fromChild);
		}
		//We add to the context any pseudoparents this guy may have
		for (int i = 0; i < cur.getDFSData().getNumberOfPseudoParents(); i++) {
			CSPVariable tmpVar = cur.getDFSData().getPseudoparents().getNeighborByIndex(i).getVariable();
			VariableId vid = tmpVar.getId();
			ContextElement tmpcontext = new ContextElement(vid, null, tmpVar.getDomainSize());
			toParent.addElementIfNotPresent(tmpcontext);
		}

		if (cur.getDFSData().getParent() != null) {
			//We add the parent
			CSPVariable tmpVar = cur.getDFSData().getParent().getVariable();
			VariableId vid = tmpVar.getId();
			ContextElement tmpcontext = new ContextElement(vid, null, tmpVar.getDomainSize());
			toParent.addElementIfNotPresent(tmpcontext);
		}

		//removing myself from the context before sending it to my parent
		ContextElement cel = new ContextElement(cur.getVariable().getId(), null, 2);
		if (toParent.containsVariable(cel.getVariableId()))
			toParent.removeContextElement(cel.getVariableId()); //this should always be the case

		//updating the largest context encountered so far
		if (largestContext.size() < toParent.size())
			largestContext = (MessageContext) toParent.clone();

		//Ok, done traversing and stuff, now I'm recording the statistics
		long thissize = MessageContext.possibleCombinations(toParent);
		//		System.out.println("Node " + cur.getId() + " returns CTX: " + toParent.toString() + " -> " + thissize + " valuations.");
		if (maxSize < thissize) {
			maxSize = thissize;
			nodeWithMaxSize = cur.getId();
		}
		totalSize += thissize;
		if (width < toParent.size())
			width = toParent.size();
		if (depth < level)
			depth = level;
		DFSData currentNodeDFSData = cur.getDFSData();
		currentNodeDFSData.setWidth(toParent.size());
		currentNodeDFSData.setUTIL_SIZE(thissize);
		currentNodeDFSData.setSeparator(toParent);
		return toParent;
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	/**
	 * This method traverses a problem along a DFS which is previously created.
	 * The point is to get some statistics about this DFS (width, height, total/max UTIL message size, etc)
	 * @param problem
	 * @return an array of longs like this:
	 *<br>stats[0] = width of the DFS
	 *<br>stats[1] = depth of the DFS
	 *<br>stats[2] = max UTIL message size (in the meeting scheduling with 8 values, this is 8^width)
	 *<br>stats[3] = total UTIL size (sum of sizes of all UTIL messages)
	 *<br>stats[4] = number of UTIL messages in DPOP (this is the number of edges, or the number of nodes-1)
	 */
	public static DFSStatistics getStatisticsOfDFS(OCOPAbstractProblem problem) {
		DFSMaster myDFSTool = new DFSMaster();
		//		root is the one with null parent
		DFSNode root = DFSMaster.findRoot(problem);
		//collect the statistics by doing a traversal
		myDFSTool.traverseDFSRecursive(problem, root, 0);
		DFSStatistics stats = new DFSStatistics(root.getId(), myDFSTool.width, myDFSTool.depth + 1, myDFSTool.maxSize, myDFSTool.nodeWithMaxSize, myDFSTool.totalSize - 1,
				problem.getNumberOfNodes() - 1);
		//total size is -1 because the one from the root is not sent, so it does not count
		//number of messages is problem.getNumberOfNodes()-1 because in DPOP we send messages only through the tree edges
		//myDFSTool.width;//this is the width of the problem
		//myDFSTool.depth - this is the depth of the problem; I add one because I start from 0
		//myDFSTool.maxSize is the maximal product of domain sizes of all separators; in the meeting scheduling with 8 values, this is 8^width

		return stats;
	}

	//-------------------------------------------------------------------------------------------------------------//
	//--DFS height related stuff-----------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	/**
	 * @param nd - the node to find the DFS root path for
	 * @return the DFS root path for the specified node (in root-leaf order)
	 */
	public Vector getNodeDFSRootPath(DFSNode nd) {
		Vector rez = new Vector();
		//		rez.add(nd);
		DFSNode tmpn = nd;
		while (tmpn.getDFSData().getParent() != null) {
			tmpn = tmpn.getDFSData().getParent();
			rez.add(0, tmpn);
		}
		return rez;
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	/**
	 * find the root of the pseudotree - this is the node without a parent.
	 * @param problem
	 * @return the ROOT node
	 */
	public static DFSNode findRoot(OCOPAbstractProblem problem) {
		for (int i = 0; i < problem.getNumberOfNodes(); i++) {
			DFSNode nd = (DFSNode) problem.getNodeByIndex(i);
			if (nd.getDFSData().getParent() == null)
				return nd;
		}
		return null;
	}

	//-------------------------------------------------------------------------------------------------------------//
	/**
	 * @param problem
	 * @return a string containing all the ids of the nodes, in the order they are visited (e.g.: 0-2-5-6-3-....)
	 */
	public static String getCurrentDFSTraversalString(OCOPAbstractProblem problem) {
		StringBuffer rez = new StringBuffer();
		//find the root of the pseudotree as the node without a parent.
		OCOPAbstractNode root = findRoot(problem);
		Vector orderDFS = dfsTraversalIterative(problem, root);
		for (int i = 0; i < orderDFS.size(); i++) {
			rez.append(((OCOPAbstractNode) orderDFS.elementAt(i)).getId() + "-");
		}
		return rez.toString();
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	private static Vector dfsTraversalIterative(OCOPAbstractProblem problem, OCOPAbstractNode cur) {
		Vector order = new Vector();
		System.out.println("------------------------------------------");
		System.out.println("DFS iterative with root=" + cur.getId());
		System.out.println("------------------------------------------");

		Vector stack = new Vector();
		boolean visited[] = new boolean[problem.getNumberOfNodes()];
		stack.add(cur);
		while (!stack.isEmpty()) {
			OCOPAbstractNode nd = (OCOPAbstractNode) stack.lastElement();
			stack.remove(stack.size() - 1);
			if (!visited[nd.getId()]) {
				order.add(nd);
			}
			visited[nd.getId()] = true;
			for (int i = 0; i < nd.getNumberOfNeighbours(); i++) {
				//nodes have to be added to the stack in reverse order s.t. they are processed in direct order as in recursive DFS
				OCOPAbstractNode kid = nd.getNeighborByIndex(nd.getNumberOfNeighbours() - i - 1);
				if (!visited[kid.getId()]) {
					// visited[kid.getId()] = true;
					stack.add(kid);
				}
			}
		}
		return order;
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	/**
	 * @param problem
	 * @return true if this problem has any cycles (is not a tree), and false if it is a tree.
	 * This works by constructing a dfs-traversal, and computing the width.
	 * If the width is 1, then this is a tree. If it is greater, it has cycles.
	 */
	/*
	 public boolean hasCycles(OCOPAbstractProblem problem) {
	 boolean rez = false;
	 int width = getGraphWidthDFS(problem, 0);
	 if (width > 1)
	 rez = true;
	 else
	 rez = false;
	 dfsCleanup(problem);
	 return rez;
	 }
	 */
	//------------------------------------------------------------------------------------------------------------------------------------//
	/**
	 * Permutes randomly the order of neighbours of each node.
	 *
	 */
	public static void randomizeNodeOrder(OCOPAbstractProblem problem) {
		for (int i = 0; i < problem.getNumberOfNodes(); i++) {
			problem.getNodeByIndex(i).randomizeNeighbourOrder();
		}
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	public static Vector getClusters(OCOPAbstractProblem problem) {
		Vector clusters = new Vector();
		boolean finished = false;
		int visited[] = new int[problem.getNumberOfVariables()];//a 1 in this array means that the var was visited
		while (!finished) {
			int ndx = HelperUtilities.findInArray(visited, 0);
			if (ndx == -1) {
				finished = true;
			} else {
				OCOPAbstractNode curn = problem.getNodeById(ndx);
				Vector newcl = dfsTraversalIterative(problem, curn);
				for (int i = 0; i < newcl.size(); i++) {
					OCOPAbstractNode tmpn = (OCOPAbstractNode) newcl.elementAt(i);
					visited[tmpn.getId()] = 1;
				}
				clusters.add(newcl);
			}
		}
		return clusters;
	}

	public static OCOPAbstractNode pickRandomNode(OCOPAbstractProblem problem) {
		int which = myRandomGenerator.nextInt(problem.getNumberOfNodes());
		OCOPAbstractNode nd = problem.getNodeByIndex(which);
		return nd;
	}
	//------------------------------------------------------------------------------------------------------------------------------------//
}
