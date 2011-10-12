package frodo.OCOP.DPOP.behaviours;

import java.util.Vector;

import frodo.OCOP.DPOP.HyperCube;
import frodo.OCOP.DPOP.DPOPAgent;
import frodo.communication.DistributedAgentInterface;
import frodo.communication.Message;
import frodo.communication.MessageQueue;
import frodo.communication.MessageQueueProcessor;


public class HandleIncomingUTILMessageBehaviour extends MessageQueueProcessor {

	//inherited from MessageQueueProcessor:
	//myAgent, myLogger, myNode
	//---------------------------------------------------------------------------//
	public HandleIncomingUTILMessageBehaviour(DistributedAgentInterface a, MessageQueue mq) {
		super(a, mq);
	}

	//---------------------------------------------------------------------------//
	public Vector processMessage(Message msg) {
		String content = msg.getPayload();
		int senderId = msg.getSenderId();
		//        myLogger.println("-------->UTILS[" + senderId + "->" + myAgent.getAgentId() + "] => " + content, 2);
		//		UTILVector uv = UTILVector.fromString(content, senderId, ((DPOPAgent) myAgent).getOCOPNode().getId());
		HyperCube hc = HyperCube.fromString(content);
		//		myLogger.println("------------>UTILS " + uv.toString(), 2);
		if (hc != null) {
			((DPOPAgent) myAgent).processUTILS(hc, senderId);
		} else {
			myLogger.println("\n\n\n GOT problematic UTILS from " + senderId + ": " + content);
		}
		return null;
	}
	//---------------------------------------------------------------------------//
}