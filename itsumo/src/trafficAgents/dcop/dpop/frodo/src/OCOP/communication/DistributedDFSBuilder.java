/*
 * Created on Jan 29, 2006 by apetcu
 * Project Public
 * Package frodo.OCOP.communication
 */
package frodo.OCOP.communication;

import java.util.Vector;

import frodo.CSP.CSPNodeInterface;
import frodo.CSP.ContextElement;
import frodo.OCOP.DFSNode;
import frodo.OCOP.OCOPAbstractAgent;
import frodo.OCOP.OCOPAbstractNode;
import frodo.communication.DistributedAgentInterface;
import frodo.communication.Environment;
import frodo.communication.Message;
import frodo.communication.MessageQueue;
import frodo.utils.Logger;

/**
 * This class implements a behavior for constructing a DFS tree in a distributed fashion.
 *<br> Each agent has such a behavior.
 *<br> The root is designated apriori through the environment, with node.getDFSData().setPseudotreeRoot(true).
 *<br> It starts this phase by sending out a token which is circulated between the agents.
 *<br> Each agent waits for, and resends the token.
 *<br> This finishes when each agent has received/sent the token to all its neighbors. 
 * @author apetcu
 *
 */
public class DistributedDFSBuilder implements DistributedAgentInterface {
	private int MESSAGE_TOPO = 1;

	public static final int DFS_HEURISTIC_AS_IS = 0;

	public static final int DFS_HEURISTIC_RANDOM = 1;

	public static final int DFS_HEURISTIC_MCN = 2;

	DFSNode myNode;

	OCOPAbstractAgent myAgent;

	Logger logger;

	protected HandleIncomingTOPOMessageBehaviour topologyHandler; //topology probing/constructing the pseudotree

	protected MessageQueue topologyQueue;

	//sync objects
	protected Object syncTOPO; //topology probing/constructing the pseudotree

	boolean receivedTOPO = false;

