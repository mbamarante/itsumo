/**********************************************************************
 * File Name: IntraSBB.java
 * Author: Jay Modi (modi@isi.edu)
 * Date: Feb 21 2002
 *
 * Implements the Synchronous Branch&Bound (SBB) algorithm for
 * multiple variables per agent.
 **********************************************************************/

package adopt.solver;

import adopt.common.*;
import adopt.problem.*;

import java.lang.*;
import java.io.*;
import java.util.*;

public class IntraSBB extends Intradopt{

  public IntraSBB(int agentID, Problem p, MessageSender mSndr){
    super(agentID,p,mSndr);
  } 

  public void run() {

    Utility.Dprint("  Running IntrSBB()...");
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
      SBB adpt = null;
      adpt = new SBB(pbm, var, this);
      pseudoAgents.add(adpt);
    }

    /* get each solver started */
    for(int i=0;i<pseudoAgents.size();i++){
      SBB adpt = (SBB) pseudoAgents.elementAt(i);
      adpt.init();
    }

    /* begin processing messages */
    handleMsgs();
  }

  public void handleOneMsg(Message msg){

    String msgstring = ((Message)msg).rawMessage();
    StringTokenizer stok = new StringTokenizer(msgstring);
    String firstword = stok.nextToken();
    
    /* Msg format: "TOKEN <destvariable> <sourcevariable> <cost> <context> " */
    if(firstword.equals(SBB.TOKEN))
      {
	String destvar = stok.nextToken();
	String sourcevar = stok.nextToken();
	StringBuffer sb = new StringBuffer();
	String coststring = stok.nextToken();
 	int cost = (new Integer(coststring)).intValue();

	while(stok.hasMoreTokens())
	  sb.append(" " + stok.nextToken());

	/* invoke the necessary solver */
	for(int i=0;i<pseudoAgents.size();i++){
	  SBB adpt = (SBB) pseudoAgents.elementAt(i);
	  if(adpt.x_i.uniqueNameOf().equals(destvar)){
	    Context vvw = parseContext(sb.toString());
	    adpt.whenReceivedTOKEN(vvw, cost, sourcevar);
	  }
	}
      }
    else if(firstword.equals(SBB.TERM)){

      String destvar = stok.nextToken();
      String sourcevar = stok.nextToken();
      String coststring = stok.nextToken();
      int cost = (new Integer(coststring)).intValue();

      StringBuffer sb = new StringBuffer();
      while(stok.hasMoreTokens())
	sb.append(" " + stok.nextToken());
      /* invoke the necessary solver */
      for(int i=0;i<pseudoAgents.size();i++){
	SBB adpt = (SBB) pseudoAgents.elementAt(i);
	if(adpt.x_i.uniqueNameOf().equals(destvar)){
	  Context vvw = parseContext(sb.toString());
	  adpt.whenReceivedTERMINATE(vvw, cost, sourcevar);
	}
      }
    }
  }
}
  
