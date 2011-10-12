/*
 * Created on 20 avr. 2005
 *
 */
package frodo.monitoring;

import frodo.OCOP.DPOP.DPOPAgent;
import frodo.OCOP.DPOP.Dimension;
import frodo.OCOP.DPOP.HyperCube;
import frodo.communication.Message;
import frodo.interaction.meeting.graph.Heuristics;
import frodo.monitoring.gui.MonitorWindow;
import frodo.monitoring.interfaces.MonitorListener;
import frodo.utils.Logger;

/**
 * @author gushakov
 *
 */
public class Monitor extends Thread {

//	public static final int TOPO = 1;
//
//	public static final int UTIL = 2;
//
//	public static final int VALUE = 3;

//	private Environment myEnvironment;

	private boolean endOfProblem;

	private Message nextMessage;

	private MonitorListener window;

	private int numOfTOPO;

	private int maxLengthTOPO;

	private double avgLengthTOPO;

	private int numOfUTIL;

	private int maxLengthUTIL;

	private double avgLengthUTIL;

	private int numOfVALUE;

	private int maxLengthVALUE;

	private double avgLengthVALUE;

	private int dimensions;

	Logger log;

	private int[] flaggedLink = null;

	private int flaggedLinkHCSize;

	private int flaggedLinkWidth;

	private MonitorListener cspViewer = null;

//	private String solutionMethod;

	private int problemWidth;

	private int problemHeight;

//	public Monitor(Environment env) {
		public Monitor() {
//		myEnvironment = env;
		endOfProblem = false;

		// Set up the listener.
		//window = new MonitorWindow();

		// Iitialize the counters.
		numOfTOPO = 0;
		maxLengthTOPO = 0;
		avgLengthTOPO = 0;

		numOfUTIL = 0;
		maxLengthUTIL = 0;
		avgLengthUTIL = 0;

		numOfVALUE = 0;
		maxLengthVALUE = 0;
		avgLengthVALUE = 0;

		dimensions = 0;

		flaggedLinkHCSize = 0;

		flaggedLinkWidth = 0;

		// Set up logging
		log = new Logger("monitor.log");

	}

	public void setCSPViewer(MonitorListener viewer) {
		cspViewer = viewer;
	}

	public MonitorListener getWindow() {
		return window;
	}

	public void setWindow(MonitorListener win) {
		window = win;
	}

	public void run() {
		log.println("=============Monitor started!=============");

		while (!endOfProblem) {
			processNextMessage();
		}

		// finished the run, update the statistics

		String[] methodOut = { "heuristic", "" + getSolutionMethod() };
		//window.updateStatistics(methodOut);

		//window.updateStatistics(makeParameterValue("width", problemWidth));
		//window.updateStatistics(makeParameterValue("height", problemHeight));

		//window.updateStatistics(makeParameterValue("UTILs", numOfUTIL));
		//window.updateStatistics(makeParameterValue("max UTIL length, characters", maxLengthUTIL));
		//window.updateStatistics(makeParameterValue("total size of HCs", dimensions));

		log.println("total messages: " + (numOfTOPO + numOfUTIL + numOfVALUE));

		//Update the viewers.
		if (cspViewer != null) {
			if (flaggedLink != null) {
				//Display the width of a flagged link.
				String msg = "Monitor: flagged link (" + flaggedLink[0] + "<->" + flaggedLink[1] + "), hypercube size is " + flaggedLinkHCSize + " units, width " + flaggedLinkWidth;
				cspViewer.displayMessage(msg);
				log.println(msg);
			} else {
				cspViewer.displayMessage("no link was flagged");
			}
		}
		log.println("=============Monitor finished!=============");
	}

	public String resultString() {
		int totalMessages = (numOfTOPO + numOfUTIL + numOfVALUE);
		double avgSize = (avgLengthTOPO + avgLengthUTIL + avgLengthVALUE) / totalMessages;
		avgLengthTOPO = avgLengthTOPO / numOfTOPO;
		avgLengthUTIL = avgLengthUTIL / numOfUTIL;
		avgLengthVALUE = avgLengthVALUE / numOfVALUE;

		return new String(
			"\nMSGS=" + totalMessages +
			"\nAVGMSGSIZE=" + avgSize + 
			"\n\ntopo = " + numOfTOPO + 
			"\ntopo max size = " + maxLengthTOPO +
			"\ntopo avg size = " + avgLengthTOPO +
			"\nutil = " + numOfUTIL + 
			"\nutil max size = " + maxLengthUTIL +
			"\nutil avg size = " + avgLengthUTIL +
			"\nvalue = " + numOfVALUE + 
			"\nvalue max size = " + maxLengthVALUE +
			"\nvalue avg size = " + avgLengthVALUE
		);
	}

