package frodo.OCOP;

import frodo.communication.DistributedAgentInterface;

/**
 * @author apetcu
 * 
 * To change this generated comment edit the template variable "typecomment":
 * Window>Preferences>Java>Templates.
 * To enable and disable the creation of type comments go to
 * Window>Preferences>Java>Code Generation.
 */

public interface OCOPAgentInterface extends DistributedAgentInterface {

	/**
	 * 
	 * @uml.property name="oCOPNode"
	 * @uml.associationEnd 
	 * @uml.property name="oCOPNode" multiplicity="(0 1)" inverse="myAgent:testbed.OCOP.OCOPAbstractNode"
	 */
	public OCOPAbstractNode getOCOPNode();

}
