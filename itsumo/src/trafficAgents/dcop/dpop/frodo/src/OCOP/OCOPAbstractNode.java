package frodo.OCOP;

import java.util.Random;
import java.util.Vector;

import frodo.COP.ValuedConstraint;
import frodo.CSP.BinaryConstraint;
import frodo.CSP.CSPNodeInterface;
import frodo.CSP.CSPVariable;
import frodo.CSP.ComplexValue;
import frodo.CSP.ComplexVariable;
import frodo.CSP.VariableId;
import frodo.utils.Logger;

/**
 * @author apetcu
 * 
 * To change this generated comment edit the template variable "typecomment": Window>Preferences>Java>Templates. To enable and disable the creation of
 * type comments go to Window>Preferences>Java>Code Generation.
 */
public abstract class OCOPAbstractNode implements CSPNodeInterface {

	protected OCOPAgentInterface myAgent;

	protected Logger myLogger;

	protected int id;

	protected Vector variables;

	protected Vector constraints;

	protected Vector neighbours;

	protected ComplexVariable myComplexVariable;

	protected ComplexValue myComplexValue;

	protected char label;

	//--------------------------------------------------------------------------------------------------//
	public OCOPAbstractNode(OCOPAgentInterface myA, int id, Logger myl) {
		this(myA, id, myl, new Vector(), new Vector(), new Vector());
	}

	//--------------------------------------------------------------------------------------------------//
	public OCOPAbstractNode(OCOPAgentInterface myA, int id, Logger myl, Vector vars, Vector constraints, Vector neighbs) {
		this.myAgent = myA;
		this.id = id;
		this.myLogger = myl;
		this.variables = vars;
		this.constraints = constraints;
		this.neighbours = neighbs;
		this.label = 'X';
		this.myComplexValue = new ComplexValue(new Vector());
	}

	//--------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//
	public void setId(int id) {
		this.id = id;
	}

	public int getId() {
		return id;
	}

	//--------------------------------------------------------------------------------------------------//
	//------VARIABLES-----------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//
	public void setVariables(Vector v) {
		this.variables = v;
	}

	public Vector getVariables() {
		return variables;
	}

	public void addVariable(Object v) {
		variables.add(v);
	}

	public boolean deleteVariable(Object v) {
		return variables.removeElement(v);
	}

	public CSPVariable getVariable(int i) {
		if (i < 0 || i > variables.size())
			return null;
		return (CSPVariable) variables.elementAt(i);
	}

	public CSPVariable getVariableByID(VariableId vid) {
		for (int i = 0; i < variables.size(); i++) {
			CSPVariable var = (CSPVariable) variables.elementAt(i);
			if (var.getId().equals(vid)) {
				return var;
			}
		}
		return null;
	}

	//this is just for convenience, to return the first variable; this is good in case there is actually
	// a single variable, and we don't want to bother about ids
	public CSPVariable getVariable() {
		return getVariable(0);
	}

	public int getNumberOfVariables() {
		return variables.size();
	}

	//--------------------------------------------------------------------------------------------------//
	//-------NEIGHBORS----------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//
	public void addNeighbour(Object n) {
		neighbours.add(n);
	}

	public boolean deleteNeighbour(Object n) {
		OCOPAbstractNode ngh = (OCOPAbstractNode) n;
		return deleteNeighbourById(ngh.getId());
	}

	public boolean deleteNeighbourById(int todel) {
		System.out.print("Node " + id + " deletes neighbor " + todel + " from " + neighbours.size() + " neighbors.");
		for (int i = 0; i < neighbours.size(); i++) {
			OCOPAbstractNode ngh = (OCOPAbstractNode) neighbours.elementAt(i);
			if (ngh.getId() == todel) {
				neighbours.removeElementAt(i);
				System.out.println(" Done!");
				return true;
			}
		}
		System.out.println(" Not found!!");
		return false;
	}

	public int getNumberOfNeighbours() {
		return neighbours.size();
	}

	//--------------------------------------------------------------------------------------------------//
	//Added by gushakov

	/**
	 * Return number of neighbors connected by a specific type constraint.
	 */
	public int getNumberOfNeighboursByConstraintType(char type) {
		int count = 0;
		for (int i = 0; i < this.getNumberOfNeighbours(); i++) {
			//Figure out what sort of constraint this is.
			//Consider only first binary constraint between neighbours.
			int neighbourId = this.getNeighborByIndex(i).getId();
			Vector v = getConstraintsWithNeighbor(neighbourId);
			BinaryConstraint bc = (v.size() > 0) ? (BinaryConstraint) v.elementAt(0) : null;
			if (bc != null && bc.getType() == type) {
				count++;
			}
		}
		return count;
	}

