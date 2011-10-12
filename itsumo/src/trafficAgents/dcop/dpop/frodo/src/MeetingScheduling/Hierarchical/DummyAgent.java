package frodo.MeetingScheduling.Hierarchical;

import frodo.OCOP.OCOPAbstractAgent;
import frodo.OCOP.communication.Environment;
import frodo.communication.MessageQueue;

/**
 * @author apetcu
 * 
 */
public class DummyAgent extends OCOPAbstractAgent {
	private static final long serialVersionUID = 1L;

	//------------------------------------------------------------------------------------------------------------------------------//
	public DummyAgent(Environment env, int id) {
		super(env, id);
	}

	public void run() {
	}

	public MessageQueue getMessageQueue(int no) {
		return null;
	}
}
//-------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------//
