package frodo.dbreakout;

import java.util.Vector;

import frodo.CSP.CSPNodeInterface;
import frodo.CSP.VariableId;
import frodo.communication.DistributedAgentInterface;
import frodo.communication.Message;
import frodo.communication.MessageDispatcher;
import frodo.communication.MessageQueue;
import frodo.communication.MessageSender;
import frodo.dbreakout.CSP.CSPNodeDB;
import frodo.dbreakout.CSP.CSPVariableDBA;
import frodo.dbreakout.behaviours.HandleDOMAINsBehaviour;
import frodo.dbreakout.behaviours.HandleIMPROVEsBehaviour;
import frodo.dbreakout.behaviours.HandleOKsBehaviour;
import frodo.dbreakout.graphics.SensorFieldGUI;
import frodo.dbreakout.graphics.SensorFieldGUI_AllTargets;
import frodo.sensors.SensorArray;
import frodo.sensors.Target;
import frodo.sensors.TargetLoader;
import frodo.utils.HelperUtilities;
import frodo.utils.Logger;

public class CSPAgent extends Thread implements frodo.CSP.CSPAgentInterface, DistributedAgentInterface {
	public final int NUMBER_OF_REQUIRED_SENSORS = 3;

	private int MAX_DISTANCE = 2;

	private final int MAX_ROUNDS = 50000;

	private final boolean showGUI = false;

	SensorFieldGUI sfg;

	SensorFieldGUI_AllTargets sfgat;

	int myId; // my index

	String myName;

	Logger myLogger;

	CSPNodeDB myNode; // variable contained by this agent

	Target myTarget;

	int agentCount;

	Vector domain;

	//message handling
	frodo.sensors.Environment myEnvironment;

	MessageDispatcher myMailMan;

	MessageQueue myDomainMQ;

	MessageQueue myOkMQ;

	MessageQueue myImproveMQ;

	HandleDOMAINsBehaviour domainHandler;

	HandleOKsBehaviour okHandler;

	HandleIMPROVEsBehaviour improveHandler;

	MessageSender sender;

	int cycles;

	int currentEvaluation;

	int myImprove;

	SensorArray mySensorArray;

	private boolean allResponded;

	private int responseCnt;

	boolean waitOKmode;

	boolean waitIMPROVEmode;

	private int terminationCnt;

	private int rounds;

	private boolean quasiLocalMinimum;

	private boolean canMove;

	private boolean consistent;

	private boolean solutionFound;

	private boolean isUnsolvable;

	//-----------------------------------------------------------------------//
	public CSPAgent(frodo.sensors.Environment env, int id) {
		super("CSPAgent" + id);
		myName = "CSPAgent" + id;
		myEnvironment = env;
		myId = id;
		initStuff();
	}

