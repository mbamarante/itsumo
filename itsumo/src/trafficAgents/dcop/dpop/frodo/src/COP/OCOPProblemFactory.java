/*
 * Created on Jan 29, 2006 by apetcu
 * Project Public
 * Package frodo.COP
 */
package frodo.COP;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.StringReader;
import java.util.Vector;

import frodo.CSP.CSPVariable;
import frodo.CSP.VariableId;
import frodo.OCOP.OCOPAbstractNode;
import frodo.OCOP.OCOPAbstractProblem;
import frodo.OCOP.OCOPBasicProblem;
import frodo.OCOP.SelfInterestedAgent;
import frodo.OCOP.DPOP.DPOPNode;
import frodo.OCOP.DPOP.Dimension;
import frodo.utils.HelperUtilities;

public class OCOPProblemFactory {
	//------------------------------------------------------------------//
	//------------------------------------------------------------------//
	/**
	 * Essentially, one takes each variable from each node, and creates a new node for it.
	 * ID translations are needed
	 * @param A problem that is in the format with multiple variables per node
	 * @return A problem converted to the format with a single variable per node
	 */
	public static OCOPBasicProblem toSingleVariablePerNode(OCOPAbstractProblem oap) {
		OCOPBasicProblem newproblem = new OCOPBasicProblem();

		//in this, idTranslations[k] = original id of the variable which was read at the kth step
		VariableId[] idTranslations = new VariableId[oap.getNumberOfVariables()];

		//copying the variables into the new problem (with the ids translated) and creating new nodes (one for each var)
		int varcnt = 0;
		for (int i = 0; i < oap.getNumberOfNodes(); i++) {
			OCOPAbstractNode nd = oap.getNodeByIndex(i);
			for (int j = 0; j < nd.getNumberOfVariables(); j++) {
				CSPVariable curvar = nd.getVariable(j);
				Vector domnew = new Vector(curvar.getDomain());
				CSPVariable varnew = new CSPVariable(new VariableId(varcnt, 0), domnew);
				idTranslations[varcnt] = curvar.getId();
				//				OCOPAbstractNode ndnew = (OCOPAbstractNode) nd.clone();
				OCOPAbstractNode ndnew = (OCOPAbstractNode) nd.getNewInstance(varnew.getId().getAgentID());
				ndnew.addVariable(varnew);
				newproblem.addNode(ndnew);
				newproblem.addVariable(varnew);
				varcnt++;
			}
		}
		int constrcnt = 0;
		//copying the constraints, and adding them between the translated variables
		//for each node
		for (int i = 0; i < oap.getNumberOfNodes(); i++) {
			OCOPAbstractNode nd = oap.getNodeByIndex(i);
			//for each variable
			for (int j = 0; j < nd.getNumberOfVariables(); j++) {
				CSPVariable curvar = nd.getVariable(j);
				//for each constraint
				for (int k = 0; k < curvar.getNumberOfConstraints(); k++) {
					ValuedConstraint srcConstraint = (ValuedConstraint) curvar.getConstraint(k);
					if (srcConstraint.getArity() > 1) {
						int srcid = HelperUtilities.findInArray(idTranslations, srcConstraint.getSourceId());
						int destid = HelperUtilities.findInArray(idTranslations, srcConstraint.getDestinationId());
						CSPVariable srcvar = newproblem.getVariable(srcid);
						CSPVariable destvar = newproblem.getVariable(destid);
						//since there is a node for each variable, srcid and destid work for both nodes and variables
						OCOPAbstractNode srcnode = newproblem.getNodeById(srcid);
						OCOPAbstractNode destnode = newproblem.getNodeById(destid);
						ValuedConstraint newConstraint = new ValuedConstraint(srcvar, destvar);
						newConstraint.setValuations(srcConstraint.getValuations());
						newConstraint.setId(constrcnt);
						newConstraint.setOwnerAgentId(srcConstraint.getOwnerAgentId());
						newConstraint.setType(srcConstraint.getType());
						srcvar.addConstraint(newConstraint);
						srcnode.addConstraint(newConstraint);
						if (srcid < destid)
							newproblem.addConstraint(newConstraint);//this is to add it only once
						if (srcnode.getNeighborById(destid) == null) //if this is the first constraint between src and dest, then make them neighbors
							srcnode.addNeighbour(destnode);
					} else {
						int srcid = HelperUtilities.findInArray(idTranslations, srcConstraint.getSourceId());
						CSPVariable srcvar = newproblem.getVariable(srcid);
						//since there is a node for each variable, srcid and destid work for both nodes and variables
						OCOPAbstractNode srcnode = newproblem.getNodeById(srcid);
						ValuedConstraint vc = new ValuedConstraint(srcvar);
						vc.setValuations(srcConstraint.getValuations());
						vc.setId(constrcnt);
						vc.setOwnerAgentId(srcConstraint.getOwnerAgentId());
						vc.setType(srcConstraint.getType());
						srcvar.addConstraint(vc);
						srcnode.addConstraint(vc);
						newproblem.addConstraint(vc);
					}
					constrcnt++;
				}
			}
		}
		return newproblem;
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	public static OCOPAbstractProblem loadFromTextFile(String fileName, OCOPAbstractNode nodeType) {
		OCOPAbstractProblem problem = null;
		try {
			StringBuffer sbuf = new StringBuffer();
			BufferedReader linein = new BufferedReader(new FileReader(fileName));
			boolean done = false;
			while (!done) {
				String line = linein.readLine();
				if (line != null) {
					sbuf.append(line + "\n");
				} else
					done = true;
			}
			linein.close();
			problem = loadFromString(sbuf.toString(),nodeType);
			problem.setName(fileName);
		} catch (Exception ioe) {
			if (ioe instanceof FileNotFoundException) {
				System.err.println("File " + fileName + " not found. Exiting.");
				return null;
			}
			ioe.printStackTrace();
		}
		//		System.out.println("Loaded problem:"  + problem.toStringSerialize());
		return problem;
	}
	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	public static OCOPAbstractProblem loadFromString(String problemDesc, OCOPAbstractNode nodeType) {
		OCOPBasicProblem problem = new OCOPBasicProblem();
		try {
			BufferedReader linein = new BufferedReader(new StringReader(problemDesc));
			String line = linein.readLine();
			int howManyVars = 0;
			int howManyAgents = 0;

			//how many variables/selfInterestedAgents?
			if (line != null) {
				String[] parts = line.split("\\s");
				howManyVars = Integer.parseInt(parts[0]);
				howManyAgents = Integer.parseInt(parts[1]);
				System.out.println(howManyVars + " variables\t" + howManyAgents + " Agents");
				System.out.println("Gonna make nodes of type " + nodeType.getClass().getCanonicalName());
			} else
				return null;

			//setup selfInterestedAgents
			Vector ags = new Vector();
			for (int i = 0; i < howManyAgents; i++) {
				SelfInterestedAgent ag = new SelfInterestedAgent(i);
				ags.add(ag);
			}
			problem.setSelfInterestedAgents(ags);

			//reading all variables
			int i = 0;
			while (i < howManyVars) {
				line = linein.readLine();
				if (line != null && !line.startsWith("#")) {
					CSPVariable var1 = CSPVariable.fromString(line);
					CSPVariable var = new CSPVariable(var1.getId(), var1.getDomain());
					//I try to see if the node already exists (this happens when I am adding multiple variables to the same node) 
					OCOPAbstractNode nd = (OCOPAbstractNode) problem.getNodeById(var.getId().getAgentID());
					if (nd == null) {
						//this is the first time a variable for this node appears
						nd = nodeType.getNewInstance(var.getId().getAgentID());
							if (nd instanceof DPOPNode) {
								DPOPNode new_name = (DPOPNode) nd;
								new_name.setDimension(new Dimension(nd.getId(), var.getDomainSize()));
							}
						problem.addNode(nd);
					} else {
						if (!problem.hasMultipleVarsPerNode()) {
							System.out.println("This problem has more variables per node: " + nd.getVariables());
							problem.setMultipleVarsPerNode(true);
						}
					}
					nd.addVariable(var);

					nd.setLabel(line.toCharArray()[0]);
					i++;
					problem.addVariable(var);
				}
			}

			//reading constraints
			boolean finish = false;
			while (!finish) {
				line = linein.readLine();
				if (line != null) {
					if (!line.startsWith("#")) {
						//I build the constraint
						ValuedConstraint newConstr = ValuedConstraint.fromString(line);
						//						System.out.println("Got a " + newConstr.getType() + " " + newConstr.getFrom().getId() + "-" + newConstr.getDestinationId());
						SelfInterestedAgent sia = problem.getSelfInterestedAgent(newConstr.getOwnerAgentId());
						newConstr.setOwnerAgent(sia);

						//I bind it to its (possibly) two variables
						VariableId fromId = newConstr.getSourceId();
						VariableId toId = newConstr.getDestinationId();
						CSPVariable fromVar = problem.getVariable(fromId);
						CSPVariable toVar = problem.getVariable(toId);
						newConstr.setFrom(fromVar);
						newConstr.setTo(toVar);

						//this is the difference: unary constraints are added only once, to their variable, while binary constraints are added twice, to both of their variables
						fromVar.addConstraint(newConstr);
						newConstr.setId(problem.getConstraints().size());
						problem.addConstraint(newConstr);
						problem.getNodeById(fromId.getAgentID()).addConstraint(newConstr);

						if (fromId.equals(toId)) {
							//that means this is a unary constraint, so don't do anything anymore
						} else {
							//this is a binary constraint
							//building the mirror constraint, to add it to the destination variable
							ValuedConstraint constrTo = new ValuedConstraint(toVar, fromVar);
							constrTo.setOwnerAgent(sia);
							constrTo.setValuations(HelperUtilities.intArrayTranspose(newConstr.getValuations()));

							//update neighboring relations
							toVar.addConstraint(constrTo);
							OCOPAbstractNode fromNode = problem.getNodeById(fromId.getAgentID());
							//add toNode as neighbor to fromNode
							if (fromNode.getNeighborById(toId.getAgentID()) == null) {
								fromNode.addNeighbour(problem.getNodeById(toId.getAgentID()));
							}
							OCOPAbstractNode toNode = problem.getNodeById(toId.getAgentID());
							//add toNode as neighbor to fromNode
							toNode.addConstraint(constrTo);
							if (toNode.getNeighborById(fromId.getAgentID()) == null) {
								toNode.addNeighbour(problem.getNodeById(fromId.getAgentID()));
							}
						}
					}
				} else
					finish = true;
			}
			linein.close();
		} catch (Exception ioe) {
			ioe.printStackTrace();
		}
		//		System.out.println("Loaded problem:"  + problem.toStringSerialize());
		return problem;
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	/**
	 * @param srcProblem this takes a centralized problem and splits it into a set of decentralized subproblems: a NODE object, each one with local copies of its variables, constraints, neighbors, etc
	 * @param nodeType this gives the type of the nodes to create from the centralized problem. If null, then the type in the source problem is used.
	 * @return a decentralized OCOPBasicProblem object 
	 */
	/*
	 public static OCOPBasicProblem decentralize(OCOPBasicProblem srcProblem, OCOPAbstractNode nodeType){
	 OCOPBasicProblem problem = new OCOPBasicProblem();
	 problem.setName(problem.getName());
	 problem.setSelfInterestedAgents(srcProblem.getSelfInterestedAgents());
	 problem.setVariables(srcProblem.getVariables());
	 problem.setConstraints(srcProblem.getConstraints());

	 if (nodeType == null)
	 nodeType = srcProblem.getNodeByIndex(0);
	 //cycle through the nodes
	 for (int i = 0; i < srcProblem.getNumberOfNodes(); i++) {
	 OCOPAbstractNode srcn = srcProblem.getNodeByIndex(i);
	 OCOPAbstractNode newNode = (OCOPAbstractNode) srcn.clone();
	 //adding the original variables
	 for (int j = 0; j < srcn.getNumberOfVariables(); j++) {
	 CSPVariable srcVar = srcn.getVariable(j);
	 CSPVariable newVar = new CSPVariable(srcVar.getId(), srcVar.getDomain(), srcVar.getConstraints(), srcVar.getValue());
	 newNode.addVariable(newVar);
	 }
	 //adding the original neighbors
	 for (int j = 0; j < srcn.getNumberOfNeighbours(); j++) {
	 OCOPAbstractNode srcNgh = srcn.getNeighborByIndex(j);
	 OCOPAbstractNode newNgh = (OCOPAbstractNode) srcNgh.clone();
	 newNode.addNeighbour(newNgh);
	 }
	 //adding the original constraints
	 for (int j = 0; j < srcn.getNumberOfConstraints(); j++) {
	 ValuedConstraint srcC = srcn.getConstraint(j);
	 ValuedConstraint newC = null;
	 CSPVariable srcV = newNode.getVariableByID(srcC.getSourceId());
	 if (srcC.getType() == '$') {
	 newC = new StabilityConstraint(srcV);
	 } else {
	 if (srcC.getArity() > 1) {
	 CSPVariable destV = newNode.getNeighborById(srcC.getDestinationId().getAgentID()).getVariableByID(srcC.getDestinationId());
	 newC = new ValuedConstraint(srcV, destV);
	 } else {
	 newC = new ValuedConstraint(srcV);
	 }
	 }
	 newC.setValuations(srcC.getValuations());
	 newNode.addConstraint(newC);
	 }
	 //adding the original dimension
	 problem.addNode(newNode);
	 }
	 return problem;
	 }
	 */
	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	/**
	 * @param srcProblem this takes a centralized problem and splits it into a set of decentralized subproblems: a NODE object, each one with local copies of its variables, constraints, neighbors, etc
	 * @param nodeType this gives the type of the nodes to create from the centralized problem. If null, then the type in the source problem is used.
	 * @return a decentralized OCOPBasicProblem object 
	 */
	public static OCOPAbstractNode getSubproblem(OCOPAbstractProblem srcProblem, OCOPAbstractNode nodeType, int nodeId) {
		if (nodeType == null)
			nodeType = srcProblem.getNodeByIndex(0);
		OCOPAbstractNode sourceNode = srcProblem.getNodeById(nodeId);
		OCOPAbstractNode newNode = nodeType.getNewInstance(nodeId);
		if (newNode instanceof DPOPNode) {
			DPOPNode new_name = (DPOPNode) newNode;
			new_name.setDimension(((DPOPNode)sourceNode).getDimension());
		}
		newNode.setId(nodeId);
		newNode.setLabel(sourceNode.getLabel());

		//adding the original variables
		for (int j = 0; j < sourceNode.getNumberOfVariables(); j++) {
			CSPVariable srcVar = sourceNode.getVariable(j);
			CSPVariable newVar = new CSPVariable(srcVar.getId(), srcVar.getDomain(), srcVar.getConstraints(), srcVar.getValue());
			newNode.addVariable(newVar);
		}

		//adding the original neighbors
		for (int j = 0; j < sourceNode.getNumberOfNeighbours(); j++) {
			OCOPAbstractNode srcNgh = sourceNode.getNeighborByIndex(j);
			OCOPAbstractNode newNgh = nodeType.getNewInstance(srcNgh.getId());
			newNode.addNeighbour(newNgh);
		}

		//adding the original constraints
		for (int j = 0; j < sourceNode.getNumberOfConstraints(); j++) {
			ValuedConstraint sourceConstraint = sourceNode.getConstraint(j);
			ValuedConstraint newC = null;
			CSPVariable srcVar = newNode.getVariableByID(sourceConstraint.getSourceId());
			if (sourceConstraint.getType() == '$') {
				newC = new StabilityConstraint(srcVar);
			} else {
				if (sourceConstraint.getArity() > 1) {
					int destinationVariableID = sourceConstraint.getDestinationId().getAgentID();
					OCOPAbstractNode targetNode = newNode.getNeighborById(destinationVariableID);
					CSPVariable destVar = targetNode.getVariableByID(sourceConstraint.getDestinationId());
					if (destVar == null) {
						destVar = (CSPVariable) sourceConstraint.getTo().clone();
						targetNode.addVariable(destVar);
					}
					newC = new ValuedConstraint(srcVar, destVar);
				} else {
					newC = new ValuedConstraint(srcVar);
				}
			}
			newC.setValuations(sourceConstraint.getValuations());
			newNode.addConstraint(newC);
		}
		return newNode;
	}
	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
}
