/**********************************************************************
 * File Name: Intradopt.java
 * Author: Jay Modi (modi@isi.edu)
 * Date: Aug 27 2001
 *
 * Implements the Adopt algorithm for multiple variables per agent.
 **********************************************************************/

package adopt.solver;

import adopt.common.*;
import adopt.problem.*;

import java.lang.*;
import java.io.*;
import java.util.*;

public class Intradopt extends Algorithm{

  /* list of pseudo-agents, one for each variable I own */
  Vector pseudoAgents;

  /* input message queues for each pseudoAgent.  */
  Hashtable msgQs = null;
  /* input message queue buffer */
  LinkedList bufferedMsgs = null;

  public Intradopt(int agentID, Problem p, MessageSender mSndr){
    super(agentID,p,mSndr);
    msgQs = new Hashtable();
    bufferedMsgs = new LinkedList();
  }

  /*Create a msg queue for the given pseudo-agent. */
  public void addMsgReceiver(String uname){
    LinkedList msgQ = new LinkedList();
    msgQs.put(uname, msgQ);
  }

  public void run() {

    Utility.Dprint("  Running Intradopt()...");
    /* problem set up */
    Vector vec= pbm.getVariablesFromAgentID(localAgentID);
    if(vec.size() == 0){
      Utility.Dprint("Can't find any variables for agent: " + localAgentID);
      return;
    }

    /* set up an input message queue for each variable */
    for(int i=0;i<vec.size();i++){
      Variable var = (Variable) vec.elementAt(i);
      addMsgReceiver(var.uniqueNameOf());
    }

    /* create a pseudoagent for each variable I own */
    pseudoAgents = new Vector();
    for(int i=0;i<vec.size();i++){
      Variable var = (Variable) vec.elementAt(i);
      Adopt adpt = null;
      adpt = new Adopt(pbm, var, true, true, this);
      pseudoAgents.add(adpt);
    }

    /* get each solver started */
    for(int i=0;i<pseudoAgents.size();i++){
      Adopt adpt = (Adopt) pseudoAgents.elementAt(i);
      adpt.init();
      adpt.backtrack();
    }

    /* begin processing messages */
    handleMsgs();
  }

