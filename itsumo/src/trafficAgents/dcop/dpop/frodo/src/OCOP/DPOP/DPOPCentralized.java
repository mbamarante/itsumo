/*
 * Created on Jul 1, 2005 by apetcu
 * Project Public
 * Package testbed.OCOP.LSDPOP
 */
package frodo.OCOP.DPOP;

import java.util.Iterator;
import java.util.Vector;

import frodo.COP.OCOPProblemFactory;
import frodo.COP.Solution;
import frodo.COP.ValuedConstraint;
import frodo.CSP.ComplexValue;
import frodo.OCOP.OCOPAbstractNode;
import frodo.OCOP.OCOPAbstractProblem;
import frodo.OCOP.OCOPBasicProblem;
import frodo.OCOP.DPOP.graphics.OCOPViewer;
import frodo.OCOP.graph.DFSMaster;
import frodo.OCOP.graph.MinfillHeuristic;
import frodo.utils.HelperUtilities;

/**
 * @author apetcu
 *
 * 
 */
public class DPOPCentralized {
	public final static String algo_name = "DPOP_C";

	public final static String algo_name1 = "VARELIM";

	//	------------------------------------------------------------//		
	//	------------------------------------------------------------//	
	//	------------------------------------------------------------//	
	//	------------------------------------------------------------//
	/**
	 * This performs the recursive variable elimination on the DFS structure (util bottom-up, value top-down)
	 * The DFS creation has to be performed *BEFORE* calling this method!! 
	 * @param problem
	 * @return the optimal solution
	 */
	public static Solution solve(OCOPAbstractProblem problem) {
		long time = System.currentTimeMillis();
		DPOPNode root = (DPOPNode) DFSMaster.findRoot(problem);
		System.out.println("---------------------------------------");
		System.out.println("Starting UTIL propagation:");
		System.out.println("---------------------------------------");
		HyperCube overallUTIL = UTILPropagation(root);
		System.out.println("---------------------------------------");
		System.out.println("Finished UTIL with " + overallUTIL);
		System.out.println("Starting VALUE propagation:");
		System.out.println("---------------------------------------");
		VALUEPropagation(problem, root);
		//System.out.println("Cycles=" + HelperUtilities.indent(HelperUtilities.vectorToStringWithDelimiter(getCycles(), "\n"), 1));
		ComplexValue result = problem.getCurrentAssignments();
		double optimalUtil = problem.getTotalEvaluation();
		System.out.println("Total util=" + optimalUtil);
		Solution solution = new Solution(result, optimalUtil, 0);
		time = System.currentTimeMillis() - time;
		solution.setTime(time);
		return solution;
	}

	//------------------------------------------------------------//	
	/**
	 * This performs the UTIL propagation from leaves up;
	 * <br> It starts recursively from the root;
	 * <br> on the way down, it does nothing;
	 * <br> on the way up, each node computes and stores its JOIN, and returns the UTIL to its parent
	 * @param node - the root of the DFS
	 */
	private static HyperCube UTILPropagation(DPOPNode node) {
		//		System.out.println("Enter node X" + node.getId());
		HyperCube rez = null;
		//get UTILs from children
		HyperCube utilsFromChildren[] = new HyperCube[node.getDFSData().getNumberOfChildren()];
		//get the UTIL messages for all my children 
		for (int i = 0; i < node.getDFSData().getNumberOfChildren(); i++) {
			//get message from child recursively
			utilsFromChildren[i] = UTILPropagation((DPOPNode) node.getDFSData().getChildren().getNeighborByIndex(i));
			//join with my current JOIN 
			rez = utilsFromChildren[i].join(rez);
		}
		//If I am not root, then join with the relation with my parent
		if (node.getDFSData().getParent() != null) {
			HyperCube withParent = new HyperCube(((ValuedConstraint) node.getConstraintsWithNeighbor(node.getDFSData().getParentID()).elementAt(0)));
			rez = withParent.join(rez);
		}
		//If I have pseudoparents, join with the relations I have with them
		for (int i = 0; i < node.getDFSData().getNumberOfPseudoParents(); i++) {
			OCOPAbstractNode pseudoparent = node.getDFSData().getPseudoparents().getNeighborByIndex(i);
			HyperCube withPseudoparent = new HyperCube(((ValuedConstraint) node.getConstraintsWithNeighbor(pseudoparent.getId()).elementAt(0)));
			rez = withPseudoparent.join(rez);
		}
		//local unary constraints
		for (int i = 0; i < node.getNumberOfConstraints(); i++) {
			ValuedConstraint vc = (ValuedConstraint) node.getConstraints().elementAt(i);
			if (vc.getArity() == 1) {
				//				myLogger.println("Found local unary constraint:\n" + vc.toString());
				HyperCube tmpCube = new HyperCube(vc);
				//				System.out.println("X" + node.getId() + "Found local unary constraint:\n" + tmpCube.toStringVectors());
				rez = tmpCube.join(rez);
			}
		}

		//		System.out.println("Node X" + node.getId()+  " has dim=" + node.getDimension());
		//store the UTIL message for future reference (VALUE propagation)
		node.setUtilMessage(rez);
		System.out.println("Node X" + node.getId() + " has UTIL=\n" + rez.toStringVectors());
		//project myself out of the UTIL msg
		rez = rez.project(node.getDimension());
		//		System.out.println("Exit node X" + node.getId());
		return rez;
	}

