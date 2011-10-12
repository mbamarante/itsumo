/*
 *    MasterThread.java
 *    Author: Jay Modi
 *    Date: Nov 13 2001
 *
 *    (Centrally) computes the quality of the current solution.
 *    
 */

package adopt.solver;

import adopt.common.*;
import adopt.problem.*;

import javax.swing.event.EventListenerList;
import java.util.*;
import java.io.*;

public class Master extends Algorithm{

  /* Current values of variables in pbm, where value[i] is the current
     value of pbm.vars.elementAt(i).  */
  Value[] values;

  static int masterID = 0;

  /* for logging output */
  Logger nLog = null;
  int bestSoFar = -1;
  int totalQuality = -1;
  StringBuffer brokenConstraints = null;
  /* are there any agents left alive? */
  boolean agentsAlive = true;

  public Master(int id, Problem p, MessageSender m)
  {
    super(id,p,m);

    values = new Value[pbm.numVars()];
    for(int i=0;i<values.length;i++)
      values[i] = null;

    PrintWriter pwtr = null;
    String fname = "Logs/Master.log";
    try{
      FileWriter ffile = new FileWriter(fname); 
      pwtr = new PrintWriter(ffile);   
    }catch(IOException e){
      System.out.println("error in opening "+fname+" for logging");
      System.exit(1);
    }
    nLog = new Logger(pwtr);
  }

  public void computeQuality(int level){

    totalQuality= 0;
    brokenConstraints = new StringBuffer();
    /* unary constraints */
    for(int i=0;i<pbm.vars.size() && totalQuality != Problem.MAX_VALUE;i++){
      Variable v1 = (Variable) pbm.vars.elementAt(i);
      if(values[i] != null){
	int q = pbm.evaluate(v1, values[i]);
	totalQuality = Problem.sumDeltas(q, totalQuality);
      }
    }
    int nullflag = 0;
    
    for(int i=0;i<pbm.vars.size() && totalQuality != Problem.MAX_VALUE;i++){
      Variable v1 = (Variable) pbm.vars.elementAt(i);
      for(int j=0;j<pbm.vars.size() && totalQuality != Problem.MAX_VALUE;j++){
	Variable v2 = (Variable) pbm.vars.elementAt(j);
	if(values[i] != null &&
	   values[j] != null){
	  if(pbm.connected(v1,v2) &&
	     pbm.comparePriority(v1,v2)){
	    int q = pbm.evaluate(v1, values[i], v2, values[j]);
	    if(q > 0)
	      brokenConstraints.append("(" + v1.uniqueNameOf() + ", " + v2.uniqueNameOf() + ")");
	    totalQuality = Problem.sumDeltas(q, totalQuality);
	  }
	}
	else{
	  nullflag = 1;
	}
      }
    }
    
    printSolution(totalQuality, level);
    
    if((totalQuality != Problem.MAX_VALUE) &&
       (bestSoFar == -1 ||
	totalQuality < bestSoFar)
       && (nullflag != 1))
      bestSoFar = totalQuality;
    
    String s = Utility.TimeStringMS();
    nLog.printToLogln(s + " " + totalQuality + " " + bestSoFar);

  }

  public int finalQuality(){
    return totalQuality;
  }

  public int bestQuality(){
    return bestSoFar;
  }

  public void logSolution(){

    nLog.printToLogln("\n\n");
    nLog.printToLogln("Solution");
    nLog.printToLogln("--------------------");
    String s = solutionString(totalQuality);
    nLog.printToLogln(s);
    pbm.logPriorityOrder(nLog);
  }

  public void printSolution(int q, int level){
    String s = solutionString(q);
    Utility.Dprint(s, level);
  }
  
  public String solutionString(int q){

    StringBuffer sb = new StringBuffer();

    for(int i=0;i<pbm.numVars();i++){
      Variable v1 = (Variable) pbm.vars.elementAt(i);
      if(values[i] != null){
	String val = values[i].toString();
	sb.append(v1.uniqueNameOf() +
		  " =  " + val + "\n");
      }
      else{
	sb.append(v1.uniqueNameOf() +
		  " =  " + "(null)" + "\n");
      }
    }
    sb.append("               quality: " + q);

	  try
	  {
		  String solnFile = "solutions/"+Simulator.inputFileName+"_"+Simulator.dprog+".qlt";
		  FileWriter solFile = new FileWriter(solnFile);
		  solFile.write(" quality " + q);
		  solFile.flush();
		  solFile.close();
	  }
	  catch(Exception e)
	  {
		  System.out.println("Could not print into file:"+e);
	  }
    sb.append("\n               broken constraints: " + brokenConstraints.toString());
    return sb.toString();
  }
  
  public void handleSTATUS(StringTokenizer stok){
    String agentname = stok.nextToken();
    String val = stok.nextToken();

    for(int i=0;i<pbm.vars.size();i++){
      Variable v = (Variable) pbm.vars.elementAt(i);
      if(agentname.equals(v.uniqueNameOf())){
	values[i] = v.getValue(val);
      }
    }
  }
  
  public void dotest(String uname, String val){
    Variable v1 = (Variable) pbm.getVariableFromUniqueVarName(uname);
    for(int i=0;i<pbm.vars.size();i++){
      Variable v2 = (Variable) pbm.vars.elementAt(i);
      if(v1.uniqueNameOf().equals(v2.uniqueNameOf())){
	values[i] =  v1.getValue(val);
      }
    }
  }

  /** Incoming messages from Node **/
  public void handleOneMsg(Message m) {
    Utility.Dprint("Received Message: " + ((Message)m).rawMessage(), Utility.MSG_LEVEL3);
    
    StringTokenizer stok = new StringTokenizer(((Message)m).rawMessage());
    String firstword = stok.nextToken();
    if(firstword.equals("STATUS"))
      handleSTATUS(stok);
  }
  
  public void wakeUp(){
    int numIter = 0;
    /* process 100 messages, then go back to sleep*/
    int maxIter = 100;
    Message m = mSender.getMessage("agent"+Master.masterID);
    while(m!=null){
      handleOneMsg(m);
      if(numIter > maxIter)
	break;
      m = mSender.getMessage("agent"+Master.masterID);
      numIter++;
    }
    computeQuality(Utility.MSG_LEVEL3);
    /* I've processed all messages, and there are no more agents alive, 
       so I want to terminate. */
    if(!agentsAlive &&
       m == null ){
      iWantToTerminate = true;
    }
  }

  public void run(){
    while(true)
      try { wakeUp();
      Thread.sleep(1000);
    } catch ( Exception e ) { }
  }

  public static void main(String [] args){
  }
}