	ContextElement myContextElement;

	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	/**
	 * @param message_topo2 
	 * @param myNode
	 */
	public DistributedDFSBuilder(OCOPAbstractAgent agent, int message_topo2) {
		super();
		// TODO Auto-generated constructor stub
		this.myAgent = agent;
		this.myNode = (DFSNode) agent.getOCOPNode();
		this.logger = agent.getLogger();
		topologyQueue = new MessageQueue("TOPO_Queue");
		topologyHandler = new HandleIncomingTOPOMessageBehaviour(this, topologyQueue);
		syncTOPO = new Object();
		//		myContextElement = new ContextElement(myNode.getVariable().getId(), myNode.getVariable().getValue(0), myNode.getVariable().getDomainSize());
		myContextElement = new ContextElement(myNode.getVariable().getId(), null, myNode.getVariable().getDomainSize());
		MESSAGE_TOPO = message_topo2;
	}

	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	//----TOPOLOGY PROBING-----------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	/**
	 *TOPO phase
	 *everybody participates in topology probing; only ROOT node initiates probes; the others just relay
	 */
	public void dfsConstruction() {
		logger.println("------------------------------------------------------------------------------");
		logger.println("#####################################");
		logger.println("# TOPO PROBING " + myNode.getId() + " ###################");
		logger.println("#####################################");
		myNode.clearDFSData();//just as a precaution,clean all previous data (if any) 

		synchronized (syncTOPO) {
			topologyHandler.start();//starting the TOPO message handler

			//if I am root, I will act as if I have received my TOPO message, so I will start sending 
			if (myNode.getDFSData().isPseudotreeRoot()) {
				receivedTOPO = true;
				//set my root path to just myself; the others will do this in processTOPO
				myNode.getDFSData().getPathFromRoot().addElement(myContextElement);
				logger.println("Root path:" + myNode.getDFSData().getPathFromRoot());
			} else
				receivedTOPO = false;

			//I will do this until I finish with all my neighbors
			boolean finishedAll = false;
			while (!finishedAll) {
				//wait until receives message from parent;
				//this is already true if the node is the root
				//otherwise, it is set to true in topoProcess when a message arrives
				while (!receivedTOPO) {
					try {
						syncTOPO.wait();
					} catch (InterruptedException ie) {
					}
				}
				//Ok, so now I received a message if I'm out of that while/wait cycle.
				//I have to find the next node to send the message to - one that had not yet replied anything 
				//this avoids sending a message to a node that already replied (being a pseudochild or child)
				//logger.println("Neighbors that sent: " + HelperUtilities.arrayToString(this.getNode().visitedNeighbors));
				DFSNode neighborToSendTo = null;
				int i = 0;
				boolean foundOne = false;//I will set this to true if I find an unvisited neighbor
				while (i < myNode.getNumberOfNeighbours() && !foundOne) {
					neighborToSendTo = (DFSNode) myNode.getNeighborByIndex(i);
					//did this guy send me a TOPO?
					if (!myNode.getDFSData().getChildrenThatSent().contains(neighborToSendTo)) {
						//he did not; now let's see whether this is is a pseudoparent or a child
						if (myNode.getDFSData().getPathFromRoot().containsVariable(neighborToSendTo.getVariable().getId())) {
							//if this node is already in my rootpath, then it is my pseudoparent
							myNode.getDFSData().getPseudoparents().addNeighbor(neighborToSendTo);
							neighborToSendTo.getDFSData().setRouterId(myNode.getDFSData().getParentID());
						} else {
							//well, he's my child, not my PP, so I do have to wait for an answer.
							receivedTOPO = false;
							foundOne = true;
						}
						//I will send the TOPO in any case; if it is a PP I will not keep on looking;
						//A PP will not send any reply in any case
						int idrecv = neighborToSendTo.getId();
						//the guy will eventually reply, so I may as well add it already to my "replied" list
						myNode.getDFSData().getChildrenThatSent().add(neighborToSendTo);
						Message msg = buildTOPOMessage(idrecv, myNode.getDFSData().getPathFromRoot());
						logger.println("<<<<<<<<< {" + myNode.getId() + "->" + idrecv + "}" + msg.toString());
						myAgent.getMyMailMan().sendMessage(msg);
					}
					i++;
				}
				finishedAll = !foundOne;//If I exhausted all neighbors, then foundOne will be false, and finishedAll will be true
			}
			//Ok, if I am here (finished the WHILE) then it means I finished visiting all my neighbors
			//Now it's time to send the reply to my parent (if I am not the root)
			if (!myNode.getDFSData().isPseudotreeRoot()) {
				//If I have a parent, send him the TOPO message back
				int idParent = myNode.getDFSData().getParentID();
				Message msg = buildTOPOMessage(idParent, myNode.getDFSData().getPathFromRoot());
				logger.println("<<<<<<<<< {" + myNode.getId() + "->" + idParent + "}" + msg.toString());
				myAgent.getMyMailMan().sendMessage(msg);
			}
			logger.println("#####################################");
			logger.println("#####################################");
			logger.println("# TOPO PROBING FINISHED FOR X" + myNode.getId() + " ######");
			logger.println("#####################################");
			logger.println("DFS info:\n" + myNode.getDFSData().toString());
			for (int i = 0; i < myNode.getNumberOfNeighbours(); i++) {
				DFSNode ngh = (DFSNode) myNode.getNeighborByIndex(i);
				logger.println("Node " + ngh.getId() + " is routed through node " + ngh.getDFSData().getRouterId());
			}
			syncTOPO.notifyAll();
		}
		topologyHandler.stopHandling();
		logger.println("-----------------------------------------------------------------------------");
		logger.println("-----------DFS Construction Phase is complete--------------------------------");
		//this is going to update the DFS relationships in the centralized problem object
		//this is just for performing statistics, or building a GUI after the DFS was created in a distributed fashion
		DFSNode nodeFromCentralProblem = (DFSNode) ((frodo.OCOP.communication.Environment) myAgent.getEnvironment()).getProblem().getNodeById(myAgent.getAgentId());
		nodeFromCentralProblem.setDFSData(myNode.getDFSData());
	}

	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	/**
	 * This can come either from my parent, children, or pseudochildren
	 * If it is the first one, it's from my parent
	 * If it has as the last-1 context element my ID, then it is my child
	 * If it has as the last-1 context element somebody else's ID, then it is my pseudochild
	 * @param mc - that's the TOPO message that just arrived. It has as elements ContextElements (variable ids), in the order the corresponding nodes were visited
	 * @param senderId
	 */
	public void processTOPO(TopoProbe mc, int senderId) {
		synchronized (syncTOPO) {
			DFSNode senderNode = (DFSNode) myNode.getNeighborById(senderId);
			myNode.getDFSData().getChildrenThatSent().add(senderNode);
			if (myNode.getDFSData().getParent() == null && !myNode.getDFSData().isPseudotreeRoot()) {
				//this means that this is the first message I get, and it comes from my parent
				myNode.getDFSData().setParent(senderNode);
				//adding my own context to the root path
				mc.addElement(myContextElement);
				logger.println("Root path:" + mc);
				myNode.getDFSData().setPathFromRoot(mc);
				//the guy who sent me this message (my parent) is going to be the one I will send my UTIL messages to
				senderNode.getDFSData().setRouterId(senderId);//it's being routed through itself
				this.receivedTOPO = true;
			} else {//then this is a child or a pseudochild
				//to find out whether it's a child or pseudochild, we look at the next to last context element
				//the last element should be the sending node
				//the next to last element should be myself if this is a child
				//or something else if it's a pseudochild (the pseudochild's parent)
				ContextElement cel = mc.getElement(mc.size() - 1);//this is the last
				if (cel.getVariableId().getAgentID() != senderId) {//just to check
					myAgent.getEnvironment().terminate("Agent " + myNode.getId() + " received inconsistent TOPO message from " + senderId + ": " + mc.toString());
				} else {
					cel = mc.getElement(mc.size() - 2);//this is the next to last
					if (cel.getVariableId().getAgentID() == myNode.getId()) {//then this is my child
						myNode.getDFSData().getChildren().addNeighbor(senderNode);
						logger.println("Child " + senderId);
						this.receivedTOPO = true;
						senderNode.getDFSData().setRouterId(senderId);//it's being routed through itself
					} else {//then this is my pseudochild
						myNode.getDFSData().getPseudochildren().addNeighbor(senderNode);
						logger.println("PseudoChild " + senderId);
						boolean found = false;
						//now I will determine through which one of my children I will route messages to this pseudochild
						for (int i = 0; i < mc.size(); i++) {
							ContextElement curCEL = mc.getElement(i);
							if (!found) {
								if (curCEL.getVariableId().getAgentID() == myNode.getId()) {
									//ok, I found myself in this guy's root path
									found = true;
									//this means that the next node after myself is my child through which the pseudochild is routed
									OCOPAbstractNode router = myNode.getNeighborById(mc.getElement(i + 1).getVariableId().getAgentID());
									senderNode.getDFSData().setRouterId(router.getId());
									logger.println("Found myself. Responsible for " + senderId + " is " + router.getId());
									logger.println("Trace: ");
								}
							} else {
								//the rest of the path is the route from myself to the pseudochild
								logger.print(curCEL.toString() + "-");
							}
						}
						logger.println(myNode.getId() + "");
					}
				}

			}
			syncTOPO.notifyAll();
		}
	}

