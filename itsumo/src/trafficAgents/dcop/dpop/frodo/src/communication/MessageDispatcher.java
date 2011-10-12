/*
 * MessageDispatcher.java
 *
 * Created on November 4, 2002, 12:00 PM
 */

package frodo.communication;

/**
 * 
 * @author apetcu
 */
public class MessageDispatcher {

	Environment myEnvironment;

	CommunicationStatistics statistics;

	//	------------------------------------------------------------------------------------------------------------------//	
	/** Creates a new instance of MessageDispatcher */
	public MessageDispatcher(Environment env) {
		this.myEnvironment = env;
		//		this.myAgentList = env.getAgents();
		if (env.getAgents() != null)
			System.out.println("MAIL MAN: MY AGENTS=" + env.getAgents().toString());
		else
			System.out.println("MAIL MAN: MY AGENTS=NULL");
	}

	//	------------------------------------------------------------------------------------------------------------------//	
	/**
	 * @param msg
	 * @return the ID of the message, as inserted by the CommunicationStatistics
	 */
	private long sendMessageNotSynchronized(Message msg) {
		//		long messageId = -1;
		//		int idRec = msg.getDestinationId();
		DistributedAgentInterface ag = myEnvironment.getAgent(msg.getDestinationId());
		if (ag != null) {
			MessageQueue theMQ = ag.getMessageQueue(msg.getType());
			if (theMQ == null) {
				myEnvironment.terminate("NULL MQ:\t" + "\t" + Thread.currentThread().getName() + "\t->" + msg.toStringComplete());
			} else {
				if (statistics.addMessageToStatistics(msg)) {
					myEnvironment.getLogger().println("----------------------------------------------------------------");
					myEnvironment.getLogger().println("First message of kind " + msg.getType() + ": " + msg.toStringComplete());
					myEnvironment.getLogger().println("----------------------------------------------------------------");
				}
				myEnvironment.getLogger().println(msg.toStringComplete());
				theMQ.addMessage(msg); //deliver the message in the corresponding queue, according to its type
			}
		} else {
			myEnvironment.terminate("Did not find Agent " + msg.getDestinationId() + " to deliver message " + msg.toStringComplete());
			return -1;
		}
		return msg.getMessageId();
	}

	//------------------------------------------------------------------------------------------------------------------//	
	public synchronized long sendMessage(Message msg) {
		long msgID = sendMessageNotSynchronized(msg);

		//Send the message to the monitor, by gushakov.
		myEnvironment.notifyMonitor(msg);
		return msgID;
	}

	//------------------------------------------------------------------------------------------------------------------//	
	/**
	 * This is a trick to ensure that messages that are supposed to be sent to multiple neighbors are not going to be interleaved
	 * with other messages from other senders; control is maintained until the whole lot is delivered, and then somebody else can
	 * deliver messages again.
	 * this is used in the cycle cutset algo, between CC nodes that send tokens  
	 * @param msg
	 * @param receivers
	 * @return
	 */
	/*
	 public boolean sendMessageBulk(Message msg, Vector receivers) {
	 synchronized (deliveryLock) {
	 for (int i = 0; i < receivers.size(); i++) {
	 int idRec = ((Integer) receivers.elementAt(i)).intValue();
	 msg.setDestinationId(idRec);
	 boolean found = sendMessageNotSynchronized(msg);
	 if (!found) {
	 return false;
	 }
	 }
	 }
	 return true;
	 }

	 //------------------------------------------------------------------------------------------------------------------//	
	 public boolean sendMessageBulk(Message msg, int[] receivers) {
	 synchronized (deliveryLock) {
	 for (int i = 0; i < receivers.length; i++) {
	 msg.setDestinationId(receivers[i]);
	 boolean found = sendMessageNotSynchronized(msg);
	 if (!found) {
	 return false;
	 }
	 }
	 }
	 return true;
	 }
	 */
	//------------------------------------------------------------------------------------------------------------------//	
	//------------------------------------------------------------------------------------------------------------------//	
	//	private void addMessageToStatistics(Message msg) {
	//	}
	//------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------//	
	//	public CommunicationStatistics getStatistics() {
	//		return statistics;
	//	}
	public void setStatistics(CommunicationStatistics statistics) {
		this.statistics = statistics;
	}
	//------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------//

}