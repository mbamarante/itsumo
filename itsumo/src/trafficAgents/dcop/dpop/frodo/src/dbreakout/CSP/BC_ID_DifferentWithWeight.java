/*
 * Created on Jun 8, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.dbreakout.CSP;

import frodo.CSP.BC_ID_Different;

/**
 * @author apetcu
 * 
 * TODO To change the template for this generated type comment go to Window - Preferences - Java - Code Style - Code Templates
 */
public class BC_ID_DifferentWithWeight extends BC_ID_Different {
	
	int weight;
	
	public BC_ID_DifferentWithWeight(CSPVariableDBA from, CSPVariableDBA to) {
		super(from, to);
		weight =1;
	}

	/**
	 * @param i
	 */
	public void increaseWeight(int i) {
		// TODO Auto-generated method stub
		weight++;
	}

	/**
	 * @return Returns the weight.
	 * 
	 * @uml.property name="weight"
	 */
	public int getWeight() {
		return weight;
	}

	/**
	 * @param weight The weight to set.
	 * 
	 * @uml.property name="weight"
	 */
	public void setWeight(int weight) {
		this.weight = weight;
	}

}