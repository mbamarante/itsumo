/*
 * Created on May 1, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.CSP;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Vector;

/**
 * @author apetcu
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class Solution {
	protected ComplexValue assignment;

	protected long time;

	/**
	 * @param assignment
	 * @param valuation
	 */
	public Solution() {
		this(null, 0);
	}

	public Solution(ComplexValue assignment) {
		this(assignment, 0);
	}

	public Solution(ComplexValue assignment, long time) {
		super();
		this.assignment = assignment;
		this.time = time;
	}

	public ComplexValue getAssignment() {
		return assignment;
	}

	public void setAssignment(ComplexValue assignment) {
		this.assignment = assignment;
	}

	public String toString() {
		return new String(assignment.toString() + ":" + time);
	}

	//-----------------------------------------------------------------------//
	public static Solution fromString(String src) {
		if (src == null || src.trim().length() == 0)
			return null;
		String parts[] = src.split(":");
		if (parts.length < 1)
			return null;
		ComplexValue cval = ComplexValue.fromString(parts[0]);
		long time = 0;
		if (parts.length > 1)
			time = Long.parseLong(parts[1]);

		Solution sol = new Solution(cval, time);
		return sol;
	}

	//-----------------------------------------------------------------------//
	public static Solution fromFile(String filename) {
		Solution sol = null;
		try {
			BufferedReader linein = new BufferedReader(new FileReader(filename));
			String line = linein.readLine();
			while (line.trim().length() == 0 || line.trim().startsWith("#"))
				line = linein.readLine();
			linein.close();
			sol = Solution.fromString(line);
		} catch (Exception ioe) {
			ioe.printStackTrace();
		}
		return sol;
	}

	//-----------------------------------------------------------------------//
	public int getDistance(Solution other) {
		return assignment.getDistance(other.getAssignment());
	}

	/**
	 * @param other
	 * @return an array of assignments that are different, in the form of [x1=0, x1=1, x2=4, x2=2], etc (assignment in first solution, then same assignment in second solution)
	 */
	public Assignment[] getDifferences(Solution other) {
		Vector rez = new Vector();
		for (int i = 0; i < assignment.size(); i++)
			if (!assignment.getITHValue(i).equals(other.getAssignment().getITHValue(i))) {
				System.out.println(assignment.getITHValue(i) + " != " + other.getAssignment().getITHValue(i));
				rez.add(assignment.getITHValue(i));
				rez.add(other.getAssignment().getITHValue(i));
			}
		Assignment[] result = new Assignment[rez.size()];
		for (int i = 0; i < rez.size(); i++)
			result[i] = (Assignment) rez.elementAt(i);
		return result;
	}

	//-----------------------------------------------------------------------//
	//-----------------------------------------------------------------------//

	public long getTime() {
		return time;
	}

	public void setTime(long time) {
		this.time = time;
	}
}
