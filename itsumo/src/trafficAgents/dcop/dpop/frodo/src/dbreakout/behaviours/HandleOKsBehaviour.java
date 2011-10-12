package frodo.dbreakout.behaviours;

import java.util.StringTokenizer;
import java.util.Vector;

import frodo.CSP.VariableId;
import frodo.communication.DistributedAgentInterface;
import frodo.communication.Message;
import frodo.communication.MessageQueue;
import frodo.communication.MessageQueueProcessor;
import frodo.dbreakout.CSPAgent;
import frodo.dbreakout.CSP.CSPVariableDBA;


public class HandleOKsBehaviour extends MessageQueueProcessor {

	//---------------------------------------------------------------------------//
	public HandleOKsBehaviour(DistributedAgentInterface a, MessageQueue mq) {
		super(a, mq);
	}
	//---------------------------------------------------------------------------//
	public Vector processMessage(Message msg) {
		String content = msg.getPayload();
		//particular case of DBreakout agent
		CSPAgent myDBAgent = (CSPAgent) myAgent;

		Vector updates = new Vector();
		//        myLogger.println("-------->"  + content);
		if (!myDBAgent.getAllResponded()) {
			//	    myLogger.println("Still goin' on... -> " +request.getPerformative() +" " + request.getSender().getLocalName() + ": " + content);
			try {
				StringTokenizer st = new StringTokenizer(content);
				if (st.hasMoreTokens()) {
					String v1 = st.nextToken();
					if (v1.equalsIgnoreCase("OK?")) {
						while (st.hasMoreTokens()) {
							v1 = st.nextToken(); // that's the var id + value (sth like "1.2=4")
							//			    myLogger.println("&&&&&&&&&&&&&&&&Gonna break up >" + v1);
							StringTokenizer st1 = new StringTokenizer(v1);
							int agentID = Integer.parseInt(st1.nextToken(" ."));
							//			    myLogger.println("&&&&&&&&&&&&&&&& AGENT_ID>" + agentID);

							String curu = st1.nextToken("=");
							//			    myLogger.println("&&&&&&&&&&&&&&&& CURU>" + curu);
							int variableID = Integer.parseInt(curu.substring(1));
							//			    myLogger.println("&&&&&&&&&&&&&&&& VARIABLE_ID>" + variableID);

							curu = st1.nextToken("=");
							//                            myLogger.println("&&&&&&&&&&&&&&&& CURU>" + curu);
							Integer variableValue = Integer.valueOf(curu);
							VariableId vid = new VariableId(agentID, variableID);
							CSPVariableDBA tvar = new CSPVariableDBA(vid, null, null);
							tvar.setValue(variableValue);
							updates.add(tvar);
						}
						if (updates.size() == 0)
							myLogger.println("\t\t$$$$$$$$$$$$" + content + " wants to finish.");
						myDBAgent.addOK(updates);
					}
				}
			} catch (Exception nsee) {
				myLogger.println(nsee.getMessage() + " - " + nsee.toString());
			}
		} else {
			myLogger.println("GOT ALL MSG's... plus this: " + content);
		}
		//    theMonitor.notifyAll();
		return updates;
		//    }//synchr
	}
	//---------------------------------------------------------------------------//
}
