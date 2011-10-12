/*
 * Created on Jun 5, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.communication;

import java.util.Vector;

import frodo.CSP.CSPAgentInterface;


/**
 * @author apetcu
 * 
 * TODO To change the template for this generated type comment go to Window - Preferences - Java - Code Style - Code Templates
 */

public interface DistributedAgentInterface extends CSPAgentInterface {
	//	public int processMessage(Message msg);
	public void terminate(String message);

	/**
	 * Method getMessageQueue. returns the message queue responsible for that particular type of message (no)
	 * 
	 * @param no
	 * @return MessageQueue
	 */
	public MessageQueue getMessageQueue(int no);

	/**
	 * Method getEnvironment. gets a reference to the environment that the agent is running in
	 * 
	 * @return Environment
	 * 
	 * @uml.property name="environment"
	 * @uml.associationEnd 
	 * @uml.property name="environment" multiplicity="(0 1)"
	 */
	public Environment getEnvironment();

	/**
	 * @return This method returns a Vector with the id's of all the neighbors of this node
	 */
	public Vector getNeighboursIDs();

}