	//	------------------------------------------------------------//	
	//	------------------------------------------------------------//	
	/**
	 * This performs the VALUE propagation recursively from root down
	 * @param node
	 */
	private static void VALUEPropagation(OCOPAbstractProblem problem, DPOPNode node) {
		//find my optimal value
		HyperCube myJoin = node.getUtilMessage();
		HyperCube myOpt = myJoin;
		Dimension dims[] = myJoin.getDimensions();
		for (int i = 0; i < dims.length; i++) {
			int nodeId = dims[i].getId();
			if (nodeId != node.getId()) {
				DPOPNode pp = (DPOPNode) problem.getNodeById(nodeId);// I need this to be like this, because it may happen that I need a value for a node which is not my neighbor
				int index = pp.getDimension().getCurrentIndex();
				dims[i].setCurrentIndex(index);
				myOpt = myOpt.slice(dims[i]);
			}
		}
		Dimension optimal[] = myOpt.optimalAssignment();
		int optindex = optimal[0].getCurrentIndex();
		node.getVariable().setValue(node.getVariable().getValue(optindex));
		node.getDimension().setCurrentIndex(optindex);
		//visit children, and find their optimal values, etc..
		for (int i = 0; i < node.getDFSData().getNumberOfChildren(); i++) {
			DPOPNode child = (DPOPNode) node.getDFSData().getChildren().getNeighborByIndex(i);
			VALUEPropagation(problem, child);
		}
		return;
	}

	//------------------------------------------------------------//	
	//------------------------------------------------------------//	
	//------------------------------------------------------------//	
	/**
	 * This uses a precomputed elimination order (in this case, coming from MinfillHeuristic)
	 * @param problem
	 * @param eliminationOrder
	 * @return the optimal assignment for this problem
	 */
	public static Solution solve(OCOPAbstractProblem problem, Vector eliminationOrder) {
		long time = System.currentTimeMillis();
		System.out.println("---------------------------------------");
		System.out.println("Starting elimination along " + eliminationOrder);
		eliminate(problem, eliminationOrder);
//		OCOPAbstractProblem clona = (OCOPAbstractProblem) problem.clone();
//		eliminate(clona, eliminationOrder);

		System.out.println("Starting VALUE propagation:");
		System.out.println("---------------------------------------");
		reconstruct(problem, eliminationOrder);
		//		VALUEPropagation(root);
		//System.out.println("Cycles=" + HelperUtilities.indent(HelperUtilities.vectorToStringWithDelimiter(getCycles(), "\n"), 1));
		ComplexValue result = problem.getCurrentAssignments();
		double optimalUtil = problem.getTotalEvaluation();
		System.out.println("Total util=" + optimalUtil);
		Solution solution = new Solution(result, optimalUtil, 0);
		time = System.currentTimeMillis() - time;
		solution.setTime(time);
		return solution;
	}

