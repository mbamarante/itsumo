/*
 * Created on Apr 11, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.COP;

import frodo.CSP.CSPVariable;

/**
 * @author apetcu
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class EqualityConstraint extends ValuedConstraint {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	//	--------------------------------------------------------------------------------------------------------------//
	public EqualityConstraint(CSPVariable from, CSPVariable to) {
		super(from, to);
		this.setType('=');
		for (int i = 0; i < from.getDomainSize(); i++) {
			for (int j = 0; j < to.getDomainSize(); j++) {
				if (i == j)
					valuations[i][j] = 0;
				else
					valuations[i][j] = -10000;
			}
		}
	}

	//	--------------------------------------------------------------------------------------------------------------//
}
