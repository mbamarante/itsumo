/**********************************************************************
 * File Name: Variable.java
 * Author: Jay Modi (modi@isi.edu)
 * Date: Nov 11 2000
 *
 **********************************************************************/
package adopt.problem;
import adopt.common.*;

import java.lang.*;
import java.io.*;
import java.util.*;


/**
 * Class for defining a DCOP Variable.
 * All variable subclasses must implement the abstract methods defined in this class.
 */

public class Variable {

  /*list of variables this variable is connected to. */
  Vector neighbors;
  /* id of this variable */
  public int varID;
  /* id of agent this variable belongs to. */
  public int agentID;
  /** The domain of possible values for this variable. */
  Value[] domain;
  /** Size of the domain */
  int domainSize;
 
 /* Thresholds for all values of variables. Added by Borar and Iyer */	
 	public Hashtable  valueThresholds; 

    public boolean[] thresholdCalculated;
 	
 	int[] valuethreshold;
  /* Initial Threshold. Added by Pradeep */
  private int initThreshold;
  

  /* Class Constructor */
  public Variable(int agentID, int varID, int domain_s) {
    this.agentID = agentID;
    this.varID = varID;
    domainSize = domain_s;
    /* set the domain of the variable to be int values between [0,<domain_size>]. */
    domain = new NominalValue[domainSize];
    for(int i=0;i<domainSize;i++){
      domain[i] = new NominalValue(i);
      
    } 
    
    valuethreshold = new int[domainSize];
      for(int i=0;i<domainSize;i++) valuethreshold[i] = 0; 
    //  for(int i=0;i<domainSize;i++)
    //  	valuethreshold[i] = 0 ;
    
    neighbors = new Vector();
    thresholdCalculated = new boolean[domainSize];
    for(int i=0; i<domainSize; i++)
    	thresholdCalculated[i]= false;
  }


  /* Added by Pradeep*/
  /* Class Constructor */
  public Variable(int agentID, int varID, int domain_s,int initThreshold) {
      this.agentID = agentID;
      this.varID = varID;
      this.initThreshold = initThreshold;
      domainSize = domain_s;
      /* set the domain of the variable to be int values between [0,<domain_size>]. */
      domain = new NominalValue[domainSize];
      for(int i=0;i<domainSize;i++){
        domain[i] = new NominalValue(i);
      }
    valuethreshold = new int[domainSize];
    for(int i=0;i<domainSize;i++) valuethreshold[i] = 0;
    
      neighbors = new Vector();
          thresholdCalculated = new boolean[domainSize];
    for(int i=0; i<domainSize; i++)
    	thresholdCalculated[i]= false;

  }

  
 
  
  /* Setting the initial threshold for a value*/
  public void setInitThreshold(NominalValue val1, int initThreshold) 
  {
 	String con;
 	con = val1.value;
 	//int conv ;
 	Integer conv = new Integer(con);
 	int index = conv.intValue();
  	valuethreshold[index] = initThreshold;
  	thresholdCalculated[index] = true;
  	
	//System.out.println("index = " + index);
	//valueThresholds.put(conv, initThreshold);
  }

public void setInitThreshold(int initThreshold) {
	this.initThreshold = initThreshold;
		
  }

  /* Getting the initial threshold, Added by Pradeep*/
  public int getInitThreshold() {
  	  return initThreshold;
  }

/*Getting the thresholds for all values */
public int getInitThreshold(NominalValue val1)
	{	
		String con1;
 		con1 = val1.value;
 		Integer conv1 = new Integer(con1);
 	//	int te = (int)valueThresholds.get(conv1);
		int index = conv1.intValue();
	//	System.out.println("Variable = "+this.varIDof() +"Index="+index+"Value = "+valuethreshold[index]);
		int te = valuethreshold[index];	
		return(te); 
	
	}
			
public int getInitThreshold(Value val1)
{
	
	String val = ((NominalValue)(val1)).value;
	NominalValue val2 = new NominalValue(val);
	return this.getInitThreshold(val2);
	
}

  /* Returns a negative integer, zero, or a positive integer as val1
     is less than, equal to, or greater val2. */
  public int compareValues(Value val1, Value val2){
    return Variable.compareIntValues(val1, val2);
  }

  /* return varID of variable */
  public int varIDof(){
    return varID;
  }

  /* return name of agent who owns this variable */
  public int agentIDof(){
    return agentID;
  }

  /* return string representing agentname/varname
     this is called a 'uName'*/
  public String uniqueNameOf(){
    return "agent"+agentID+"-var"+varID;
  }

  /* return true if the given uname is 'this' variable */
  public boolean isUniqueNameOf(String uname){
    if(uname.equals(uniqueNameOf()))
       return true;
    return false;
  }

  /* is the given variable equal to 'this'?*/
  public boolean equalVar(Variable v){
    if(v.uniqueNameOf().equals(uniqueNameOf()))
      return true;
    return false;
  }

  /** Get the domain index of the given value.
   * @param v a value within the domain of this variable.
   * @return domain index corresponding to the given value or -1 if
   * the value is not within the domain. */
  public int indexOf(Value v){
    for(int i=0;i<domain.length;i++)
      if(v.equal(domain[i]))
	return i;
    return -1;
  }

  public void printVariable(){
    Utility.Dprintc("VARIABLE " + varID + " ", 0);
    for(int j=0;j<domain.length;j++){
      Utility.Dprintc(domain[j].toString() + " ", 0);
    }
    Utility.Dprint("", 0);
  }

  /* Get value with given name */
  public Value getValue(String val){

    for(int i=0;i<domain.length;i++)
      if(domain[i].toString().equals(val))
	return domain[i];

    Utility.Dprint("Variable.getValue(): Unknown Value: " + val,
		   Utility.TRACE_FUNCTION_LEVEL);
    return null;
  }

  /** Get the size of this variable's domain.
   *
   * @return the size of domain
   */
  public int domainSize(){
    return domain.length;
  }

  /** Returns the domain of this variable. DO NOT MODIFY this array.
   *@return domain of this variable.
   */
  public Value[] domain(){
    return domain;
  }

  /* Returns a negative integer, zero, or a positive integer as v1
     is less than, equal to, or greater v2.
     NOTE: assume values are ints.
     */
  public static int compareIntValues(Value val1, Value val2){
    try{
      int vval1 = new Integer(val1.toString()).intValue();
      int vval2 = new Integer(val2.toString()).intValue();
      if(vval1 > vval2)
	return 1;
      else if(vval2 > vval1)
	return -1;
      return 0;
    }catch(Exception e){
      Utility.Dprint("  Error in Variable.compareValues(): Vals are not ints." + e,0);
    }
    return 0;
  }

  /* parse the name and return the id */
  public static int uniqueNameToAgentID(String name){
    int idx = name.lastIndexOf("agent");
    if(idx < 0){
	System.out.println("nameToId(): Could not parse agent name");
	return -1;
      }

    String s = name.substring(idx + 5);
    int result = new Integer(s).intValue();
    return result;
  }


  /* is the given variable a neighbor of this variable?*/
  public boolean isNeighbor(Variable v){

    for(int i=0;i<neighbors.size();i++){
      Variable vv = (Variable) neighbors.elementAt(i);
      if(v.equalVar(vv))
	return true;
    }
    return false;
  }

  public void addNeighbor(Variable v){
    neighbors.addElement(v);
  }

//   public static void main(String args[])
//   {
//     Variable a = new A();
//     Value v = new NominalValue("0yes");
//     System.out.println(((NominalValue)v).getString());
//     System.out.println(a.indexOf(v));
//     a.setValue(a.indexOf(v));
//   }
}
