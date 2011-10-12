/*
 * Created on Jan 26, 2006 by apetcu
 * Project Public
 * Package frodo.OCOP
 */
package frodo.OCOP.graph;

import java.util.Vector;

import frodo.OCOP.DFSNode;
import frodo.OCOP.MessageContext;
import frodo.OCOP.communication.TopoProbe;

/**
 * @author apetcu
 *  This is the data structure that contains the DFS-related lists: parent/children, pseudo parent/children
 *  A node that extends the DFS-node type will have a data structure like this.
 *  This structure is used by and filled by any DFS-traversal method (in particular the centralized ones from DFSMaster, or the distributed one from Distributed DFS builder    
 */
public class DFSData implements Cloneable {
	protected DFSNode parent;

	protected NeighborGroup pseudoparents;

	protected NeighborGroup pseudochildren;

	protected NeighborGroup children;

	//this is used by a node to denote through which one of its neighbors should it route a message for a desired neighbor 
	protected int routerId = -1;

	private boolean pseudotreeRoot = false;

	protected TopoProbe pathFromRoot;

	protected MessageContext separator;

	protected Vector childrenThatSent;

	private long UTIL_SIZE;

	private int width;

	/**
	 * 
	 */
	public DFSData() {
		this(null, new NeighborGroup(), new NeighborGroup(), new NeighborGroup());
	}

	/**
	 * @param parent
	 * @param pseudoparents
	 * @param pseudochildren
	 * @param children
	 * @param treeLevel
	 */
	public DFSData(DFSNode parent, NeighborGroup pseudoparents, NeighborGroup pseudochildren, NeighborGroup children) {
		super();
		this.parent = parent;
		this.pseudoparents = pseudoparents;
		this.pseudochildren = pseudochildren;
		this.children = children;
		this.pathFromRoot = new TopoProbe();
		this.separator = new MessageContext();
		this.childrenThatSent = new Vector();
	}

	/**
	 * @return The set of children of the current node
	 */
	public NeighborGroup getChildren(){
		return children;
	}

	/**
	 * @param children -  The set of children of the current node
	 */
	public void setChildren(NeighborGroup children){
		this.children = children;
	}

	/**
	 * @return The parent of the current node
	 */
	public DFSNode getParent(){
		return parent;
	}

	public void setParent(DFSNode parent){
		this.parent = parent;
	}

	/**
	 * @return  The set of pseudo-children of the current node
	 */
	public NeighborGroup getPseudochildren(){
		return pseudochildren;
	}

	/**
	 * @param pseudochildren - The set of pseudochildren of the current node
	 */
	public void setPseudochildren(NeighborGroup pseudochildren){
		this.pseudochildren = pseudochildren;
	}

	/**
	 * @return  The set of pseudoparents of the current node
	 */
	public NeighborGroup getPseudoparents(){
		return pseudoparents;
	}

	/**
	 * @param pseudoparents - The set of pseudoparents of the current node
	 */
	public void setPseudoparents(NeighborGroup pseudoparents){
		this.pseudoparents = pseudoparents;
	}

	/**
	 * @return Returns the number of pseudoparents.
	 */
	public int getNumberOfPseudoParents(){
		return pseudoparents.getSize();
	}

	/**
	 * @return Returns the number of children.
	 */
	public int getNumberOfChildren(){
		return children.getSize();
	}

	/**
	 * @return - this is either 0 (if this node is the root) or 1 otherwise
	 */
	public int getNumberOfParents(){
		if (parent == null)
			return 0;
		else
			return 1;
	}

	/**
	 * @return the number of pseudochildren
	 */
	public int getNumberOfPseudoChildren(){
		return pseudochildren.getSize();
	}

	/**
	 * @return Returns the parent's ID.
	 */
	public int getParentID(){
		if (parent == null)
			return -1;
		else
			return parent.getId();
	}

	//--------------------------------------------------------------------------------------------------//
	public int getRouterId(){
		return routerId;
	}

	public void setRouterId(int routerId){
		this.routerId = routerId;
	}

	//	-------------------------------------------------------------------------------------------------------------//
	/**
	 * @return true if this node is designated as the pseudotree root
	 */
	public boolean isPseudotreeRoot(){
		return pseudotreeRoot;
	}

	//	-------------------------------------------------------------------------------------------------------------//
	/**
	 * @param pseudotreeRoot - set to true if this node is designated as the pseudotree root
	 */
	public void setPseudotreeRoot(boolean pseudotreeRoot){
		this.pseudotreeRoot = pseudotreeRoot;
	}

	//--------------------------------------------------------------------------------------------------//
	/**
	 * @return Returns the pathFromRoot.
	 */
	public TopoProbe getPathFromRoot(){
		return pathFromRoot;
	}

	/**
	 * @param pathFromRoot The pathFromRoot to set.
	 */
	public void setPathFromRoot(TopoProbe largestContext){
		this.pathFromRoot = largestContext;
	}

	//--------------------------------------------------------------------------------------------------//
	/**
	 * @return The set of children who have sent me a message
	 */
	public Vector getChildrenThatSent(){
		return childrenThatSent;
	}

	//--------------------------------------------------------------------------------------------------//
	/*
	 public Object clone() {
	 DFSData newd = new DFSData(parent, pseudoparents, pseudochildren, children, treeLevel);
	 newd.setPathFromRoot((TopoProbe) pathFromRoot.clone());
	 newd.setRouterId(routerId);
	 newd.childrenThatSent = (Vector) childrenThatSent.clone();
	 return newd;
	 }
	 */
	//--------------------------------------------------------------------------------------------------//
	public String toString(){
		StringBuffer rez = new StringBuffer();
		if (parent == null)
			rez.append("Parent=" + "null" + "\n");
		else
			rez.append("Parent=" + parent.getId() + "\n");
		rez.append("Pseudo parents =" + pseudoparents + "\n");
		rez.append("Pseudo children=" + pseudochildren + "\n");
		rez.append("Children	   =" + children + "\n");
		rez.append("Path from root =" + pathFromRoot + "\n");
		return rez.toString();
	}

	/**
	 * @return this is the set of nodes composed of:
	 *  1 - my parent
	 *  2 - my pseudoparents
	 *  3 - the set of ancestors which are connected with any of my descendants through a back edge
	 *  This is the context of the DPOP UTIL message that this node will send out. 
	 */
	public MessageContext getSeparator(){
		return separator;
	}

	public void setSeparator(MessageContext separator){
		this.separator = separator;
	}

	/**
	 * Reset all the data in this structure. This is equivalent to undoing all the DFS generation algorithm. 
	 */
	public void clear(){
		this.children.getNeighbors().clear();
		this.pseudochildren.getNeighbors().clear();
		this.pseudoparents.getNeighbors().clear();
		this.parent = null;
		this.pathFromRoot.clear();
		this.separator.clear();
	}

	public long getUTIL_SIZE(){
		return UTIL_SIZE;
	}

	public void setUTIL_SIZE(long util_size){
		UTIL_SIZE = util_size;
	}

	public int getWidth(){
		return width;
	}

	public void setWidth(int width){
		this.width = width;
	}

}
