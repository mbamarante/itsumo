/*
 * SensorArray.java
 *
 * Created on October 15, 2002, 5:22 PM
 */

package frodo.sensors;

import java.util.Vector;

import frodo.utils.Logger;


/**
 *
 * @author  apetcu
 */
public class SensorArray {

	//---------------------------------------------------------//
	int[][] connections;

	Sensor[] sensors;

	int size;

	Logger myLogger;

	/** Creates a new instance of SensorArray */
	//---------------------------------------------------------//
	public SensorArray(Logger logger, int no) {
		connections = new int[no][no];
		sensors = new Sensor[no];
		size = no;
		myLogger = logger;
	}

	//---------------------------------------------------------//
	public boolean areConnected(int first, int second) {
		return (connections[first][second] != 0);
	}

	//---------------------------------------------------------//
	/*
	 public boolean areConnected(Sensor first, Sensor second){
	 return (connections[first][second]!=0);
	 }
	 */
	//---------------------------------------------------------//
	public void addConnection(int first, int second, int w) {
		connections[first][second] = w;
		//        connections[second][first]=w;
	}

	//---------------------------------------------------------//
	public void deleteConnection(int first, int second) {
		connections[first][second] = 0;
		//        connections[second][first]=0;
	}

	//---------------------------------------------------------//
	public void addSensor(Sensor s, int pos) {
		sensors[pos] = s;
	}

	//---------------------------------------------------------//
	public Sensor getSensor(int pos) {
		return sensors[pos];
	}

	//---------------------------------------------------------//
	public Vector getSensors(Vector ids) {
		Vector res = new Vector();
		for (int i = 0; i < ids.size(); i++) {
			res.add(sensors[((Integer) ids.elementAt(i)).intValue()]);
		}
		return res;
	}

	//---------------------------------------------------------//
	public Vector getAllSensors() {
		Vector res = new Vector();
		for (int i = 0; i < size; i++) {
			res.add(sensors[i]);
		}
		return res;
	}

	//---------------------------------------------------------//
	public int[][] getSensorCoordinates() {
		int[][] res = new int[size][2];
		for (int i = 0; i < size; i++) {
			Position p = sensors[i].getPosition();
			res[i][0] = p.getX();
			res[i][1] = p.getY();
		}
		return res;
	}

	//---------------------------------------------------------//
	public int[] getSensorRanges() {
		int[] res = new int[size];
		for (int i = 0; i < size; i++) {
			res[i] = sensors[i].getRange();
		}
		return res;
	}

	//---------------------------------------------------------//
	public Vector getIDsSensorsWithinRange(Target t) {
		Vector obs = new Vector();
		for (int i = 0; i < size; i++) {
			if (sensors[i].sees(t)) {
				obs.add(new Integer(i));
			}
		}
		return obs;
	}

	//---------------------------------------------------------//
	public Vector getSensorsWithinRange(Target t) {
		Vector obs = new Vector();
		for (int i = 0; i < size; i++) {
			if (sensors[i].sees(t)) {
				obs.add(sensors[i]);
			}
		}
		return obs;
	}

	//---------------------------------------------------------//
	public Vector getCompatibleSensors(Target t) {
		if (t == null)
			return null;
		Vector obs = this.getIDsSensorsWithinRange(t);
		myLogger.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Sensors within range:" + obs);
		Vector incomp = new Vector();
		for (int i = 0; i < obs.size(); i++) {
			int connections = 0;
			int j = 0;
			while ((connections < 2) && (j < obs.size())) {
				if ((i != j) && (areConnected(((Integer) (obs.elementAt(i))).intValue(), ((Integer) (obs.elementAt(j))).intValue()))) {
					connections++;
				}
				j++;
			}
			if (connections < 2) {
				incomp.add(obs.elementAt(i));
			}
		}
		obs.removeAll(incomp);
		myLogger.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Compatible sensors:" + obs);
		if (obs.size() == 0)
			return null;
		return obs;
	}

	//---------------------------------------------------------//
	public String toString() {
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < size; i++)
			sb.append(sensors[i].toString() + " ");
		sb.append("\n");
		for (int i = 0; i < size; i++) {
			sb.append("\n");
			for (int j = 0; j < size; j++) {
				sb.append(connections[i][j] + " ");
			}
		}
		return sb.toString();
	}

	//---------------------------------------------------------//
	/**
	 * Returns the size.
	 * @return int
	 * 
	 * @uml.property name="size"
	 */
	public int getSize() {
		return size;
	}

}
