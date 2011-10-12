package frodo.sensors;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.StringTokenizer;
import java.util.Vector;

import frodo.dbreakout.CSPAgent;
import frodo.sensors.graphics.SensorFieldGUI_AllTargets;
import frodo.utils.Logger;


public class SensorLoader {
	//    private CSP problem;

	Logger			myLogger;

	CSPAgent		myAgent;

	//    Environment myEnvironment;
	//	--------------------------------------------------------------------//
	public SensorLoader(CSPAgent myAgent) {
		this.myAgent = myAgent;
		myLogger = myAgent.getLogger();
	}
	//	--------------------------------------------------------------------//
	public SensorLoader(Environment env) {
		//        myEnvironment = nev;
		myLogger = env.getLogger();
	}
	//--------------------------------------------------------------------//
	public SensorLoader(Logger lo) {
		//        myEnvironment = nev;
		myLogger = lo;
	}
	//--------------------------------------------------------------------//
	public SensorArray loadSensors(String filename) {

		SensorArray sa = null;
		String line = null;
		try {
			BufferedReader linein = new BufferedReader(new FileReader(filename));
			line = linein.readLine();
			int size = Integer.parseInt(line);
			sa = new SensorArray(myLogger, size);
			while ((line = (linein.readLine())) != null) {
				String tmp;
				StringTokenizer st = new StringTokenizer(line);
				tmp = st.nextToken();
				String sname = tmp;
				tmp = sname.substring(1);
				myLogger.print("Loading id=" + tmp);
				int no = Integer.parseInt(tmp);
				tmp = st.nextToken(" ,");
				tmp = tmp.substring(1);
				myLogger.print(" x=" + tmp);
				int x = Integer.parseInt(tmp);
				tmp = st.nextToken(" ,");
				myLogger.print(" y=" + tmp);
				int y = Integer.parseInt(tmp);
				tmp = st.nextToken(" )");
				tmp = tmp.substring(1);
				myLogger.print(" r=" + tmp);
				myLogger.print(" (");
				int r = Integer.parseInt(tmp);
				sa.addSensor(new Sensor(sname, new Position(x, y), r), no);
				while (st.hasMoreTokens()) {
					tmp = st.nextToken().substring(1);
					myLogger.print(" ->" + tmp);
					int cur = Integer.parseInt(tmp);
					sa.addConnection(no, cur, 1);
				}
				myLogger.println(")");

			}
			linein.close();
		} catch (Exception ioe) {
			myLogger.println("Problem occured at line: " + line);
			ioe.printStackTrace();
		}
		return sa;
	}
	//---------------------------------------------------------//
	//---------------------------------------------------------//
	public static void main(String[] args) {
		if (args.length < 1) {
			System.out.println("Usage: loader sensor_file target_file magnification");
		} else {
			int mag = Integer.parseInt(args[2]);
			Environment env = new Environment(args[0], args[1], 0, mag);
			SensorLoader sl = new SensorLoader(env);
			SensorArray sa = sl.loadSensors(args[0]);
			System.out.println(sa.toString());
			new SensorFieldGUI_AllTargets(null, sa, new Vector(), mag);
		}
	}
}