  public void handleMsgs(){
    Message msg = null;
    long tprime = Utility.TimeMS();

    Utility.Dprint("Entering handleMsgs():...", Utility.TRACE_RIDICULOUS_LEVEL);
    Utility.Dprint("  Waiting for incoming msg...", Utility.MSG_LEVEL3);
    while(!iWantToTerminate){
      cycleCnt++;
      if(cycleCnt % 2000 == 0) ;
      	//System.out.println("Cycle Count now is " + cycleCnt);
      /* deliver messages to each pseudoagent */
      for(int i=0;i<pseudoAgents.size();i++){
	Adopt adpt = (Adopt) pseudoAgents.elementAt(i);
	String uname = adpt.x_i.uniqueNameOf();
	boolean msgDelivered = false;

	LinkedList q = (LinkedList) msgQs.get(uname);
	try{msg = (Message) q.removeFirst();}
	catch(NoSuchElementException ex){ msg = null;}
	while(msg != null){
			//System.out.println(" Message is msg = " + msg.message + " from src = " + msg.sourceAgent + " to dest = " + msg.destAgent + " *");
	  /* deliver message */
	  handleOneMsg(msg);
	  msgDelivered = true;
	  try{msg = (Message) q.removeFirst();}
	  catch(NoSuchElementException ex){ msg = null;}
	}
	/* allow pseudoagent to execute */
	if(msgDelivered){
	  Utility.Dprint("   "+uname+" executing...", Utility.MSG_LEVEL);
	  adpt.backtrack();
	}
      }

      /* report current variable values to Master agent (every 10 secs) */
      if(Utility.TimeMS() - lastReportTime > reportInterval){
	reportValuesToMaster();
	lastReportTime = Utility.TimeMS();
	/* also log some data */
	logData();
      }

      /* get messages from other agents */
      msg = mSender.getMessage("agent"+localAgentID);
      while(msg!=null){
	//System.out.println(" Message is msg = " + msg.message + " from src = " + msg.sourceAgent + " to dest = " + msg.destAgent + " *");
	bufferedMsgs.addLast(msg);
	numMsgsRcvd++;
	msg = mSender.getMessage("agent"+localAgentID);
      }

      /* put buffered messages on appropriate input message queue */
      msg=null;
      try{msg = (Message) bufferedMsgs.removeFirst();}
      catch(NoSuchElementException ex){ msg = null;}
      while(msg != null){
	String msgstring = ((Message)msg).rawMessage();
	StringTokenizer stok = new StringTokenizer(msgstring);
	String firstword = stok.nextToken();
	String dest = stok.nextToken();
	LinkedList q = (LinkedList) msgQs.get(dest);
	q.addLast(msg);
	try{msg = (Message) bufferedMsgs.removeFirst();}
	catch(NoSuchElementException ex){ msg = null;}
      }
      /* check for termination */
      if(checkForTermination2()){
	iWantToTerminate = true;
	/* record time of termination */
	timeEnd = Utility.TimeMS();
	reportValuesToMaster();
	logData();
	float avgAccuracy1 = 0;
	float avgAccuracy2 = 0;
	float avgAccuracy3 = 0;
	float avgRContexts = 0;
	float avgUContexts = 0;
	int avgLength = 0;
	
		try{

		String threshFile = "solutions/"+Simulator.inputFileName+"_"+Simulator.dprog+".thr";
		FileWriter thresholdFile = new FileWriter(threshFile);
		System.out.println("Printing Lower bound values in to a file");

		for(int k =0;k<pseudoAgents.size();k++)
			{
				int min=-1;
				Adopt nextAgent = ((Adopt)pseudoAgents.elementAt(k));

				    //	avgAccuracy1 += nextAgent.getAccuracy1();
				avgAccuracy2 += nextAgent.getAccuracy2();
				
				PriorityTree nextAgentSubTree = Simulator.pbm.pOrderTree.getSubTree(nextAgent.x_i);
				
				if (nextAgentSubTree.children.size() != 0){
				avgLength++; 	
				avgAccuracy3 += nextAgent.getAccuracy2();
				}
			
				avgRContexts += nextAgent.getRepeatedContexts();
				avgUContexts += nextAgent.getUniqueContexts();
				
				thresholdFile.write("Var "+nextAgent.x_i.varIDof()+"\n");
				
				
				for(int r=0; r<nextAgent.x_i.domainSize(); r++)
				{	int LB = nextAgent.getLB(r);
					thresholdFile.write("Val "+r+" "+LB+"\r");					
				}
				thresholdFile.flush();
			
			}
			thresholdFile.close();
	}
	catch(Exception e){System.out.println(e);}

	avgAccuracy1 /= pseudoAgents.size();
	avgAccuracy2 /= pseudoAgents.size();
	avgAccuracy3 /= avgLength; 
	avgRContexts /= pseudoAgents.size();
	avgUContexts /= pseudoAgents.size();

		
	System.out.println("       agent" + localAgentID + "'s  cycle cnt: " + cycleCnt);
	System.out.println("    ****** \nAccuracy 1 = " + avgAccuracy1);
    System.out.println("    Accuracy 2 = " + avgAccuracy2);
    System.out.println("    Accuracy 3 = " + avgAccuracy3);
    System.out.println("    Repeated contexts = " + avgRContexts );
    System.out.println("    Unique Contexts = " + avgUContexts+"\n******");
    
    try{
    String solnFile = "solutions/"+Simulator.inputFileName+"_"+Simulator.dprog+".sol";
	FileWriter solFile = new FileWriter(solnFile);
	
	//PrintWriter printFile = new PrintWriter(solFile);
	   
    solFile.write(" agent" + localAgentID + "'s  cycle cnt: " + cycleCnt);
	//solFile.write("    ****** \nAccuracy 1 = " + avgAccuracy1);
    solFile.write(" Depth = " +pbm.orderDepth());
    solFile.write(" Accuracy 2 = " + avgAccuracy2);
    solFile.write(" Accuracy 3 = " + avgAccuracy3+"\n");
    solFile.write("   Repeated contexts = " + avgRContexts );
    solFile.write("   Unique Contexts = " + avgUContexts+"\n******");
    solFile.flush();
    solFile.close();
    
	}
	catch(Exception e)
	{
    	System.out.println("Could not print into file:"+e);
		}
      }
    }
  }

