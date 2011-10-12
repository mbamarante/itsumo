package frodo.CSP;

/**
 * This class represents a Binary constraint NotEquals between integers.
 */
public class BC_ID_Different extends BinaryConstraint {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private static char type = '#';

	public BC_ID_Different(CSPVariable from, CSPVariable to) {
		super(from, to);
	}

	public char getType() {
		return type;
	}

	public boolean satisfies(Integer i, Integer j) {
		return (i.intValue() != j.intValue());
	}

	public boolean satisfies(Object v1, Object v2) {
		return (!v1.equals(v2));
	}

	public boolean isSatisfied() {
		//        System.out.println("%%%%%%%%%%%%%%%CHK constr for:" + from + "-" + to);
		if ((from.getValue() == null) || (to.getValue() == null)) {
			//    	    System.out.println("\n%%%%%%%%%%%%%%% WARNING: CHK constr FOR NULL VALUES");
			return true;
		}
		boolean result = satisfies(from.getValue(), to.getValue());
		return result;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see testbed.CSP.BinaryConstraint#getValuation()
	 */
	public int getValuation() {
		if (isSatisfied())
			return 0;
		else
			return 1;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see testbed.CSP.BinaryConstraint#getValuation(java.lang.Object,
	 *      java.lang.Object)
	 */
	public int getValuation(Object v1, Object v2) {
		if (satisfies(v1, v2))
			return 0;
		else
			return 1;
	}

	/* (non-Javadoc)
	 * @see testbed.CSP.BinaryConstraint#getValuation(java.lang.Object)
	 */
	public int getValuation(Object v1) {
		// this is just to implement everything from parent
		return -1;
	}
}