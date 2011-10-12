/*
 * Environment.java
 *
 * Created on November 4, 2002, 1:21 AM
 */

package frodo.OCOP.communication;

import frodo.COP.Solution;
import frodo.CSP.ComplexValue;
import frodo.OCOP.DFSNode;
import frodo.OCOP.OCOPAbstractAgent;
import frodo.OCOP.OCOPAbstractProblem;
import frodo.OCOP.OCOPAgentInterface;
import frodo.communication.CommunicationStatistics;

/**
 * 
 * @author root
 */
public class Environment extends frodo.communication.Environment {

	private OCOPAbstractProblem problem;

	int[][] snapshotsNdx;

	double[][] snapshotsDelta;

	public int cycles = 0;

	//------------------------------------------------------------------------------------//
	/** Creates a new instance of Environment */
	//------------------------------------------------------------------------------------//
	public Environment(String file, int mode, int mag) {
		super();
		problemFile = file;
		solveMode = mode;
		myMagnification = mag;
	}

	//------------------------------------------------------------------------------------//
	/**
	 * @return Returns the problem.
	 * 
	 */
	public OCOPAbstractProblem getProblem(){
		return problem;
	}

	/**
	 * @param problem
	 *            The problem to set.
	 */
	public void setProblem(OCOPAbstractProblem problem){
		this.problem = problem;
	}

	//------------------------------------------------------------------------------------//
	//	public void addAgent(OCOPAgentInterface ag) {
	//		this.myAgents.add(ag);
	//	}

	//------------------------------------------------------------------------------------//
	public void setMonitorAgent(OCOPAgentInterface ag){
		this.myMonitor = ag;
	}

	//------------------------------------------------------------------------------------//
	/**
	 * @param id - I will setup this agent as the ROOT of the DFS. Its node has to be of type DFSNode!!!
	 */
	public void setRootAgent(int id){
		((DFSNode) ((OCOPAbstractAgent) this.getAgent(id)).getOCOPNode()).getDFSData().setPseudotreeRoot(true);
	}

	//------------------------------------------------------------------------------------//
	/**
	 * An agent announces the environment that it has finished, and tells the environment about its solution
	 * @param i
	 * @param hisAssignments
	 */
	public synchronized void agentFinished(int i, ComplexValue hisAssignments){
		System.out.println("Agent " + i + " finished (" + (finishedCounter + 1) + " out of " + myAgents.size() + "); assi=" + hisAssignments.toString());
		problem.setCurrentAssignments(hisAssignments);
		super.agentFinished(i);
	}

	//------------------------------------------------------------------------------------//
	/* (non-Javadoc)
	 * @see testbed.communication.Environment#allFinished()
	 */
	public void allFinished(long time){
		/*
		 * notify the monitor that the problem has been finished, by gushakov
		 * UPDATED: Moved to parent class
		 if (monitor != null) {
		 monitor.setEndOfProblem(true);
		 }
		 */
		System.out.println("All finished, will compute total util.");
		ComplexValue result = problem.getCurrentAssignments();
		double optimalUtil = problem.getTotalEvaluation();
		System.out.println("Total util=" + optimalUtil);
		Solution solution = new Solution(result, optimalUtil, time);
		System.out.println("Solution=" + solution.toString());


		String complement = "\nTOTALTIME=" + solution.getTime();
		complement = complement + "\nCYCLES=" + this.cycles;
		complement = complement + monitor.resultString();
		
		Solution.writeResultToFile(solution, complement, resultFile);
		CommunicationStatistics.writeToFile(statistics, resultFile + ".stats", time);
	}

	//------------------------------------------------------------------------------------//
	public synchronized void takeSnapshot(int agentId, int value, double delta){
		if (snapshotsNdx == null) {
			snapshotsNdx = new int[getNumberOfAgents()][100];
			snapshotsDelta = new double[getNumberOfAgents()][100];
			for (int i = 0; i < getNumberOfAgents(); i++)
				for (int j = 0; j < 10; j++)
					snapshotsNdx[i][j] = -1;
		}
		int ndx = 0;
		while (snapshotsNdx[agentId][ndx] >= 0)
			ndx++;
		snapshotsNdx[agentId][ndx] = value;
		snapshotsDelta[agentId][ndx] = delta;
	}
}