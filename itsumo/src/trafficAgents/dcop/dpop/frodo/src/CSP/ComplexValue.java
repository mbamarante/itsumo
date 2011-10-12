package frodo.CSP;

import java.util.Vector;

/**
 * @author apetcu
 * Gigi are mere
 * To change this generated comment edit the template variable "typecomment":
 * Window>Preferences>Java>Templates.
 * To enable and disable the creation of type comments go to
 * Window>Preferences>Java>Code Generation.
 */
public class ComplexValue {

	/**
	 *these are Assignment objects, thus they have the VarId + value. VarId also has the agent ID+local var Id 
	 */
	Vector values;

	int localCost;

	int lowerBound;

	//-----------------------------------------------------------------------//
	public ComplexValue(Vector vals, int localCost) {
		this.values = vals;
		this.localCost = localCost;
		this.lowerBound = localCost; //initialize the lower bound with the local cost (that's the least this value will cost...)
	}

	//-----------------------------------------------------------------------//
	public ComplexValue(Vector vals) {
		this(vals, 0);
	}

	//-----------------------------------------------------------------------//
	public ComplexValue(Assignment assi) {
		this();
		this.addValue(assi);
	}

	//-----------------------------------------------------------------------//
	public ComplexValue() {
		this(new Vector(), 0);
	}

	//-----------------------------------------------------------------------//
	public Integer getITHIntegerValue(int i) {
		return ((Assignment) values.elementAt(i)).getValue();
	}

	//-----------------------------------------------------------------------//
	public Assignment getITHValue(int i) {
		return (Assignment) values.elementAt(i);
	}

	//-----------------------------------------------------------------------//
	public void setITHValue(Assignment val, int i) {
		values.setElementAt(val, i);
	}

	//-----------------------------------------------------------------------//
	public void addValue(Assignment val) {
		values.add(val);
	}

	//-----------------------------------------------------------------------//
	public void clear() {
		values.clear();
	}

	//-----------------------------------------------------------------------//
	/**
	 * Returns the localCost.
	 * @return int
	 * 
	 * @uml.property name="localCost"
	 */
	public int getLocalCost() {
		return localCost;
	}

	//-----------------------------------------------------------------------//
	/**
	 * Sets the localCost.
	 * @param localCost The localCost to set
	 * 
	 * @uml.property name="localCost"
	 */
	public void setLocalCost(int cost) {
		this.localCost = cost;
	}

	//-----------------------------------------------------------------------//
	/**
	 * Returns the values.
	 * @return Vector
	 * 
	 * @uml.property name="values"
	 */
	public Vector getValues() {
		return values;
	}

	//-----------------------------------------------------------------------//
	/**
	 * Sets the values.
	 * @param values The values to set
	 * 
	 * @uml.property name="values"
	 */
	public void setValues(Vector values) {
		this.values = values;
	}

	//-----------------------------------------------------------------------//
	public String toStringWithoutIdWithCost() {
		String val = "[";
		if (values == null)
			val = "null";
		else {
			for (int i = 0; i < values.size() - 1; i++)
				val = val + values.elementAt(i).toString() + ",";
			val = val + values.lastElement().toString();
		}
		val = val + "]->" + localCost + "+" + lowerBound;
		return val;
	}

	//-----------------------------------------------------------------------//
	public String toString() {
		StringBuffer content = new StringBuffer("[");
		for (int i = 0; i < values.size() - 1; i++)
			content.append(values.elementAt(i).toString() + " ");
		content.append(values.lastElement().toString() + "]");
		return content.toString();
	}

	//-----------------------------------------------------------------------//
	public static ComplexValue fromString(String src) {
		if (src == null)
			return null;
		src.trim();
		if (src.length() == 0)
			return null;
		// this works with input like: [1.0=2 1.1=3 1.2=6]
		// this works with default cost 0
		//	System.out.println(" Building complex value from: " + src);
		ComplexValue cval = null;
		//get rid of []
		String source = src.substring(1, src.length() - 1);
		String[] parts = source.split("\\s");
		Vector values = new Vector();
		for (int i = 0; i < parts.length; i++)
			values.add(Assignment.fromString(parts[i]));
		cval = new ComplexValue(values); //this works with default cost 0
		return cval;
	}

	//-----------------------------------------------------------------------//
	//-----------------------------------------------------------------------//
	public int size() {
		return values.size();
	}

	//-----------------------------------------------------------------------//
	public boolean equals(Object to) {
		if (to != null && to.getClass().equals(this.getClass())) {
			ComplexValue other = (ComplexValue) to;
			if (size() != other.size())
				return false;
			for (int i = 0; i < other.size(); i++) {
				if (!getITHValue(i).equals(other.getITHValue(i)))
					return false;
			}
			//		System.out.println(" -> " + rez);
			return true;
		} else
			return false;
	}

	//-----------------------------------------------------------------------//
	/**
	 * @return
	 * 
	 * @uml.property name="lowerBound"
	 */
	public int getLowerBound() {
		return lowerBound;
	}

	//-----------------------------------------------------------------------//
	/**
	 * @param i
	 */
	public void setLowerBound(int i) {
		lowerBound = i;
	}

	//-----------------------------------------------------------------------//
	public void resetLowerBound() {
		lowerBound = 0;
	}

	//-----------------------------------------------------------------------//
	public Assignment getValue(VariableId varid) {
		for (int i = 0; i < values.size(); i++) {
			if (getITHValue(i).getVariableId().equals(varid))
				return getITHValue(i);
		}
		return null;
	}

	//-----------------------------------------------------------------------//
	//-----------------------------------------------------------------------//

	/**
	 * @param assignment
	 */
	public int getDistance(ComplexValue other) {
		int diff = 0;
		for (int i = 0; i < other.size(); i++) {
			Assignment cura = getITHValue(i);
			Assignment curb = other.getValue(cura.getVariableId());
			if (!cura.equals(curb))
				diff++;
		}
		return diff;
	}
	//-----------------------------------------------------------------------//
}
