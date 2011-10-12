package frodo.sensors;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.StringTokenizer;
import java.util.Vector;

import frodo.utils.Logger;


public class TargetLoader {

	/**
	 * 
	 * @uml.property name="myLogger"
	 * @uml.associationEnd 
	 * @uml.property name="myLogger" multiplicity="(1 1)"
	 */
	Logger myLogger;

	//---------------------------------------------------------------------------//
	public TargetLoader(Logger lg) {
		myLogger = lg;
	}

	//---------------------------------------------------------------------------//
	public Target loadMyTarget(String filename, String variableName) {

		String line;
		Target myT = null;
		try {
			BufferedReader linein = new BufferedReader(new FileReader(filename));
			boolean found = false;
			while (!found) {
				line = linein.readLine();
				if (line != null) {
					if (myLogger != null)
						myLogger.println("(((((((((((((((((((((((((((((((((( Reading targets: " + line);
					StringTokenizer st = new StringTokenizer(line);
					if (st.nextToken().equalsIgnoreCase(variableName)) {
						found = true;
						int x = Integer.parseInt(st.nextToken());
						int y = Integer.parseInt(st.nextToken());
						myT = new Target(x, y, variableName);
					}
				} else
					found = true;
			}
			linein.close();
		} catch (Exception ioe) {
			ioe.printStackTrace();
		}
		return myT;
	}

	//---------------------------------------------------------------------------//
	public Vector loadAllTargets(String filename) {

		String line;
		Vector targets = new Vector();
		try {
			BufferedReader linein = new BufferedReader(new FileReader(filename));
			boolean end = false;
			while (!end) {
				line = linein.readLine();
				if (line != null) {
					if (myLogger != null)
						myLogger.println("(((((((((((((((((((((((((((((((((( Reading target: " + line);
					StringTokenizer st = new StringTokenizer(line);
					String variableName = st.nextToken();
					int x = Integer.parseInt(st.nextToken());
					int y = Integer.parseInt(st.nextToken());
					targets.add(new Target(x, y, variableName));
				} else
					end = true;
			}
			linein.close();
		} catch (Exception ioe) {
			ioe.printStackTrace();
		}
		return targets;
	}

	//---------------------------------------------------------------------------//
	public int targetCount(String filename) {

		String line;
		int cnt = 0;
		try {
			BufferedReader linein = new BufferedReader(new FileReader(filename));
			boolean found = false;
			while (!found) {
				line = linein.readLine();
				if (line == null)
					found = true;
				else {
					if (myLogger != null)
						myLogger.println("(((((((((((((((((((((((((((((((((( Reading targets: " + line);
					if (line.startsWith("T")) {
						cnt++;
					}
				}
			}
			linein.close();
		} catch (Exception ioe) {
			ioe.printStackTrace();
		}
		return cnt;
	}

	//---------------------------------------------------------------------------//
	/*
	 public int getTargetCount(String filename) {
	 String line;
	 int cnt = 0;
	 try {
	 BufferedReader linein = new BufferedReader(new FileReader(filename));
	 boolean found = false;
	 while (!found) {
	 line = linein.readLine();
	 if (line == null)
	 found = true;
	 else {
	 //					myLogger.println("(((((((((((((((((((((((((((((((((( Reading targets: " + line);
	 if (line.startsWith("T")) {
	 cnt++;
	 }
	 }
	 }
	 linein.close();
	 } catch (Exception ioe) {
	 ioe.printStackTrace();
	 }
	 return cnt;
	 }
	 */
	//---------------------------------------------------------------------------//
	public static void main(String[] args) {
		if (args.length < 1) {
			System.out.println("Usage: TargetLoader file");
			return;
		}
		TargetLoader tl = new TargetLoader(null);
		Vector trgs = tl.loadAllTargets(args[0]);
		System.out.println("Loaded targets: " + trgs);
	}
	//---------------------------------------------------------------------------//
}
