/*
 * VariableId.java
 *
 * Created on October 18, 2002, 3:45 PM
 */

package frodo.CSP;

import java.io.Serializable;

/**
 * 
 * @author apetcu
 */
public class VariableId implements Serializable, Comparable, Cloneable {
	//if one agent has more than 1000 variables, then the hashcode will not work properly
	//I hope to live long enough to see *that* becoming a limitation ;-)
	public static final int MAX_VARIABLES_PER_AGENT = 100;

	private static final long serialVersionUID = 1L;

	private int localId;

	private int myAgentId;

	/* Code modified by gushakov */
	private String label = null;

	/*----------------------------------------*/

	//-----------------------------------------------------------------------//
	/** Creates a new instance of VariableId */
	public VariableId(int agid, int loid) {
		this.myAgentId = agid;
		this.localId = loid;
	}

	//-----------------------------------------------------------------------//
	public int getLocalID(){
		return localId;
	}

	//-----------------------------------------------------------------------//
	public int getAgentID(){
		return myAgentId;
	}

	//-----------------------------------------------------------------------//
	public String toString(){
		return new String(myAgentId + "." + localId);
	}

	//-----------------------------------------------------------------------//
	public boolean equals(Object other){
		if (other != null && other.getClass().equals(this.getClass())) {
			VariableId tocomp = (VariableId) other;
			//            System.out.println("XXXXXXXXXXX comparing XXXXXXXX " + id + " vs " + tocomp.getId());
			if (tocomp.getAgentID() == myAgentId && tocomp.getLocalID() == localId)
				return true;
		}
		return false;
	}

	//-----------------------------------------------------------------------//
	/**
	 * 
	 * @throws ClassCastException
	 */
	public int compareTo(Object ref){
		//		if (ref instanceof VariableId) {
		/* no need to test instanceof, Comparable interface instructs to throw ClassCastException */
		VariableId new_name = (VariableId) ref;
		int r = myAgentId - new_name.getAgentID();
		if (r == 0)
			r = localId - new_name.getLocalID();
		return r;
		//		} else {
		//throw new ClassCastException();
		//			System.exit(-1);
		//			return -1;
		//		}
	}

	//-----------------------------------------------------------------------//

	/**
	 * @param str - the string to construct the identifier from; e.g.: N97.0(8) 0 1 2 3 4 5 6 7 (a whole variable line), or 97.0
	 * @return
	 */
	public static VariableId fromString(String str){
		str = str.trim();
		VariableId vid = null;
		if (!str.equalsIgnoreCase("")) {
			//			System.out.print("Gonna build varID from:|" + str + "|");
			String[] parts = str.split("\\s");
			String tmp = null;
			if (!Character.isDigit(parts[0].charAt(0))) {
				//when this is like N97.0(8) 0 1 2 3 4 5 6 7....
				tmp = (parts[0].substring(1).split("\\("))[0];
			} else {
				//when this is like 97.0...
				tmp = (parts[0].split("\\("))[0];
			}
			String[] idstr = tmp.split("\\.");
			int agid = Integer.parseInt(idstr[0]);
			int lid = 0;
			if (idstr.length > 1)
				lid = Integer.parseInt(idstr[1]);
			//trick to have only one variable per node
			//			agid = agid * 10 + lid;
			vid = new VariableId(agid, lid);
			//			vid = new VariableId(agid, 0);
		}
		//		System.out.println("->" + vid.toString());
		return vid;
	}

	public String getLabel(){
		return label;
	}

	public void setLabel(String somelabel){
		label = somelabel;
	}

	public Object clone(){
		VariableId newVid = new VariableId(this.myAgentId, this.localId);
		if (this.label == null)
			newVid.label = null;
		else
			newVid.label = new String(this.label);
		return newVid;
	}

	public static VariableId fromIntegers(int agentId, int varid){
		return new VariableId(agentId, varid);
	}

	public int hashCode(){
		return myAgentId * MAX_VARIABLES_PER_AGENT + localId;
	}
}