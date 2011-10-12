/*
 * Created on Jun 20, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.OCOP.graph;

import java.io.Serializable;
import java.util.Hashtable;
import java.util.Iterator;

import frodo.CSP.ContextElement;
import frodo.OCOP.MessageContext;
import frodo.OCOP.OCOPAbstractNode;


/**
 * @author apetcu
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class NeighborGroup implements Serializable {
	private static final long serialVersionUID = 1L;

	MessageContext context;

	Hashtable neighbors;

	int receivedMessages;

	public NeighborGroup(MessageContext mc, Hashtable nghs) {
		this.context = mc;
		this.neighbors = nghs;
	}

	public NeighborGroup(MessageContext mc) {
		this(mc, new Hashtable());
	}

	public NeighborGroup() {
		this(new MessageContext(), new Hashtable());
	}

	public OCOPAbstractNode getNeighborByIndex(int i) {
		return (OCOPAbstractNode) neighbors.values().toArray()[i];
	}

	/**
	 * @return Returns the context.
	 */
	public MessageContext getContext() {
		return context;
	}

	/**
	 * @param context The context to set.
	 */
	public void setContext(MessageContext context) {
		this.context = context;
	}

	/**
	 * @return Returns the neighbors.
	 */
	public Hashtable getNeighbors() {
		return neighbors;
	}

	/**
	 * @param neighbors The neighbors to set.
	 */
	public void setNeighbors(Hashtable neighbors) {
		this.neighbors = neighbors;
	}

	public void addNeighbor(OCOPAbstractNode nd) {
		neighbors.put(new Integer(nd.getId()), nd);
	}

	public String toString() {
		StringBuffer sb = new StringBuffer();
		if (context == null)
			sb.append("[]: ");
		else
			sb.append(context.toString() + ": ");
		for (Iterator iter = neighbors.values().iterator(); iter.hasNext();) {
			OCOPAbstractNode element = (OCOPAbstractNode) iter.next();
			sb.append(element.getId() + " ");

		}
		return sb.toString();
	}

	public boolean containsNeighbor(int id) {
		return neighbors.containsKey(new Integer(id));
	}

	public OCOPAbstractNode findNeighborById(int id) {
		return (OCOPAbstractNode) neighbors.get(new Integer(id));
	}

	public boolean containsInContext(ContextElement mc) {
		return context.containsVariable(mc.getVariableId());
	}

	/**
	 * @return Returns the receivedMessages.
	 * 
	 * @uml.property name="receivedMessages"
	 */
	public int getReceivedMessages() {
		return receivedMessages;
	}

	/**
	 * @param receivedMessages The receivedMessages to set.
	 * 
	 * @uml.property name="receivedMessages"
	 */
	public void setReceivedMessages(int receivedMessages) {
		this.receivedMessages = receivedMessages;
	}

	/**
	 * @return
	 */
	public int getSize() {
		return neighbors.size();
	}

}