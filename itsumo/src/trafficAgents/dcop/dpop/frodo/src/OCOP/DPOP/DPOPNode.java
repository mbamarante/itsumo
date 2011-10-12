package frodo.OCOP.DPOP;

import java.util.Vector;

import frodo.COP.ValuedConstraint;
import frodo.OCOP.DFSNode;
import frodo.OCOP.OCOPAbstractAgent;
import frodo.OCOP.OCOPAbstractNode;
import frodo.utils.HelperUtilities;
import frodo.utils.Logger;

/**
 * @author apetcu
 * 
 * To change this generated comment edit the template variable "typecomment": Window>Preferences>Java>Templates. To enable and disable the creation of
 * type comments go to Window>Preferences>Java>Code Generation.
 */
public class DPOPNode extends DFSNode {
	private static final long serialVersionUID = 1L;

	protected HyperCube utilMessage;

	protected Dimension dimension;

	//--------------------------------------------------------------------------------------------------//
	public DPOPNode(OCOPAbstractAgent myA, int id, Logger myl) {
		this(myA, id, myl, new Vector(), new Vector(), new Vector());
	}

	//--------------------------------------------------------------------------------------------------//
	public DPOPNode(OCOPAbstractAgent myA, int id, Logger myl, Vector vars, Vector constraints, Vector neighbs) {
		super(myA, id, myl, vars, constraints, neighbs);
//		dimension = new Dimension(id,0);
	}

	//--------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//
	/**
	 * This is called after:
	 *  1. I have received all UTIL messages from my children (ommitted if I am a leaf)
	 *  2. I have sent out my UTIL message to my parent
	 *  3. I have received the VALUE message from my parent (ommitted if I am the root) 
	 * @return the optimal value for my variable
	 */
	public Integer optimization(){
		HyperCube bigJoin = this.utilMessage;//this is the big JOIN hypercube
		myLogger.println("Big JOIN:\n" + HelperUtilities.arrayToString(bigJoin.getDimensions()));
		Dimension[] allDims = bigJoin.getDimensions();
		Dimension[] myDims = { this.getDimension() };
		Dimension toRemove[] = Dimension.setMinusDimensions(allDims, myDims);
		myLogger.println("dimensions: total=" + HelperUtilities.arrayToString(allDims) + "\tmine=" + HelperUtilities.arrayToString(myDims) + "\ttoremove="
				+ HelperUtilities.arrayToString(toRemove));
		//I slice across all dimensions but mine.
		//This depends on the fact that the indexes of these dimensions are set to the values received in the incoming VALUE message 
		HyperCube myBest = bigJoin.slice(toRemove);
		myLogger.println("Obtained section: " + myBest.toString());
		if (myBest.getNumberOfDimensions() == 1) {//this is supposed to have a single dimension now, mine!
			if (myBest.getDimensions()[0].equals(this.getDimension())) {//is this mine?
				//ok, so now I iterate through all my values, and pick the one with the highest valuation 
				int bestVal = Integer.MIN_VALUE;
				int bestNdx = -1;
				for (int i = 0; i < myBest.getTotalSize(); i++) {
					if (bestVal < myBest.getValue(i)) {
						bestVal = myBest.getValue(i);
						bestNdx = i;
					}
				}
				//so, now my best value is the one pointed by bestNdx
				//I am updating the indexes of the corresponding dimensions in the messages I received from my Children/Pseudochildren
				//this will be used later, when I will send them my VALUE message
				for (int i = 0; i < dfsData.getNumberOfChildren(); i++) {
					DPOPNode dest = (DPOPNode) dfsData.getChildren().getNeighborByIndex(i);
					Dimension[] dims = dest.getUtilMessage().getDimensions();
					Dimension md = dims[HelperUtilities.findInArray(dims, this.getDimension())];
					md.setCurrentIndex(bestNdx);
				}
				//now I will perform my own assignment, and return it
				getVariable().setValue(getVariable().getValue(bestNdx));
				myLogger.println("Obtained best: " + getVariable().getValue() + ", that is: " + getVariable().getAssignment() + " -> util = " + bestVal);
				return new Integer(bestNdx);
			}
		}
		myAgent.getEnvironment().terminate(
				"Problem occured with dimensions while optimizing: got " + HelperUtilities.arrayToString(myBest.getDimensions()) + " instead of " + this.getDimension().toString());
		return getVariable().getValue(0);
	}

