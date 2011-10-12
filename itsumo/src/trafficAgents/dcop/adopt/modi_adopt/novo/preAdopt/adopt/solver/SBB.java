/**********************************************************************
 * File Name: SBB.java
 * Author: Jay Modi (modi@isi.edu)
 * Date: Feb 21 2002
 *
 * Implements the Synchronous Branch&Bound (SBB) algorithm
 **********************************************************************/

package adopt.solver;

import adopt.problem.*;
import adopt.common.*;

import java.lang.*;
import java.io.*;
import java.util.*;

public class SBB extends Adopt{

  /* inherited variables from Adopt.java */
  /* 
     public static String TERM = "TERMINATE";
     Context CurrentContext;
     public Variable x_i;
     Value d_i;
     Problem pbm;
     boolean iWantToTerminate = false;
     MessageSender mSender;
     Logger nLog = null;
     */

  public static String TOKEN = "TOKEN";

  /* previous and next variable in ordering */
  Variable previous;
  Variable next;

  /* CurrentContext holds the current partial solution being explored.
     CurrentContextCost holds the cost of CurrentContext */
  int CurrentContextCost = 0;

  /* best extension of CurrentContext so far */
  Context bestExtension = new Context();
  /* Upper bound on cost for best extension of CurrentContext so far */ 
  int bestExtensionCost = Problem.MAX_VALUE;

  /* best partial solution seen so far */
  Context bestPartialSolution = new Context();
  /* Upper bound on cost of best partial solution seen so far */
  int bestPartialSolutionCost = Problem.MAX_VALUE;
  /* My globally best value for the best partial solution seen so far */
  Value bestValueForBestPartialSolution;

  /* valueTried[i] is true iff value in Di[i] has been explored */
  boolean[] valueTried;
  /* index of d_i in Di */
  int currentValIdx;
  /* domain of values for x_i */ 
  Value[] Di;
  
  /* 'this' is responsible for variable 'vvar' in problem 'p' */
  public SBB(Problem p, Variable vvar, MessageSender mSndr){

    super(p, vvar, true,true,mSndr);
    Di = x_i.domain();
    valueTried = new boolean[Di.length];
    resetExploredValues();

    /* determine if I am the lowest or highest priority agent */
    next = pbm.nextVariable(x_i);
    previous = pbm.previousVariable(x_i);

    /****Debug Print Block *****/
    Utility.Dprintc(x_i.uniqueNameOf()+ ": ",  
		    Utility.TRACE_FUNCTION_EXECUTION_LEVEL);              
    if(previous != null)
      Utility.Dprintc("previous = "+previous.uniqueNameOf(), 
		      Utility.TRACE_FUNCTION_EXECUTION_LEVEL);              
    else
      Utility.Dprintc("previous = null", 
		      Utility.TRACE_FUNCTION_EXECUTION_LEVEL);

    if(next != null)
      Utility.Dprint(", next = "+next.uniqueNameOf(), 
		     Utility.TRACE_FUNCTION_EXECUTION_LEVEL);
    else
      Utility.Dprint(", next = null", 
		     Utility.TRACE_FUNCTION_EXECUTION_LEVEL);

    /****End Print Block ****/
  }

  public void init(){
    /* set current value to first value in domain */
    currentValIdx = 0;
    d_i = Di[currentValIdx];
    /* am i the root? */
    if(previous == null){
      /* send partial solution to next agent*/
      int nextValIdx = branchStep(CurrentContext, CurrentContextCost);
      if(nextValIdx >= 0 ){
	d_i = Di[nextValIdx];
	valueTried[nextValIdx] = true;
	Context extendedSolution = extendSolution(CurrentContext, d_i);
	int delta =  pbm.delta(x_i, extendedSolution);
	int extendedSolutionCost = pbm.sumDeltas(CurrentContextCost, delta);
	SEND_TOKEN(extendedSolution, extendedSolutionCost, next);
      }
      else{
	/* error check */
	System.out.println(" Error: SBB algorithm " );
	System.out.println(" Root cannot find initial solution"+
			   " ...I'm confused!!! " );
	System.exit(0);
      }
    }
  }

  public void resetExploredValues(){
    for(int j=0;j<Di.length;j++){
      valueTried[j] = false;
    }
  }

