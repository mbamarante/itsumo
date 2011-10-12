package frodo.OCOP.DPOP;

import frodo.COP.OCOPProblemFactory;
import frodo.OCOP.DFSNode;
import frodo.OCOP.OCOPAbstractAgent;
import frodo.OCOP.DPOP.behaviours.HandleIncomingUTILMessageBehaviour;
import frodo.OCOP.DPOP.behaviours.HandleIncomingVALUEMessageBehaviour;
import frodo.OCOP.communication.DistributedDFSBuilder;
import frodo.OCOP.communication.Environment;
import frodo.communication.Message;
import frodo.communication.MessageQueue;
import frodo.utils.HelperUtilities;

/**
 * @author apetcu
 * 
 */
public class DPOPAgent extends OCOPAbstractAgent {

	//	MESSAGE_TOPO is 1 in DistributedDFSBuilder.MESSAGE_TOPO;

	public static final int MESSAGE_TOPO = 1;

	public static final int MESSAGE_UTIL = 2;

	public static final int MESSAGE_VALUE = 3;

	public int cycles = 0;

	/**
	 * This is an Agent.
	 * It uses the functionality defined in the abstract agent
	 * Defines and implements the handler methods that are called on message receival.
	 */
	private static final long serialVersionUID = 1L;

	//message handlers
	protected HandleIncomingUTILMessageBehaviour utilityHandler; //receiving the UTIL messages from the children

	protected HandleIncomingVALUEMessageBehaviour valueHandler; //receiving the UTIL messages from the children

	protected Object syncUtils; //receiving the UTIL messages from the children

	protected Object syncValue; //receiving the VALUE assignments from the parent + pseudoparents

	//message queues
	protected MessageQueue utilQueue;

	protected MessageQueue valueQueue;

	int utilMessages;

	int valueMessages;

	DistributedDFSBuilder myDFSBuilder;

