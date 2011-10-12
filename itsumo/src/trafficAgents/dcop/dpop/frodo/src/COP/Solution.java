/*
 * Created on May 1, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.COP;

import java.io.BufferedReader;
import java.io.FileReader;

import frodo.CSP.ComplexValue;
import frodo.utils.FileUtilities;

/**
 * @author apetcu
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class Solution extends frodo.CSP.Solution {
	double valuation;

	/**
	 * @param assignment
	 * @param valuation
	 */
	public Solution() {
		this(null, 0, 0);
	}

	public Solution(ComplexValue assignment, double valuation) {
		this(assignment, valuation, 0);
	}

	public Solution(ComplexValue assignment, double valuation, long time) {
		super(assignment, time);
		this.valuation = valuation;
	}

	public double getValuation() {
		return valuation;
	}

	public void setValuation(double valuation) {
		this.valuation = valuation;
	}

	public String toString() {
		return new String(assignment.toString() + ":" + time + ":" + valuation);
	}

	public static void writeResultToFile(Solution solution, String complement, String resultFile) {
		String finalText = new String(solution.toString() + "\n**********\n" + complement);
		FileUtilities.writeStringToFile(finalText, resultFile);
	}

	//-----------------------------------------------------------------------//
	public Solution(String src) {
		frodo.CSP.Solution temps = frodo.CSP.Solution.fromString(src);
		this.assignment = temps.getAssignment();
		this.time = temps.getTime();
		String parts[] = src.split(":");
		//parts[0] is the assignment;
		//parts[1] is the time;
		//		if (parts.length > 2) {
		this.valuation = Double.parseDouble(parts[2]);
		//		}
	}

	//-----------------------------------------------------------------------//
	public static Solution loadFromFile(String filename) {
		Solution sol = null;
		try {
			BufferedReader linein = new BufferedReader(new FileReader(filename));
			String line = linein.readLine();
			while (line.trim().length() == 0 || line.trim().startsWith("#"))
				line = linein.readLine();
			linein.close();
			sol = new Solution(line);
			//			this.setAssignment(sol.getAssignment());
			//			this.setTime(sol.getTime());
			//			this.setValuation(sol.getValuation());
		} catch (Exception ioe) {
			ioe.printStackTrace();
		}
		return sol;
	}

	//-----------------------------------------------------------------------//
	//-----------------------------------------------------------------------//
	//-----------------------------------------------------------------------//
}