  /* Extend the given solution with the given value for x_i */
  public Context extendSolution(Context solution, Value val){
    Context result = new Context(solution);
    result.addVarToContext(x_i, val);
    return result;
  }

  public void SEND_TERMINATE(Context vw, int cost, Variable vvar){
    String aname = "agent" + vvar.agentIDof();  
    String header = this.TERM;
      
    /* Msg format: "TERMINATE  <destvar> <sourcevariable> <Context>" */
    String msg = header 
      + " " + vvar.uniqueNameOf() 
      + " " + x_i.uniqueNameOf() 
      + " " + cost  
      + " " + vw.toMsg();

    mSender.sendMessage(new Message(vvar.agentID, x_i.agentID, msg), aname, true);    
  }

  public void SEND_TOKEN(Context vw, int cost, Variable vvar){

    String aname = "agent" + vvar.agentIDof();  
    String header = this.TOKEN;
    
    /* Msg format: "TOKEN  <destvariable> <sourcevar> <cost> <Context>" */
    String msg = header + 
      " " + vvar.uniqueNameOf() + 
      " " + x_i.uniqueNameOf() + 
      " " + cost + 
      " " + vw.toMsg();
    mSender.sendMessage(new Message(vvar.agentID, x_i.agentID, msg), aname, true);    
  }

  /* return the locally best branch for the given Context */
  public Context getBestBranch(Context vw){
    Context result = new Context();
    int minCost = Problem.MAX_VALUE;
    for(int i=0;i<Di.length;i++){
      Value val = Di[i];
      Context vvw = extendSolution(vw, val);
      int localcost =  pbm.delta(x_i,vvw);
      
      if(pbm.compareDeltas(minCost, localcost) > 0){
	result = vvw;
	minCost = localcost;
      }
    }
    return result;
  }

  /* Find an untried value of my variable that extends the given
     solution vw and minimizes cost.  this cost must be better than
     known upper bound. Return null if not possible */
  public int branchStep(Context vw, int cost){

    Context result = null;
    int minCost = Problem.MAX_VALUE;
    int minValIdx = -1;

    for(int i=0;i<Di.length;i++){
      if(!valueTried[i]){
	Value val = Di[i];
	Context extendedSolution = extendSolution(vw, val);
	int delta =  pbm.delta(x_i, extendedSolution);
	int extendedCost = pbm.sumDeltas(cost, delta);

	if(pbm.compareDeltas(minCost, extendedCost) > 0){
	  minCost = extendedCost;
	  minValIdx = i;
	}
      }
    }

    /* BOUNDING STEP:
       if the cost of the extended solution is already greater than a known
       upper bound, return null */

    if(pbm.compareDeltas(minCost, bestPartialSolutionCost) > 0){
      return -1;
    }
    else
      return minValIdx;

  }

  public void whenReceivedTERMINATE(Context vw, int cost, String sourcevar){

    logTerminateMsgRcvd(vw, cost, sourcevar);

    if(!vw.equals(bestPartialSolution) ||
       cost != bestPartialSolutionCost){
      System.out.println(" Error: SBB algorithm " );
      System.out.println(" Termination Solution rcvd from parent"+
			 " and my best solution are not equal...I'm confused!!! " );
      System.out.println(" bestPartialSolution: " + bestPartialSolution.toString());
      System.out.println(" bestPartialSolutionCost: " + bestPartialSolutionCost);
      System.out.println(" vw: " + vw.toString());
      System.out.println(" cost: " + cost);
      System.exit(0);
    }

    iWantToTerminate = true;
    d_i = bestValueForBestPartialSolution;
    if(next != null){
      Context extendedSolution = extendSolution(vw, d_i);
      SEND_TERMINATE(extendedSolution, bestPartialSolutionCost, next);
    }
  }