	//------------------------------------------------------------------------------------------------------------------------------//
	public DPOPAgent(Environment env, int id) {
		super(env, id);
		initialization();
	}

	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	/**
	 * This method constructs the message queues, handlers, monitors, etc.
	 */
	private void initialization() {
		if (environment != null) {
			valueQueue = new MessageQueue("VALUE_Queue");
			utilQueue = myMailQueue;//this is the default message queue in the super class; may just as well use it :)
			utilityHandler = new HandleIncomingUTILMessageBehaviour(this, utilQueue);
			valueHandler = new HandleIncomingVALUEMessageBehaviour(this, valueQueue);
			syncUtils = new Object();
			syncValue = new Object();

			node = OCOPProblemFactory.getSubproblem(environment.getProblem(), null, id);
			node.setAgent(this);
			node.setLogger(logger);
			logger.println("Loaded node type=" + node.getClass().getCanonicalName());
			logger.println("Node X" + node.getId() + " has dimension " + ((DPOPNode) node).getDimension());
			//this is an abuse; it's correct only when there is only one variable per node
			//			((DPOPNode) node).setDimension(new Dimension(node.getId(), node.getVariable().getDomainSize()));
			myDFSBuilder = new DistributedDFSBuilder(this, MESSAGE_TOPO);
		}
	}

	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	public void run() {
		try {
			logger.println(this.getMyName());
			logger.println(this.getOCOPNode().toStringSerializeFull());
			logger.println(getMyName() + " has " + getNumberOfNeighbors() + " neighbours");
			for (int i = 0; i < this.getOCOPNode().getNumberOfNeighbours(); i++) {
				DPOPNode nd = (DPOPNode) this.getOCOPNode().getNeighborByIndex(i);
				logger.println("Has neighbor " + nd.getId());
			}
			//			logger.println("Node " + getMyName() + " starts with initial assignment=" + this.getOCOPNode().getVariable().getAssignment());

			/*
			 logger.println("------------------------------------------------------------------------------");
			 logger.println("------------------------------------------------------------------------------");
			 logger.println("Checking for dominated values...");
			 Vector dominatedValues = this.getOCOPNode().getDominatedValues();
			 if (dominatedValues.size() > 0)
			 logger.println("DOMINATED values=" + HelperUtilities.vectorToString(dominatedValues));
			 else
			 logger.println("No dominated values found.");
			 */
			logger.println("------------------------------------------------------------------------------");
			logger.println("------------------------------------------------------------------------------");
			//everybody participates in topology probing; only ROOT nodes initiates probes; the others just relay
			logger.println(getMyName() + " starts PSEUDOTREE creation");
			logger.println("------------------------------------------------------------------------------");
			myDFSBuilder.dfsConstruction();
			logger.println(getMyName() + " finishes PSEUDOTREE creation");
			logger.println("------------------------------------------------------------------------------");
			logger.println("------------------------------------------------------------------------------");
			logger.println("Starting UTIL propagation.");
			logger.println("------------------------------------------------------------------------------");
			//------------------------------------------------------------------------------//
			//----FIRST Round of MESSAGES (until k-1)------------------------------------//
			//-----------------------------------------------------------------------------//
			int numberOfChildren = ((DFSNode) this.getOCOPNode()).getDFSData().getNumberOfChildren();
			if (numberOfChildren > 0) {
				logger.println("Waiting for " + numberOfChildren + " messages from children.");
				synchronized (syncUtils) { // wait for the first round of k-1 messages
					utilityHandler.start();
					while (utilMessages < numberOfChildren) {
						try {
							syncUtils.wait();
						} catch (InterruptedException ie) {
						}
					}
					utilityHandler.stopHandling();
				}
			} else {
				logger.println("No children, sending UTIL to parent.");
			}
			//----------------------------------------------------------------------//
			//----Send UTIL message to parent---------------------------------------//
			//----------------------------------------------------------------------//
			//this computes the message for my parent, and saves it within my this.getOCOPNode().
			//in the case of the root, computes the overall optimal utility message, but does not send it to anyone
			HyperCube hyperCubeToParent = ((DPOPNode) this.getOCOPNode()).buildUTIL();

			//send it to my parent only if I am not the root
			if (((DFSNode) this.getOCOPNode()).getDFSData().getParentID() >= 0) {
				logger.println("Sending my UTIL message to my parent " + ((DFSNode) this.getOCOPNode()).getDFSData().getParentID());
				Message msgToParent = buildUTILMessage(((DFSNode) this.getOCOPNode()).getDFSData().getParentID(), hyperCubeToParent);
				sender.sendNeighbor(msgToParent);

				logger.println("------------------------------------------------------------------------------");
				logger.println("Beginning VALUE propagation.");
				logger.println("------------------------------------------------------------------------------");
				//----------------------------------------------------------------------//
				//----Get VALUE messages from pseudoparents + parent----------------//
				//---------------------------------------------------------------------//
				//this is not necessary for the root
				synchronized (syncValue) { // wait for 1 VALUE message from parent
					valueHandler.start();
					while (valueMessages < 1) {
						try {
							syncValue.wait();
						} catch (InterruptedException ie) {
						}
					}
				}
			}
			logger.println("------------------------------------------------------------------------------");
			logger.println("------------------------------------------------------------------------------");
			logger.println("Optimizing:");
			//----------------------------------------------------------------------//
			//-find optimal value for myself, given the values my pseudoparents took//
			//----------------------------------------------------------------------//
			Integer optVal = ((DPOPNode) this.getOCOPNode()).optimization();
			//----------------------------------------------------------------------//
			//----Send VALUE messages to my children + pseudochildren---------------//
			//----------------------------------------------------------------------//
			logger.println("Computed optimal VALUE =" + optVal);
			logger.println("Sending VALUE =" + optVal + " to " + ((DFSNode) this.getOCOPNode()).getDFSData().getNumberOfChildren() + " children:");
			// send the VALUE message to all my children
			for (int i = 0; i < ((DFSNode) this.getOCOPNode()).getDFSData().getNumberOfChildren(); i++) {
				//get the child
				DPOPNode dest = (DPOPNode) ((DFSNode) this.getOCOPNode()).getDFSData().getChildren().getNeighborByIndex(i);
				//get the set of dimensions that it sent me. I will send back the same set of dimensions.
				Dimension[] dims = dest.getUtilMessage().getDimensions();
				//I simply build a message out of this set; this relies on the fact that I set the correct values for these variables when I receive a VALUE message
				Message valueMsg = buildVALUEMessage(dims);
				valueMsg.setDestinationId(dest.getId());
				sender.sendNeighbor(valueMsg);
			}
			logger.println("Done! Agent " + id + " exits cleanly.");
			environment.agentFinished(id, node.getCurrentAssignments());
			valueHandler.stopHandling();
		} catch (Exception ex) {
			logger.println("Exception:" + ex.toString());
			ex.printStackTrace(logger.getLogger());
			System.out.println("Agent " + id + " gives exception:");
			ex.printStackTrace();
			terminate(ex.toString());
		}
	} //-------------------------------------------------------------------------------------------------------------//	//-------------------------------------------------------------------------------------------------------------//	//-------------------------------------------------------------------------------------------------------------//	//-------------------------------------------------------------------------------------------------------------//	//-------------------------------------------------------------------------------------------------------------//	//-------------------------------------------------------------------------------------------------------------//	//-------------------------------------------------------------------------------------------------------------//