	/**
	 * Rearranges randomly the order of neighbours of this node.
	 * This method can be used for random DFS traversals from the same root,
	 * or to clear the imposed order of previous traversals.
	 */
	public void randomizeNeighbourOrder() {
		if (getNumberOfNeighbours() > 1) {
			// Seed the random number generator differently for each node.
			Random rnd = new Random(System.currentTimeMillis() * getId());
			for (int i = 0; i < neighbours.size(); i++) {
				Object nextNeighbour = neighbours.remove(i);
				neighbours.insertElementAt(nextNeighbour, rnd.nextInt(neighbours.size()));
			}
		}
	}

	//--------------------------------------------------------------------------------------------------//

	/**
	 * Method getNeighbour. returns the neighbor with the specified id, or null if not found. Note that this is not the i-th neighbor!!!!
	 * 
	 * @param id
	 * @return OCOPAbstractNode
	 */
	public OCOPAbstractNode getNeighborByIndex(int id) {
		OCOPAbstractNode ngh = (OCOPAbstractNode) neighbours.elementAt(id);
		return ngh;
	}

	/**
	 * @param id
	 * @return the neighbor of this node that has the specified ID, or null if there is no neighbor with this ID
	 */
	public OCOPAbstractNode getNeighborById(int id) {
		int cnt = 0;
		while (cnt < getNumberOfNeighbours()) {
			// now searching through the known values (external ones) for the incoming one
			OCOPAbstractNode ngh = (OCOPAbstractNode) neighbours.elementAt(cnt);
			if (ngh.getId() == id)
				return ngh;
			cnt++;
		}
		if (myLogger != null) {
			myLogger.println("Neighbor " + id + " not found. returning null", 2);
			myLogger.println("Currently, only these neighbours are available: " + neighbours, 2);
		}
		return null;
	}

	public Vector getNeighbours() {
		return neighbours;
	}

	//------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------//
	//---------CONSTRAINTS----------------------------------------------------------//
	//------------------------------------------------------------------------------//
	public void addConstraint(Object bc) {
		constraints.add(bc);
	}

	public boolean deleteConstraint(Object bc) {
		return constraints.removeElement(bc);
	}

	public ValuedConstraint getConstraint(int i) {
		return (ValuedConstraint) constraints.elementAt(i);
	}

	public int getNumberOfConstraints() {
		return constraints.size();
	}

	//--------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//
	/**
	 * @return Returns the myAgent.
	 */
	public OCOPAgentInterface getAgent() {
		return myAgent;
	}

	/**
	 * @param myAgent
	 *            The myAgent to set.
	 */
	public void setAgent(OCOPAgentInterface myAgent) {
		this.myAgent = myAgent;
	}

	//--------------------------------------------------------------------------------------------------//
	/**
	 * @return Returns the myLogger.
	 */
	public Logger getLogger() {
		return myLogger;
	}

	//--------------------------------------------------------------------------------------------------//
	/**
	 * @param myLogger
	 *            The myLogger to set.
	 */
	public void setLogger(Logger myLogger) {
		this.myLogger = myLogger;
	}

	//--------------------------------------------------------------------------------------------------//
	/**
	 * @return Returns the constraints.
	 */
	public Vector getConstraints() {
		return constraints;
	}

	//--------------------------------------------------------------------------------------------------//
	public Vector getUnaryConstraints() {
		return getNaryConstraints(1);
	}

	//--------------------------------------------------------------------------------------------------//
	public Vector getBinaryConstraints() {
		return getNaryConstraints(2);
	}

	//--------------------------------------------------------------------------------------------------//
	/**
	 * @param n
	 * @return returns a vector with the constraints that have the specified arity
	 */
	public Vector getNaryConstraints(int n) {
		Vector res = new Vector();
		for (int j = 0; j < constraints.size(); j++) {
			BinaryConstraint bc = getConstraint(j);
			if (bc.getArity() == n)
				res.add(bc);
		}
		return res;
	}

