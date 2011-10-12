package frodo.OCOP.communication;

import java.util.Vector;

import frodo.communication.DistributedAgentInterface;
import frodo.communication.Message;
import frodo.communication.MessageQueue;
import frodo.communication.MessageQueueProcessor;

public class HandleIncomingTOPOMessageBehaviour extends MessageQueueProcessor {

	//inherited from MessageQueueProcessor:
	//myAgent, myLogger, myNode
	//---------------------------------------------------------------------------//
	public HandleIncomingTOPOMessageBehaviour(DistributedAgentInterface a, MessageQueue mq) {
		super(a, mq);
	}

	//---------------------------------------------------------------------------//
	public Vector processMessage(Message msg) {
		String content = msg.getPayload();
		int senderId = msg.getSenderId();
		myLogger.println("-------->incoming TOPO[" + senderId + "->" + myAgent.getAgentId() + "]" + content, 2);
		TopoProbe mc = TopoProbe.buildFromString(content);
		((DistributedDFSBuilder) myAgent).processTOPO(mc, senderId);
		return null;
	}
	//---------------------------------------------------------------------------//
}