  public void whenReceivedTOKEN(Context vw, int cost, String sourcevar){

    Variable vvar = pbm.getVariableFromUniqueVarName(sourcevar);

    logTokenMsgRcvd(vw, cost, sourcevar);

    /* did token come from lower agent? */
    if(pbm.comparePriority(x_i, vvar)){
      /* is this solution better than best so far */
      if(pbm.compareDeltas(bestPartialSolutionCost, cost) > 0){

	/* remember it */
	bestPartialSolutionCost = cost;
	bestPartialSolution = CurrentContext;
	bestValueForBestPartialSolution = vw.valueOf(x_i);

	/* error check */
	if(!CurrentContext.compatible(vw)){
	  System.out.println(" Error: SBB algorithm " );
	  System.out.println(" CurrentContext and Solution rcvd from child"+
			     " are not compatible...I'm confused!!! " );
	  System.exit(0);
	}
      }

      /* is this solution better than best so far for the Current
         Context? */
      if(pbm.compareDeltas(bestExtensionCost, cost) > 0){
	bestExtension = vw;
	bestExtensionCost = cost;
      }

      /* are we still searching? */
      if(!iWantToTerminate){

	int nextValIdx = branchStep(CurrentContext, CurrentContextCost);

	if(nextValIdx >= 0){
	  d_i = Di[nextValIdx];
	  valueTried[nextValIdx] = true;
	  Context extendedSolution = extendSolution(CurrentContext, d_i);
	  int delta =  pbm.delta(x_i, extendedSolution);
	  int extendedSolutionCost = pbm.sumDeltas(CurrentContextCost, delta);
	  SEND_TOKEN(extendedSolution, extendedSolutionCost, next);
	}
	else{
	  /* no more branches left */
	  /* am I highest var? */
	  if (previous == null){
	    /* choose best val. */
	    d_i = bestExtension.valueOf(x_i);
	    iWantToTerminate = true;
	    SEND_TERMINATE(bestExtension, bestExtensionCost, next);
	  }
	  else{
	    /* backtrack */
	    SEND_TOKEN(CurrentContext, bestExtensionCost, previous);
	    nLog.printToLogln(  "   " + Utility.TimeStringMS() 
				+ "---backtracking "  
				+ " cc:" + CurrentContext.toString()
				+ "\n bpsc: " + bestPartialSolution.toString()
				+ "\n cost: " + bestPartialSolutionCost);
	  }
	}
      }
    }
    /* token came from higher agent */
    else{ 
      CurrentContext = vw;
      CurrentContextCost = cost;
      resetExploredValues();
      
      /* I am leaf */
      if(next == null){
	/* extend current solution */
	bestExtension = getBestBranch(CurrentContext);
	int delta =  pbm.delta(x_i,bestExtension);
	bestExtensionCost = pbm.sumDeltas(cost, delta);
	/* is this solution the best I've seen so far ? */
	if(pbm.compareDeltas(bestPartialSolutionCost, bestExtensionCost) > 0){
	  bestPartialSolution = CurrentContext;
	  bestPartialSolutionCost = bestExtensionCost;
	  bestValueForBestPartialSolution = bestExtension.valueOf(x_i);
	}
	SEND_TOKEN(CurrentContext, bestExtensionCost, previous);
      }else{

	int nextValIdx = branchStep(CurrentContext, CurrentContextCost);
	
	if(nextValIdx >= 0){
	  d_i = Di[nextValIdx];
	  valueTried[nextValIdx] = true;
	  Context extendedSolution = extendSolution(CurrentContext, d_i);
	  int delta =  pbm.delta(x_i, extendedSolution);
	  int extendedSolutionCost = pbm.sumDeltas(CurrentContextCost, delta);
	  SEND_TOKEN(extendedSolution, extendedSolutionCost, next);
	}
	else {
	  /* all branches are worse than upper bound, so backtrack */
	  nLog.printToLogln(  "   " + Utility.TimeStringMS() 
			      + "---backtracking "  
			      + " cc:" + CurrentContext.toString()
			      + " bpsc: " + bestPartialSolution.toString());
	  
	  Utility.Dprint("    ---Backtracking: " + x_i.uniqueNameOf() + "---");
	  Utility.Dprint("    Upper Bound: " + bestPartialSolutionCost);
	  Utility.Dprint("    ------");
	  /*** end print block ****/

	  SEND_TOKEN(CurrentContext, Problem.MAX_VALUE, previous);
	}
      }
    }
  }

  

  public void logTerminateMsgRcvd(Context vw, int cost, String source){
    
    nLog.printToLogln(  "   " + Utility.TimeStringMS() 
			+ "---term from " + source + ": " 
			+ " " + cost
			+ " " + vw.toString());
  }

  public void logTokenMsgRcvd(Context vw, int cost, String source){
    
    nLog.printToLogln(  "   " + Utility.TimeStringMS() 
			+ "---from " + source + ": " 
			+ " " + cost
			+ " " + vw.toString());
  }

}




