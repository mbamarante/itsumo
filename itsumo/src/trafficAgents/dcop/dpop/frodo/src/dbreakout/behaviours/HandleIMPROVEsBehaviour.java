package frodo.dbreakout.behaviours;

/**
 * Behaviour to receive and reply a message from another agent
 */

import java.util.StringTokenizer;
import java.util.Vector;

import frodo.CSP.VariableId;
import frodo.communication.DistributedAgentInterface;
import frodo.communication.Message;
import frodo.communication.MessageQueue;
import frodo.communication.MessageQueueProcessor;
import frodo.dbreakout.CSPAgent;
import frodo.dbreakout.CSP.CSPVariableDBA;


public class HandleIMPROVEsBehaviour extends MessageQueueProcessor {
	//---------------------------------------------------------------------------//
	public HandleIMPROVEsBehaviour(DistributedAgentInterface a, MessageQueue mq) {
		super(a, mq);
	}
	//---------------------------------------------------------------------------//
	public Vector processMessage(Message msg) {
		String content = msg.getPayload();
		/*
		        Object theMonitor=myAgent.getSynchronizer();
		        System.out.println("--------> IMPROVE:"  + request);
		        synchronized(theMonitor){
		            while(!myAgent.isIMPROVEMode()){
		                try{
		                    System.out.println("WAITING IN GOT IMPROVE..");
		                    theMonitor.wait(10000);
		                }catch(InterruptedException ie){
		                    }
		            }
		 */
		//particular case of DBreakout agent
		CSPAgent myDBAgent = (CSPAgent) myAgent;

		Vector updates = new Vector();

		if (!myDBAgent.getAllResponded()) {
			//	    System.out.println("Still goin' on... -> " +request.getPerformative() +" " + request.getSender().getLocalName() + ": " + content);
			try {
				StringTokenizer st = new StringTokenizer(content);
				if (st.hasMoreTokens()) {
					String v1 = st.nextToken();
					if (v1.equalsIgnoreCase("IMPROVE")) {
						int howmuch = Integer.parseInt(st.nextToken());
						int hisEval = Integer.parseInt(st.nextToken());
						int hisTermCnt = Integer.parseInt(st.nextToken());
						if (howmuch > 0) {
							while (st.hasMoreTokens()) {
								v1 = st.nextToken(); // that's the var id + value (sth like "1.2=4")
								//			    System.out.println("&&&&&&&&&&&&&&&&Gonna break up >" + v1);
								StringTokenizer st1 = new StringTokenizer(v1);
								int agentID = Integer.parseInt(st1.nextToken(" ."));
								//			    System.out.println("&&&&&&&&&&&&&&&& AGENT_ID>" + agentID);

								String curu = st1.nextToken("=");
								//			    System.out.println("&&&&&&&&&&&&&&&& CURU>" + curu);
								int variableID = Integer.parseInt(curu.substring(1));
								//			    System.out.println("&&&&&&&&&&&&&&&& VARIABLE_ID>" + variableID);

								curu = st1.nextToken("=");
								//			    System.out.println("&&&&&&&&&&&&&&&& CURU>" + curu);
								Integer variableValue = Integer.valueOf(curu);
								VariableId vid = new VariableId(agentID, variableID);
								CSPVariableDBA tvar = new CSPVariableDBA(vid, null, null);
								tvar.setValue(variableValue);
								updates.add(tvar);
							}
						}
						myDBAgent.addImprove(updates, howmuch, hisEval, hisTermCnt);
					}
				}
			} catch (Exception nsee) {
				nsee.printStackTrace();
			}
		} else {
			System.out.println("GOT ALL MSG's... plus this: " + content);
		}
		//        theMonitor.notifyAll();
		return updates;
		//    }//synchr
	}
	//---------------------------------------------------------------------------//
	/*
	    protected ACLMessage prepareResultNotification(ACLMessage request, ACLMessage response){
	        ACLMessage reply = request.createReply();
	        String content = request.getContent();
	        ctrl.println("-> REQUEST " + request.getSender().getName() + "-> " + content);
	//        ctrl.println(request.toString());
	        if ((content != null) && (content.indexOf("?") != -1)){
	            // get the values
	            // The syntax of the content is:
	            // val1 val2 ? vn
	            try{
	                StringTokenizer params = new StringTokenizer(content);
	                String val1 = params.nextToken(); // First parameter
	                String val2 = params.nextToken(); // Second parameter
	                params.nextToken(); // word ?
	                String src = params.nextToken(); // index of the variable of the sender
	//                ctrl.println("\nSource: " + src);
	 
	                boolean sol= false;
	                sol=myagent.neighbourhoodInterchangeable(val1, val2, src);
	 
	                // We have the result, and we must send it following the REQUEST protocol
	                reply.setPerformative(ACLMessage.INFORM);
	                if (sol)
	                    content = "Interchangeable " +val1 + " " + val2 + " YES";
	                else
	                    content = "Interchangeable " +val1 + " " + val2 + " NO";
	                reply.setContent(content);
	                ctrl.println("\n <- INFORM:" + content);
	                return reply;
	            }catch (Exception e){
	                e.printStackTrace();
	            }
	        }
	        ctrl.println("\n Received trash: "+ request.toString());
	        reply.setPerformative(ACLMessage.NOT_UNDERSTOOD);
	        reply.setContent("Unexpected Content (" + content + ")");
	        ctrl.println("\n Sending the trash back...");
	        return reply;
	    }
	 */
	//---------------------------------------------------------------------------//
} //End class ReplyBehaviour