	//--------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//

	/**
	 * @return Returns the utilMessage.
	 */
	public HyperCube getUtilMessage(){
		return utilMessage;
	}

	/**
	 * @param utilMessage The utilMessage to set.
	 */
	public void setUtilMessage(HyperCube utilMessage){
		this.utilMessage = utilMessage;
	}

	//--------------------------------------------------------------------------------------------------//
	/**
	 * @return my UTIL message, to be sent to my parent
	 */
	public HyperCube buildUTIL(){
		Dimension[] myDimensions = { dimension };
		HyperCube rez = buildJOIN();
		myLogger.println("Projecting " + HelperUtilities.arrayToString(myDimensions) + " out of " + HelperUtilities.arrayToString(rez.getDimensions()));
		rez = rez.project(myDimensions);
		return rez;
	}

	//--------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//
	/**
	 * This performs the join of all the pieces: UTILs from children, local relation, and relations with parent/pseudoparents
	 * @return my big JOIN hypercube
	 */
	protected HyperCube buildJOIN(){
		Dimension[] myDimensions = { dimension };
		HyperCube rez = null;

		//take parent into account
		if (dfsData.getParent() == null) {//then I am the root
			rez = new HyperCube(myDimensions);
		} else {//then I am a normal node
			Vector constrNgh = getConstraintsWithNeighbor(dfsData.getParentID());
			if (constrNgh.size() == 1) {
				ValuedConstraint vc = (ValuedConstraint) constrNgh.firstElement();
				rez = new HyperCube(vc);
				myLogger.println("Found constraint with parent=" + vc.getDestinationId() + ":\n" + rez.toStringVectors());
				//				myLogger.println("HC(constraint with " + vc.getDestinationId() + ")=\n" + rez.toString());
				//				myLogger.println("HC(constraint with " + vc.getDestinationId() + ")=\n" + rez.toStringVectors());
			} else {
				myLogger.println("Found " + constrNgh.size() + " constraints with parent=" + dfsData.getParentID() + ":\n" + constrNgh.toString());
				myAgent.getEnvironment().terminate("Found " + constrNgh.size() + " constraints with parent=" + dfsData.getParentID() + ":\n" + constrNgh.toString());
			}
		}

		//take pseudoparents into account
		for (int i = 0; i < getNumberOfConstraints(); i++) {
			ValuedConstraint vc = (ValuedConstraint) constraints.elementAt(i);
			if (dfsData.getPseudoparents().containsNeighbor(vc.getDestinationId().getAgentID())) {
				HyperCube tmpCube = new HyperCube(vc);
				myLogger.println("Found constraint with pseudoparent=" + vc.getDestinationId() + ":\n" + tmpCube.toStringVectors());
				rez = rez.join(tmpCube);
			}
		}

		//take local unary constraints into account
		for (int i = 0; i < getNumberOfConstraints(); i++) {
			ValuedConstraint vc = (ValuedConstraint) constraints.elementAt(i);
			if (vc.getArity() == 1) {
				//				myLogger.println("Found local unary constraint:\n" + vc.toString());
				HyperCube tmpCube = new HyperCube(vc);
				myLogger.println("Found local unary constraint:\n" + tmpCube.toStringVectors());
				rez = rez.join(tmpCube);
			}
		}

		//join UTIL from children
		for (int i = 0; i < dfsData.getNumberOfChildren(); i++) {
			DPOPNode child = (DPOPNode) dfsData.getChildren().getNeighborByIndex(i);
			HyperCube tmpCube = child.getUtilMessage();
			//			myLogger.println("UTIL from " + child.getId() + " = " + tmpCube.toString());
			rez = rez.join(tmpCube);
			if (!rez.isSatisfiable()) {
				myLogger.println("joining with UTIL from " + child.getId() + "-> UNSATISFIABLE!", 2);
			}
		}

		//I will save this message, since I will need it later, when the VALUE message arrives
		//i am saving the brute message since I want to find out which value to pick (section in the hypercube -> vector, then pick best)
		setUtilMessage(rez);
		return rez;
	}

	//--------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//
	public Dimension getDimension(){
		return dimension;
	}

	public void setDimension(Dimension dimension){
		this.dimension = dimension;
	}

	//--------------------------------------------------------------------------------------------------//
	public OCOPAbstractNode getNewInstance(int id){
		return new DPOPNode(null, id, null);
	}
}