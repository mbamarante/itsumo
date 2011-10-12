/*
 * Environment.java
 *
 * Created on November 4, 2002, 1:21 AM
 */

package frodo.communication;

import java.util.Calendar;
import java.util.HashMap;
import java.util.Iterator;

import frodo.CSP.CSPAgentInterface;
import frodo.monitoring.Monitor;
import frodo.utils.HelperUtilities;
import frodo.utils.Logger;

/**
 * 
 * @author root
 */
public abstract class Environment {

	//	protected Vector myAgents;
	protected HashMap myAgents;

	protected MessageDispatcher mailMan;

	protected int solveMode;

	protected CSPAgentInterface myMonitor;

	protected boolean showMonitor = false;

	protected int myMagnification = 1;

	protected int finishedCounter = 0;

	protected final boolean logsMessages = true;

	protected long timeInit;

	protected Logger myLogger;

	protected String problemFile;

	protected String resultFile;

	// gushakov
	public Monitor monitor;

	public CommunicationStatistics statistics;

	boolean storeMessages = false;

	//------------------------------------------------------------------------------------//
	/** Creates a new instance of Environment */
	//------------------------------------------------------------------------------------//
	public Environment() {
		myLogger = new Logger("environment.log");

		//		myAgents = new Vector();
		myAgents = new HashMap();
		mailMan = new MessageDispatcher(this);

		monitor = new Monitor();
	}

	//------------------------------------------------------------------------------------//
	public void addAgent(CSPAgentInterface ag) {
		myAgents.put(Integer.valueOf(ag.getAgentId()), ag);
	}

	//------------------------------------------------------------------------------------//
	public void setMonitorAgent(CSPAgentInterface ag) {
		myMonitor = ag;
	}

	//------------------------------------------------------------------------------------//
	public DistributedAgentInterface getAgent(int id) {
//		for (int i = 0; i < myAgents.size(); i++)
//			if (((DistributedAgentInterface) myAgents.elementAt(i)).getAgentId() == id)
//				return (DistributedAgentInterface) myAgents.elementAt(i);
//		return null;
		return (DistributedAgentInterface) myAgents.get(Integer.valueOf(id));
	}

	//------------------------------------------------------------------------------------//
	public HashMap getAgents() {
		return myAgents;
	}

	//------------------------------------------------------------------------------------//
	public MessageDispatcher getMessageDispatcher() {
		return mailMan;
	}

	//------------------------------------------------------------------------------------//
	public void displayMessage(String s) {
		if (myMonitor != null)
			myMonitor.displayMessage(s);
	}

	public String messagesResult() {
		return monitor.resultString();
	}

	//------------------------------------------------------------------------------------//
	public void start() {
		timeInit = Calendar.getInstance().getTimeInMillis();
		finishedCounter = 0;
		statistics = new CommunicationStatistics(myAgents.size());
		if (storeMessages)
			statistics.setLogsMessages(true);
		mailMan.setStatistics(statistics);

		monitor.start();

		System.out.println("Monitor=" + myMonitor);
		if (myMonitor != null && myMonitor.getAgentId() < 0) {
			System.out.println("Starting Monitor!!!");
			myMonitor.start();
		}
		Iterator myIterator = myAgents.values().iterator(); 
		while (myIterator.hasNext()) {
			((Thread) myIterator.next()).start();
			//			System.out.println("Starting " + i);
			//			((Thread) (myAgents.elementAt(i))).start();
		}

		try {
			synchronized (myAgents) {
				myAgents.wait();
			}
			notifyMonitorEndProblem();
			// just a bodycount routine :)
			myIterator = myAgents.values().iterator(); 
			while (myIterator.hasNext()) {
				((Thread) myIterator.next()).join();
			}
//				for (int i = 0; i < myAgents.size(); i++) {
//				((Thread) (myAgents.elementAt(i))).join();
//			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		//		System.out.println("Working...");
		/*
		 * try{ Thread.currentThread().sleep(5000); } catch(InterruptedException ie){ }
		 */
	}

	//------------------------------------------------------------------------------------//
	public void terminate(String message) {
		myLogger.println("!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()");
		myLogger.println("!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()");
		myLogger.println("!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()");
		myLogger.println("!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()");
		myLogger.println("Terminating JVM: \n" + HelperUtilities.indent(message, 2));
		myLogger.println("!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()");
		myLogger.println("!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()");
		myLogger.println("!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()");
		myLogger.println("!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()!@#$%^&*()");
		System.exit(1);
	}

	/**
	 * 
	 * @uml.property name="solveMode"
	 */
	//------------------------------------------------------------------------------------//
	public int getSolveMode() {
		return solveMode;
	}

	//------------------------------------------------------------------------------------//
	public int getMagnification() {
		return myMagnification;
	}

	//------------------------------------------------------------------------------------//
	/**
	 * Method getLogMessages.
	 */
	public boolean getLogMessages() {
		return logsMessages;
	}

	//------------------------------------------------------------------------------------//
	/**
	 * Method getLogger.
	 * 
	 * @return Logger
	 */
	public Logger getLogger() {
		return myLogger;
	}

	//------------------------------------------------------------------------------------//
	public int getNumberOfAgents() {
		return myAgents.size();
	}

	//------------------------------------------------------------------------------------//
	/**
	 * @return Returns the problem_file.
	 * 
	 * @uml.property name="problem_file"
	 */
	public String getProblemFile() {
		return problemFile;
	}

	/**
	 * @param problem_file
	 *            The problem_file to set.
	 * 
	 * @uml.property name="problem_file"
	 */
	public void setProblemFile(String problem_file) {
		this.problemFile = problem_file;
	}

	//------------------------------------------------------------------------------------//
	/**
	 * Method agentFinished.
	 * 
	 * @param i
	 */
	protected synchronized void agentFinished(int i) {
		finishedCounter++;
		if (finishedCounter == myAgents.size()) {
			System.out.println("ENVIRONMENT Finished!!");
			long timeFinish = Calendar.getInstance().getTimeInMillis();
			System.err.println(timeFinish - timeInit);
			//			mailMan.statisticsToString();
			myLogger.println("\n");
			myLogger.println(statistics.toString(timeFinish - timeInit));

			allFinished(timeFinish - timeInit);

			// last agent notifies main thread (which is waiting on myAgents) 
			synchronized (myAgents) {
				myAgents.notify();
			}
		}
	}

	public abstract void allFinished(long time);

	//------------------------------------------------------------------------------------//
	public String getResultFile() {
		return resultFile;
	}

	public void setResultFile(String resultFile) {
		this.resultFile = resultFile;
	}

	//------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------//
	/**
	 * Registers a message listener for two way communication
	 * between the environment and the meeting explorer.
	 * @see Monitor
	 * @see frodo.interaction.meeting.gui.MeetingExplorer
	 * @see frodo.interaction.meeting.roundtrip.DPOPMaker
	 * @author gushakov
	 */
	public void addMessageListener(Monitor aMonitor) {
		monitor = aMonitor;
	}

	public void notifyMonitor(frodo.communication.Message msg) {
		if (monitor != null) {
			monitor.setNextMessage(msg);
		}
	}

	private void notifyMonitorEndProblem() {
		if (monitor != null) {
			monitor.setEndOfProblem(true);
		}
	}

	//--------------------------------------------------------

	public void setStoreMessages(boolean storeMessages) {
		this.storeMessages = storeMessages;
	}
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