	//-----------------------------------------------------------------------//
	public void initStuff() {
		myMailMan = myEnvironment.getMessageDispatcher();
		myLogger = new Logger("logs/" + myName + ".log", myEnvironment.getLogMessages());
		TargetLoader tl = new TargetLoader(myLogger);
		mySensorArray = myEnvironment.getSensorArray();
		if (myId < 0) {
			myLogger.println("----------------------------------------");
			myLogger.println("Let me introduce myself... I am Monitor" + myId);
			Vector targets = tl.loadAllTargets(myEnvironment.getTargetFile());
			sfgat = new SensorFieldGUI_AllTargets(this, mySensorArray, targets, myEnvironment.getMagnification());
		} else {
			myLogger.println("----------------------------------------");
			myLogger.println("Let me introduce myself... I am TargetAgent" + myId);
			//load the target
			agentCount = tl.targetCount(myEnvironment.getTargetFile());
			MAX_DISTANCE = agentCount - 1;
			//        MAX_DISTANCE= 3;
			myTarget = tl.loadMyTarget(myEnvironment.getTargetFile(), "T" + myId);
			myLogger.println("-------------------))))))))))))))) Agent " + myName + " loaded target " + myTarget + "/" + agentCount);
			// Read the problem
			if (myTarget != null) {
				domain = mySensorArray.getCompatibleSensors(myTarget);
				if (domain != null) {
					//create the local node
					myNode = new CSPNodeDB(myId, "T" + myId, myLogger);
					myDomainMQ = new MessageQueue();
					myOkMQ = new MessageQueue();
					myImproveMQ = new MessageQueue();

					domainHandler = new HandleDOMAINsBehaviour(this, myDomainMQ);
					okHandler = new HandleOKsBehaviour(this, myOkMQ);
					improveHandler = new HandleIMPROVEsBehaviour(this, myImproveMQ);
					sender = new MessageSender(this);

					//create the local variables
					for (int i = 0; i < NUMBER_OF_REQUIRED_SENSORS; i++) {
						VariableId vid = new VariableId(myId, i);
						CSPVariableDBA var = new CSPVariableDBA(vid, new Vector(domain), null);
						myNode.addVariable(var);
					}
					//setup local constraints (this builds constraints of mutual exclusion between all the local variables)
					myNode.setupLocalConstraints();
					//create the neighbours
					for (int i = 0; i < agentCount; i++) {
						if (i != myId) {
							CSPNodeDB ngh = new CSPNodeDB(i, "T" + i, null);
							for (int j = 0; j < NUMBER_OF_REQUIRED_SENSORS; j++) {
								VariableId vid = new VariableId(i, j);
								CSPVariableDBA var = new CSPVariableDBA(vid, null, null);
								ngh.addVariable(var);
							}
							myNode.addNeighbour(ngh);
						}
					}

				} else { //that is, I can't find any compatible sensors... tough luck!
					myLogger.println("Could not find any value for my domain..", 2);
					System.out.println("Could not find any value for my domain..");
					isUnsolvable = true;
				}
			} else { //that is, I did not find my target... tough luck again!!
				myLogger.println("Could not find my target..", 2);
				System.out.println("Could not find my target..");
				isUnsolvable = true;
			}
		}
	}

	//-----------------------------------------------------------------------//
	public void run() {
		if (myId >= 0) {
			if (showGUI) {
				sfg = new SensorFieldGUI(this, mySensorArray, myTarget, domain);
				sfg.displayMessage("Exchanging domains");
			}
			sendDOMAIN();
			domainHandler.processMessagesFromAllNeighbors();
			if (showGUI) {
				sfg.repaint();
			}
			while ((!solutionFound) && (!isUnsolvable)) {
				myLogger.println("//--------------------------- ROUND " + rounds + " -------------------------------------//");
				myLogger.flush();
				if (showGUI) {
					sfg.displayMessage("Round " + rounds + " termination counter=" + terminationCnt);
				}
				sendOK();
				okHandler.processMessagesFromAllNeighbors();
				if (showGUI) {
					sfg.repaint();
				}
				if (!solutionFound && !isUnsolvable) {
					myLogger.println("//--------------------------- SENDING ALSO IMPROVE in round " + rounds + " -------------------------------------//");
					sendImprove();
					improveHandler.processMessagesFromAllNeighbors();
				}
			}
			if (showGUI) {
				sfg.repaint();
			}
			myLogger.println("\n\n\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$ FINISHED in " + rounds + " rounds!!! $$$$$$$$$$$$$$$$$$$$$$$$$$\n\n\n");
			myEnvironment.agentFinished(this.myId);
		} else {
			/*
			 * try { sleep(2000); } catch (InterruptedException e) {}
			 */
			sfgat.startShowing();
		}
	}

	//-----------------------------------------------------------------------//
	Message buildDOMAINMessage() {
		String content = "DOMAIN " + myNode.getAllDomainsString();
		return new Message(content, 1, myId, -1);
	}

	//-----------------------------------------------------------------------//
	public void sendDOMAIN() {
		myLogger.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@ >>>>>>>>>>>>>>>>>>>>  DOMAIN:");
		Message domain_msg = buildDOMAINMessage();
		this.setAllResponded(false);
		responseCnt = 0;
		sender.sendNeighbors(domain_msg);
		//        myLogger.println("\t\t\t----------------------||||| " + myInbox );
	}

