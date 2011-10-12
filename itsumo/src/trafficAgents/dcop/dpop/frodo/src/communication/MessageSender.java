package frodo.communication;

import java.util.Vector;

import frodo.utils.Logger;

/**
 * A simple behaviour to send custom messages to all neighboring agents.
 */

public class MessageSender {

	protected DistributedAgentInterface myAgent = null;

	protected Logger myLogger;

	protected MessageDispatcher messageDispatcher;

	//---------------------------------------------------------------------------//
	public MessageSender(DistributedAgentInterface a) {
		this.myAgent = a;
		this.myLogger = a.getLogger();
		this.messageDispatcher = a.getEnvironment().getMessageDispatcher();
	}

	//---------------------------------------------------------------------------//
	public void sendAll(Message msg) {
		int myId = myAgent.getAgentId();
		//		myLogger.println("<--------" + msg.toString());
		for (int i = 0; i < myAgent.getEnvironment().getNumberOfAgents(); i++) {
			if (i != myId) {
				msg.setDestinationId(i);
				sendNeighbor(msg);
			}
		}
	}

	//---------------------------------------------------------------------------//
	public void sendNeighbors(Message msg) {
		myLogger.println("<--------" + msg.toString());
		Vector ngh = myAgent.getNeighboursIDs();
		for (int i = 0; i < ngh.size(); i++) {
			int idRec = ((Integer) ngh.elementAt(i)).intValue();
			msg.setDestinationId(idRec);
			sendNeighbor(msg);
		}
	}

	//---------------------------------------------------------------------------//
	public void sendNeighbor(Message msg) {
		//		myLogger.println("<--------<< " + msg.getDestinationId() + " <<\t" + msg.toString());
//		MessageDispatcher md = myAgent.getEnvironment().getMessageDispatcher();
		messageDispatcher.sendMessage(msg);
	}
	//---------------------------------------------------------------------------//
}