/*
 *    MaxCSPProblem.java
 *    Author: Jay Modi
 *    Date: Feb 22 2002
 *
 *    Specifies a DCO problem for (Weighted) MaxCSP. 
 *  
 *    Method for reading a problem from a file, evaluating constraints.  
 *     
 */

package adopt.problem;

import adopt.common.*;

import java.io.*;
import java.util.*;
import java.net.*;

public class MaxCSPProblem extends Problem{
  /* Agents, Variables in superclass */

  /* Set of Constraints */
  Hashtable constraints;

  public MaxCSPProblem(String fname, int _order){
    super("MaxCSPProblem");

    constraints = new Hashtable();

    System.out.println("Reading input file... ");
    /* set agents and variables */
    readTaskFile(fname);
    System.out.println("  Forming DFS tree... ");
    _orderSwitch = _order;
    if(_orderSwitch == CHAIN){
      pOrderChain = priorityChain();
    }
    else if(_orderSwitch == TREE){
      pOrderTree = priorityTree();
    }
    else if(_orderSwitch == RANDOMCHAIN){
      pOrderChain = priorityRandom();
    }
    else{
      System.out.println("MaxCSPProblem(): Unknown _orderSwitch: " + _orderSwitch);
      System.exit(0);
    }
    System.out.println("  ...done forming DFS tree.");
    System.out.println("...done reading input file. ");
  }

  /* Read MaxCSP input file. Format is like:

     AGENT <agentid>
     VARIABLE <varid> <agentid> <domain size>

     CONSTRAINT <varid1> <varid2> <weight (optional)>
     NOGOOD <value1> <value2> #<value1> must be some integer less than <domain size> of <varid1>
     NOGOOD <value1> <value2>
     etc.


     Example of a well-formatted file for graph coloring with a single
     agent with 8 variables.
     -------------------------------------------------------------------

     AGENT 1
     VARIABLE 0 1 3
     VARIABLE 1 1 3
     VARIABLE 2 1 3
     VARIABLE 3 1 3
     VARIABLE 4 1 3
     VARIABLE 5 1 3
     VARIABLE 6 1 3
     VARIABLE 7 1 3
     CONSTRAINT 0 2
     NOGOOD 0 0
     NOGOOD 1 1
     NOGOOD 2 2
     CONSTRAINT 0 5
     NOGOOD 0 0
     NOGOOD 1 1
     NOGOOD 2 2
     CONSTRAINT 0 6
     NOGOOD 0 0
     NOGOOD 1 1
     NOGOOD 2 2
     CONSTRAINT 1 2
     NOGOOD 0 0
     NOGOOD 1 1
     NOGOOD 2 2
     CONSTRAINT 1 3
     NOGOOD 0 0
     NOGOOD 1 1
     NOGOOD 2 2
     CONSTRAINT 1 5
     NOGOOD 0 0
     NOGOOD 1 1
     NOGOOD 2 2
     CONSTRAINT 1 7
     NOGOOD 0 0
     NOGOOD 1 1
     NOGOOD 2 2
     CONSTRAINT 2 4
     NOGOOD 0 0
     NOGOOD 1 1
     NOGOOD 2 2
     CONSTRAINT 2 5
     NOGOOD 0 0
     NOGOOD 1 1
     NOGOOD 2 2
     CONSTRAINT 2 7
     NOGOOD 0 0
     NOGOOD 1 1
     NOGOOD 2 2
     CONSTRAINT 3 5
     NOGOOD 0 0
     NOGOOD 1 1
     NOGOOD 2 2
     CONSTRAINT 3 6
     NOGOOD 0 0
     NOGOOD 1 1
     NOGOOD 2 2
     CONSTRAINT 4 5
     NOGOOD 0 0
     NOGOOD 1 1
     NOGOOD 2 2
     CONSTRAINT 4 6
     NOGOOD 0 0
     NOGOOD 1 1
     NOGOOD 2 2
     CONSTRAINT 5 7
     NOGOOD 0 0
     NOGOOD 1 1
     NOGOOD 2 2
     CONSTRAINT 6 7
     NOGOOD 0 0
     NOGOOD 1 1
     NOGOOD 2 2 */