	//-----------------------------------------------------------------------//
	public boolean addDOMAINs(Vector variables) {
		responseCnt++;
		myLogger.println("\t\t\t----------------------> incoming DOMAIN counter=" + responseCnt + " " + variables);
		myNode.updateNeighbourDomains(variables);
		//        if(responseCnt==myNode.getNeighboursCount()){
		if (responseCnt == agentCount - 1) {
			this.setAllResponded(true);
			myLogger.println("Last DOMAIN received. Now my neighbours are:\n" + myNode.getNeighbours());
			myNode.preprocess();
			//            if(myNode.setRandomInitial()!=0)
			switch (myEnvironment.getSolveMode()) {
			case 0: //random initial
				myNode.setRandomInitial();
				break;
			case 1: //NI initial
				myNode.setRandomInitial();
				myNode.computeNI();
				break;
			case 2: //Weighted NI
				myNode.setRandomInitial();
				myNode.computeWeightedNI();
				break;
			case 3: //NPI - here is same as random initial, but in send improve we also compute NPI
				myNode.setRandomInitial();
				break;
			case 4: //NPI + weighted NI -> NPI is done in send_improve
				myNode.setRandomInitial();
				myNode.computeWeightedNI();
				break;
			default:
				myNode.setRandomInitial();
			}
			if (myNode.getNumberOfNeighbours() == 0)
				solutionFound = true;
		}
		return this.getAllResponded();
	}

	//-----------------------------------------------------------------------//
	Message buildOKMessage(Vector values) {
		String content = "OK? ";
		if (values != null)
			for (int i = 0; i < values.size(); i++) {
				content = content + values.elementAt(i).toString() + " ";
			}
		return new Message(content, 2, myId, -1);
	}

	//-----------------------------------------------------------------------//
	public void sendOK() {
		waitIMPROVEmode = false;
		waitOKmode = true;
		Message ok_msg = null;
		rounds++;
		myEnvironment.nextRound(rounds);
		/*
		 * try { sleep(1000); } catch (InterruptedException e) {}
		 */
		myLogger.println("******************* ROUND " + rounds + " *************************************************");
		if (rounds > MAX_ROUNDS) {
			myLogger.println("\t\t******************* MAX ROUNDS reached (" + rounds + "). I'm tired and I go for a beer.");
			myEnvironment.displayMessage("Problem unsolvable in " + rounds + " rounds from " + myId);
			myEnvironment.isUnsolvable(myId);
			isUnsolvable = true;
			//            sfg.repaint();
			return;
			//            System.exit(0);
		}
		if (consistent) {
			terminationCnt++;
			myLogger.println("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^TERMINATION COUNTER=" + terminationCnt);
			if (terminationCnt >= MAX_DISTANCE) {
				myLogger.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Problem solved!!!!!!!!!!!!!!!!!!!!!");
				solutionFound = true;
				ok_msg = buildOKMessage(null);
				//                sfg.repaint();
				//               System.exit(0);
			} else
				ok_msg = buildOKMessage(myNode.getVariables());
		} else {
			if (quasiLocalMinimum) {
				myNode.increaseWeights();
			} else if (canMove) {
				myNode.executeImprovement();
			}
			ok_msg = buildOKMessage(myNode.getVariables());
		}
		//        myLogger.println(ok_msg);
		this.setAllResponded(false);
		responseCnt = 0;
		sender.sendNeighbors(ok_msg);
	}

	//-----------------------------------------------------------------------//
	public synchronized boolean addOK(Vector updates) {
		boolean finishedRequired = false;
		myLogger.println("\t\t\t----------------------> incoming OK counter=" + responseCnt + " " + updates + " and size=" + updates.size());
		if (updates.size() == 0) {
			//that means somebody said it's finished. we'll check later if that's ok with me
			finishedRequired = true;
			myLogger.println("\t\t\t+++++++++++++++++++++||||||||||||||||+++++++++++++++++++++++++ FINISH REQUIRED!");
		}
		responseCnt++;
		myNode.updateVariables(updates);
		if (responseCnt == myNode.getNumberOfNeighbours()) {
			myLogger.println("Last OK received.");
			this.setAllResponded(true);
			//            ctrl.repaint();
			//            sfg.repaint();
			myLogger.println("\t\t\tNow my node is:\n" + myNode.toString());
			if (finishedRequired) { //check if it's ok for me
				if (myNode.computeCurrentEval() == 0) {
					this.solutionFound = true;
					myLogger.println("\t\t\t++++++++++++++++++||||||| Problem solved |||||||||+++++++++++++++++++++++++");
					myEnvironment.displayMessage("Problem solved from " + myId + " in " + rounds + " rounds.");
				} else
					myLogger.println("&&&&&&&&&&&&&&&& ERROR - finish required, but I do not comply.");
			} else {
				//                this.sendImprove();
			}
		} else
			this.setAllResponded(false);
		//	modeSynchronize.notifyAll();
		//	}
		return this.getAllResponded();
	}