	//--------------------------------------------------------------------------------------------------//
	/**
	 * @param neighborId
	 * @return a vector with the constraints between this node and its neighborId neighbor. Normally this is just a single, binary constraint. 
	 */
	public Vector getConstraintsWithNeighbor(int neighborId) {
		Vector result = new Vector();
		for (int i = 0; i < getNumberOfConstraints(); i++) {
			ValuedConstraint vc = (ValuedConstraint) constraints.elementAt(i);
			if (vc.getDestinationId().getAgentID() == neighborId) {
				result.add(vc);
			}
		}
		return result;
	}

	//--------------------------------------------------------------------------------------------------//

	public ComplexValue getCurrentAssignments() {
		myComplexValue.getValues().removeAllElements();
		for (int j = 0; j < variables.size(); j++) {
			//            myComplexValue.setITHValue(getVariable(j).getAssignment(), j);
			myComplexValue.addValue(getVariable(j).getAssignment());
		}
		return myComplexValue;
	}

	//--------------------------------------------------------------------------------------------------//
	public void setCurrentAssignments(ComplexValue asss) {
		for (int j = 0; j < variables.size(); j++) {
			getVariable(j).setValue(asss.getITHIntegerValue(j));
		}
		myComplexValue = asss;
	}

	//--------------------------------------------------------------------------------------------------//
	public int getEvaluationUnaryConstraints(ComplexValue value) {
		int sum = 0;
		Vector ucs = getUnaryConstraints();
		ComplexValue bak_ass = getCurrentAssignments();
		setCurrentAssignments(value);
		for (int j = 0; j < ucs.size(); j++) {
			ValuedConstraint bc = (ValuedConstraint) ucs.elementAt(j);
			sum += bc.getValuation();
		}
		setCurrentAssignments(bak_ass);
		return sum;
	}

	//--------------------------------------------------------------------------------------------------//
	public int getEvaluationUnaryConstraints() {
		return this.getEvaluationUnaryConstraints(getCurrentAssignments());
	}

	//--------------------------------------------------------------------------------------------------//

	public void setNeighbours(Vector neighbours) {
		this.neighbours = neighbours;
	}

	//------------------------------------------------------------//

	public void removeDisconnectedVariables() {
		Vector disco = new Vector();
		for (int i = 0; i < variables.size(); i++) {
			if (getVariable(i).getNumberOfConstraints() == 0) {
				disco.add(getVariable(i));
			}
		}
		System.out.println("Removing variables: " + disco.toString());
		variables.removeAll(disco);
	}

	//--------------------------------------------------------------------------------------------------//
	public boolean equals(Object other) {
		if (other != null && other.getClass().equals(this.getClass())) {
			OCOPAbstractNode tocomp = (OCOPAbstractNode) other;
			//            System.out.println("XXXXXXXXXXX comparing XXXXXXXX " + id + " vs " + tocomp.getId());
			if (id == tocomp.getId())
				return true;
		}
		return false;
	}

	//--------------------------------------------------------------------------------------------------//

	public void setConstraints(Vector constraints) {
		this.constraints = constraints;
	}

	/*
	 public ComplexVariable getComplexVariable() {
	 return myComplexVariable;
	 }
	 */
	public ComplexValue getComplexValue() {
		return myComplexVariable.getValue();
	}

	//--------------------------------------------------------------------------------------------------//
	public String toStringSerialize() {
		StringBuffer rez = new StringBuffer();
		if (getNumberOfVariables() > 0) {
			for (int i = 0; i < getNumberOfVariables() - 1; i++)
				rez.append(label + getVariable(i).toStringSerialize() + "\n");
			try {
				//		System.out.println("Serializing node " + id + "-" + variables.size());
				rez.append(label + getVariable(getNumberOfVariables() - 1).toStringSerialize());
			} catch (Exception e) {
				System.out.println("Problem occured while serializing node " + id + "-" + variables.size());
				e.printStackTrace();
			}
		}
		return rez.toString();
	}

	//--------------------------------------------------------------------------------------------------//
	public String toStringSerializeFull() {
		StringBuffer rez = new StringBuffer(this.toStringSerialize() + "\n");
		int constrSz = getNumberOfConstraints();
		for (int i = 0; i < constrSz; i++) {
			rez.append((getConstraint(i).toStringSerialize() + "\n"));
		}
		return rez.toString();
	}

	public String toString() {
		return id + "";
	}

	public char getLabel() {
		return label;
	}

	public void setLabel(char label) {
		this.label = label;
	}

	public abstract Object clone();
	
	public abstract OCOPAbstractNode getNewInstance(int id);
}
//--------------------------------------------------------------------------------------------------//
