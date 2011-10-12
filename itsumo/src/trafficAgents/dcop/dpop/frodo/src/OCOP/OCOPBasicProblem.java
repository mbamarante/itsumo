/*
 * Created on Jul 21, 2003
 *
 * To change the template for this generated file go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
package frodo.OCOP;

import java.util.Vector;

import frodo.COP.ValuedConstraint;
import frodo.CSP.CSPVariable;
import frodo.CSP.VariableId;
import frodo.utils.HelperUtilities;

/**
 * @author apetcu
 * 
 * To change the template for this generated type comment go to Window>Preferences>Java>Code Generation>Code and Comments
 */
public class OCOPBasicProblem extends OCOPAbstractProblem {

	/**
	 * 
	 */
	private static final long serialVersionUID = -3681460621301174169L;

	private Vector selfInterestedAgents;

	//-----------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------------------------------------------//
	//----AGENTS------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------------------------------------------//
	public OCOPAbstractAgent getAgent(int i) {
		return (OCOPAbstractAgent) selfInterestedAgents.elementAt(i);
	}

	//---------------------------------------------------------------------------------//
	public Vector getSelfInterestedAgents() {
		return selfInterestedAgents;
	}

	//---------------------------------------------------------------------------------//
	public SelfInterestedAgent getSelfInterestedAgent(int i) {
		return (SelfInterestedAgent) selfInterestedAgents.elementAt(i);
	}

	//---------------------------------------------------------------------------------//
	public int getNumberOfAgents() {
		return selfInterestedAgents.size();
	}

	/**
	 * @param selfInterestedAgents
	 */
	public void setSelfInterestedAgents(Vector agents) {
		this.selfInterestedAgents = agents;
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	//	public OCOPAbstractNode getMyPiece(int id, OCOPAbstractNode model){
	//		OCOPAbstractNode newNode = model.clone();
	//	}
	public OCOPBasicProblem toSingleVariablePerNode() {
		OCOPBasicProblem newproblem = new OCOPBasicProblem();

		//in this, idTranslations[k] = original id of the variable which was read at the kth step
		VariableId[] idTranslations = new VariableId[getNumberOfVariables()];

		//copying the variables into the new problem (with the ids translated) and creating new nodes (one for each var)
		int varcnt = 0;
		for (int i = 0; i < getNumberOfNodes(); i++) {
			OCOPAbstractNode nd =  getNodeByIndex(i);
			for (int j = 0; j < nd.getNumberOfVariables(); j++) {
				CSPVariable curvar = nd.getVariable(j);
				Vector domnew = new Vector(curvar.getDomain());
				CSPVariable varnew = new CSPVariable(new VariableId(varcnt, 0), domnew);
				idTranslations[varcnt] = curvar.getId();
				OCOPAbstractNode ndnew = new OCOPBasicNode(null, varcnt, null);
				ndnew.addVariable(varnew);
				newproblem.addNode(ndnew);
				newproblem.addVariable(varnew);
				varcnt++;
			}
		}
		int constrcnt = 0;
		//copying the constraints, and adding them between the translated variables
		//for each node
		for (int i = 0; i < getNumberOfNodes(); i++) {
			OCOPAbstractNode nd = getNodeByIndex(i);
			//for each variable
			for (int j = 0; j < nd.getNumberOfVariables(); j++) {
				CSPVariable curvar = nd.getVariable(j);
				//for each constraint
				for (int k = 0; k < curvar.getNumberOfConstraints(); k++) {
					ValuedConstraint bc = (ValuedConstraint) curvar.getConstraint(k);
					if (bc.getArity() > 1) {
						int srcid = HelperUtilities.findInArray(idTranslations, bc.getSourceId());
						int destid = HelperUtilities.findInArray(idTranslations, bc.getDestinationId());
						CSPVariable srcvar = newproblem.getVariable(srcid);
						CSPVariable destvar = newproblem.getVariable(destid);
						//since there is a node for each variable, srcid and destid work for both nodes and variables
						OCOPAbstractNode srcnode = newproblem.getNodeById(srcid);
						OCOPAbstractNode destnode = newproblem.getNodeById(destid);
						ValuedConstraint vc = new ValuedConstraint(srcvar, destvar);
						vc.setValuations(bc.getValuations());
						vc.setId(constrcnt);
						vc.setOwnerAgentId(bc.getOwnerAgentId());
						vc.setType(bc.getType());
						srcvar.addConstraint(vc);
						srcnode.addConstraint(vc);
						if (srcid < destid)
							newproblem.addConstraint(vc);
						if (srcnode.getNeighborById(destid) == null)
							srcnode.addNeighbour(destnode);
					} else {
						int srcid = HelperUtilities.findInArray(idTranslations, bc.getSourceId());
						CSPVariable srcvar = newproblem.getVariable(srcid);
						//since there is a node for each variable, srcid and destid work for both nodes and variables
						OCOPAbstractNode  srcnode = newproblem.getNodeById(srcid);
						ValuedConstraint vc = new ValuedConstraint(srcvar);
						vc.setValuations(bc.getValuations());
						vc.setId(constrcnt);
						vc.setOwnerAgentId(bc.getOwnerAgentId());
						vc.setType(bc.getType());
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
}

//-------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------//
