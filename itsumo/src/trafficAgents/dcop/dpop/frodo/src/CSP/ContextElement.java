/*
 * Created on Jun 9, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.CSP;


/**
 * @author apetcu
 */
public class ContextElement extends Assignment {

	private static final long serialVersionUID = 1L;

	protected int domainSize;

	public ContextElement(VariableId vid, Integer val, int sz) {
		super(vid, val);
		this.domainSize = sz;
	}

	/**
	 * @param source is the string of the form 1.1=2/4 (variable 1.1 gets value 2 out of 4 possible ones
	 * @return a newly constructed element from the string
	 */
	public static ContextElement buildFromString(String source){
		source = source.trim();
		if (source.equalsIgnoreCase(""))
			return null;
		int domainSize = 0;
		//		System.out.println("Converting from: " + source);
		String[] parts = source.split("/");
		Assignment assi = Assignment.fromString(parts[0]);
		if (assi == null)
			return null;

		if (parts.length == 2) {
			domainSize = Integer.parseInt(parts[1]);
		}
		ContextElement rez = new ContextElement(assi.variableId, assi.value, domainSize);
		return rez;
	}

	public String toString(){
		String rez = super.toString();
		if (domainSize > 0) {
			rez += "/" + domainSize;
		}
		return rez;
	}

	/**
	 * @return Returns the domainSize.
	 * 
	 * @uml.property name="domainSize"
	 */
	public int getDomainSize(){
		return domainSize;
	}

	/**
	 * @param domainSize The domainSize to set.
	 * 
	 * @uml.property name="domainSize"
	 */
	public void setDomainSize(int domainSize){
		this.domainSize = domainSize;
	}

	public Object clone(){
		VariableId newVid = (VariableId) variableId.clone();
		return new ContextElement(newVid, value, this.domainSize);
	}
}