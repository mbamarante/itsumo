/*
 * Created on Nov 9, 2005 by apetcu
 * Project Public
 * Package testbed.OCOP.ODPOP
 */
package frodo.COP;

import java.io.Serializable;
import java.util.Comparator;

import frodo.OCOP.MessageContext;

public class ValuedNogood implements Comparable, Cloneable, Serializable {

	private static final long serialVersionUID = 1L;

	public static final long CONFLICT_VALUATION = -10000;

	public static final long MINIMAL_VALUATION = -100000;

	public static final long MAXIMAL_VALUATION = 1000000;

	MessageContext context;

	double valuation;

	double upperBound;

	double lowerBound;

	static NogoodComparatorLowerBound comparatorLowerBound;

	static NogoodComparatorUpperBound comparatorUpperBound;

	/**
	 * @param context
	 * @param valuation
	 */
	public ValuedNogood(MessageContext context, double valuation) {
		this.context = context;
		this.valuation = valuation;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public boolean equals(Object src) {
		ValuedNogood new_src = (ValuedNogood) src;
		if (new_src.context.equals(this.context)) {
			return true;
		}
		return false;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public String toString() {
		if (context == null)
			return null;
		else
			return new String(this.context.toString() + ":" + this.valuation);
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public String toStringWithBounds() {
		if (context == null)
			return null;
		else
			return new String(this.context.toString() + ":" + this.lowerBound + ":" + this.upperBound + ":" + this.valuation);
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public static ValuedNogood fromString(String str) {
		if (str == null)
			return null;
		str = str.trim();
		if (!str.equalsIgnoreCase("")) {
			//			System.out.println("Gonna build context from:|" + str + "|");
			String[] parts = str.split(":");
			MessageContext mc = MessageContext.fromString(parts[0]);
			double valuation = java.lang.Double.parseDouble(parts[1]);
			ValuedNogood nog = new ValuedNogood(mc, valuation);
			return nog;
		}
		return null;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public int compareTo(Object arg0) {
		if (arg0 instanceof ValuedNogood) {
			ValuedNogood new_name = (ValuedNogood) arg0;
			if (new_name.getValuation() == valuation)
				return 0;
			else if (new_name.getValuation() < valuation)
				return -1;
			else
				return 1;
		} else {
			System.exit(-1);
			return -1;
		}
	}

	public double getValuation() {
		return valuation;
	}

	public void setValuation(double valuation) {
		this.valuation = valuation;
	}

	public MessageContext getContext() {
		return context;
	}

	public void setContext(MessageContext context) {
		this.context = context;
	}

	public double getLowerBound() {
		return lowerBound;
	}

	public void setLowerBound(double lowerBound) {
		this.lowerBound = lowerBound;
	}

	public double getUpperBound() {
		return upperBound;
	}

	public void setUpperBound(double upperBound) {
		this.upperBound = upperBound;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	public class NogoodComparatorUpperBound implements Comparator {

		public int compare(Object arg0, Object arg1) {
			if (arg0 instanceof ValuedNogood && arg1 instanceof ValuedNogood) {
				ValuedNogood one = (ValuedNogood) arg0;
				ValuedNogood two = (ValuedNogood) arg1;
				if (one.getUpperBound() == two.getUpperBound())
					return 0;
				else if (one.getUpperBound() > two.getUpperBound())
					return -1;
				else
					return 1;
			} else {
				System.exit(-1);
				return -1;
			}
		}
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public static NogoodComparatorUpperBound getComparatorUpperBound() {
		if (comparatorUpperBound == null) {
			ValuedNogood ng = new ValuedNogood(new MessageContext(), 0);
			comparatorUpperBound = ng.new NogoodComparatorUpperBound();
		}
		return comparatorUpperBound;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	public class NogoodComparatorLowerBound implements Comparator {

		public int compare(Object arg0, Object arg1) {
			if (arg0 instanceof ValuedNogood && arg1 instanceof ValuedNogood) {
				ValuedNogood one = (ValuedNogood) arg0;
				ValuedNogood two = (ValuedNogood) arg1;
				return (int) (two.getLowerBound() - one.getLowerBound());
			} else {
				System.exit(-1);
				return -1;
			}
		}
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public static NogoodComparatorLowerBound getComparatorLowerBound() {
		if (comparatorLowerBound == null) {
			ValuedNogood ng = new ValuedNogood(new MessageContext(), 0);
			comparatorLowerBound = ng.new NogoodComparatorLowerBound();
		}
		return comparatorLowerBound;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	public class NogoodComparatorValuationAscending implements Comparator {

		public int compare(Object arg0, Object arg1) {
			if (arg0 instanceof ValuedNogood && arg1 instanceof ValuedNogood) {
				ValuedNogood one = (ValuedNogood) arg0;
				ValuedNogood two = (ValuedNogood) arg1;
				if (one.getValuation() == two.getValuation())
					return 0;
				else if (one.getValuation() < two.getValuation())
					return -1;
				else
					return 1;
			} else {
				System.exit(-1);
				return -1;
			}
		}
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public boolean isCompatible(ValuedNogood other) {
		return context.isCompatible(other.getContext());
	}

	public boolean isCompatible(MessageContext other) {
		return context.isCompatible(other);
	}

	public boolean isConflict() {
		if (valuation == CONFLICT_VALUATION)
			return true;
		else
			return false;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public Object clone() {
		MessageContext mc = (MessageContext) context.clone();
		ValuedNogood newNG = new ValuedNogood(mc, valuation);
		newNG.upperBound = upperBound;
		newNG.lowerBound = lowerBound;
		return newNG;
	}

	public static ValuedNogood join(ValuedNogood unu, ValuedNogood doi) {
		MessageContext joinCtx = MessageContext.combine(unu.getContext(), doi.getContext());
		ValuedNogood rez = new ValuedNogood(joinCtx, unu.getValuation() + doi.getValuation());
		return rez;
	}

	public int hashCode() {
		return context.hashCode();
	}
}
