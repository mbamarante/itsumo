/*
 * Created on May 1, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.COP;

import java.util.Random;

import frodo.CSP.CSPVariable;


/**
 * @author apetcu
 */
//	--------------------------------------------------------------------------------------------------------------//
public class StabilityConstraint extends ValuedConstraint {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	//	--------------------------------------------------------------------------------------------------------------//
	public StabilityConstraint(CSPVariable var) {
		super(var, var);
		this.setType('$');
	}

	//	--------------------------------------------------------------------------------------------------------------//

	public static int[][] randomValuations(int domSize, int maxValue) {
		int[][] valuations = new int[domSize][domSize];
		Random rnd = new Random();
		for (int i = 0; i < domSize; i++) {
			for (int j = 0; j < domSize; j++) {
				if (i == j)
					valuations[i][j] = 0;
				else
					valuations[i][j] = -rnd.nextInt(maxValue);
			}
		}
		return valuations;
	}
	//	--------------------------------------------------------------------------------------------------------------//
}