	//-------------------------------------------------------------------------------------------------------------//
	public void processUTILS(HyperCube hc, int senderId) {
		synchronized (syncUtils) {
			//            logger.println(">>>>>>>>>>>>>UTILS from  " +senderId+ "\n"+ hc.toStringVectors(), 2);
			utilMessages++;
			DPOPNode sender = (DPOPNode) this.getOCOPNode().getNeighborById(senderId);
			sender.setUtilMessage(hc);
			if (!hc.isSatisfiable()) {
				logger.println(">>>>>>>>>>>>>UTILS from " + senderId + " UNSATISFIABLE!", 2);
			}
			syncUtils.notify();
		}
	}

	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	public Message buildUTILMessage(int idRec, HyperCube hc) {
		//builds a UTIL message with the specified context
		Message msg = new Message(hc.toString(), MESSAGE_UTIL, id, idRec);
		return msg;
	}

	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	public Message buildVALUEMessage(Dimension[] dims) {
		//builds a VALUE message with the specified context
		Message msg = new Message(HelperUtilities.arrayToString(dims), MESSAGE_VALUE, id, -1);
		return msg;
	}

	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	public MessageQueue getMessageQueue(int no) {

		switch (no) {
		case MESSAGE_UTIL:
			return utilQueue;
		case MESSAGE_TOPO:
			return myDFSBuilder.getMessageQueue(0);
		case MESSAGE_VALUE:
			return valueQueue;
		case 4:
		//				return utilPhase2Queue;
		default:
			return null;
		}
	}

	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	/**
	 * @param id of the sender node, and values for all variables in the context of the outgoing message
	 */
	public void processVALUE(int senderId, Dimension[] dims) {
		synchronized (syncValue) {
			logger.println(">>>>>>>>>>>>>VALUE " + senderId + "<-" + HelperUtilities.arrayToString(dims), 2);
			valueMessages++;
			//for each neighbor, set the indexes of its dimensions to the received values (if they are in the context, otherwise do nothing)
			for (int i = 0; i < getNumberOfNeighbors(); i++) {
				DPOPNode toSet = (DPOPNode) this.getOCOPNode().getNeighborByIndex(i);
				HyperCube util = toSet.getUtilMessage();
				if (util != null) {//this is null for pseudo/parent, and not null for self and pseudo/children
					logger.println("Gonna set " + HelperUtilities.arrayToString(dims), 2);
					logger.println("Into " + HelperUtilities.arrayToString(util.getDimensions()), 2);
					Dimension.setIndexes(dims, util.getDimensions());
				}
			}
			Dimension.setIndexes(dims, ((DPOPNode) this.getOCOPNode()).getUtilMessage().getDimensions());
			logger.println("Gonna set " + HelperUtilities.arrayToString(dims), 2);
			logger.println("Gonna set " + HelperUtilities.arrayToString(((DPOPNode) this.getOCOPNode()).getUtilMessage().getDimensions()), 2);
			for (int i = 0; i < dims.length; i++) {
				DPOPNode neighbor = (DPOPNode) this.getOCOPNode().getNeighborById(dims[i].getId());
				if (neighbor != null) {
					neighbor.getVariable().setValue(neighbor.getVariable().getValue(dims[i].getCurrentIndex()));
				}
			}
			syncValue.notify();
		}
	}
	//-------------------------------------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------//