	//-----------------------------------------------------------------------//
	Message buildImproveMessage(Vector values, int howMuch, int cur_eval, int term_count) {
		String content = "IMPROVE " + howMuch + " " + cur_eval + " " + term_count + " ";
		if (values != null)
			for (int i = 0; i < values.size(); i++) {
				content = content + values.elementAt(i).toString() + " ";
			}
		return new Message(content, 3, myId, -1);
	}

	//-----------------------------------------------------------------------//
	public synchronized boolean addImprove(Vector updates, int howmuch, int hisEval, int hisTermCnt) {
		responseCnt++;
		myLogger.println("--------->incoming IMPROVE " + "counter=" + responseCnt + " -> howmuch=" + howmuch + " hisTermCnt=" + hisTermCnt + " myTermCnt=" + terminationCnt + " IMPROVE " + updates);
		if (hisTermCnt < terminationCnt)
			terminationCnt = hisTermCnt;
		if (myImprove < howmuch) {
			canMove = false;
			quasiLocalMinimum = false;
		} else if (howmuch == 0) {
		} else if (myImprove == howmuch) {
			//breaking ties in favor of the node with the smaller ID
			int otherID = ((CSPVariableDBA) updates.elementAt(0)).getId().getAgentID();
			if (otherID < myNode.getId())
				canMove = false;
		}
		//        myLogger.println("IMPROVE counter:" + responseCnt + " -> howmuch=" + howmuch);
		if (responseCnt == myNode.getNumberOfNeighbours()) {
			myLogger.println("Last IMPROVE received.");
			this.setAllResponded(true);
			myLogger.println("\t\t\t can_move:" + canMove + " qlm=" + quasiLocalMinimum);
			//            this.sendOK();
		} else
			this.setAllResponded(false);
		return this.getAllResponded();
	}

	//-----------------------------------------------------------------------//
	public void sendImprove() {
		waitIMPROVEmode = true;
		waitOKmode = false;
		if (solutionFound) {
			myLogger.println("I AM IN SEND_IMPROVE EVEN THOUGH I SHOULDN'T BE.");
		} else {
			currentEvaluation = myNode.computeCurrentEval();
			myLogger.println("Evaluated curr_val=" + "-> " + currentEvaluation);
			myImprove = 0;
			if (currentEvaluation == 0) {
				myLogger.println("Current values are consistent.");
				consistent = true;
			} else {
				consistent = false;
				terminationCnt = 0;
				if ((myEnvironment.getSolveMode() == 3) || (myEnvironment.getSolveMode() == 4))
					myNode.preprocess_NPI();
				myImprove = myNode.computeImprovement();
				//        myLogger.println("COMPUTE IMPROVEMENT BT:" + myNode.computeImprovementBT(new Vector()));
				if (myImprove > 0) {
					canMove = true;
					quasiLocalMinimum = false;
				} else {
					canMove = false;
					quasiLocalMinimum = true;
				}
			}
			Message improve_msg = buildImproveMessage(myNode.getImprovements(), myImprove, currentEvaluation, terminationCnt);
			//        myLogger.println(improve_msg);
			this.setAllResponded(false);
			responseCnt = 0;
			sender.sendNeighbors(improve_msg);
		}
	}

	//-----------------------------------------------------------------------//
	public CSPNodeInterface getCSPNode() {
		return myNode;
	}

	/**
	 * 
	 * @uml.property name="allResponded"
	 */
	//-----------------------------------------------------------------------//
	public boolean getAllResponded() {
		return allResponded;
	}

	/**
	 * 
	 * @uml.property name="allResponded"
	 */
	//-----------------------------------------------------------------------//
	public void setAllResponded(boolean v) {
		allResponded = v;
	}