  public void readTaskFile(String fileName){
    
    BufferedReader instream = null;
    boolean done = false;
    String line = null;
    StringTokenizer t;
    Vector agnts = new Vector();
    MaxCSPConstraint currentConstraint = null;

    Utility.Dprint("  Reading config file " + fileName, Utility.TRACE_RIDICULOUS_LEVEL);

    try{ 
      instream = new BufferedReader(new FileReader(fileName)); 
    }catch(Exception e){
      System.out.println(e);
      System.out.println("Could not open " + fileName);
      System.exit(0);
    }

    while(!done){
      try {
	line = instream.readLine();
      }	catch(Exception e) {
	System.out.println(e);
	done = true;
      }
      
      if (done || line == null)	{
	done = true;
	Utility.Dprint("  End of file reached.", Utility.TRACE_RIDICULOUS_LEVEL);
	break;
      } 

      if(line.startsWith("#") ||
	 line.startsWith("\n") ){
	continue;
      }
      
      t = new StringTokenizer(line);
      if(t.hasMoreElements())
	t.nextToken(); // get rid of first token	      
      else
	continue;

      if(line.startsWith("AGENT")){
	try {
	  Integer id = new Integer(t.nextToken());
	  agnts.add(id);
	} catch(Exception e){
	  Utility.Dprint("  Error in AGENT line : " + e, 0);
	  Utility.Dprint("    " + line, 0);
	}	
      }
      else if(line.startsWith("VARIABLE")){
	try{
	  /* create the variable, and add them to the 'vars' list */
	  int tid = new Integer(t.nextToken()).intValue(); // var id
	  int aid = new Integer(t.nextToken()).intValue(); // agent id
	  int size = new Integer(t.nextToken()).intValue(); // domain size
	  int[] agentidList;
	  Variable tv  = new Variable(aid, tid, size);
	  vars.add(tv);
	}catch(Exception e){
	  Utility.Dprint("  Error in VARIABLE line : " + e, 0);
	  Utility.Dprint("    " + line, 0);
	}
      }
      else if(line.startsWith("CONSTRAINT")){
	try{
	  int v1 = new Integer(t.nextToken()).intValue(); // var id
	  Variable var1 = getVariableFromID(v1);
	  int v2 = new Integer(t.nextToken()).intValue(); // var id
	  Variable var2 = getVariableFromID(v2);
	  int cost = 0;
	  try{
	    cost = new Integer(t.nextToken()).intValue(); // cost
	  }catch(Exception e){
	    cost = 1;
	  }
	  if(var1 == null || var2 == null){
	    throw new Exception("Unknown variable");
	  }
	  for(int i=0;i<vars.size();i++){
	    Variable v =  (Variable) vars.elementAt(i);
	    if (v.varID == var1.varID)
	      var1.addNeighbor(var2);
	    else if(v.varID == var2.varID)
	      var2.addNeighbor(var1);
	  }
	  currentConstraint = new MaxCSPConstraint(var1, var2, cost);
	  constraints.put(new Integer(currentConstraint.hashCode()), currentConstraint);
	}catch(Exception e){
	  Utility.Dprint("  Error in CONSTRAINT line : " + e, 0);
	  Utility.Dprint("    " + line, 0);
	}
      }
      else if(line.startsWith("NOGOOD")){
	int v1 = new Integer(t.nextToken()).intValue(); // value
	Value val1 = new NominalValue(v1);
	int v2 = new Integer(t.nextToken()).intValue(); // value
	Value val2 = new NominalValue(v2);
	currentConstraint.addNoGood(val1,val2);
      }
      else{
	// we don't know this line, ignore and go back...
	Utility.Dprint("Unknown Line...", 0);
	continue;
      } // end while	 
    }
    setAgents(agnts);


    /* do some consistency checks */
    /*1. varIDs must be [0...n] */
    int[] map2 = new int[vars.size()];
    for(int j=0;j<vars.size();j++){
      map2[j] = -1;
    }
    for(int j=0;j<vars.size();j++){
      for(int i=0;i<vars.size();i++){
	Variable v1 = (Variable) vars.elementAt(i);
	if (v1.varID == j)
	  map2[j] = 1;
      }
    }
    for(int j=0;j<vars.size();j++){
      if(map2[j] < 0 ){
	System.out.println(" Error: Variable ID's must be [0...n]!");
      	System.out.println("    (" + fileName + " is missing variable ID " + j + ")");
	System.exit(0);
      }
    }
  }

