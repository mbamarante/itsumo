package frodo.CSP;

import java.io.Serializable;

import frodo.CSP.CSPVariable;

/**
 * This abstract class represents a Binary constraint and can be extended to make a new BinaryConstraint class.
 */

public abstract class BinaryConstraint implements Serializable, Cloneable {

	protected CSPVariable from;

	protected CSPVariable to;

	protected int arity;

	CSPAgentInterface ownerAgent;

	int ownerAgentId;

	boolean active;

	char type;

	/**
	 * Creates a new BinaryConstraint object.
	 * 
	 * @return a new <code>BinaryConstraint</code> object.
	 * 
	 */
	public BinaryConstraint(CSPVariable from, CSPVariable to) {
		this.from = from;
		this.to = to;
		arity = 2;
		active = true;
		ownerAgentId = -1;
	}

	/**
	 * Returns the type of the BinaryConstraint.
	 * 
	 * @return the type of the <code>BinaryConstraint</code>.
	 * 
	 * @uml.property name="type"
	 */
	/**
	 * @return
	 */
	public boolean isActive() {
		return active;
	}

	/**
	 * @param active
	 * 
	 * @uml.property name="active"
	 */
	public void setActive(boolean active) {
		this.active = active;
	}

	public char getType() {
		return type;
	}

	/**
	 * 
	 * @uml.property name="from"
	 */
	//	--------------------------------------------------------------------------------------------------------------//
	public CSPVariable getFrom() {
		return from;
	}

	/**
	 * 
	 * @uml.property name="to"
	 */
	public CSPVariable getTo() {
		return to;
	}

	/**
	 * @param from
	 *            The from to set.
	 */
	public void setFrom(CSPVariable from) {
		this.from = from;
	}

	/**
	 * @param to
	 *            The to to set.
	 */
	public void setTo(CSPVariable to) {
		this.to = to;
	}

	//--------------------------------------------------------------------------------------------------------------//
	public VariableId getSourceId() {
		return from.getId();
	}

	public VariableId getDestinationId() {
		return to.getId();
	}

	/**
	 * 
	 * @uml.property name="arity"
	 */
	//	--------------------------------------------------------------------------------------------------------------//
	public int getArity() {
		return arity;
	}

	/**
	 * 
	 * @uml.property name="arity"
	 */
	public void setArity(int arity) {
		this.arity = arity;
	}

	//	--------------------------------------------------------------------------------------------------------------//
	/**
	 * @return the valuation of the constraint with the current assignments
	 */
	public abstract int getValuation();

	/**
	 * @return the valuation of the constraint
	 */
	public abstract int getValuation(Object v1, Object v2);

	public abstract int getValuation(Object v1);

	//--------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------//
	public String toString() {
		return new String("<>[" + from.toString() + "," + to.toString() + "]");
	}

	//--------------------------------------------------------------------------------------------------------------//
	public String toStringSerialize() {
		//this only prints the id of the agent
		//		StringBuffer rez = new StringBuffer(from.getID().getAgentID() + "<->" + to.getID().getAgentID() + "\t");
		//this prints the full ID
		StringBuffer rez = new StringBuffer();
		if (getOwnerAgentId() == -1)
			rez.append("0\t");
		else
			rez.append(ownerAgentId + "\t");
		rez.append(from.getId() + "@" + getType() + "@" + to.getId());
		for (int i = 0; i < from.getDomainSize(); i++) {
			Object fromVal = from.getValue(i);
			if (arity == 1) {
				rez.append("\t" + fromVal + "," + fromVal + ":" + getValuation(fromVal) + "");
			} else if (arity == 2) {
				for (int j = 0; j < to.getDomainSize(); j++) {
					Object toVal = to.getValue(j);
					rez.append("\t" + fromVal + "," + toVal + ":" + getValuation(fromVal, toVal) + "");
				}
			}
		}
		return rez.toString();
	}

	//	--------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------//
	//	--------------------------------------------------------------------------------------------------------------//
	/**
	 * @return
	 * 
	 * @uml.property name="ownerAgent"
	 */
	public CSPAgentInterface getOwnerAgent() {
		return ownerAgent;
	}

	/**
	 * @param ownerAgent
	 * 
	 * @uml.property name="ownerAgent"
	 */
	public void setOwnerAgent(CSPAgentInterface owner) {
		this.ownerAgent = owner;
	}

	public int getOwnerAgentId() {
		return ownerAgentId;
	}

	public void setOwnerAgentId(int ownerAgentId) {
		this.ownerAgentId = ownerAgentId;
	}

	public void setType(char type) {
		this.type = type;
	}

//	public Object clone() {
//		BinaryConstraint clona = new BinaryConstraint();
//		return clona;
//	}
}

// BinaryConstraint
