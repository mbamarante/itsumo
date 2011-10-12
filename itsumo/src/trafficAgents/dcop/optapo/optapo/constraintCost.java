package optapo;

public class constraintCost implements Cloneable{

	String value;
	String otherValue;
	int cost;
	
	public constraintCost(String value, String otherValue, int cost) {
		super();
		this.value = value;
		this.otherValue = otherValue;
		this.cost = cost;
	}
	
	public String toString(){
		return value+"+"+otherValue+"="+cost;
	}
	
	public Object clone(){
		constraintCost temp = new constraintCost(this.value, this.otherValue, this.cost);
		return temp;
	}
}
