/*
 * Created on Apr 14, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.CSP;

import java.io.Serializable;

/**
 * @author apetcu
 */
public class Assignment implements Comparable, Cloneable, Serializable {

	//this should not be a problem, typically people solve problems with 3,4 values :))
	public static final int MAX_VALUES_PER_VARIABLE = 1000;

	private static final long serialVersionUID = 1L;

	VariableId variableId;

	Integer value;

	/**
	 * @param variableId
	 * @param value
	 */
	public Assignment(VariableId variableId, Integer value) {
		super();
		this.variableId = variableId;
		this.value = value;
	}

	public Assignment(VariableId variableId) {
		this(variableId, null);
	}

	public Integer getValue(){
		return value;
	}

	public void setValue(Integer value){
		this.value = value;
	}

	public VariableId getVariableId(){
		return variableId;
	}

	public void setVariableId(VariableId variableId){
		this.variableId = variableId;
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 * Returns the string representation: something like 1.2=9, or 1.2=*, if unassigned
	 */
	public String toString(){
		if (value == null)
			return new String(variableId.toString() + "=*");
		else
			return new String(variableId.toString() + "=" + value);
	}

	/**
	 * @param src - this should be like 1.2=9, or 1.2=*, if unassigned
	 * @return a new Assignment object
	 */
	public static Assignment fromString(String src){
		//this should be like 1.2=9
		if (src != null) {
			src.trim();
			if (src.length() > 0) {
				String[] parts = src.split("=");
				VariableId vid = VariableId.fromString(parts[0]);
				Integer value = null;
				if (parts[1].charAt(0) != '*')
					value = Integer.valueOf(parts[1]);
				return new Assignment(vid, value);
			}
		}
		return null;
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 * Returns true if the IDs and values are equal. Values can be unassigned, but then both have to be unassigned
	 */
	public boolean equals(Object to){
		if (to != null && to instanceof Assignment) {
			Assignment other = (Assignment) to;
			if (variableId.equals(other.getVariableId()))
				if ((value == null && other.getValue() == null) || value.equals(other.getValue()))
					return true;
		}
		return false;
	}

	/**
	 * @param arg0 
	 * @return a comparison of the IDs of the variables (values are ignored)
	 */
	public int compareTo(Object arg0){
		Assignment new_name = (Assignment) arg0;
		int vidcmp = variableId.compareTo(new_name.getVariableId());
		if (vidcmp == 0)
			return value.compareTo(new_name.value);
		else
			return vidcmp;
	}

	public Object clone(){
		VariableId newVid = (VariableId) this.variableId.clone();
		Assignment newAss = new Assignment(newVid, this.value);
		return newAss;
	}

	public int hashCode(){
		return variableId.hashCode() * MAX_VALUES_PER_VARIABLE + value.intValue();
	}
}