  public boolean checkForTermination2(){
    for(int i=0;i<pseudoAgents.size();i++){
      Adopt adpt = (Adopt) pseudoAgents.elementAt(i);
      if(!adpt.iWantToTerminate)
	return false;
    }
    /* All pseudoAgents terminated. must be done. */
    return true;
  }

  public boolean messageWanted(Adopt adpt, String destvar){
    if(adpt.x_i.uniqueNameOf().equals(destvar) &&
       !adpt.iWantToTerminate){
      return true;
    }
    return false;
  }

  public void handleOneMsg(Message msg){

    String msgstring = ((Message)msg).rawMessage();
    StringTokenizer stok = new StringTokenizer(msgstring);
    String firstword = stok.nextToken();

    if(firstword.equals(Adopt.TERM)){
      String destvar = stok.nextToken();
      String sourcevar = stok.nextToken();
      /* invoke the necessary pseudoAgent */
      for(int i=0;i<pseudoAgents.size();i++){
	Adopt adpt = (Adopt) pseudoAgents.elementAt(i);
	if(messageWanted(adpt, destvar)){
	  adpt.whenReceivedTERMINATE();
	}
      }
    }
    else if(!timeToLoseMsg2()){

      /* Msg format: "VALUE  <destvariable> <THRESHOLD> <variable> <value> " */
      if(firstword.equals(Adopt.VALUE)){
	String destvar = stok.nextToken();
	int flimit = (new Integer(stok.nextToken())).intValue();
	String v2 = stok.nextToken();
	String val2 = stok.nextToken();
	/* invoke the necessary pseudoAgent */
	for(int i=0;i<pseudoAgents.size();i++){
	  Adopt adpt = (Adopt) pseudoAgents.elementAt(i);
	  if(messageWanted(adpt, destvar)){
	    Variable vvar = (Variable) pbm.getVariableFromUniqueVarName(v2);
	    Value vval = vvar.getValue(val2);
	    adpt.whenReceivedVALUE(vvar, vval, flimit);
	  }
	}
      }
      /* Msg format: "COST  <destvariable> <sourcevariable> <lb> <ub> <context>" */
      else if(firstword.equals(Adopt.COST)){
	String destvar = stok.nextToken();
	String sourcestr = stok.nextToken();
	int lb = (new Integer(stok.nextToken())).intValue();
	int ub = (new Integer(stok.nextToken())).intValue();
	StringBuffer sb = new StringBuffer();
	while(stok.hasMoreTokens())
	  sb.append(" " + stok.nextToken());
	/* invoke the necessary pseudoAgent */
	for(int i=0;i<pseudoAgents.size();i++){
	  Adopt adpt = (Adopt) pseudoAgents.elementAt(i);
	  if(messageWanted(adpt, destvar)){
	    Context vvw = parseContext(sb.toString());
	    adpt.whenReceivedCOST(vvw, lb, ub, sourcestr);
	  }
	}
      }
    }
  }

  public void reportValuesToMaster(){
    for(int i=0;i<pseudoAgents.size();i++){
      Adopt adpt = (Adopt) pseudoAgents.elementAt(i);
	/* Msg format: "STATUS <unique name> <current val> <cost>" */
	String rawmsg = "STATUS " + adpt.x_i.uniqueNameOf() +
	  " " + adpt.d_i.toString() + " " + 0;

	mSender.sendMessage(new Message(Master.masterID, localAgentID, rawmsg),
			    "agent"+Master.masterID, false);
    }
  }

  public void setLogger(Logger l){
    nLog = l;
    nLog.printToLogln("# <time> <cycles> <nummsgsrcvd> <iwanttoterminate>");
  }

  public void logData(){
    /* total number of messages sent */
    nLog.printToLogln(Utility.TimeStringMS() + " " + cycleCnt + " " +
		      numMsgsRcvd + " " + iWantToTerminate );
  }

  /* Override Algorithm.sendMessage:
     (check to see if message is going from one pseudoagent to another)
     'cntFlag': count this message in the final stats? */
  public void sendMessage(Message m, String aname, boolean cntFlag){
    /*check to see if message is going from one pseudoagent to another */
    if (aname.equals("agent"+localAgentID))
      bufferedMsgs.addLast(m);
    else{
      mSender.sendMessage(m, aname, cntFlag);
    }
    LogMsg_SEND(m, localAgentID);
  }

}

