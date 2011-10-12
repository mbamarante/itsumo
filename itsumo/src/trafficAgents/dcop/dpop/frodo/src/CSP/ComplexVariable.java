package frodo.CSP;

import java.util.Vector;

import frodo.utils.Logger;


/**
 * @author apetcu
 * This class is a wrapper for multiple local values, and also has an associated localCost for each combination of values
 * 
 */
public class ComplexVariable {
	//	Vector values; //my domain, made of ComplexValues = this is an explicit representation of all the possible combinations of values of my variables

	Vector variables;

	Logger myLogger;

	ComplexValue value;

	int agentId;

	//-----------------------------------------------------------------------//
	public ComplexVariable(Vector vars, int agentId) {
		//		variables = new Vector(vars);
		variables = vars;
		//		values = new Vector();
		this.agentId = agentId;
		//		makeLocalComplexVariable(null, 0);
		/*
		 myLogger.println(toString());
		 myLogger.println("========================||||||||||||||||||||||=====================");
		 */
	}

	//-----------------------------------------------------------------------//
	/**
	 * Returns the values.
	 * @return Vector
	 */
	public Vector getValues() {
		return null;
	}

	//-----------------------------------------------------------------------//
	/**

	 public ComplexValue getValue(int i) {
	 if (i >= 0 && i < values.size())
	 return (ComplexValue) values.elementAt(i);
	 else
	 return null;
	 }
	 */
	//-----------------------------------------------------------------------//
	public CSPVariable getVariable(VariableId vid) {
		for (int i = 0; i < variables.size(); i++) {
			if (((CSPVariable) variables.elementAt(i)).getId().equals(vid))
				return (CSPVariable) variables.elementAt(i);
		}
		return null;
	}

	//-----------------------------------------------------------------------//
	/*	
	 public String toString() {
	 myLogger.println(values.size() + " values.");
	 String val = "{";
	 if (values == null)
	 val += "null";
	 else
	 for (int i = 0; i < values.size(); i++)
	 val = val + "," + values.elementAt(i).toString() + "\n";
	 val = val + "}";
	 return val;
	 }
	 */
	//-----------------------------------------------------------------------//
	/*
	 private void makeLocalComplexVariable(Vector vars, int var) {
	 //			Vector rez = new Vector();
	 //		if (var == 0)
	 //		myLogger.println(((CSPAbstractVariable) vars.elementAt(0)).toStringFull());

	 //		myLogger.println("Entering recursive: level=" + var + "\tvars=" + vars + "\tvariables.size=" + variables.size());
	 if (var < variables.size()) {
	 CSPVariable tmpVar = (CSPVariable) variables.elementAt(var);
	 for (int i = 0; i < tmpVar.getDomain().size(); i++) {
	 Vector tmpvars = null;
	 Object tmpVal = tmpVar.getDomain().elementAt(i);

	 if (vars == null) {
	 tmpvars = new Vector();
	 } else {
	 //					if (i != 0)
	 tmpvars = new Vector(vars);
	 //					else
	 //						tmpvars = new Vector();
	 }
	 tmpvars.add(tmpVal);

	 //				myLogger.println("Recursive level=" + var + "\ttmpvars=" + tmpvars + "\t");
	 //				myLogger.print(tmpVal.toString());
	 makeLocalComplexVariable(tmpvars, var + 1);

	 }
	 } else {
	 howm++;
	 ComplexValue cmplVal = new ComplexValue(vars);
	 values.add(cmplVal);
	 //			myLogger.println("level " + var + "Created " + howm + "-> " + cmplVal);
	 //			myLogger.println("");
	 //			return rez;
	 }
	 }
	 */
	//-----------------------------------------------------------------------//
	/**
	 * @return
	 */
	public ComplexValue getValue() {
		return value;
	}

	//-----------------------------------------------------------------------//
	/**
	 * @param value
	 */
	public void setValue(ComplexValue value1) {
		value = value1;
	}

	//-----------------------------------------------------------------------//
	/*
	 public void resetLowerBounds() {
	 for (int i = 0; i < this.values.size(); i++)
	 getValue(i).resetLowerBound();
	 }
	 */
	//-----------------------------------------------------------------------//
	public int size() {
		return variables.size();
	}
	//-----------------------------------------------------------------------//
}