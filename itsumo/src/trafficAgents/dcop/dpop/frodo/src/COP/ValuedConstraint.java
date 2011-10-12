package frodo.COP;

import java.util.Random;
import java.util.Vector;

import frodo.CSP.BinaryConstraint;
import frodo.CSP.CSPVariable;
import frodo.CSP.VariableId;
import frodo.OCOP.DPOP.HyperCube;
import frodo.utils.HelperUtilities;

/**
 * @author apetcu
 * 
 * To change the template for this generated type comment go to Window>Preferences>Java>Code Generation>Code and Comments
 */
//--------------------------------------------------------------------------------------------------------------//
public class ValuedConstraint extends BinaryConstraint {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	int[][] valuations;

	int id;

	public int getId() {
		return id;
	}

	/**
	 * @param id - this is the id to be set for this constraint
	 * @return nothing, this is a void method 
	 */
	public void setId(int id) {
		this.id = id;
	}

	//	--------------------------------------------------------------------------------------------------------------//
	public ValuedConstraint(CSPVariable from, CSPVariable to) {
		super(from, to);
		this.setType('-');
		valuations = new int[from.getDomain().size()][to.getDomain().size()];
		arity = 2;
	}

	//	--------------------------------------------------------------------------------------------------------------//
	public ValuedConstraint(CSPVariable from) {
		super(from, from);
		this.setType('!');
		valuations = new int[from.getDomain().size()][1];
		arity = 1;
	}

	//	--------------------------------------------------------------------------------------------------------------//
	/**
	 *  
	 */
	public int getValuation(Object local, Object remote) {
		if (!isActive())
			return 0;
		int ndxLocal = from.findValue(local);
		if (ndxLocal == -1) {
			System.out.println("\n Could not find: localVal= " + local + " localNDX=" + ndxLocal);
			System.exit(0);
			return -1;
		}
		if (arity == 2) {
			int ndxRemote = to.findValue(remote);
			if (ndxRemote == -1) {
				System.out.println("\nX" + getSourceId() + "-" + getDestinationId() + ": Could not find: remoteVAL=" + remote + " remote=" + ndxRemote);
				System.exit(0);
				return -1;
			} else
				return valuations[ndxLocal][ndxRemote];
		} else {
			return valuations[ndxLocal][0];
		}
	}

	//	--------------------------------------------------------------------------------------------------------------//
	public int getValuation() {
		if (!isActive())
			return 0;
		else {
			if (arity == 2)
				return this.getValuation(from.getValue(), to.getValue());
			else
				return this.getValuation(from.getValue());
		}
	}

	//	--------------------------------------------------------------------------------------------------------------//
	//	--------------------------------------------------------------------------------------------------------------//
	public int getValuation(Object val) {
		if (arity != 1) {
			System.out.println("Called getEvaluation like for an unary constraint, but on a " + arity + "-ary constraint...");
			System.exit(1);
			return 1;
		} else {
			int ndxLocal = from.findValue(val);
			if (ndxLocal < 0) {
				System.out.println("\n Could not find: localVal= " + val + " localNDX=" + ndxLocal);
				System.exit(0);
				return -1;
			} else
				return valuations[ndxLocal][0];
		}
	}

	//	--------------------------------------------------------------------------------------------------------------//
	public void setValuation(Integer local, Integer remote, int val) {
		int ndxLocal = from.findValue(local);
		int ndxRemote = to.findValue(remote);

		if ((ndxLocal == -1) || (ndxRemote == -1))
			System.out.println("Could not find tuple: (" + local + "," + remote + ") -> indexLocal=" + ndxLocal + " indexRemote=" + ndxRemote);
		else
			valuations[ndxLocal][ndxRemote] = val;
	}

	//	--------------------------------------------------------------------------------------------------------------//
	public void setValuation(Integer local, int val) {
		int ndxLocal = from.findValue(local);

		if (ndxLocal == -1)
			System.out.println("Could not find value: (" + local + ") -> indexLocal=" + ndxLocal + " indexRemote=");
		else
			valuations[ndxLocal][0] = val;
	}

	//	--------------------------------------------------------------------------------------------------------------//
	public String toString() {
		//		String line = "(" + from.getId() + "-" + to.getId() + ")\n";
		//		line = line + HelperUtilities.arrayToString(valuations);
		String line = new HyperCube(this).toStringVectors();
		return line;
	}

	//	--------------------------------------------------------------------------------------------------------------//
	public String toStringSimple() {
		String line = "Agent " + getOwnerAgent().getAgentId() + " (" + from.getId() + "-" + to.getId() + ") -> " + getValuation() + "\n";
		line += "\n";
		return line;
	}

	//--------------------------------------------------------------------------------------------------------------//
	public void translateEvaluations() {
		for (int i = 0; i < valuations.length; i++) {
			for (int j = 0; j < valuations[0].length; j++) {
				valuations[i][j] = 5 - valuations[i][j];
			}
		}
	}

	//	--------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------//
	public int[] getRemoteValuations(Integer localValue) {
		int ndxLocal = from.findValue(localValue);
		return valuations[ndxLocal];
	}

