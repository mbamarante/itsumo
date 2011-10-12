/*
 * Created on Jun 4, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.OCOP;

import java.util.Vector;

import frodo.CSP.CSPNodeInterface;
import frodo.communication.Environment;
import frodo.communication.MessageQueue;
import frodo.utils.Logger;


/**
 * @author apetcu
 * 
 * TODO To change the template for this generated type comment go to Window - Preferences - Java - Code Style - Code Templates
 */
public class SelfInterestedAgent implements OCOPAgentInterface {

	/**
	 * 
	 * @uml.property name="id"
	 */
	private int id;

	/**
	 * 
	 * @uml.property name="constraints"
	 */
	protected Vector constraints;

	public SelfInterestedAgent(int id) {
		this(id, new Vector());
	}

	public SelfInterestedAgent(int id, Vector constr) {
		this.id = id;
		this.constraints = constr;
	}

	/**
	 * @return Returns the id.
	 * 
	 * @uml.property name="id"
	 */
	public int getAgentId() {
		return id;
	}

	/**
	 * @param id
	 *            The id to set.
	 * 
	 * @uml.property name="id"
	 */
	public void setId(int id) {
		this.id = id;
	}

	/**
	 * @return Returns the constraints.
	 * 
	 * @uml.property name="constraints"
	 */
	public Vector getConstraints() {
		return constraints;
	}

	/**
	 * @param constraints
	 *            The constraints to set.
	 * 
	 * @uml.property name="constraints"
	 */
	public void setConstraints(Vector constraints) {
		this.constraints = constraints;
	}

	/* (non-Javadoc)
	 * @see testbed.CSP.OCOPAgentInterface#getLogger()
	 */
	public Logger getLogger() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see testbed.CSP.OCOPAgentInterface#getMyName()
	 */
	public String getMyName() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see testbed.CSP.OCOPAgentInterface#getCSPNode()
	 */
	public CSPNodeInterface getCSPNode() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see testbed.CSP.OCOPAgentInterface#isUnsolvable()
	 */
	public boolean isUnsolvable() {
		// TODO Auto-generated method stub
		return false;
	}

	/* (non-Javadoc)
	 * @see testbed.CSP.OCOPAgentInterface#setIsUnsolvable(boolean)
	 */
	public void setIsUnsolvable(boolean isUnsolvable) {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see testbed.CSP.OCOPAgentInterface#getEnvironment()
	 */
	public Environment getEnvironment() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see testbed.CSP.OCOPAgentInterface#getMySensorArray()
	 */
	/*	
	 public SensorArray getMySensorArray() {
	 // TODO Auto-generated method stub
	 return null;
	 }
	 */
	/* (non-Javadoc)
	 * @see testbed.CSP.OCOPAgentInterface#displayMessage(java.lang.String)
	 */
	public void displayMessage(String s) {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see testbed.CSP.OCOPAgentInterface#start()
	 */
	public void start() {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see testbed.CSP.OCOPAgentInterface#getMessageQueue(int)
	 */
	public MessageQueue getMessageQueue(int no) {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see testbed.CSP.OCOPAgentInterface#getAgentCount()
	 */
	public int getNumberOfNeighbors() {
		// TODO Auto-generated method stub
		return 0;
	}

	/* (non-Javadoc)
	 * @see testbed.OCOP.OCOPAgentInterface#getOCOPNode()
	 */
	public OCOPAbstractNode getOCOPNode() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see testbed.communication.DistributedAgentInterface#terminate(java.lang.String)
	 */
	public void terminate(String message) {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see testbed.communication.DistributedAgentInterface#getNeighboursIDs()
	 */
	public Vector getNeighboursIDs() {
		// TODO Auto-generated method stub
		return null;
	}
}