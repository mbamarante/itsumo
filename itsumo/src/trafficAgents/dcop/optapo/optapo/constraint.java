package optapo;

import java.util.Vector;


public class constraint implements Cloneable{

	agent variable;
	agent otherVariable;
	Vector costVector = new Vector();
	
	public constraint(agent variable, agent otherVariable) {
		super();
		this.otherVariable = otherVariable;
		this.variable = variable;
	}
	
	public void addConstraintCost(String value, String otherValue, int cost) {
		this.costVector.add(new constraintCost(value, otherValue, cost));
	}
	
	public int getCost(String value, String otherValue) {
		for (int i = 0; i < costVector.size(); i++) {
			if (((constraintCost)costVector.elementAt(i)).value.equals(value) &&
				((constraintCost)costVector.elementAt(i)).otherValue.equals(otherValue))
			return ((constraintCost)costVector.elementAt(i)).cost;
			
		}
		return 0;
	}
	
	public String toString() {
		return variable+" | "+otherVariable+" : "+costVector+"\n";
	}
	
	public Object clone(){
		constraint temp = new constraint((agent)this.variable.clone(), (agent)this.otherVariable.clone());
		Vector t = new Vector();
		for (int i = ((Vector)this.costVector).size()-1; i >=0; i--) {
			t.addElement(((constraintCost)((Vector)this.costVector).elementAt(i)).clone());		
		}
		temp.costVector = t;
		return temp;
	}
	
}