	//--------------------------------------------------------------------------------------------------------------//
	public static ValuedConstraint fromString(String str) {
		ValuedConstraint vc = null;
		str.trim();
		int agentId = -1;
		//				System.out.println("Gonna build constraint out of: |" + str + "|");
		if (str.length() > 0) {
			char type;
			String[] parts = str.split("\\s");
			agentId = Integer.parseInt(parts[0]);
			//			String[] ids = parts[1].split("<->");
			//			String[] ids = parts[1].split("<[^\\s]*>");
			String[] ids = parts[1].split("@");
			//			String[] ids = parts[1].split("[-\\D]*");
			VariableId fromId = VariableId.fromString(ids[0]);
			type = ids[1].charAt(0);
			VariableId toId = VariableId.fromString(ids[2]);
			Integer lvals[] = new Integer[parts.length - 2];
			Integer rvals[] = new Integer[parts.length - 2];
			Integer evals[] = new Integer[parts.length - 2];

			//reading tuples and valuations
			for (int i = 2; i < parts.length; i++) {
				//this breaks the tuple into components; sth. like 1,2:3 - value 1 of var1, value 2 of var2 give util 3
				//				String[] tuple = parts[i].split("^[^[-\\d*]]");
				//				System.out.println("PARTS (" + i+ ")=" + parts[i]);
				String[] tuple = parts[i].split("[[:]*[,]*]+");
				//				System.out.println("SPLITS (" + i+ ")=" + HelperUtilities.arrayToString(tuple));
				lvals[i - 2] = Integer.valueOf(tuple[0]);
				rvals[i - 2] = Integer.valueOf(tuple[1]);
				evals[i - 2] = Integer.valueOf(tuple[2]);
			}
			if (fromId.equals(toId)) {
				//this is a unary constraint. either a real unary, or a stability constraint.
				//				System.out.println("Unary constraint on " + fromId);

				Vector dom = new Vector();
				for (int i = 0; i < lvals.length; i++) {
					//have to add values, since this is a newly constructed variable
					if (!dom.contains(lvals[i]))
						dom.add(lvals[i]);
				}
				CSPVariable from = new CSPVariable(fromId, dom);
				if (type == '$') {
					//					System.out.println("Stability constraint on " + fromId + " with " + lvals.length + " tuples.");
					vc = new StabilityConstraint(from);
					for (int i = 0; i < lvals.length; i++) {
						//						System.out.println("LVALS[" + i + "]=" + lvals[i] + "\tRVALS[" + i + "]=" + rvals[i] + "\tEVALS[" + i + "]=" + evals[i]);
						vc.setValuation(lvals[i], rvals[i], evals[i].intValue());
					}
				} else {
					//					System.out.println("Unary constraint on " + fromId);
					vc = new ValuedConstraint(from);
					for (int i = 0; i < dom.size(); i++) {
						vc.setValuation((Integer) dom.elementAt(i), evals[i].intValue());
					}
				}
			} else {
				//this is a binary constraint
				//first we need to build the domains, because the valuation structures are initialized with the sizes equal with the size of the domains
				Vector ldom = new Vector();
				Vector rdom = new Vector();
				for (int i = 0; i < lvals.length; i++) {
					if (!ldom.contains(lvals[i]))
						ldom.add(lvals[i]);
					if (!rdom.contains(rvals[i]))
						rdom.add(rvals[i]);
				}
				CSPVariable fromVar = new CSPVariable(fromId, ldom);
				CSPVariable toVar = new CSPVariable(toId, ldom);
				vc = new ValuedConstraint(fromVar, toVar);

				//reading tuples and valuations
				for (int i = 0; i < lvals.length; i++) {
					vc.setValuation(lvals[i], rvals[i], evals[i].intValue());
				}
			}
			vc.setOwnerAgentId(agentId);
			vc.setType(type);
		}
		return vc;
	}

	//--------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------//

	public int[][] getValuations() {
		return valuations;
	}

	public void setValuations(int[][] valuations) {
		this.valuations = valuations;
	}

	//--------------------------------------------------------------------------------------------------------------//
	public void setRandomValuations(Random rnd, int bound) {
		for (int i = 0; i < from.getDomainSize(); i++) {
			for (int j = 0; j < to.getDomainSize(); j++) {
				valuations[i][j] = rnd.nextInt(bound);
			}
		}
	}

	/**
	 * @param rnd - an initialized generator
	 * @param normal_bound - the maximal valuation for "normal" tuples
	 * @param max_bound - the maximal valuation for "peak" tuples
	 * @param percentage_peaks - how many "peak" tuples
	 */
	public void setRandomValuationsWithPeaks(Random rnd, int normal_bound, int max_bound, double percentage_peaks) {
		for (int i = 0; i < from.getDomainSize(); i++) {
			for (int j = 0; j < to.getDomainSize(); j++) {
				valuations[i][j] = rnd.nextInt(normal_bound);
			}
		}
		int howmanyTotal = valuations.length * valuations[0].length;
		int howmanyToAlter = (int) (howmanyTotal * percentage_peaks);
		for (int i = 0; i < howmanyToAlter; i++) {
			int which = rnd.nextInt(howmanyTotal);
			int what = rnd.nextInt(max_bound);
			valuations[which / valuations.length][which % valuations.length] = what;
		}
	}

	//--------------------------------------------------------------------------------------------------------------//
	public static ValuedConstraint transposed(ValuedConstraint src) {
		ValuedConstraint constrTo = new ValuedConstraint((CSPVariable) src.getTo(), (CSPVariable) src.getFrom());
		constrTo.setOwnerAgent(src.getOwnerAgent());
		constrTo.setValuations(HelperUtilities.intArrayTranspose(src.getValuations()));
		return constrTo;
	}

	//--------------------------------------------------------------------------------------------------------------//
	public void deductFromValuations(int value) {
		for (int i = 0; i < valuations.length; i++) {
			for (int j = 0; j < valuations[i].length; j++) {
				valuations[i][j] = value - valuations[i][j];
			}
		}
	}

}
//--------------------------------------------------------------------------------------------------------------//
