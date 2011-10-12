/*
 * CSPAbstractVariable.java
 *
 * Created on October 18, 2002, 3:44 PM
 */

package frodo.CSP;

import java.io.Serializable;
import java.util.Vector;

import frodo.COP.ValuedConstraint;

/**
 * @author apetcu
 */
public class CSPVariable implements Serializable, Cloneable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	protected Vector constraints;

	protected Vector domain;

	protected VariableId myID;

	//	protected Integer myValue;

	protected Assignment assignment;

	//-----------------------------------------------------------------------//
	public CSPVariable(VariableId vid) {
		this(vid, new Vector(), new Vector(), null);
	}

	//-----------------------------------------------------------------------//
	public CSPVariable(VariableId vid, Vector dom) {
		this(vid, dom, new Vector(), null);
	}

	//-----------------------------------------------------------------------//
	public CSPVariable(VariableId vid, Integer myv) {
		this(vid, new Vector(), new Vector(), myv);
	}

	//-----------------------------------------------------------------------//
	/** Creates a new instance of CSPAbstractVariable */
	public CSPVariable(VariableId vid, Vector domain, Vector constr) {
		this(vid, domain, constr, null);
	}

	//-----------------------------------------------------------------------//
	public CSPVariable(VariableId vid, Vector domain, Vector constr, Integer val) {
		this.myID = vid;
		if (domain == null)
			domain = new Vector();
		else
			this.domain = domain;
		if (constr == null)
			constraints = new Vector();
		else
			constraints = constr;
		assignment = new Assignment(vid, val);
	}

	//-----------------------------------------------------------------------//
	public void addConstraint(BinaryConstraint bc){
		constraints.add(bc);
	}

	//	------------------------------------------------------------------------------------------------------------------------------//

	public void addValue(Object nv){
		domain.add(nv);
	}

	public void deleteConstraint(BinaryConstraint bc){
		constraints.remove(bc);
	}

	/**
	 * @param varId
	 * @return the index of the constraint that points to the specified variableId, or -1 in case no such constraint was found
	 */
	//	------------------------------------------------------------------------------------------------------------------------------//
	public int findConstraint(VariableId varId){
		int rez = -1;
		boolean found = false;
		int i = 0;
		try {
			while (!found && i < constraints.size()) {
				BinaryConstraint constr = getConstraint(i);
				if (constr.getDestinationId().equals(varId)) { //I am checking only in the destination id because I am certain that the constraint is FROM this
					// variable TO the other one, and not the other way around
					rez = i;
					found = true;
				}
				i++;
			}
			if (!found) {
				//				System.out.println("Could not find constraint pointing to " + varId + " in " + this.toString());
			}
		} catch (Exception e) {
			System.out.println("Exception while searching in X" + myID + " for " + varId);
		}
		return rez;
	}

	//------------------------------------------------------------------------------------------------------------------------------//
	public int findValue(Object val){
		int rez = -1;
		boolean found = false;
		int i = 0;
		try {
			while (!found && i < domain.size()) {
				if (val.equals(domain.elementAt(i))) {
					rez = i;
					found = true;
				}
				i++;
			}
			if (!found) {
				System.out.println("Could not find value " + val + " in " + this.toString());
				//				System.exit(0);
				return -1;
			}
		} catch (Exception e) {
			System.out.println("Exception while searching in X" + myID + " for " + val);
		}
		return rez;
	}

	public BinaryConstraint getConstraint(int i){
		return (BinaryConstraint) constraints.elementAt(i);
	}

	//	------------------------------------------------------------------------------------------------------------------------------//
	/**
	 * @return
	 * 
	 * @uml.property name="constraints"
	 */
	public Vector getConstraints(){
		return constraints;
	}

	/**
	 * 
	 * @uml.property name="domain"
	 */
	//-----------------------------------------------------------------------//
	public Vector getDomain(){
		return domain;
	}

	//-----------------------------------------------------------------------//
	public int getDomainSize(){
		return domain.size();
	}

	//-----------------------------------------------------------------------//
	public String getDomainString(){
		return new String(myID.toString() + domain);
	}

	//-----------------------------------------------------------------------//
	public VariableId getId(){
		return myID;
	}

	//-----------------------------------------------------------------------//
	public int getNumberOfConstraints(){
		return constraints.size();
	}

	public Integer getValue(){
		return assignment.getValue();
	}

	//------------------------------------------------------------------------------------------------------------------------------//
	public Assignment getAssignment(){
		return assignment;
	}

	//------------------------------------------------------------------------------------------------------------------------------//
	public Integer getValue(int i){
		return (Integer) domain.elementAt(i);
	}

	//	------------------------------------------------------------------------------------------------------------------------------//
	//	------------------------------------------------------------------------------------------------------------------------------//
	public boolean hasConstraintsWith(VariableId vid){
		if (findConstraint(vid) < 0)
			return false;
		else
			return true;
	}

	//	------------------------------------------------------------------------------------------------------------------------------//
	public void removeConstraint(int id){
		constraints.remove(id);
	}

	//	------------------------------------------------------------------------------------------------------------------------------//
	//	------------------------------------------------------------------------------------------------------------------------------//
	/**
	 * @param vector
	 */
	public void setConstraints(Vector vector){
		constraints = vector;
	}

	public void setDomain(Vector dom){
		domain = dom;
	}

	//	------------------------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------//
	public void setValue(Integer v){
		assignment.value = v;
	}

	//-----------------------------------------------------------------------//
	public String toString(){
		return assignment.toString();
	}

	//-----------------------------------------------------------------------//
	public String toStringSerialize(){
		StringBuffer val = new StringBuffer(myID.toString());
		val.append("(" + domain.size() + ") ");
		for (int i = 0; i < domain.size() - 1; i++) {
			val.append(domain.elementAt(i) + " ");
		}
		val.append(domain.lastElement());
		return val.toString();
	}

	//-----------------------------------------------------------------------//
	public String toStringWithConstraints(){
		String line = new String();
		//		line = toStringWithDomains();
		//		line = line + "\n";
		for (int i = 0; i < constraints.size(); i++)
			line += getConstraint(i).toString();
		return line;
	}

	//------------------------------------------------------------------------------------------------------------------------------//
	public static CSPVariable fromString(String str){
		CSPVariable cvar = null;
		str = str.trim();
		if (!str.equalsIgnoreCase("")) {
			//			System.out.println("Gonna build variable from:|" + str + "|");
			String[] parts = str.split("\\s");
			VariableId vid = VariableId.fromString(parts[0]);
			//			System.out.println(vid);

			if (vid != null) {
				cvar = new CSPVariable(vid);
				for (int i = 1; i < parts.length; i++)
					cvar.addValue(new Integer(Integer.parseInt(parts[i])));
			}
		}
		return cvar;
	}

	//--------------------------------------------------------------------------------------------------//
	public boolean equals(Object other){
		if (other != null && other.getClass().equals(this.getClass())) {
			CSPVariable tocomp = (CSPVariable) other;
			//            System.out.println("XXXXXXXXXXX comparing XXXXXXXX " + id + " vs " + tocomp.getId());
			if (myID.equals(tocomp.getId()))
				return true;
		}
		return false;
	}

	//--------------------------------------------------------------------------------------------------//

	public void setAssignment(Assignment assignment){
		this.assignment = assignment;
	}

	//--------------------------------------------------------------------------------------------------//
	public Vector getConstraintsWith(VariableId vid, int ownerId){
		Vector rez = new Vector();
		for (int i = 0; i < constraints.size(); i++) {
			ValuedConstraint constr = (ValuedConstraint) getConstraint(i);
			//I am checking only in the destination id because I am certain that the constraint is FROM this
			// variable TO the other one, and not the other way around
			if (constr.getDestinationId().equals(vid) && constr.getOwnerAgent().getAgentId() == ownerId) {
				rez.add(constr);
			}
		}
		return rez;
	}

	//--------------------------------------------------------------------------------------------------//
	public Object clone(){
		CSPVariable clona = new CSPVariable((VariableId) this.getId().clone());
		clona.setAssignment((Assignment) this.assignment.clone());
		clona.setDomain((Vector) domain.clone());
		clona.setConstraints((Vector) constraints.clone());
		return clona;
	}

}
//--------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------//