	//------------------------------------------------------------//
	/**
	 * @param problem - this is the problem that has to be variable-eliminated
	 * @param eliminationOrder - this is a vector of the nodes of the problem, arranged in the order that should be used for the elimination
	 * This does not return anything, it just saves in each node the hypercube produced as a result of its elimination
	 */
	private static void eliminate(OCOPAbstractProblem problem, Vector eliminationOrder) {
		HyperCube[] sentMessages = new HyperCube[problem.getNumberOfNodes()];
		for (int i = 0; i < eliminationOrder.size(); i++) {
			System.out.println("---------------------------------------");
			System.out.println("---------------------------------------");
			System.out.println("---------------------------------------");
			DPOPNode curnode = (DPOPNode) problem.getNodeById(((DPOPNode) eliminationOrder.elementAt(i)).getId());
			System.out.println("Eliminating X" + curnode.getId());
			System.out.print("Neighbors: ");
			for (Iterator iter = curnode.getNeighbours().iterator(); iter.hasNext();) {
				OCOPAbstractNode element = (OCOPAbstractNode) iter.next();
				System.out.print("X" + element.getId() + "\t");
			}
			System.out.println("");

			HyperCube toParent = null;

			//join with local unaries, if any
			for (int k = 0; k < curnode.getConstraints().size(); k++) {
				ValuedConstraint cos = curnode.getConstraint(k);
				if (cos.getArity() == 1) {
					HyperCube local = HyperCube.fromRelation(cos);
					toParent = local.join(toParent);
				}
			}
			//			Vector toconnect = new Vector();
			//join with ancestors, if any
			for (int j = 0; j < curnode.getNumberOfNeighbours(); j++) {
				DPOPNode neighbor = (DPOPNode) curnode.getNeighborByIndex(j);
				int elim_index = eliminationOrder.indexOf(neighbor);
				if (elim_index > i) {//this means that this node is later in the ordering (thus an ancestor)
					System.out.println("Join with PP X" + neighbor.getId());
					HyperCube tmp = new HyperCube((ValuedConstraint) curnode.getConstraintsWithNeighbor(neighbor.getId()).firstElement());
					toParent = tmp.join(toParent);
					//					toconnect.add(neighbor);
				}
				//				else {//this means that this neighbor was earlier in the ordering, and was already eliminated
				//					if (sentMessages[elim_index] != null) {//this means that the constraint induced by its elimination was not yet taken into account 
				//						toParent = sentMessages[elim_index].join(toParent);//I jion with it
				//						System.out.println("Joining with PC X" + neighbor.getId() + "\n" + sentMessages[elim_index].toStringVectors());
				//						sentMessages[elim_index] = null;//I set it to null s.t. it does not get counted twice
				//					}
				//				}
			}
			for (int l = 0; l < i; l++) {
				if (sentMessages[l] != null) {//this means that the constraint induced by its elimination was not yet taken into account
					if (sentMessages[l].hasDimension(new Dimension(curnode.getId(),0,0))) {
						toParent = sentMessages[l].join(toParent);//I jion with it
						System.out.println("Joining with NON Neighbor X" + ((DPOPNode)eliminationOrder.elementAt(l)).getId() + "\n" + sentMessages[l].toStringVectors());
						sentMessages[l] = null;//I set it to null s.t. it does not get counted twice
					}
				}
			}
			//makeClique(toconnect);			
			curnode.utilMessage = toParent;
			System.out.println("JOIN=\n" + HelperUtilities.arrayToString(toParent.getDimensions()));
			toParent = toParent.project(curnode.getVariable().getId());
			sentMessages[i] = toParent;
			System.out.println("Finished eliminating " + curnode.getId());
			System.out.println("UTIL=\n" + HelperUtilities.arrayToString(toParent.getDimensions()));
		}
	}
/*
	private static void makeClique(Vector nodes) {
		for (int j = 0; j < nodes.size() - 1; j++) {
			for (int k = j + 1; k < nodes.size(); k++) {
				OCOPAbstractNode srcngh = (OCOPAbstractNode) nodes.elementAt(j);
				OCOPAbstractNode targngh = (OCOPAbstractNode) nodes.elementAt(k);
				if (srcngh.getNeighborById(targngh.getId()) == null) {
					//					BinaryConstraint bc = new ValuedConstraint(srcngh.getVariable(), targngh.getVariable());
					//					srcngh.addConstraint(bc);
					srcngh.addNeighbour(targngh);
					//					bc = new ValuedConstraint(targngh.getVariable(), srcngh.getVariable());
					//					targngh.addConstraint(bc);
					targngh.addNeighbour(srcngh);
				}
			}
		}
	}
*/
	//------------------------------------------------------------//
	private static void reconstruct(OCOPAbstractProblem problem, Vector eliminationOrder) {
		for (int i = eliminationOrder.size() - 1; i >= 0; i--) {
			System.out.println("---------------------------------------");
			//			DPOPNode node = (DPOPNode) eliminationOrder.elementAt(i);
			DPOPNode node = (DPOPNode) problem.getNodeById(((DPOPNode) eliminationOrder.elementAt(i)).getId());
			System.out.println("Optimizing X" + node.getId());

			//			HyperCube myJoin = node.utilMessage;
			HyperCube myOpt = node.utilMessage;
			System.out.println("I have JOIN=\n" + myOpt.toStringVectors());
			//slice through my join, using the assignments of my ancestors; this relies on the fact that all ancestors are already instantiated!!!
			//this is achieved by processing the elimination order in reverse (last first)
			Dimension dims[] = node.utilMessage.getDimensions();
			for (int j = 0; j < dims.length; j++) {
				int nodeId = dims[j].getId();
				if (nodeId != node.getId()) {
					DPOPNode pp = (DPOPNode) problem.getNodeById(nodeId);
					int index = pp.getDimension().getCurrentIndex();
					System.out.println("Slicing through " + nodeId + " which has optimal index=" + index);
					dims[j].setCurrentIndex(index);
					myOpt = myOpt.slice(dims[j]);
				}
			}
			//find my optimal value
			Dimension optimal[] = myOpt.optimalAssignment();
			int optindex = optimal[0].getCurrentIndex();
			//			System.out.println("Found optimal index=" + optindex);
			node.getVariable().setValue(node.getVariable().getValue(optindex));
			node.getDimension().setCurrentIndex(optindex);
			//			System.out.println("Found optimal index=" + node.getDimension().getCurrentIndex());
		}
	}