	//-------------------------------------------------------------------------------------------------------------//
	public Message buildTOPOMessage(int idRec, TopoProbe cont) {
		Message msg = new Message(cont.toString(), MESSAGE_TOPO, myNode.getId(), idRec);
		return msg;
	}

	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------//
	public void terminate(String message) {
		// TODO Auto-generated method stub

	}

	public MessageQueue getMessageQueue(int no) {
		return this.topologyQueue;
	}

	public Environment getEnvironment() {
		return myAgent.getEnvironment();
	}

	public Vector getNeighboursIDs() {
		// TODO Auto-generated method stub
		return null;
	}

	public Logger getLogger() {
		return logger;
	}

	public String getMyName() {
		// TODO Auto-generated method stub
		return null;
	}

	public CSPNodeInterface getCSPNode() {
		// TODO Auto-generated method stub
		return null;
	}

	public boolean isUnsolvable() {
		// TODO Auto-generated method stub
		return false;
	}

	public void setIsUnsolvable(boolean isUnsolvable) {
		// TODO Auto-generated method stub

	}

	public void displayMessage(String s) {
		// TODO Auto-generated method stub

	}

	public void start() {
		// TODO Auto-generated method stub

	}

	public int getNumberOfNeighbors() {
		// TODO Auto-generated method stub
		return 0;
	}

	public int getAgentId() {
		// TODO Auto-generated method stub
		return 0;
	}
}
