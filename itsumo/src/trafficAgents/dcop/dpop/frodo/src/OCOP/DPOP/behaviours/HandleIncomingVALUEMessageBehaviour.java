package frodo.OCOP.DPOP.behaviours;

import java.util.Vector;

import frodo.OCOP.DPOP.Dimension;
import frodo.OCOP.DPOP.DPOPAgent;
import frodo.communication.DistributedAgentInterface;
import frodo.communication.Message;
import frodo.communication.MessageQueue;
import frodo.communication.MessageQueueProcessor;


public class HandleIncomingVALUEMessageBehaviour extends MessageQueueProcessor {

	//inherited from MessageQueueProcessor:
	//myAgent, myLogger, myNode
	//---------------------------------------------------------------------------//
	public HandleIncomingVALUEMessageBehaviour(DistributedAgentInterface a, MessageQueue mq) {
		super(a, mq);
	}

	//---------------------------------------------------------------------------//
	public Vector processMessage(Message msg) {
		String content = msg.getPayload();
		int senderId = msg.getSenderId();
		//		myLogger.println("-------->incoming UTILS2[" + senderId + "->" + myAgent.getId() + "] => " + content, 2);
		String[] parts = content.split("\\s");
		Dimension[] dims = new Dimension[parts.length];
		for (int i = 0; i < parts.length; i++) {
			dims[i] = Dimension.fromString(parts[i]);
		}
		//		myLogger.println("~~~~~~~~~~~~~~~~~~~~~~~~~>VALUE msg from X" + senderId + ": " + HelperUtilities.arrayToString(dims), 2);
		((DPOPAgent) myAgent).processVALUE(senderId, dims);
		//		myLogger.println("\n\n\n GOT problematic UTILS2: " + content);
		return null;
	}
	//---------------------------------------------------------------------------//
}