	//------------------------------------------------------------//	
	//------------------------------------------------------------//	
	//------------------------------------------------------------//	

	public static void main(String[] args) {
		if (args.length < 1) {
			System.out.println("Usage: TesterDPOP problem_file result_file on_screen");
			System.out.println("\ton_screen = 0(no visual) / 1 (visual)");
			System.exit(0);
		}
		String problem_file = args[0];
		String result_file = problem_file;
		if (args.length >= 2)
			result_file = args[1];
		int onscreen = 0;
		if (args.length >= 3)
			onscreen = Integer.parseInt(args[2]);
		OCOPBasicProblem myProblem = (OCOPBasicProblem) OCOPProblemFactory.loadFromTextFile(problem_file, new DPOPNode(null, 0, null));

		//have a look at the problem before solving it
		int howMany = myProblem.getNumberOfNodes();
		int constrNo = myProblem.getNumberOfConstraints();
		System.out.println("Loaded problem from " + problem_file + " with " + howMany + " variables and " + constrNo + " constraints; ");

		DFSMaster.getMinimalWidthDFS_MCN(myProblem);
		System.out.println("DFS stats:\n" + DFSMaster.getStatisticsOfDFS(myProblem));

		//if desired, I bring up a display
		if (onscreen == 1) {
			System.out.println("ON_SCREEN=" + onscreen + " constructing video.");
			new OCOPViewer(myProblem);
		}

		//solve the problem with the DFS-DPOP, on the DFS generated previously with MCN
		Solution solutionDFS = DPOPCentralized.solve(myProblem);
		System.out.println("Solution=" + solutionDFS);

		myProblem = (OCOPBasicProblem) OCOPProblemFactory.loadFromTextFile(problem_file, new DPOPNode(null, 0, null));
		//find best ordering according to minfill
		Vector ordering = MinfillHeuristic.bestOrdering(myProblem);
		//solve problem with VAR-ELIM based on the min-fill ordering
		Solution solutionMINFILL = DPOPCentralized.solve(myProblem, ordering);
		System.out.println("Solution DFS    =" + solutionDFS);
		System.out.println("Solution MINFILL=" + solutionMINFILL);
		System.out.println("Differences=" + HelperUtilities.arrayToString(solutionDFS.getDifferences(solutionMINFILL)));

		Solution.writeResultToFile(solutionDFS, result_file + "." + algo_name);
		Solution.writeResultToFile(solutionMINFILL, result_file + "." + algo_name1);
		//		CommunicationStatistics.writeToFile(statistics, resultFile + ".stats", time);
	}
}
//------------------------------------------------------------//	
//------------------------------------------------------------//	