	public Object[] makeParameterValue(String parameter, int value) {
		Object[] parameterValue = { parameter, "" + value };
		return parameterValue;
	}

	/**
	 * Processes the next message. Messages are aggregated by their type (topo,
	 * util, value) and counted. The monitor window is updated at the end of the
	 * problem's run.
	 * 
	 * @see MonitorWindow
	 * @see frodo.OCOP.communication.Environment#allFinished(long)
	 * @author gushakov
	 */
	private synchronized void processNextMessage() {
		while (!endOfProblem && nextMessage == null) {
			try {
				wait();
			} catch (Exception e) {
			}
		}
		if (!endOfProblem) {
			int type = nextMessage.getType();
			String msg = nextMessage.toStringComplete();
			int len = msg.length();
			switch (type) {
			case DPOPAgent.MESSAGE_TOPO:
				numOfTOPO++;
				log.println(numOfTOPO + "\tTOPO: [" + nextMessage.getSenderId() + "->" + nextMessage.getDestinationId() + "]");
				if (len > maxLengthTOPO) {
					maxLengthTOPO = len;
				}
				avgLengthTOPO += len;
				break;
			case DPOPAgent.MESSAGE_UTIL:
				numOfUTIL++;
				log.println(numOfUTIL + "\t>>>UTIL: [" + nextMessage.getSenderId() + "->" + nextMessage.getDestinationId() + "]");
				if (len > maxLengthUTIL) {
					maxLengthUTIL = len;
				}
				avgLengthUTIL += len;
				//Determine the dimensionality of the message.
				HyperCube hc = HyperCube.fromString(nextMessage.getPayload());
				Dimension[] dims = hc.getDimensions();
				int d = 1;
				for (int i = 0; i < dims.length; i++) {
					d = d * dims[i].getSize();
				}
				dimensions = dimensions + d;

				//Also record the hypercube size and width for the flagged link.

				if (flaggedLink != null) {
					if ((nextMessage.getSenderId() == flaggedLink[0] && nextMessage.getDestinationId() == flaggedLink[1])
							|| (nextMessage.getSenderId() == flaggedLink[1] && nextMessage.getDestinationId() == flaggedLink[0])) {
						flaggedLinkHCSize = d;
						flaggedLinkWidth = dims.length;
					}
				}
				break;
			case DPOPAgent.MESSAGE_VALUE:
				numOfVALUE++;
				log.println(numOfVALUE + "\tVALUE: [" + nextMessage.getSenderId() + "->" + nextMessage.getDestinationId() + "]");
				if (len > maxLengthVALUE) {
					maxLengthVALUE = len;
				}
				avgLengthVALUE += len;
				break;
			default:
				break;
			}
			nextMessage = null;
			notifyAll();
		} // end of if
	}

	public synchronized void setEndOfProblem(boolean endOfProblem) {
		this.endOfProblem = endOfProblem;
		notifyAll();
	}

	public synchronized boolean getEndOfProblem() {
		return endOfProblem;
	}

	public synchronized void setNextMessage(Message msg) {
		while (nextMessage != null) {
			try {
				wait();
			} catch (Exception e) {
			}
		} // end of nextMessage while
		nextMessage = msg;
		notifyAll();
	}

	/**
	 * Records the coordinates of a communication link
	 * between two agents for further investigation
	 * of the messages (total width) passed trough this
	 * link.
	 *
	 */
	public void flagCommunicationLink(int[] link) {
		flaggedLink = link;
	}

	public int getFlaggedLinkWidth() {
		return flaggedLinkHCSize;
	}

	public int getProblemHeight() {
		return problemHeight;
	}

	public void setProblemHeight(int problemHeight) {
		this.problemHeight = problemHeight;
	}

	public int getProblemWidth() {
		return problemWidth;
	}

	public void setProblemWidth(int problemWidth) {
		this.problemWidth = problemWidth;
	}

	public String getSolutionMethod() {
		return Heuristics.getCurrentHeuristic();
	}
}