	//-----------------------------------------------------------------------//
	//-----------------------------------------------------------------------//
	/*
	 public int getMyID() {
	 try {
	 return Integer.parseInt(myName.substring(8));
	 } catch (Exception e) {
	 myLogger.println("Name does not comply to \"CSPAgentXX\"");
	 return -1;
	 }
	 }
	 */
	//-----------------------------------------------------------------------//
	public boolean solutionFound() {
		return solutionFound;
	}

	/**
	 * 
	 * @uml.property name="rounds"
	 */
	//-----------------------------------------------------------------------//
	public int getRounds() {
		return rounds;
	}

	//-----------------------------------------------------------------------//
	public Logger getLogger() {
		return myLogger;
	}

	//-----------------------------------------------------------------------//
	public frodo.communication.Environment getEnvironment() {
		return myEnvironment;
	}

	//-----------------------------------------------------------------------//
	public MessageQueue getDomainMessageQueue() {
		return myDomainMQ;
	}

	//-----------------------------------------------------------------------//
	public MessageQueue getOkMessageQueue() {
		return myOkMQ;
	}

	//-----------------------------------------------------------------------//
	public MessageQueue getImproveMessageQueue() {
		return myImproveMQ;
	}

	/**
	 * 
	 * @uml.property name="myName"
	 */
	//-----------------------------------------------------------------------//
	public String getMyName() {
		return myName;
	}

	//-----------------------------------------------------------------------//
	public int getAgentId() {
		return myId;
	}

	//-----------------------------------------------------------------------//
	//-----------------------------------------------------------------------//
	public boolean isOKMode() {
		return waitOKmode;
	}

	//-----------------------------------------------------------------------//
	public boolean isIMPROVEMode() {
		return waitIMPROVEmode;
	}

	//-----------------------------------------------------------------------//
	public void displayMessage(String s) {
		sfgat.displayMessage(s);
	}

	//-----------------------------------------------------------------------//
	/**
	 * Returns the myNode.
	 * 
	 * @return CSPNodeDB
	 * 
	 * @uml.property name="myNode"
	 */
	public CSPNodeDB getMyNode() {
		return myNode;
	}

	/**
	 * Returns the sa.
	 * 
	 * @return SensorArray
	 * 
	 * @uml.property name="mySensorArray"
	 */
	public SensorArray getMySensorArray() {
		return mySensorArray;
	}

	/**
	 * Returns the isUnsolvable.
	 * 
	 * @return boolean
	 */
	public boolean isUnsolvable() {
		return isUnsolvable;
	}

	/**
	 * Sets the isUnsolvable.
	 * 
	 * @param isUnsolvable
	 *            The isUnsolvable to set
	 * 
	 * @uml.property name="isUnsolvable"
	 */
	public void setIsUnsolvable(boolean isUnsolvable) {
		this.isUnsolvable = isUnsolvable;
	}

	//-----------------------------------------------------------------------//
	public MessageQueue getMessageQueue(int no) {

		switch (no) {
		case 1:
			return myDomainMQ;
		case 2:
			return myOkMQ;
		case 3:
			return myImproveMQ;
		default:
			return null;
		}
	}

	//-----------------------------------------------------------------------//

	/**
	 * Returns the agentCount.
	 * 
	 * @return int
	 */
	public int getNumberOfNeighbors() {
		return myNode.getNumberOfNeighbours();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see testbed.communication.DistributedAgentInterface#terminate(java.lang.String)
	 */
	public void terminate(String message) {
		myLogger.println("Agent" + myId + " terminates: " + message);
		System.err.println("Agent " + myId + " is terminating: " + message);
		myEnvironment.terminate("Agent" + myId + " terminates:\n" + HelperUtilities.indent(message, 2));
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see testbed.OCOP.OCOPAbstractAgent#getNeighboursIDs()
	 */
	public Vector getNeighboursIDs() {
		Vector ids = new Vector();
		for (int i = 0; i < myNode.getNumberOfNeighbours(); i++) {
			ids.add(new Integer(((CSPNodeDB) myNode.getNeighbours().elementAt(i)).getId()));
		}
		return ids;
	}

}