/*
 * Created on Jan 30, 2006 by apetcu
 * Project Public
 * Package frodo.OCOP
 */
package frodo.OCOP;

import java.util.Vector;

import frodo.COP.StabilityConstraint;
import frodo.COP.ValuedConstraint;
import frodo.CSP.CSPVariable;
import frodo.utils.Logger;

public class OCOPBasicNode extends OCOPAbstractNode {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public OCOPBasicNode(OCOPAgentInterface myA, int id, Logger myl) {
		super(myA, id, myl);
		// TODO Auto-generated constructor stub
	}

	public OCOPBasicNode(OCOPAgentInterface myA, int id, Logger myl, Vector vars, Vector constraints, Vector neighbs) {
		super(myA, id, myl, vars, constraints, neighbs);
		// TODO Auto-generated constructor stub
	}

	//	-------------------------------------------------------------------------------//
	//	-------------------------------------------------------------------------------//
	public Object clone(){
		//create the new node
		OCOPBasicNode newNode = new OCOPBasicNode(null, this.id, myLogger);
		newNode.setAgent(myAgent);
		newNode.setLabel(label);

		//adding clones of the original variables
		for (int k = 0; k < getNumberOfVariables(); k++) {
			newNode.addVariable(getVariable(k).clone());
		}

		//recreating and adding the original neighbors
		for (int j = 0; j < getNumberOfNeighbours(); j++) {
			//create the new guy
			OCOPAbstractNode srcNgh = getNeighborByIndex(j);
			OCOPBasicNode newNgh = new OCOPBasicNode(null, srcNgh.getId(), null);
			newNgh.setLabel(srcNgh.getLabel());
			newNode.addNeighbour(newNgh);//add the new guy to my neighbors
		}
		//adding the original constraints
		for (int j = 0; j < getNumberOfConstraints(); j++) {
			ValuedConstraint sourceConstraint = getConstraint(j);
			ValuedConstraint newC = null;
			//careful, the following assumes that the local constraints always have source_id as THE LOCAL VARIABLE!! 
			CSPVariable sourceVariable = newNode.getVariableByID(sourceConstraint.getSourceId());
			if (sourceConstraint.getType() == '$') {
				newC = new StabilityConstraint(sourceVariable);
			} else {
				if (sourceConstraint.getArity() > 1) {
					int destinationVariableID = sourceConstraint.getDestinationId().getAgentID();
					OCOPAbstractNode targetNode = newNode.getNeighborById(destinationVariableID);
					CSPVariable destV = targetNode.getVariableByID(sourceConstraint.getDestinationId());
					if (destV == null)
						destV = (CSPVariable) sourceConstraint.getTo().clone();
					newC = new ValuedConstraint(sourceVariable, destV);
				} else {
					newC = new ValuedConstraint(sourceVariable);
				}
			}
			newC.setValuations(sourceConstraint.getValuations());
			newNode.addConstraint(newC);
		}
		return newNode;
	}
	//-------------------------------------------------------------------------------//
	public OCOPAbstractNode getNewInstance(int id){
		return new OCOPBasicNode(null, id, null);
	}
	//------------------------------------------------------------------------------//
}
