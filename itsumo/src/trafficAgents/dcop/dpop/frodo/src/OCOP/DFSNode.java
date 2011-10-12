package frodo.OCOP;

import java.util.Vector;

import frodo.OCOP.graph.DFSData;
import frodo.utils.Logger;

/**
 * @author apetcu
 * This extends the basic OCOPNode with a DFS-related data structure and related functionality.
 * This is useful for algorithms that operate on a DFS ordering (DPOP, ODPOP, soon also ABTSimple :) )
 *
 */
public class DFSNode extends OCOPBasicNode {

	protected DFSData dfsData;

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public DFSNode(OCOPAgentInterface myA, int id, Logger myl) {
		this(myA, id, myl, new Vector(), new Vector(), new Vector());
	}

	public DFSNode(OCOPAgentInterface myA, int id, Logger myl, Vector vars, Vector constraints, Vector neighbs) {
		super(myA, id, myl, vars, constraints, neighbs);
		this.dfsData = new DFSData();
	}

	/**
	 * @return The data structure that contains the DFS-related lists: parent/children, pseudo parent/children
	 */
	public DFSData getDFSData() {
		return dfsData;
	}

	/**
	 * @param dfsData -  The data structure that contains the DFS-related lists: parent/children, pseudo parent/children
	 */
	public void setDFSData(DFSData dfsData) {
		this.dfsData = dfsData;
	}

	/**
	 * This clears all the data in the DFS data structure; it's like the DFS traversal has not been run yet. 
	 */
	public void clearDFSData() {
		this.dfsData.clear();
	}

	public Object clone() {
		OCOPBasicNode tmpNode = (OCOPBasicNode) super.clone();

		DFSNode newNode = new DFSNode(null, 0, null);
		newNode.myAgent = myAgent;
		newNode.myLogger = myLogger;
		newNode.id = id;
		newNode.label = label;
		newNode.variables = tmpNode.variables;
		newNode.constraints = tmpNode.constraints;
		newNode.neighbours = tmpNode.neighbours;
		newNode.myComplexVariable = tmpNode.myComplexVariable;
		newNode.myComplexValue = tmpNode.myComplexValue;

		//		newNode.setDFSData((DFSData) dfsData.clone());
		return newNode;
	}

	public OCOPAbstractNode getNewInstance(int id) {
		return new DFSNode(null, id, null);
	}
}
