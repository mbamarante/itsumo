/*
 * MessageQueueProcessor.java
 *
 * Created on November 4, 2002, 5:18 PM
 */

package frodo.communication;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.Vector;

import frodo.utils.Logger;


/**
 * 
 * @author apetcu
 */
public abstract class MessageQueueProcessor extends Thread {

	/**
	 * 
	 * @uml.property name="myAgent"
	 * @uml.associationEnd 
	 * @uml.property name="myAgent" multiplicity="(1 1)"
	 */
	public DistributedAgentInterface myAgent;

	/**
	 * 
	 * @uml.property name="myLogger"
	 * @uml.associationEnd 
	 * @uml.property name="myLogger" multiplicity="(1 1)"
	 */
	public Logger myLogger;

	/**
	 * 
	 * @uml.property name="myMQ"
	 * @uml.associationEnd 
	 * @uml.property name="myMQ" multiplicity="(1 1)"
	 */
	//	public CSPNodeInterface				myNode		= null;
	public MessageQueue myMQ;

	private volatile boolean finished;

	//---------------------------------------------------------------------------//
	/** Creates a new instance of MessageQueueProcessor */
	public MessageQueueProcessor(DistributedAgentInterface a, MessageQueue myMQ) {
		this.myAgent = a;
		this.myMQ = myMQ;
		myLogger = a.getLogger();
		//		myNode = myAgent.getCSPNode();
		myLogger.println("QQQQQQQQQQQQQQQQQ constructing message queue: agent=" + myAgent.getAgentId());
	}

	//---------------------------------------------------------------------------//
	public void processMessagesFromAllNeighbors() {
		myLogger.println("QQQQQQQQQQQQQQQQQ Processing message queue: myMQ=" + myMQ);
		myLogger.println("QQQQQQQQQQQQQQQQQ Processing message queue: myNode=" + myAgent.getAgentId());
		//		for (int i = 0; i < myAgent.getCSPNode().getNumberOfNeighbours(); i++) {
		for (int i = 0; i < myAgent.getNumberOfNeighbors(); i++) {
			Message msg = myMQ.getMessageBlocking();
			processMessage(msg);
		}
	}

	//---------------------------------------------------------------------------//
	public void run() {
		myLogger.println("Message handler running on queue \"" + myMQ.getName() + "\", with " + getMessageCounter() + " messages already in queue.");
		//        if(myMQ.getMessageCounter()>0){
		//            myLogger.println("Message queue: " + myMQ.toString());
		//            for(int i=0;i<myMQ.getMessageCounter();i++){
		//            }
		//        }
		finished = false;
		Message msg = null;
		try {
			while (true) {
				msg = myMQ.getMessageBlocking();
				if (msg.getType() == -1) {
					finished = true;
					break;
				}
				processMessage(msg);
			}
		} catch (Exception ex) {
			System.err.println("Agent " + myAgent.getAgentId() + " encounters problem while processing message: " + msg);
			ex.printStackTrace();
			myAgent.terminate("Problem while processing message: " + ex);
		}
	}

	//---------------------------------------------------------------------------//
	/**
	 * Returns the finished.
	 * 
	 * @return boolean
	 */
	public boolean isFinished() {
		return finished;
	}

	//---------------------------------------------------------------------------//
	/**
	 * Sets the finished.
	 * 
	 * @param finished
	 *            The finished to set
	 */
	/*
	 * public void setFinished(boolean finished) { this.finished = finished; }
	 */
	//---------------------------------------------------------------------------//
	public void stopHandling() {
		myMQ.addMessage(new Message("QUIT", -1, myAgent.getAgentId(), myAgent.getAgentId()));
		try {
			join(); // wait here
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	//---------------------------------------------------------------------------//
	public abstract Vector processMessage(Message msg);

	//---------------------------------------------------------------------------//

	public int getMessageCounter() {
		return this.myMQ.getMessageCounter();
	}


	//-----------------------------------------------------------------------------------------------------------------//
	//--Daniel's serialize/deserialize methods----------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	static public String serialize(Object obj) {
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		try {
			ObjectOutputStream oos = new ObjectOutputStream(baos);
			oos.writeObject(obj);
			oos.close();
			return baos.toString();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return null;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	static public Object unserialize(String str) {
		ByteArrayInputStream bais = new ByteArrayInputStream(str.getBytes());
		try {
			ObjectInputStream ois = new ObjectInputStream(bais);
			Object obj = ois.readObject();
			ois.close();
			return obj; 
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	//-----------------------------------------------------------------------------------------------------------------//
}