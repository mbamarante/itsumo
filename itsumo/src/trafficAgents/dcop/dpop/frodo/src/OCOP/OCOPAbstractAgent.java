/*
 * Created on Oct 6, 2003
 *
 * To change the template for this generated file go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
package frodo.OCOP;

import java.io.Serializable;
import java.util.Vector;

import frodo.CSP.CSPNodeInterface;
import frodo.OCOP.communication.Environment;
import frodo.communication.MessageDispatcher;
import frodo.communication.MessageQueue;
import frodo.communication.MessageSender;
import frodo.utils.HelperUtilities;
import frodo.utils.Logger;

/**
 * @author apetcu
 * 
 * 
 */

public abstract class OCOPAbstractAgent extends Thread implements Serializable, OCOPAgentInterface {
	protected Environment environment;

	protected OCOPAbstractNode node;

	protected int id; // my index (1)

	protected Logger logger; //my logging device (file, usually called "myName.log")

	//message handling
	protected MessageDispatcher myMailMan;

	protected MessageQueue myMailQueue;

	//this guy is going to send the outgoing messages for me
	protected MessageSender sender;

	//------------------------------------------------------------------------------------------------------------------------------//

	public OCOPAbstractAgent(Environment env, int id) {
		super("Agent" + id);
		this.id = id;
		if (env != null) {
			this.environment = env;
			myMailMan = environment.getMessageDispatcher();
			logger = new Logger("logs/" + getMyName() + ".log", environment.getLogMessages());
			myMailQueue = new MessageQueue();
		}
		sender = new MessageSender(this);
	}

	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	public abstract void run();
	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//

	/**
	 * @return Returns the environment.
	 */
	public frodo.communication.Environment getEnvironment(){
		return environment;
	}

	/**
	 * @return Returns the id.
	 * 
	 * @uml.property name="id"
	 */
	public int getAgentId(){
		return id;
	}

	/**
	 * @return Returns the logger.
	 * 
	 * @uml.property name="logger"
	 */
	public Logger getLogger(){
		return logger;
	}

	/**
	 * @return Returns the myName.
	 */
	public String getMyName(){
		return "Agent" + id;
	}

	/**
	 * @return Returns the node.
	 */

	/*
	 * (non-Javadoc)
	 * 
	 * @see testbed.OCOP.OCOPAgentInterface#displayMessage(java.lang.String)
	 */
	public void displayMessage(String s){
		System.out.println(s);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see testbed.OCOP.OCOPAgentInterface#getMessageQueue(int)
	 */
	public abstract MessageQueue getMessageQueue(int no);

	/**
	 * @param utils
	 * @param senderId
	 */

	/*
	 * (non-Javadoc)
	 * 
	 * @see testbed.communication.DistributedAgentInterface#terminate(java.lang.String)
	 */
	public void terminate(String message){
		logger.println("Agent" + id + " terminates: " + message);
		environment.terminate("Agent" + id + " terminates:\n" + HelperUtilities.indent(message, 2));
		//		System.exit(1);
	}

	//	---------------------------------------------------------------------------------------------//
	//	---------------------------------------------------------------------------------------------//
	//	---------------------------------------------------------------------------------------------//
	public void setIsUnsolvable(boolean isUnsolvable){
		//this just comes from the CSP interface.
	}

	public boolean isUnsolvable(){
		//this just comes from the CSP interface.
		return false;
	}

	public CSPNodeInterface getCSPNode(){
		//this just comes from the CSP interface.
		return this.getOCOPNode();
	}

	//---------------------------------------------------------------------------------------------//
	public boolean equals(OCOPAbstractAgent ag){
		if (ag.hashCode() == this.hashCode())
			return true;
		else
			return false;
	}

	//---------------------------------------------------------------------------------------------//
	public int hashCode(){
		return getAgentId();
	}

	//---------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//

	/**
	 * @return Returns the node.
	 */
	public OCOPAbstractNode getOCOPNode(){
		return node;
	}

	/**
	 * @param node The node to set.
	 */
	public void setNode(OCOPAbstractNode node){
		this.node = node;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see testbed.communication.DistributedAgentInterface#getNumberOfNeighbours()
	 */
	public int getNumberOfNeighbors(){
		return node.getNumberOfNeighbours();
	}

	//-------------------------------------------------------------------------------------------------------------//
	/*
	 * (non-Javadoc)
	 * 
	 * @see testbed.OCOP.OCOPAbstractAgent#getNeighboursIDs()
	 */
	/* (non-Javadoc)
	 * @see testbed.communication.DistributedAgentInterface#getNeighboursIDs()
	 */
	public Vector getNeighboursIDs(){
		Vector ids = new Vector();
		for (int i = 0; i < node.getNumberOfNeighbours(); i++) {
			ids.add(new Integer(node.getNeighborByIndex(i).getId()));
		}
		return ids;
	}

	public MessageDispatcher getMyMailMan(){
		return myMailMan;
	}

	public void setMyMailMan(MessageDispatcher myMailMan){
		this.myMailMan = myMailMan;
	}
}