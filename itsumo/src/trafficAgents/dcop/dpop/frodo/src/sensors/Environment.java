/*
 * Environment.java
 *
 * Created on November 4, 2002, 1:21 AM
 */

package frodo.sensors;

import java.util.Calendar;
/**
 * 
 * @author root
 */
public class Environment extends frodo.communication.Environment {
	private String	sensor_file;
	int				currentRound;

	public SensorArray		mySensorArray;

	//------------------------------------------------------------------------------------//
	/** Creates a new instance of Environment */
	//------------------------------------------------------------------------------------//
	public Environment(String tf, String sf, int mode, int mag) {
		super();
		problemFile = tf;
		sensor_file = sf;
		solveMode = mode;
		myMagnification = mag;
		//load the sensors
		SensorLoader sl = new SensorLoader(this);
		mySensorArray = sl.loadSensors(getSensorFile());
	}
	//------------------------------------------------------------------------------------//
	public void nextRound(int i) {
		if (i > currentRound) {
			currentRound++;
			displayMessage("Round " + i + " starting.");
		}
	}
	//------------------------------------------------------------------------------------//
	public String getSensorFile() {
		return sensor_file;
	}
	//------------------------------------------------------------------------------------//
	public String getTargetFile() {
		return problemFile;
	}
	//------------------------------------------------------------------------------------//
	/**
	 * Method agentFinished.
	 * 
	 * @param i
	 */
	public void agentFinished(int i) {
		finishedCounter++;
		System.out.println("Agent " + i + " finished.");
		if (finishedCounter == myAgents.size()) {
			System.out.println("Finished in " + currentRound + " rounds.");
			long timeFinish = Calendar.getInstance().getTimeInMillis();
			System.err.println((timeFinish - timeInit) + "\t1\t" + currentRound);
		}
	}
	//------------------------------------------------------------------------------------//
	public void isUnsolvable(int i) {
		System.out.println("Agent " + i + " unsolvable... after " + currentRound + "rounds. Gonna exit.");
		long timeFinish = Calendar.getInstance().getTimeInMillis();
		System.err.println((timeFinish - timeInit) + "\t0\t" + currentRound);
		myLogger.println((timeFinish - timeInit) + "\t0\t" + currentRound);
		terminate("Agent " + i + " unsolvable... after " + currentRound + "rounds. Gonna exit.");
	}
	//------------------------------------------------------------------------------------//
	/**
	 * Method getSensorArray.
	 * 
	 * @return SensorArray
	 */
	public SensorArray getSensorArray() {
		return mySensorArray;
	}
	//------------------------------------------------------------------------------------//
	public void allFinished(long time) {
		// TODO Auto-generated method stub
		
	}
}