  public void printMaxCSPProblem(){
    
    for(int i=0;i<agents.size();i++){
      int aid  = ((Integer) agents.elementAt(i)).intValue();
      Utility.Dprint("AGENT " + aid, 0);
      Vector vvars = getVariablesFromAgentID(aid);
      for(int j=0;j<vvars.size();j++){
	Variable tv = (Variable) vvars.elementAt(j);
	tv.printVariable();
	for(int k=0;k<tv.neighbors.size();k++){
	  Variable v = (Variable) tv.neighbors.elementAt(k);
	  MaxCSPConstraint mc = getConstraint(tv, v);
	  Utility.Dprintc("  ");
	  v.printVariable();
	  mc.printConstraint();
	}
      }
    }
  }

  /* return true if priority(v1) > priority(v2), false otherwise */
  public boolean comparePriority(Variable v1, Variable v2){
    
    if(isChainOrder())
      return comparePriorityChain(v1,  v2);
    else if(isTreeOrder())
      return comparePriorityTree(v1,  v2);

    System.out.println("MaxCSPProblem.comparePriority(): Unknown _orderSwitch");
    System.exit(0);
    return false;
  }

  /* no unary constraints */
  public int evaluate(Variable q1, Value v1){
    return 0;
  }

  /* look up the constraint between these two variables, 
     return null if no constraint */
  public MaxCSPConstraint getConstraint(Variable q1, Variable q2){
    MaxCSPConstraint mc = new MaxCSPConstraint(q1,q2, 0);
    MaxCSPConstraint mmc = (MaxCSPConstraint) 
      constraints.get(new Integer(mc.hashCode()));
    return mmc;
  }

  /* evaluate binary constraints */
  public int evaluate(Variable q1, Value v1, Variable q2, Value v2){
    int rc = 0;

    Utility.Dprint("Entering MaxCSPProblem.evaluate", 
		   Utility.TRACE_RIDICULOUS_LEVEL);
    int id1 = q1.varID;
    int id2 = q2.varID;
    int val1 = new Integer(v1.toString()).intValue();
    int val2 = new Integer(v2.toString()).intValue();
    Utility.Dprint("  Comparing ("+id1+","+val1+") to ("+id2+","+val2+")", 
		   Utility.TRACE_RIDICULOUS_LEVEL);

    if(connected(q1, q2)){
      MaxCSPConstraint mmc = getConstraint(q1,q2);
      rc = mmc.evaluate(q1,v1,q2,v2);
    }
    return rc;
  }

  /* no n-ary constraint */
  public int evaluate(Variable q1, Value v1, Context vw){

    Utility.Dprint("Entering MaxCSP.evaluate(nary)", 
		   Utility.TRACE_RIDICULOUS_LEVEL);
    Utility.Dprint("    context: " + vw.toString(), 
		   Utility.TRACE_RIDICULOUS_LEVEL);
    int rc = 0;
    return rc;
  }

  
  public static void main(String args[])
  {
    System.exit(0);
  }
}

