/**********************************************************************
 * File Name: MaxCSPConstraint.java
 * Author: Jay Modi (modi@isi.edu)
 * Date: Feb 22 2002
 *
 * Description: the MaxCSP constraint.
 * Stores sets of nogoods for a given pair of variables.
 **********************************************************************/

package adopt.problem;
import adopt.common.*;

import java.lang.*;
import java.io.*;
import java.util.*;

/** The MaxCSPConstraint.  
 */
public class MaxCSPConstraint {

  /* all constraints are binary. */
  Variable var1;
  Variable var2;
  /* weight of this constraint */
  int cost;
  /* prohibited value pairs */
  Hashtable nogoods;

  class nogood{
    NominalValue val1;
    NominalValue val2;
    
    nogood(Value v1, Value v2){
      val1 = (NominalValue)v1;
      val2 = (NominalValue)v2;
    }
    /* are two nogood equal? */
    public boolean equals(nogood ng){
      if (ng.val1.equal(val1) && ng.val2.equal(val2))
	return true;
      return false;
    }

    public int hashCode(){
      int n1 = new Integer(val1.toString()).intValue();
      int n2 = new Integer(val2.toString()).intValue();
      return n1*1000 + n2;
    }
  }

  /** Return a new MaxCSPConstraint. 
   */
  public MaxCSPConstraint(Variable v1, Variable v2, int cst){
    super();
    nogoods = new Hashtable();
    var1 = v1;
    var2 = v2;
    cost = cst;
  }
  
  public int hashCode(){
    int n1 = var1.varID;
    int n2 = var2.varID;
    if(n1 > n2)
      return n1*1000 + n2;
    else
      return n2*1000 + n1;
  }

  /* add a prohibited value pair to this constraint */
  public void addNoGood(Value v1, Value v2){
    nogood ng = new nogood(v1,v2);
    nogoods.put(new Integer(ng.hashCode()), ng);
    /* error check */
    nogood n = new nogood(v1,v2);
    if(nogoods.get(new Integer(n.hashCode())) == null){
      System.out.println("Hashtable is hosed");
      System.out.println("Exiting.");
      System.exit(0);
    }
  }

  /* returns 1 if (vvar1 = v1, vvar2 = v2) is a nogood
     return 0 otherwise.
     */
  public int evaluate(Variable vvar1, Value v1, Variable vvar2, Value v2){
    int rc = 0;
    /*** print block ****/
    Utility.Dprint("Entering MaxCSPConstraint.evaluate", 
		   Utility.TRACE_RIDICULOUS_LEVEL);
    int id1 = vvar1.varID;
    int id2 = vvar2.varID;
    int val1 = new Integer(v1.toString()).intValue();
    int val2 = new Integer(v2.toString()).intValue();
    Utility.Dprint("  Comparing ("+id1+","+val1+") to ("+id2+","+val2+")", 
		   Utility.TRACE_RIDICULOUS_LEVEL);
    /*** end print block ****/

    nogood n = null;
    if(var1.equalVar(vvar1) && var2.equalVar(vvar2))
      n = new nogood(v1,v2);
    else if(var1.equalVar(vvar2) && var2.equalVar(vvar1))
      n = new nogood(v2,v1);
    else{
      System.out.println("MaxCSPConstraint.evaluate(): Error!");
      System.out.println("  Unknown Variables:");
      vvar1.printVariable();
      vvar2.printVariable();
      System.exit(0);
    }
    if(nogoods.get(new Integer(n.hashCode())) != null){
      rc = cost;
    }
    Utility.Dprint("  returning MaxCSPConstraint.evaluate(): " + rc,
		   Utility.TRACE_RIDICULOUS_LEVEL);
    return rc;
  }

  public void printConstraint(){
    Enumeration enum = nogoods.keys();
    while(enum.hasMoreElements()){
      nogood n = (nogood) nogoods.get(enum.nextElement());
      System.out.println("     Nogood: " + n.val1.toString() +" " + n.val2.toString());
    }
  }

  public static void main(String[] args)
  {

  }

}



