/*
 *    MaxCOPProblem.java
 *    Author: Jay Modi
 *    Date: Feb 22 2002
 *
 *    Specifies a DCO problem for (Weighted) MaxCOP.
 *
 *    Method for reading a problem from a file, evaluating constraints.
 *
 *    Minimally altered by Emma Bowring, Sep 22 2003
 */

package adopt.problem;
import adopt.common.*;

import java.io.*;
import java.util.*;
import java.net.*;

public class MaxCOPProblem extends Problem{
  /* Agents, Variables in superclass */
	
  /* Set of Constraints */
 public static Hashtable constraints;
  int[][] constraintValues;

  public MaxCOPProblem(String fname, int _order,int treeType){
    super("MaxCOPProblem");

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
		if(treeType == 1) {
      		pOrderTree = priorityTree();
      		/*********************************/
      		//System.out.println("Hi I am printing");
      		//System.out.println(pOrderTree.toString());
      		//System.out.println("Hi here");
      		System.out.println("sub is " + sub);
      		System.out.println("div is " + div);
      		calculateVarThresholds(pOrderTree,constraintValues);
      		calculateThresholds(pOrderTree);
      		/*********************************/
		}
      	else if(treeType == 2) {
      		pOrderTree = priorityTree21();
      		calculateVarThresholds(pOrderTree,constraintValues);
      		calculateThresholds(pOrderTree);
		}
      	else if(treeType == 3) {
      		pOrderTree = priorityTree31();
      		calculateVarThresholds(pOrderTree,constraintValues);
      		calculateThresholds(pOrderTree);
		}
      System.out.println(pOrderTree.toString());
    }
    else if(_orderSwitch == RANDOMCHAIN){
      pOrderChain = priorityRandom();
    }
    else{
      System.out.println("MaxCOPProblem(): Unknown _orderSwitch: " + _orderSwitch);
      System.exit(0);
    }
    System.out.println("  ...done forming DFS tree.");
    System.out.println("...done reading input file. ");
    
