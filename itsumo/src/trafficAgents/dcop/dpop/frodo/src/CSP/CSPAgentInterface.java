package frodo.CSP;

import frodo.utils.Logger;

/**
 * @author apetcu
 * 
 * To change this generated comment edit the template variable "typecomment": Window>Preferences>Java>Templates. To enable and disable the creation of
 * type comments go to Window>Preferences>Java>Code Generation.
 */

public interface CSPAgentInterface {

	/**
	 * Method getLogger. returns an object that knows how to log messages coming from the agent
	 * 
	 * @return Logger
	 */
	public Logger getLogger();

	/**
	 * Method getMyName. returns the name of the agent (string)
	 * 
	 * @return String
	 */
	public String getMyName();

	/**
	 * Method getCSPNode. returns the CSP node of the agent
	 * 
	 * @return CSPNodeInterface
	 */
	public CSPNodeInterface getCSPNode();

	public boolean isUnsolvable();

	public void setIsUnsolvable(boolean isUnsolvable);

	/**
	 * Method getMySensorArray. returns the sensor array object
	 * 
	 * @return SensorArray
	 */
	//	public SensorArray getMySensorArray();
	public void displayMessage(String s);

	public void start(); //coming from Thread

	/**
	 * Returns the agentCount. how many agents(targets) are in the system. note that this can be different than what the environment returns, since
	 * the environment also knows about monitors, etc
	 * 
	 * @return int
	 */
	public int getNumberOfNeighbors();

	/**
	 * @return
	 */
	public int getAgentId();
	//	public int hashCode ();
	//	public boolean equals();
}