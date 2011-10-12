/*
 * Created on Jul 8, 2005
 *
 */
package frodo.monitoring.interfaces;

import frodo.OCOP.OCOPBasicProblem;

/**
 * @author ushakov
 *
 */
public class OCOPModelEvent {

	public static final int SHOW_PSEUDO_TREE = 0;

	public static final int NEW_NODE_ADDED = 1;

	public static final int SET_NEW_PROBLEM = 2;

	public static final int NODE_DELETED = 3;

	public static final int UNARY_CONSTRAINT_CHANGED = 4;

	int type;

	OCOPBasicProblem problem;

	public OCOPModelEvent(int type, OCOPBasicProblem problem) {
		this.type = type;
		this.problem = problem;
	}

	public int getType() {
		return type;
	}

	public void setType(int type) {
		this.type = type;
	}

	public OCOPBasicProblem getProblem() {
		return problem;
	}

	public void setProblem(OCOPBasicProblem problem) {
		this.problem = problem;
	}
}