   /*try{
    MaxCOPConstraint currConstraint = null;

	System.out.println("Adding variables that are not directly connected");
	
    int b=0;
    while(b < vars.size() && notConnectedVars[b] != -1) 
    {
		int v1 = notConnectedVars[b];
		int v2 = pOrderTree.getRootVariable().varIDof();
		System.out.println("Connecting "+v1+"and"+v2);
		if(v1 > v2) 
		{
			int temp = v1;
			v1 = v2;
			v2= temp;
		}
		
		Variable var1 = getVariableFromID(v1);
		Variable var2 = getVariableFromID(v2);
		  
		
	  currConstraint = new MaxCOPConstraint(var1, var2);
	  constraints.put(new Integer(currConstraint.hashCode()), currConstraint);
	
		for(int m=0; m < var1.domainSize; m++)
			for(int n=0; n < var2.domainSize; n++)
			{
				
				Value val1 = new NominalValue(m);
				Value val2 = new NominalValue(n);
				currConstraint.addNoGood(val1,val2,1);//value 
	
				
				} 
    
    	b++;
    }
    
	} // end of try
	catch(Exception e)
	{
		System.out.println(e);
	}*/
  }

  /* Read MaxCOP input file. Format is like:

     AGENT <agentid>
     VARIABLE <varid> <agentid> <domain size>

     CONSTRAINT <varid1> <varid2>
     NOGOOD <value1> <value2> #<value1>  must be some integer less than <domain size> of <varid1>
     NOGOOD <value1> <value2> <weight (optional)>
     etc.

Example of a well formatted meeting scheduling problem with 3 meetings involving
common agents
     -------------------------------------------------------------------

AGENT 1
AGENT 2
AGENT 3
VARIABLE 0 1 5
VARIABLE 1 2 5
VARIABLE 2 3 5
CONSTRAINT 0 1
NOGOOD 0 0 84
NOGOOD 0 1 66
NOGOOD 0 2 67
NOGOOD 0 3 69
NOGOOD 0 4 63
NOGOOD 1 0 65
NOGOOD 1 1 50000
NOGOOD 1 2 69
NOGOOD 1 3 71
NOGOOD 1 4 65
NOGOOD 2 0 65
NOGOOD 2 1 68
NOGOOD 2 2 50000
NOGOOD 2 3 71
NOGOOD 2 4 65
NOGOOD 3 0 69
NOGOOD 3 1 72
NOGOOD 3 2 73
NOGOOD 3 3 50000
NOGOOD 3 4 69
NOGOOD 4 0 63
NOGOOD 4 1 66
NOGOOD 4 2 67
NOGOOD 4 3 69
NOGOOD 4 4 50000
CONSTRAINT 1 2
NOGOOD 0 0 84
NOGOOD 0 1 67
NOGOOD 0 2 68
NOGOOD 0 3 70
NOGOOD 0 4 63
NOGOOD 1 0 66
NOGOOD 1 1 50000
NOGOOD 1 2 71
NOGOOD 1 3 73
NOGOOD 1 4 66
NOGOOD 2 0 67
NOGOOD 2 1 71
NOGOOD 2 2 50000
NOGOOD 2 3 74
NOGOOD 2 4 67
NOGOOD 3 0 69
NOGOOD 3 1 73
NOGOOD 3 2 74
NOGOOD 3 3 50000
NOGOOD 3 4 69
NOGOOD 4 0 63
NOGOOD 4 1 67
NOGOOD 4 2 68
NOGOOD 4 3 70
NOGOOD 4 4 50000
CONSTRAINT 0 2
NOGOOD 0 0 84
NOGOOD 0 1 65
NOGOOD 0 2 65
NOGOOD 0 3 69
NOGOOD 0 4 63
NOGOOD 1 0 67
NOGOOD 1 1 50000
NOGOOD 1 2 70
NOGOOD 1 3 72
NOGOOD 1 4 65
NOGOOD 2 0 68
NOGOOD 2 1 69
NOGOOD 2 2 50000
NOGOOD 2 3 72
NOGOOD 2 4 65
NOGOOD 3 0 70
NOGOOD 3 1 73
NOGOOD 3 2 74
NOGOOD 3 3 50000
NOGOOD 3 4 69
NOGOOD 4 0 63
NOGOOD 4 1 67
NOGOOD 4 2 68
NOGOOD 4 3 70
NOGOOD 4 4 50000 */

  public void readTaskFile(String fileName){

    BufferedReader instream = null;
    boolean done = false;
    String line = null;
    StringTokenizer t;
    Vector agnts = new Vector();
    MaxCOPConstraint currentConstraint = null;
    int temp = 0;

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
	  int initThreshold = 0;
	  //System.out.println(size);
	  //if(t.hasMoreTokens())
	  //	  initThreshold = new Integer(t.nextToken()).intValue();
	  int[] agentidList;
	  Variable tv  = new Variable(aid, tid, size,initThreshold);
	  vars.add(tv);
	}catch(Exception e){
	  Utility.Dprint("  Error in VARIABLE line : " + e, 0);
	  Utility.Dprint("    " + line, 0);
	}
      }
      else if(line.startsWith("CONSTRAINT")){
	try{
		if(temp == 0)
			constraintValues = new int[vars.size()][vars.size()];
		temp++;
	  int v1 = new Integer(t.nextToken()).intValue(); // var id
	  Variable var1 = getVariableFromID(v1);
	  int v2 = new Integer(t.nextToken()).intValue(); // var id
	  Variable var2 = getVariableFromID(v2);

	  if(var1 == null || var2 == null){
	    throw new Exception("Unknown variable");
	  }

	  if(t.hasMoreElements()) {
	  	constraintValues[v1][v2] = new Integer(t.nextToken()).intValue();
	  	constraintValues[v2][v1] = constraintValues[v1][v2];
	  }
	  for(int i=0;i<vars.size();i++){
	    Variable v =  (Variable) vars.elementAt(i);
	    if (v.varID == var1.varID)
	      var1.addNeighbor(var2);
	    else if(v.varID == var2.varID)
	      var2.addNeighbor(var1);
	  }
	  currentConstraint = new MaxCOPConstraint(var1, var2);
	  constraints.put(new Integer(currentConstraint.hashCode()), currentConstraint);
	}catch(Exception e){
	  Utility.Dprint("  Error in CONSTRAINT line : " + e, 0);
	  Utility.Dprint("    " + line, 0);
	}
      }
      else if(line.startsWith("F")){
	int v1 = new Integer(t.nextToken()).intValue(); // value
	Value val1 = new NominalValue(v1);
	int v2 = new Integer(t.nextToken()).intValue(); // value
	Value val2 = new NominalValue(v2);
	  int cost = 0;
	  try{
	    cost = new Integer(t.nextToken()).intValue(); // cost
	  }catch(Exception e){
	    cost = 1;
	  }
		currentConstraint.addNoGood(val1,val2,cost);//value 
      
     if (v1 == 4 && v2 ==4)  DSNmaxCost += cost;
      
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

  public void printMaxCOPProblem(){

    for(int i=0;i<agents.size();i++){
      int aid  = ((Integer) agents.elementAt(i)).intValue();
      Utility.Dprint("AGENT " + aid, 0);
      Vector vvars = getVariablesFromAgentID(aid);
      for(int j=0;j<vvars.size();j++){
	Variable tv = (Variable) vvars.elementAt(j);
	tv.printVariable();
	for(int k=0;k<tv.neighbors.size();k++){
	  Variable v = (Variable) tv.neighbors.elementAt(k);
	  MaxCOPConstraint mc = getConstraint(tv, v);
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

    System.out.println("MaxCOPProblem.comparePriority(): Unknown _orderSwitch");
    System.exit(0);
    return false;
  }

  /* no unary constraints */
  public int evaluate(Variable q1, Value v1){
    //int temp1 = q1.getInitThreshold(v1);
    return (0);
  }

  /* look up the constraint between these two variables,
     return null if no constraint */
  public MaxCOPConstraint getConstraint(Variable q1, Variable q2){
    MaxCOPConstraint mc = new MaxCOPConstraint(q1,q2);
    MaxCOPConstraint mmc = (MaxCOPConstraint)
      constraints.get(new Integer(mc.hashCode()));
    return mmc;
  }

  /* evaluate binary constraints */
  public int evaluate(Variable q1, Value v1, Variable q2, Value v2){
    int rc = 0;

    Utility.Dprint("Entering MaxCOPProblem.evaluate",
		   Utility.TRACE_RIDICULOUS_LEVEL);
    int id1 = q1.varID;
    int id2 = q2.varID;
    int val1 = new Integer(v1.toString()).intValue();
    int val2 = new Integer(v2.toString()).intValue();
    Utility.Dprint("  Comparing ("+id1+","+val1+") to ("+id2+","+val2+")",
		   Utility.TRACE_RIDICULOUS_LEVEL);

    if(connected(q1, q2)){
      MaxCOPConstraint mmc = getConstraint(q1,q2);
      rc = mmc.evaluate(q1,v1,q2,v2);
    }
    return rc;
  }

  /* no n-ary constraint */
  public int evaluate(Variable q1, Value v1, Context vw){

    Utility.Dprint("Entering MaxCOP.evaluate(nary)",
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

