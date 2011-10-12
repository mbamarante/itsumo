/*
 * Created on Feb 13, 2006 by apetcu
 * Project Public
 * Package frodo.COP.Centralized.BranchAndBound
 */
package frodo.COP.Centralized.BranchAndBound;

import frodo.COP.Solution;
import frodo.OCOP.OCOPAbstractProblem;

/**
 * @author apetcu
 * This class provides an implementation of the BranchAndBound optimization algorithm.
 * It operates on a DFS, which should be established before calling the "solve" method.
 * For this, see the DFSMaster class.
 */
public class BranchAndBound {
	public final static String algo_name = "BranchAndBound";

	/**
	 * @param problem
	 * @return a Solution object, which contains the best assignment
	 */
	public static Solution solve(OCOPAbstractProblem problem) {
		Solution solution = new Solution();
		//well, here I should also write some implementation... :)
		return solution;
	}
}
